#include "griewank.hpp"

Griewank::Griewank(int dimension){
  this->dimension = dimension;
}

Griewank::Griewank() {}
Griewank::~Griewank() {}

double Griewank::getUpperBound(int pos){
  return 600.0;
}

double Griewank::getLowerBound(int pos){
  return -600.0;
}

double Griewank::evaluateFitness(std::vector<double> solution){
  double fitness = 0;
  double auxSum = 0;
  double auxMul = 1;
  for(unsigned int i = 0 ; i < this->dimension; i++) {
    auxSum += (pow(solution[i], 2) / 4000);
    auxMul *= cos(solution[i] / sqrt(i + 1));
  }
  fitness = 1 + auxSum - auxMul;
  return fitness;
}

double Griewank::getDimension(){
  return this->dimension;
}

std::vector<double> Griewank::validatePosition(std::vector<double> position){
  std::vector<double> newPosition(position);
  for (int i = 0; i < this->dimension; i++) {
    if (position[i] >= getUpperBound(i)) {
      newPosition[i] = getUpperBound(i);
    }
    if (position[i] <= getLowerBound(i)) {
      newPosition[i] = getLowerBound(i);
    }
  }
  return newPosition;
}

std::string Griewank::getName(){
  return "Griewank";
}

bool Griewank::fitnesIsBetter(double newFit, double oldFit){
  return newFit<oldFit;
}

bool Griewank::isMinimization(){
  return true;
}