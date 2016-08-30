#ifndef _PROBLEM_H_
#define _PROBLEM_H_
#include <iostream>
#include <vector>
#include <cmath>
#include <string>

class Problem {
  public:
    virtual double getUpperBound() = 0;
    virtual double getLowerBound() = 0;
    virtual double evaluateFitness(std::vector<double> solution) = 0;
    virtual double getDimension() = 0;
    virtual std::vector<double> validatePosition(std::vector<double> position) = 0;
    virtual std::string getName() = 0;
    virtual bool fitnesIsBetter(double newFit, double oldFit) = 0;
    virtual bool isMinimization() = 0;
};

#endif
