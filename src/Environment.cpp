//Environment.cpp
#include "Environment.hpp"

Environment::Environment()
{

}

void Environment::simulate(int n, int epochs)
{
    // heading
    std::cout << "epoch";
    for (int weight = 0; weight < 9; weight++)
        std::cout << ",mean_" << weight << "," << "med_" << weight << ",sd_" << weight;
    std::cout << "\n";


    if (n % 4 != 0 )
    {
        std::cout << "ERROR: N MUST BE A MULTIPLE OF 4" << std::endl;
        return;
    }

    // Define random
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> parentIndex(0, (n / 4) - 1); // for choosing random parents for mutation
    std::uniform_real_distribution<double> prob(0.0, 1.0); // for mutation

    // initialise agents
    Solver agents[n];
    for (int i = 0; i < n; i++) {
        agents[i] = Solver();
        // set the agent's weights
        agents[i].generateRandomWeights();
    }
    printAgentSummary(0, agents, n);
    
    Solver bestAgents[n / 4];
    for (int epoch = 1; epoch <= epochs; epoch++)
    {
        // Play tournament bracket
        for (int i = 0; i < (n / 4); i++)
        {
            int groupIndex = i * 4;

            Game semi1 = Game(agents[groupIndex], agents[groupIndex + 1]);
            Game semi2 = Game(agents[groupIndex + 2], agents[groupIndex + 3]);
            Game finals = Game(semi1.play(), semi2.play());
            Solver bestAgent = finals.play();

            bestAgents[i] = bestAgent;
            // std::cout << (i + 1) << "/" << (n / 4) << std::endl;
        }

        if (epoch % 10 == 0)
            printAgentSummary(epoch, bestAgents, n / 4);
        
        if (epoch == epochs)
            printAgentCoefficients(bestAgents, n / 4);

        // Keep best agents
        for (int i = 0; i < (n / 4); i++)
        {
            agents[i] = bestAgents[i];
        }

        // Crossover
        for (int i = 25; i < n; i++)
        {
            int parent1, parent2;
            parent1 = parentIndex(gen);
            
            do parent2 = parentIndex(gen);
            while (parent2 == parent1); // repeat if the parents have the same index.

            agents[i] = Crossover(bestAgents[parent1], bestAgents[parent2]);
        }
        
        // Mutation
        for (int i = 0; i < n; i++)
            if (prob(gen) < 0.1)
                agents[i] = Mutate(agents[i]);

        // Shuffle
        std::shuffle(agents, agents + 100, gen);
    }
}

Solver Environment::Crossover(const Solver parent1, const Solver parent2) const
{
    // Random number generation setup
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> choose_parent(0, 1); // Choose between 0 (parent1) and 1 (parent2)

    Solver child;

    // Uniform crossover for the child's weights.
    double weight;
    for (int i = 0; i < 9; ++i)
    {
        if (choose_parent(gen) == 0)
            weight = parent1.getWeight(i);
        else
            weight = parent2.getWeight(i);
        child.setWeight(i, weight);
    }

    return child;
}

Solver Environment::Mutate(Solver agent)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> prob(0.0, 1.0);
    std::normal_distribution<double> mutation(-5, 10);

    double weight;
    for (int i = 0; i < 9; ++i)
    {
        if (prob(gen) < 0.5) 
        {
            // Mutate weight
            weight = agent.getWeight(i) + mutation(gen);
            weight = std::min(99.999, std::max(0.0, weight)); //  Clamp [0, 100)
            agent.setWeight(i, weight);
        }
    }

    return agent;
}

void Environment::printAgentSummary(int epoch, Solver agents[], int n)
{  
    std::cout << epoch;
    for (int weight = 0; weight < 9; weight++)
    {
        double values[n];
        double sum = 0.0;
        double sumSq = 0.0;
        int nZeros = 0;
        for (int i = 0; i < n; i++)
        {
            double w = agents[i].getWeight(weight);
            values[i] = w;
            sum += w;
            sumSq += w * w;
            if (w == 0)
                nZeros++;
        }

        double mean = sum / n;
        std::sort(values, values + n);
        double median = values[n / 2];
        double variance = (sumSq / n) - (mean * mean);
        double stddev = std::sqrt(variance);
        
        std::cout << std::fixed << std::setprecision(3);
        std::cout << "," << mean << "," << median << "," << stddev;
        std::cout << std::defaultfloat;
    }
    std::cout << std::endl;
}

void Environment::printAgentCoefficients(Solver agents[], int n)
{
    std::cout << std::fixed << std::setprecision(1);
    for (int i = 0; i < n; i++)
    {
        std::cout << (i+1);
        for (int weight = 0; weight < 9; weight++)
        {
            std::cout << "," << agents[i].getWeight(weight);
        }
        std::cout << std::endl;
    }
    std::cout << std::defaultfloat;
}
