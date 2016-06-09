#include "fish.hpp"

using namespace std;

Fish::Fish(double weight, std::vector<double> position){
  this->setWeight(weight);
  this->setCurrentPosition(position);
  this->setImproved(false);
}

Fish::~Fish() {}

double Fish::getWeight(){
  return this->weight;
}

double Fish::getWeightVariation(){
  return this->weightVariation;
}

std::vector<double> Fish::getCurrentPosition(){
  return this->currentPosition;
}

std::vector<double> Fish::getPreviuosPosition(){
  return this->previuosPosition;
}

std::vector<double> Fish::getIndividualDisplacement(){
  return this->individualDisplacement;
}

double Fish::getFitnessVariation(){
  return this->fitnessVariation;
}

bool Fish::getImproved(){
  return this->improved;
}

double Fish::getPosition(int pos){
  return currentPosition[pos];
}

void Fish::setWeight(double weight){
  this->weight = weight;
}

void Fish::setWeightVariation(double weightVariation){
  this->weightVariation = weightVariation;
}

void Fish::setCurrentPosition(std::vector<double> currentPosition){
  this->setPreviuosPosition(getCurrentPosition());
  this->currentPosition.clear();
  this->currentPosition = currentPosition;
}

void Fish::setPreviuosPosition(std::vector<double> previuosPosition){
  this->previuosPosition.clear();
  this->previuosPosition = previuosPosition;
}

void Fish::setIndividualDisplacement(std::vector<double> individualDisplacement){
  this->individualDisplacement.clear();
  this->individualDisplacement = individualDisplacement;
}

void Fish::setFitnessVariation(double fitnessVariation){
  this->fitnessVariation = fitnessVariation;
}

void Fish::setImproved(bool improved){
  this->improved = improved;
}