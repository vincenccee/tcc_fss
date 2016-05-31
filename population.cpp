#include "population.hpp"

using namespace std;

Population::Population(int tamPopulation, int dimension, double lowerBound, double upperBound){
  this->tamPopulation = tamPopulation;
  this->dimension = dimension;
  this->lowerBound = lowerBound;
  this->upperBound = upperBound;
  srand(time(NULL));
}

Population::Population(){}

Population::~Population(){}

void Population::showPopulation(){
  for(unsigned int i=0; i<tamPopulation; i++){
    cout << "ind " << i << "\t";
    for(unsigned int j=0; j<dimension; j++){
      cout << population[i].getPosition(j) << " * ";
    }
    cout << endl;
  }
}

void Population::initializePipulation(){
  this->population.clear();
  std::vector<double> position;
  Fish *tmpFish;
  for(unsigned int i=0; i< tamPopulation; i++){
    position.clear();
    for(unsigned int j=0; j<dimension; j++){
      position.push_back(fRand(lowerBound, upperBound));
    }
    tmpFish = new Fish(2500, position);
    this->population.push_back(*tmpFish);
  }
}

void Population::evaluatePopulation(){

}

double Population::getStepInd(){
  return this->stepInd;
}

double Population::getStepVol(){
  return this->stepVol;
}

double Population::getMinWeight(){
  return this->minWeight;
}

double Population::getMaxWeight(){
  return this->maxWeight;
}

void Population::setStepInd(double stepInd){
  this->stepInd = stepInd;
}

void Population::setStepVol(double stepVol){
  this->stepVol = stepVol;
}

void Population::setMinWeight(double minWeight){
  this->minWeight = minWeight;
}

void Population::setMaxWeight(double maxWeight){
  this->maxWeight = maxWeight;
}

double Population::fRand(double fMin, double fMax){
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}