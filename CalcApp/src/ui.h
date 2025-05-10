// === ui.h ===
#ifndef UI_H
#define UI_H

#include <vector>
#include "utility.h"
#include "insurance.h"

double promptDouble(int y, int x, const char* prompt);
int    promptInt(int y, int x, const char* prompt, int minVal, int maxVal);

void showInsuranceScreen();
void showUtilitiesScreen(std::vector<Utility>& rates);
void showEditRatesScreen(std::vector<Utility>& rates);
void showHistoryScreen(const std::vector<Utility>& rates);
void showClearHistoryScreen(const std::vector<Utility>& rates);

#endif // UI_H