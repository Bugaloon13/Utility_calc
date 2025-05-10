// === utility.cpp ===
#include "utility.h"
#include <fstream>
#include <sstream>
#include <ctime>

double Utility::charge() const {
    return rate * consumption;
}

bool loadUtilityRates(std::vector<Utility>& rates) {
    std::ifstream fin("utility_rates.txt");
    if (!fin) {
        rates = { {"Electricity", 0.10, 0},
                 {"Water",       0.05, 0},
                 {"Gas",         0.03, 0} };
        saveUtilityRates(rates);
        return true;
    }
    std::string name;
    double r;
    while (fin >> name >> r) {
        rates.push_back({ name, r, 0 });
    }
    return true;
}

bool saveUtilityRates(const std::vector<Utility>& rates) {
    std::ofstream fout("utility_rates.txt");
    if (!fout) return false;
    for (const auto& u : rates) {
        fout << u.name << " " << u.rate << "\n";
    }
    return true;
}

void logUtilityHistory(const std::vector<Utility>& entries) {
    bool newFile = false;
    std::ifstream chk("utility_history.csv");
    if (!chk) newFile = true;
    chk.close();

    std::ofstream hist("utility_history.csv", std::ios::app);
    if (newFile) {
        hist << "Date";
        for (const auto& u : entries) hist << "," << u.name;
        hist << ",Total\n";
    }
    time_t t = time(nullptr);
    struct tm tm;
    localtime_s(&tm, &t);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d", &tm);
    hist << buf;
    double total = 0.0;
    for (const auto& u : entries) {
        double c = u.charge();
        total += c;
        hist << "," << c;
    }
    hist << "," << total << "\n";
}

bool clearUtilityHistory(const std::vector<Utility>& rates) {
    std::ofstream hist("utility_history.csv", std::ios::trunc);
    if (!hist) return false;
    // recreate header
    hist << "Date";
    for (const auto& u : rates) hist << "," << u.name;
    hist << ",Total\n";
    return true;
}