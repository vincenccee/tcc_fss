#ifndef _PROBLEM_H_
#define _PROBLEM_H_
#include <iostream>
#include <vector>
#include <cmath>
#include <string>

class Problem {
  public:
    double dimension;

    Problem(int dimension);
    Problem();
    ~Problem();

    double getUpperBound();
    double getLowerBound();
    double evaluateFitness(std::vector<double> solution);
    std::vector<double> validatePosition(std::vector<double> position);
    std::string getName();
    bool fitnesIsBetter(double newFit, double oldFit);
    bool isMinimization();
};

#endif
