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

struct Person {
    double bankBalance;
    double homeValue;
    double mortgageBalance;
    double totalEquity;
    double totalPaidOnMortgage;

    double monthlyRent;
    double monthlyMortgage;
    double monthlyPropertyTax;
    double monthlyHOA;

    double preTaxIncome;
    double netIncome;
    double capitalGainsTax;

    bool employed;
    bool homeOwner;
};

void simulate(simulationParams params);