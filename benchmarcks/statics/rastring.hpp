#ifndef _RASTRING_H_
#define _RASTRING_H_
#include <iostream>
#include <vector>
#include <cmath>
#include <string>

#include "../problem.hpp"

using namespace std;

class Rastring: public Problem {
  public:
    Rastring(int dimension);
    Rastring();
    ~Rastring();

    double getUpperBound(int pos);
    double getLowerBound(int pos);
    double evaluateFitness(std::vector<double> solution);
    double getDimension();
    std::vector<double> validatePosition(std::vector<double> position);
    std::string getName();
    bool fitnesIsBetter(double newFit, double oldFit);
    bool isMinimization();

  private:
    double dimension;
}; 

#endif