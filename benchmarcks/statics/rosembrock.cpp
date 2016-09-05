#include "rosembrock.hpp"

Rosembrock::Rosembrock(int dimension){
  this->dimension = dimension;
}

Rosembrock::Rosembrock() {}
Rosembrock::~Rosembrock() {}

double Rosembrock::getUpperBound(){
  return 30.0;
}

double Rosembrock::getLowerBound(){
  return -30.0;
}

double Rosembrock::evaluateFitness(std::vector<double> solution){
  double auxSum = 0;
  double aux = 0;
  for (unsigned int i = 0; i < this->dimension; i++)
  {
    aux = 100*(pow((solution[i+1] - pow(solution[1], 2)), 2));
    aux += pow(1.0 - solution[i], 2);
    auxSum += aux;
  }
  return auxSum;
}

double Rosembrock::getDimension(){
  return this->dimension;
}

std::vector<double> Rosembrock::validatePosition(std::vector<double> position){
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

std::string Rosembrock::getName(){
  return "Rosembrock";
}

bool Rosembrock::fitnesIsBetter(double newFit, double oldFit){
  return newFit<oldFit;
}

bool Rosembrock::isMinimization(){
  return true;
}