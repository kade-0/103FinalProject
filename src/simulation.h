/*
    * simulation.h
    * This header file defines the structures and function prototypes for the home ownership simulation.
    * It includes the Person structure to represent an individual's financial state and the simulationParams
    * structure to hold parameters for the simulation.
    * 
    * Contributors: Kade Miller
*/

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

    bool homeOwner;

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

    double etfBalance;

    bool employed;
    bool homeOwner;
};

void simulate(simulationParams params);