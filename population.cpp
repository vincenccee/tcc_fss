#include "population.hpp"

using namespace std;

Population::Population(int tamPopulation, int dimension, double lowerBound, double upperBound, double initialWeight){
  this->tamPopulation = tamPopulation;
  this->dimension = dimension;
  this->lowerBound = lowerBound;
  this->upperBound = upperBound;
  this->initialWeight = initialWeight;
  this->tools = new GeneralTools;
  srand(time(NULL));
}

Population::Population(){}

Population::~Population(){}

void Population::initializePopulation(){
  this->population.clear();
  std::vector<double> position;
  Fish *tmpFish;
  for(int i=0; i< tamPopulation; i++){
    position.clear();
    for(int j=0; j<dimension; j++){
      position.push_back(tools->fRand(lowerBound, upperBound));
    }
    tmpFish = new Fish(this->initialWeight, position);
    this->population.push_back(*tmpFish);
  }
}

void Population::updatePopulationDisplacement(){
  std::vector<double> displacement;
  for(int i=0; i< tamPopulation; i++){
    if(population[i].getImproved()){
      displacement.clear();
      // cout << "displacement: " << i << " - ";
      for(int j=0; j<dimension; j++){
        displacement.push_back(population[i].getCurrentPosition()[j] - population[i].getPreviuosPosition()[j]);
        // cout << " * " << displacement[j];
      }
      // cout << endl;
      population[i].setIndividualDisplacement(displacement);
    }
  }
}

std::vector<Fish> Population::getPopulation(){
  return population;
}

Fish * Population::getFish(int pos){
  return &population[pos];
}

void Population::updateFish(Fish fish, int pos){
  this->population[pos] = fish;
}

int Population::getTamPopulation(){
  return tamPopulation;
}

double Population::getMinWeight(){
  return this->minWeight;
}

double Population::getMaxWeight(){
  return this->maxWeight;
}

void Population::setMinWeight(double minWeight){
  this->minWeight = minWeight;
}

void Population::setMaxWeight(double maxWeight){
  this->maxWeight = maxWeight;
}