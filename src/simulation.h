#include <iostream>

struct simulationParams
{
    double preTaxIncome;
    double homePrice;
    double loanLength;
    double hoaAnnual;
    double startingRent;

    double downPayRatio;
    double mortgageInterest;
    double propertyTaxRate;
    double purchaseSaleTax;
    double appreciationRate;
    double rentInflation;
    double etfAnnual;

    double simulationDuration;

};

void simulate(simulationParams params);