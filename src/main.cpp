/*
    * main.cpp
    * This source file contains the main function and user interaction logic for the home ownership simulation.
    * It includes parameter setup, user input handling, and the simulation execution.
    *
    * Contributors: Kade Miller, Alex Janigan, Eli Brunner, Camron Smith
*/


#include "simulation.h"
#include <map>
#include <string>
#include "error-func.h"

void prettyParams(std::map<std::string, double*> args)
{
    std::cout << "--------------------------------" << std::endl;
    for(const auto& [key, value] : args)
        std::cout << "[" << key << "]       - " << *value << std::endl;
    std::cout << "--------------------------------" << std::endl;
}

void readArgs(simulationParams* params)
{
    std::map<std::string, double*> args = std::map<std::string, double*>();
    
    args["Pre-Tax income"] = &params->preTaxIncome;
    args["Home price"] = &params->homePrice;
    args["Loan length"] = &params->loanLength;
    args["HOA Annual"] = &params->hoaAnnual;
    args["Starting Rent"] = &params->startingRent;
    args["Downpayment Ratio"] = &params->downPayRatio;
    args["Mortgage Interest"] = &params->mortgageInterest;
    args["Property Tax Rate"] = &params->propertyTaxRate;
    args["Appreciation Rate"] = &params->appreciationRate;
    args["Rent Inflation"] = &params->rentInflation;
    args["ETF Annual"] = &params->etfAnnual;
    args["Simulation Duration"] = &params->simulationDuration;

    for(const auto& [key, value] : args)
    {
        std::cout << "Please input a value for " << key << std::endl;
        std::string x = "";
        while(x.length() <= 0)
        {
            std::getline(std::cin, x);
            if (!is_float(x))
            {
                std::cout << "That is not an acceptable input." << std::endl;
                x = "";
            }
        }

        *args[key] = std::stod(x);
    }

    prettyParams(args);
}

void defaultParams(simulationParams* params)
{
    params->preTaxIncome = 90000;
    params->homePrice = 600000;
    params->downPayRatio = 20;
    params->loanLength = 30;
    params->mortgageInterest = 5;
    params->propertyTaxRate = 1.2f;
    params->hoaAnnual = 3000;
    params->appreciationRate = 4;
    params->purchaseSaleTax = 1.5f;
    params->propertyTaxRate = 9;
    params->startingRent = 2000;
    params->rentInflation = 2;
    params->etfAnnual = 7;
    params->simulationDuration = 30;
}

int main()
{
    simulationParams params;

    std::cout << "Do you want to set params, or take default.\n[1] - Default. [2] - Custom" << std::endl;

    std::string def = "";
    std::getline(std::cin, def);

    if (def == "2")
    {
        readArgs(&params);
        std::cout << "Is this okay?\n[1] - Yes. [2] - No" << std::endl;
        std::string cont = "";
        std::getline(std::cin, cont);
    
        if (cont == "2")
            return main();
    }
    else
        defaultParams(&params);

    // Do you want to be a homeowner?
    std::cout << "Do you want to be a homeowner?\n[1] - Yes. [2] - No" << std::endl;
    std::string home = "";
    std::getline(std::cin, home);
    params.homeOwner = (home == "1");

    // Override simulation duration length
    if (def == "1")
    {
        std::cout << "Override simulation duration length (current: " << params.simulationDuration << " years): ";
        std::string duration;
        while (duration.length() <= 0)
        {
            std::cout << "\nEnter a new duration in years: ";
            std::getline(std::cin, duration);
            if (is_float(duration))
            {
                params.simulationDuration = std::stof(duration);
            }
            else
            {
                std::cout << "That is not an acceptable input." << std::endl;
                duration = "";
            }
        }
    }

    simulate(params);

    // Ask if the user wants to run another simulation
    std::cout << "Do you want to run another simulation?\n[1] - Yes. [2] - No" << std::endl;
    std::string again = "";
    std::getline(std::cin, again);
    if (again == "1")
        return main();

    return 0;
}
