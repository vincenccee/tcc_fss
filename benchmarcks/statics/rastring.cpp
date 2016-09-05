#include "rastring.hpp"

Rastring::Rastring(int dimension){
  this->dimension = dimension;
}

Rastring::Rastring() {}
Rastring::~Rastring() {}

double Rastring::getUpperBound(int pos){
  return 5.12;
}

double Rastring::getLowerBound(int pos){
  return -5.12;
}

double Rastring::evaluateFitness(std::vector<double> solution){
  double obj = 0;
  for(unsigned int j = 0 ; j < this->dimension; j++) {
    obj += (pow(solution[j], 2) - 10 * cos(2 * M_PI*solution[j]) + 10);
  }
  return obj;
}

double Rastring::getDimension(){
  return this->dimension;
}

std::vector<double> Rastring::validatePosition(std::vector<double> position){
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

std::string Rastring::getName(){
  return "Rastring";
}

bool Rastring::fitnesIsBetter(double newFit, double oldFit){
  return newFit<oldFit;
}

bool Rastring::isMinimization(){
  return true;
}