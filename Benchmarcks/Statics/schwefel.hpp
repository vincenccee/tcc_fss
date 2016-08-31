#ifndef _SCHWEFEL_H_
#define _SCHWEFEL_H_
#include <iostream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

class Schwefel: public Problem {
  public:
    Schwefel(int dimension);
    Schwefel();
    ~Schwefel();

    double getUpperBound();
    double getLowerBound();
    double evaluateFitness(std::vector<double> solution);
    double getDimension();
    std::vector<double> validatePosition(std::vector<double> position);
    std::string getName();
    bool fitnesIsBetter(double newFit, double oldFit);
    bool isMinimization();

  private:
    double dimension;
};

Schwefel::Schwefel(int dimension){
  this->dimension = dimension;
}

Schwefel::Schwefel() {}
Schwefel::~Schwefel() {}

double Schwefel::getUpperBound(){
  return 500.0;
}

double Schwefel::getLowerBound(){
  return -500.0;
}

double Schwefel::evaluateFitness(std::vector<double> solution){
  double auxSum = 0;
  for (unsigned i = 0; i < this->dimension; i++)
  {
    auxSum += solution[i] * sin(sqrt(fabs(solution[i])));
  }
  return (418.9829 * this->dimension) - auxSum;
}

double Schwefel::getDimension(){
  return this->dimension;
}

std::vector<double> Schwefel::validatePosition(std::vector<double> position){
  std::vector<double> newPosition(position);
  for (int i = 0; i < this->dimension; i++) {
    if (position[i] >= getUpperBound()) {
      newPosition[i] = getUpperBound();
    }
    if (position[i] <= getLowerBound()) {
      newPosition[i] = getLowerBound();
    }
  }
  return newPosition;
}

std::string Schwefel::getName(){
  return "Schwefel";
}

bool Schwefel::fitnesIsBetter(double newFit, double oldFit){
  return newFit<oldFit;
}

bool Schwefel::isMinimization(){
  return true;
} 

#endif
