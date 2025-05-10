// === insurance.cpp ===
#include "insurance.h"

double InsurancePolicy::calculateBase() const {
    return insuredSum * (premiumRate / 100.0);
}

double InsurancePolicy::calculateWithCoeffs() const {
    return calculateBase() * ageCoef * regionCoef;
}

double InsurancePolicy::calculateAfterDeductible() const {
    double v = calculateWithCoeffs() - deductible;
    return v > 0.0 ? v : 0.0;
}

double InsurancePolicy::calculateWithVAT() const {
    return calculateAfterDeductible() * (1.0 + vatRate / 100.0);
}