// === main.cpp ===
#include <clocale>
#include <curses.h>
#include <vector>
#include "utility.h"
#include "insurance.h"
#include "ui.h"

int main() {
    setlocale(LC_ALL, ""); initscr(); cbreak(); noecho(); keypad(stdscr, TRUE);
    std::vector<Utility> rates; loadUtilityRates(rates);
    int ch;
    while (true) {
        clear();
        mvprintw(1, 2, "===== Main Menu =====");
        mvprintw(3, 2, "1) Insurance Premium Calculation");
        mvprintw(4, 2, "2) Utilities Bill Calculation");
        mvprintw(5, 2, "3) Edit Utility Rates");
        mvprintw(6, 2, "4) View Utilities History");
        mvprintw(7, 2, "5) Clear Utilities History");
        mvprintw(8, 2, "6) Exit");
        mvprintw(10, 2, "Select (1-6): "); refresh(); ch = getch();
        switch (ch) {
        case '1': showInsuranceScreen(); break;
        case '2': showUtilitiesScreen(rates); break;
        case '3': showEditRatesScreen(rates); break;
        case '4': showHistoryScreen(rates); break;
        case '5': showClearHistoryScreen(rates); break;
        case '6': goto end;
        default: mvprintw(12, 2, "Invalid choice. Press any key..."); refresh(); getch();
        }
    }
end: endwin(); return 0;
}