// === insurance.h ===
#ifndef INSURANCE_H
#define INSURANCE_H

struct InsurancePolicy {
    double insuredSum;
    double premiumRate;
    double ageCoef;
    double regionCoef;
    double deductible;
    double vatRate;

    double calculateBase() const;
    double calculateWithCoeffs() const;
    double calculateAfterDeductible() const;
    double calculateWithVAT() const;
};

#endif // INSURANCE_H