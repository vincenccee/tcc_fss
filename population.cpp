#include "population.hpp"

Population::Population(int tamPopulation, int generetions){
  this->tamPopulation = tamPopulation;
  this->generetions = generetions;
}

Population::Population(){}

Population::~Population(){}

void Population::initializePipulation(){
  for(unsigned int i=0; i< tamPopulation; i++){
    tmpFish = Fish.new()
    this->population.push_back()
  }
}

void Population::evaluatePopulation(){

}

double Population::getStepInd(){
  return this->stepInd;
}

double Population::getStepVol(){
  return thi->stepVol;
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