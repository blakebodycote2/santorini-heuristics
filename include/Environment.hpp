#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "Game.hpp"
#include <iostream>
#include <random>
#include <iomanip>

class Environment 
{
private:

public:
    Environment();

    void simulate(int n, int epochs);
    Solver Crossover(const Solver parent1, const Solver parent2) const;
    Solver Mutate(Solver agent);
    void printAgentSummary(int epoch, Solver agents[], int n);
    void printAgentCoefficients(Solver agents[], int n);
};

#endif // ENVIRONMENT_HPP

