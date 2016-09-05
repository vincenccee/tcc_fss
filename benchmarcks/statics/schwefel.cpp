#include "schwefel.hpp"

Schwefel::Schwefel(int dimension){
  this->dimension = dimension;
}

Schwefel::Schwefel() {}
Schwefel::~Schwefel() {}

double Schwefel::getUpperBound(){
  return 100.0;
}

double Schwefel::getLowerBound(){
  return -100.0;
}

double Schwefel::evaluateFitness(std::vector<double> solution){
  double fitness = 0;
  double auxFit = 0;

  for (unsigned int i = 0; i < this->dimension; i++) {
    for (unsigned int j = 0; j <= i; j++) {
      auxFit += solution[j];
    }
    fitness += (auxFit * auxFit);
    auxFit = 0;
  }
  return fitness;
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