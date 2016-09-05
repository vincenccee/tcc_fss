#ifndef _MOVING_PEAKS_H_
#define _MOVING_PEAKS_H_
#include <iostream>
#include <vector>
#include <cmath>
#include <string>

// Source: https://github.com/DEAP/deap/blob/master/deap/benchmarks/movingpeaks.py
#include "../problem.hpp"

using namespace std;

class MovingPeaks: public Problem {
  public:
    MovingPeaks(int dimension);
    MovingPeaks();
    ~MovingPeaks();

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
