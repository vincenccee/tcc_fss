#ifndef _SCHAFEL12_H_
#define _SCHAFEL12_H_
#include <iostream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

class Schafel12: public Problem {
  public:
    Schafel12(int dimension);
    Schafel12();
    ~Schafel12();

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

Schafel12::Schafel12(int dimension){
  this->dimension = dimension;
}

Schafel12::Schafel12() {}
Schafel12::~Schafel12() {}

double Schafel12::getUpperBound(){
  return 32.0;
}

double Schafel12::getLowerBound(){
  return -32.0;
}

double Schafel12::evaluateFitness(std::vector<double> solution){
  double aux = 0;
  double aux1 = 0;
  unsigned short int i;
  for (i = 0; i < this->dimension; i++)
  {
    aux += solution[i]*solution[i];
  }
  for (i = 0; i < this->dimension; i++)
  {
    aux1 += cos(2.0*M_PI*solution[i]);
  }

  aux = -20.0*(exp(-0.2*sqrt(1.0/(float)this->dimension*aux)))-exp(1.0/(float)this->dimension*aux1)+20.0+exp(1);
  return 35 - aux;
}

double Schafel12::getDimension(){
  return this->dimension;
}

std::vector<double> Schafel12::validatePosition(std::vector<double> position){
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

std::string Schafel12::getName(){
  return "Schafel12";
}

bool Schafel12::fitnesIsBetter(double newFit, double oldFit){
  return newFit<oldFit;
}

bool Schafel12::isMinimization(){
  return true;
} 

#endif
