#include <iostream>

struct simulationParams
{
    float preTaxIncome;
    float homePrice;
    float loanLength;
    float hoaAnnual;
    float startingRent;

    float downPayRatio;
    float mortgageInterest;
    float propertyTaxRate;
    float purchaseSaleTax;
    float appreciationRate;
    float rentInflation;
    float etfAnnual;

    float simulationDuration;

};

void simulate(simulationParams params);