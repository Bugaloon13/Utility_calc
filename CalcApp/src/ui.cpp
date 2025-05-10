// === ui.cpp ===
#include "ui.h"
#include <curses.h>
#include <sstream>
#include <fstream>
#include <string>

static double promptDouble(int y, int x, const char* prompt) {
    char buf[64]; double val;
    while (true) {
        mvprintw(y, x, "%s", prompt); clrtoeol(); echo();
        getnstr(buf, sizeof(buf) - 1); noecho();
        std::stringstream ss(buf);
        if (ss >> val && ss.eof()) return val;
        mvprintw(y + 1, x, "Invalid input; enter a number (e.g. 123.45)");
        clrtoeol(); refresh(); getch(); mvprintw(y + 1, x, ""); clrtoeol();
    }
}

static int promptInt(int y, int x, const char* prompt, int minVal, int maxVal) {
    char buf[64]; int val;
    while (true) {
        mvprintw(y, x, "%s", prompt); clrtoeol(); echo();
        getnstr(buf, sizeof(buf) - 1); noecho();
        std::stringstream ss(buf);
        if (ss >> val && ss.eof() && val >= minVal && val <= maxVal) return val;
        mvprintw(y + 1, x, "Invalid input; enter integer %d..%d", minVal, maxVal);
        clrtoeol(); refresh(); getch(); mvprintw(y + 1, x, ""); clrtoeol();
    }
}
void showInsuranceScreen() {
    clear(); mvprintw(1, 2, "=== Insurance Premium Calculation ===");
    double sum = promptDouble(3, 2, "Insured sum (e.g. 10000.50): ");
    double rate = promptDouble(4, 2, "Base rate % (e.g. 2.5): ");
    int    age = promptInt(5, 2, "Age (years, e.g. 30): ", 0, 120);
    int    region = promptInt(6, 2, "Region (1-Low,2-Med,3-High): ", 1, 3);
    double ded = promptDouble(7, 2, "Deductible (e.g. 500): ");
    double vat = promptDouble(8, 2, "VAT rate % (e.g. 20): ");

    // calculate…
    InsurancePolicy p{ sum, rate, age < 25 ? 1.2 : (age > 60 ? 1.3 : 1.0),
                           region == 1 ? 0.9 : (region == 3 ? 1.2 : 1.0),
                           ded, vat };
    double b = p.calculateBase();
    double c = p.calculateWithCoeffs();
    double d = p.calculateAfterDeductible();
    double v = p.calculateWithVAT();

    mvprintw(10, 2, "Base premium:        %.2f", b);
    mvprintw(11, 2, "After coefficients: %.2f", c);
    mvprintw(12, 2, "After deductible:   %.2f", d);
    mvprintw(13, 2, "Total with VAT:     %.2f", v);
    mvprintw(15, 2, "Press any key to return to menu..."); refresh(); getch();
}

// — showUtilitiesScreen —
void showUtilitiesScreen(std::vector<Utility>& rates) {
    clear(); mvprintw(1, 2, "=== Utilities Bill Calculation ===");
    std::vector<Utility> entries;
    int y = 3;
    for (auto& r : rates) {
        char prompt[80];
        snprintf(prompt, sizeof(prompt), "%s (units, e.g. 150.00): ", r.name.c_str());
        double c = promptDouble(y++, 2, prompt);
        entries.push_back({ r.name, r.rate, c });
    }
    clear(); mvprintw(1, 2, "=== Results ===");
    double total = 0;
    y = 3;
    for (auto& u : entries) {
        double amt = u.charge();
        total += amt;
        mvprintw(y++, 2, "%s: %.2f", u.name.c_str(), amt);
    }
    mvprintw(y + 1, 2, "Total amount due: %.2f", total);
    logUtilityHistory(entries);
    mvprintw(y + 3, 2, "Press any key to return to menu..."); refresh(); getch();
}

// — showEditRatesScreen —
void showEditRatesScreen(std::vector<Utility>& rates) {
    clear(); mvprintw(1, 2, "=== Edit Utility Rates ===");
    int y = 3;
    for (size_t i = 0; i < rates.size(); ++i)
        mvprintw(y++, 2, "%zu) %s: %.4f", i + 1, rates[i].name.c_str(), rates[i].rate);

    int choice = promptInt(y + 1, 2, "Select # to edit (0 cancel): ", 0, rates.size());
    if (choice > 0) {
        char prompt[80];
        snprintf(prompt, sizeof(prompt), "New rate for %s (e.g. 0.07): ",
            rates[choice - 1].name.c_str());
        rates[choice - 1].rate = promptDouble(y + 3, 2, prompt);
        saveUtilityRates(rates);
    }
}

void showHistoryScreen(const std::vector<Utility>& rates) {
    clear(); mvprintw(1, 2, "=== Utilities Expense History ===");
    std::ifstream fin("utility_history.csv");
    // Header
    std::vector<std::string> header{ "Date" };
    for (const auto& r : rates) header.push_back(r.name);
    header.push_back("Total");
    // Data rows
    std::vector<std::vector<std::string>> rows;
    if (fin && fin.peek() != EOF) {
        std::string line; std::getline(fin, line); // skip file header
        while (std::getline(fin, line)) {
            std::vector<std::string> row; std::stringstream ss(line); std::string cell;
            while (std::getline(ss, cell, ',')) row.push_back(cell);
            if (!row.empty()) rows.push_back(row);
        }
    }
    fin.close();
    // Compute widths
    int cols = header.size();
    std::vector<int> widths(cols);
    for (int i = 0; i < cols; ++i) widths[i] = header[i].size();
    for (auto& row : rows) for (int i = 0; i < (int)row.size() && i < cols; ++i)
        widths[i] = std::max(widths[i], (int)row[i].size());
    // Print header
    int y = 3, x = 2;
    for (int i = 0; i < cols; ++i) { mvprintw(y, x, "%-*s", widths[i], header[i].c_str()); x += widths[i] + 2; } y++;
    // Print rows or placeholder
    if (rows.empty()) {
        mvprintw(y + 1, 2, "<No entries>");
    }
    else {
        for (auto& row : rows) {
            x = 2;
            for (int i = 0; i < cols; ++i) {
                std::string cell = (i < (int)row.size() ? row[i] : "");
                mvprintw(y, x, "%-*s", widths[i], cell.c_str()); x += widths[i] + 2;
            }
            if (++y >= LINES - 2) break;
        }
    }
    mvprintw(LINES - 2, 2, "Press any key to return to menu..."); refresh(); getch();
}

void showClearHistoryScreen(const std::vector<Utility>& rates) {
    clear(); mvprintw(1, 2, "=== Clear Utilities History ===");
    mvprintw(3, 2, "Delete all history? (y/n): "); refresh(); int ch = getch();
    if (ch == 'y' || ch == 'Y') {
        if (clearUtilityHistory(rates)) mvprintw(5, 2, "History cleared successfully.");
        else mvprintw(5, 2, "Failed to clear history.");
    }
    else mvprintw(5, 2, "Operation cancelled.");
    mvprintw(7, 2, "Press any key to return to menu..."); refresh(); getch();
}