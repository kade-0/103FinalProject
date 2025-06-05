#include "simulation.h"
#include <cmath>
#include <iomanip>

double calculateMonthlyIncome(double annualIncome, int currentYear, bool isEmployed) {
    if (!isEmployed) return 0.0f;
    double adjustedAnnual = annualIncome * pow(1.05f, currentYear-1);
    return adjustedAnnual / 12.0f;
}

double calculateMonthlyMortgage(double principal, double rate, int termYears) {
    double monthlyRate = rate / 12.0f / 100.0f;
    int termMonths = termYears * 12.0f;
    return principal * monthlyRate * pow(1 + monthlyRate, termMonths) / 
           (pow(1 + monthlyRate, termMonths) - 1);
}

void simulate(simulationParams params)
{
    int year = 0;
    int month = 0;

    double balance = 0;

    double price = params.homePrice;
    double downpayment = price * (params.downPayRatio / 100.0f);
    double principal = price - downpayment;
    double term = params.loanLength;

    bool employed = true;

    double monthlyPropertyTax = price * (params.propertyTaxRate / 12.0f / 100.0f);
    double monthlyHOA = params.hoaAnnual / 12.0f;

    double mortgagePayment = calculateMonthlyMortgage(principal, params.mortgageInterest / 100.0f, term);

    while(month < params.simulationDuration*12)
    {
        if (month % 12 == 0)
            year++;
        month++;

        double income = calculateMonthlyIncome(params.preTaxIncome, year, employed);
        balance += income;

        // Home owner
        balance -= mortgagePayment;

        std::cout << std::fixed << std::setprecision(0) << balance << std::endl;
    }
}