#include "simulation.h"
#include <cmath>
#include <iomanip>
#include "error-func.h"

double calculateMonthlyIncome(double annualIncome, int currentYear, bool isEmployed) {
    if (!isEmployed) return 0.0;
    // 5% salary growth
    double adjustedAnnual = annualIncome * pow(1.05, currentYear-1);
    return adjustedAnnual / 12.0;
}

double calculateMonthlyMortgage(double principal, double rate, int termYears) {
    double monthlyRate = rate / 12.0 / 100.0;
    int termMonths = termYears * 12.0;
    return principal * monthlyRate * pow(1 + monthlyRate, termMonths) / 
           (pow(1 + monthlyRate, termMonths) - 1);
}

void simulate(simulationParams params)
{
    int year = 0;
    int month = 0;

    Person p;
    p.bankBalance = 0;
    p.employed = true;

    double price = params.homePrice;
    double downpayment = price * (params.downPayRatio / 100.0);
    double principal = price - downpayment;
    double term = params.loanLength;

    double monthlyPropertyTax = price * (params.propertyTaxRate / 12.0f / 100.0);
    double monthlyHOA = params.hoaAnnual / 12.0;

    double mortgagePayment = calculateMonthlyMortgage(principal, params.mortgageInterest / 100.0f, term);

    p.homeValue = price;
    p.mortgageBalance = price - downpayment;
    p.totalEquity = p.homeValue - p.mortgageBalance;
    p.totalPaidOnMortgage = principal; // add tax to this too later

    p.monthlyRent = 0; // set later for non-homeowners
    p.monthlyMortgage = mortgagePayment;
    p.monthlyPropertyTax = monthlyPropertyTax;
    p.monthlyHOA = monthlyHOA;

    p.preTaxIncome = params.preTaxIncome;
    p.netIncome = 0; // set later for post tax
    p.capitalGainsTax = 0; // set later for tax stuff

    while(month < params.simulationDuration*12)
    {
        if (month % 12 == 0)
            year++;
        month++;

        double income = calculateMonthlyIncome(p.preTaxIncome, year, p.employed);
        p.bankBalance += income;

        // Home owner
        p.bankBalance -= p.monthlyMortgage;
        p.bankBalance -= p.monthlyPropertyTax;
        p.bankBalance -= p.monthlyHOA;

        std::cout << std::fixed << std::setprecision(0) << p.bankBalance << std::endl;
    }
}