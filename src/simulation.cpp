/*
    * simulation.cpp
    * This source file implements the functions and logic for the home ownership simulation.
    * It includes the simulation loop, financial calculations, and user interactions.
    * 
    * Contributors: Kade Miller, Eli Briggs
*/

#include "simulation.h"
#include <cmath>
#include <iomanip>
#include "error-func.h"

void clearConsole() {
    //G++ platform specific console clear
#ifdef _WIN32
    system("cls");
#else
    // POSIX compliant console clear
    system("clear");
#endif
}

double calculateMonthlyIncome(double annualIncome, int currentYear, bool isEmployed) {
    if (!isEmployed) return 0.0;
    return annualIncome / 12.0;
}

double calculateMonthlyMortgage(double principal, double rate, int termYears) {
    double monthlyRate = rate / 12.0 / 100.0;
    int termMonths = termYears * 12.0;
    return principal * monthlyRate * pow(1 + monthlyRate, termMonths) / 
           (pow(1 + monthlyRate, termMonths) - 1);
}

// random unemployment chance
bool didWeBetItAllOnBlack()
{
    // 10% chance of unemployment
    return (rand() % 100) < 10;
}

bool getAJob()
{
    // 1d6 every week (so 4 times a month)
    for (int i = 0; i < 4; i++) {
        if ((rand() % 6) + 1 >= 6) { // 4,5,6 succeeds
            return true;
        }
    }
    return false;
}

double getETFReturn(double annualReturn) {
    double fluctuation = (rand() % 400 - 200) / 10000.0; // -2% to +2%
    double monthlyRate = pow(1.0 + (annualReturn/100.0 + fluctuation), 1.0/12.0) - 1.0;
    return monthlyRate;
}

void updateETFBalance(double& etfBalance) {
    double previousBalance = etfBalance;
    double growthRate = getETFReturn(7.0); // 7% base annual return
    etfBalance *= (1.0 + growthRate);
    
    double difference = etfBalance - previousBalance;
    if (difference > 0) {
        std::cout << "ETF balance increased by $" << difference << std::endl;
    } else if (difference < 0) {
        std::cout << "ETF balance decreased by $" << -difference << std::endl;
    }
}

// player choice to invest in EFT
void investInETF(double& bankBalance, double& etfBalance) {
    std::string amount;
    std::cout << "Do you want to invest in ETF? (Current ETF balance: $" << etfBalance << ", Current bank balance: $" << bankBalance << ")\n"
              << "Enter amount to invest (or '0' to skip): ";
    while(amount.length() <= 0) {
        std::getline(std::cin, amount);
        if (!is_float(amount)) {
            std::cout << "That is not an acceptable input." << std::endl;
            amount = "";
        }
    }

    double investAmount = std::stod(amount);
    if (investAmount > 0 && investAmount <= bankBalance) {
        bankBalance -= investAmount;
        etfBalance += investAmount;
        std::cout << "Invested $" << investAmount << " in ETF. New ETF balance: $" << etfBalance << std::endl;
    } else if (investAmount > bankBalance) {
        std::cout << "Insufficient funds to invest that amount." << std::endl;
    } else {
        std::cout << "No investment made." << std::endl;
    }
}

void sellETF(double& bankBalance, double& etfBalance) {
    std::string amount;
    std::cout << "Do you want to sell ETF? (Current ETF balance: $" << etfBalance << ", Current bank balance: $" << bankBalance << ")\n"
              << "Enter amount to sell (or '0' to skip): ";
    while(amount.length() <= 0) {
        std::getline(std::cin, amount);
        if (!is_float(amount)) {
            std::cout << "That is not an acceptable input." << std::endl;
            amount = "";
        }
    }

    double sellAmount = std::stod(amount);
    if (sellAmount > 0 && sellAmount <= etfBalance) {
        // Transaction fee of 0.5% on the sell amount
        double transactionFee = sellAmount * 0.005;
        etfBalance -= sellAmount;
        bankBalance += sellAmount - transactionFee;
        std::cout << "Sold $" << sellAmount << " of ETF. New bank balance: $" << bankBalance << std::endl;
    } else if (sellAmount > etfBalance) {
        std::cout << "Insufficient ETF balance to sell that amount." << std::endl;
    } else {
        std::cout << "No sale made." << std::endl;
    }
}

bool attemptHomeSale() {
    return (rand() % 6) + 1 > 3; // 4,5,6 succeeds
}

double calculateCapitalGainsTax(double purchasePrice, double salePrice) {
    double gain = salePrice - purchasePrice;
    if (gain > 250000) {
        return gain * 0.09; // 9% tax
    }
    return 0.0;
}

void simulate(simulationParams params)
{
    int year = 0;
    int month = 0;

    Person states[(int)params.simulationDuration + 1]; // +1 for initial state

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
    p.capitalGainsTax = 0; // set later if home is sold

    std::cout << std::fixed << std::setprecision(2); // Set precision for monetary values
    srand(time(0)); // Seed the random number generator
    p.etfBalance = 0; // Initial ETF balance
    p.homeOwner = params.homeOwner;

    while(month < params.simulationDuration*12)
    {
        std::cout << "Year: " << year + 1 << ", Month: " << (month % 12) + 1 << std::endl;

        updateETFBalance(p.etfBalance);
        if (p.bankBalance > 0) {
            investInETF(p.bankBalance, p.etfBalance);
        }
        else {
            std::cout << "Insufficient funds to invest in ETF." << std::endl;
        }

        // If not a homeowner, calculate rent and update bank balance
        if (!p.homeOwner) {
            p.monthlyRent = params.startingRent * pow(1 + params.rentInflation / 100.0, year);
            p.bankBalance -= p.monthlyRent;
        } else {
            p.monthlyRent = 0; // No rent if homeowner
        }

        // Calculate monthly income
        p.netIncome = calculateMonthlyIncome(p.preTaxIncome, year, p.employed);
        if (p.employed) {
            p.bankBalance += p.netIncome;
        } else {
            // If unemployed, we lose money
            p.bankBalance -= p.monthlyMortgage + p.monthlyPropertyTax + p.monthlyHOA;
        }

        // Update home value based on appreciation
        float fluctuation = (rand() % 400 - 200) / 10000.0; // -5% to +5%
        p.homeValue *= (1 + params.appreciationRate / 100.0 / 12.0 + fluctuation);

        // If homeowner, pay mortgage and property tax
        if (p.homeOwner) {
            p.bankBalance -= p.monthlyMortgage + p.monthlyPropertyTax + p.monthlyHOA;

            double interest = p.mortgageBalance * (params.mortgageInterest / 100.0);
            double principalPayment = p.monthlyMortgage - interest;
            p.mortgageBalance -= principalPayment;
            p.totalPaidOnMortgage += principalPayment + p.monthlyPropertyTax + p.monthlyHOA;

            p.totalEquity = p.homeValue - p.mortgageBalance;
        } else {
            // If not a homeowner, update bank balance with rent
            p.bankBalance -= p.monthlyRent;
        }

        // If unemployed, try to get a job
        if (!p.employed) {
            if (getAJob()) {
                std::cout << "Got a job!" << std::endl;
                p.employed = true;
            }
        }

        // Check for unemployment
        if (didWeBetItAllOnBlack()) {
            std::cout << "Unemployment occurred!" << std::endl;
            p.employed = false;
        }

        // Sell ETF if bank balance is low
        if (p.bankBalance < 0 && p.etfBalance >= -p.bankBalance) {
            sellETF(p.bankBalance, p.etfBalance);
        }

        // Check if we want to sell the home
        if (p.homeOwner && p.bankBalance <= 0) {
            if (attemptHomeSale()) {
                double salePrice = p.homeValue;
                double tax = calculateCapitalGainsTax(p.homeValue, salePrice);
                p.bankBalance += (salePrice - tax);
                p.homeValue = 0;
                p.mortgageBalance = 0;
                p.totalEquity = 0;
                p.totalPaidOnMortgage = 0;
                std::cout << "Home sold for $" << salePrice << " with $" << tax << " in taxes." << std::endl;
            }
        }

        if (!p.homeOwner) {
            // No equity or mortgage calculations for renters
            p.mortgageBalance = 0;
            p.totalEquity = 0;
            p.totalPaidOnMortgage = 0;
        }
        states[year] = p; // Save state for this month

        // Check for bankruptcy
        if (p.bankBalance < 0 && !p.employed) {
            std::cout << "Bankruptcy occurred!" << std::endl;
            break;
        }
        if (year > 0) {
            p.totalIncome = (p.bankBalance - states[year -1].bankBalance); // Calculate total income for the year
        }

        // Increment month and check if we need to move to the next year
        month++;
        if (month % 12 == 0) {
            clearConsole();
            year++;
            p.preTaxIncome *= 1.05; // Assume a 5% salary increase each year
        }
    }

    std::cout << "--- Simulation ended ---" << std::endl;

    // Final state after simulation
    states[year] = p;

    double absoluteIncome = 0;
    for (int i = 0; i <= year; i++) {
        absoluteIncome += states[i].totalIncome;
    }

    // Print each year's state
    for (int i = 0; i <= year; i++) {
        double netWorth = states[i].bankBalance - states[i].mortgageBalance + states[i].etfBalance;
        if (states[i].homeOwner) {
            netWorth += states[i].homeValue;
        }
        std::cout << "Year " << i + 1 << ":"
                  << " Bank Balance: $" << states[i].bankBalance << ", "
                  << " Home Value: $" << states[i].homeValue << ", "
                  << " Total Equity: $" << states[i].totalEquity << ", "
                  << " Total Paid on Mortgage: $" << states[i].totalPaidOnMortgage << ", "
                  << " Pre-Tax Income: $" << states[i].preTaxIncome << ", "
                  << " Net Income: $" << states[i].totalIncome
                  << " Net Worth: $" << netWorth
                  << std::endl;
    }

    std::cout << "Final Assets:" << std::endl;
    std::cout << "Bank Balance: $" << p.bankBalance << std::endl;
    std::cout << "ETF Balance: $" << p.etfBalance << std::endl;
    if (p.homeOwner) {
        float netWorth = p.bankBalance + p.homeValue - p.mortgageBalance + p.etfBalance;
        std::cout << "Home Value: $" << p.homeValue << std::endl;
        std::cout << "Mortgage Balance: $" << p.mortgageBalance << std::endl;
        std::cout << "Total Equity: $" << p.totalEquity << std::endl;
        std::cout << "Total Paid on Mortgage: $" << p.totalPaidOnMortgage << std::endl;
        std::cout << "Net Worth: $" << netWorth << std::endl;
    }
    else
    {
        float netWorth = p.bankBalance + p.etfBalance;
        std::cout << "Net Worth: $" << netWorth << std::endl;
    }
    std::cout << "Total Income over simulation: $" << absoluteIncome << std::endl;
    if (year < params.simulationDuration)
    {
        float yearFraction = (float)(month % 12) / 12.0f;
        std::cout << "Simulation Duration: " << yearFraction << "/" << params.simulationDuration << " years" << std::endl;
    }
    else
        std::cout << "Simulation Duration: " << params.simulationDuration << " years" << std::endl;

    std::cout << "--------------------------------" << std::endl;
    std::cout << "Simulation completed successfully!" << std::endl;
    std::cout << "--------------------------------" << std::endl;
}
