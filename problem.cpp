#include "problem.hpp"

using namespace std;

Problem::Problem(int dimension){
  this->dimension = dimension;
}

Problem::Problem() {}
Problem::~Problem() {}

double Problem::getUpperBound(){
  return 600.0;
}

double Problem::getLowerBound(){
  return -600.0;
}

double Problem::evaluateFitness(std::vector<double> solution){
  double fitness = 0;
  double auxSum = 0;
  double auxMul = 1;
  
  for (unsigned int i = 0; i < this->dimension; i++) {
      auxSum += (pow(solution[i], 2) / 4000);
      auxMul *= cos(solution[i] / sqrt(i + 1));
  }
  
  fitness = 1 + auxSum - auxMul;
  return 451 - fitness;
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
  return "Griewank";
}