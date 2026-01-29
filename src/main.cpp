#include <iostream>
#include <chrono>
#include "Environment.hpp"


int main() 
{
    Solver p0;
    Environment env;

    auto start = std::chrono::high_resolution_clock::now();
    env.simulate(100, 200);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    std::cout << "Time : " << duration.count() << " seconds " << std::endl;

    return 0;
}
