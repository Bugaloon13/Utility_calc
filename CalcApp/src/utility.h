// === utility.h ===
#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <vector>

struct Utility {
    std::string name;     // Название услуги
    double rate;          // Тариф (цена за единицу)
    double consumption;   // Объём потребления
    double charge() const; // rate * consumption
};

bool loadUtilityRates(std::vector<Utility>& rates);
bool saveUtilityRates(const std::vector<Utility>& rates);
void logUtilityHistory(const std::vector<Utility>& entries);
// Удалить историю коммунальных платежей (truncate + recreate header)
bool clearUtilityHistory(const std::vector<Utility>& rates);

#endif // UTILITY_H
