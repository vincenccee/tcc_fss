#include "ackley.hpp"

Ackley::Ackley(int dimension){
  this->dimension = dimension;
}

Ackley::Ackley() {}
Ackley::~Ackley() {}

double Ackley::getUpperBound(int pos){
  return 32.0;
}

double Ackley::getLowerBound(int pos){
  return -32.0;
}

double Ackley::evaluateFitness(std::vector<double> solution){
  double aux = 0;
  double aux1 = 0;
  unsigned short int i;
  for (i = 0; i < this->dimension; i++)
  {
    aux += solution[i]*solution[i];
    aux1 += cos(2.0*M_PI*solution[i]);
  }

  aux = -20.0*(exp(-0.2*sqrt(1.0/(float)this->dimension*aux)))-exp(1.0/(float)this->dimension*aux1)+20.0+exp(1);
  return aux;
}

double Ackley::getDimension(){
  return this->dimension;
}

std::vector<double> Ackley::validatePosition(std::vector<double> position){
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

std::string Ackley::getName(){
  return "Ackley";
}

bool Ackley::fitnesIsBetter(double newFit, double oldFit){
  return newFit<oldFit;
}

bool Ackley::isMinimization(){
  return true;
}