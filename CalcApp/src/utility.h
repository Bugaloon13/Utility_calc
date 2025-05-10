// === utility.h ===
#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <vector>

struct Utility {
    std::string name;     // �������� ������
    double rate;          // ����� (���� �� �������)
    double consumption;   // ����� �����������
    double charge() const; // rate * consumption
};

bool loadUtilityRates(std::vector<Utility>& rates);
bool saveUtilityRates(const std::vector<Utility>& rates);
void logUtilityHistory(const std::vector<Utility>& entries);
// ������� ������� ������������ �������� (truncate + recreate header)
bool clearUtilityHistory(const std::vector<Utility>& rates);

#endif // UTILITY_H
