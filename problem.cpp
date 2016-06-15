#include "problem.hpp"

using namespace std;

Problem::Problem(int dimension){
  this->dimension = dimension;
}

Problem::Problem() {}
Problem::~Problem() {}

double Problem::getUpperBound(){
  return 32.0;
}

double Problem::getLowerBound(){
  return -32.0;
}

double Problem::evaluateFitness(std::vector<double> solution){
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

std::vector<double> Problem::validatePosition(std::vector<double> position){
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

std::string Problem::getName(){
  return "Ackley";
}

bool Problem::fitnesIsBetter(double newFit, double oldFit){
  return newFit<oldFit;
}

bool Problem::isMinimization(){
  return true;
} 