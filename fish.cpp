#include "fish.hpp"

using namespace std;

Fish::Fish(double weight, std::vector<double> position){
  this.setWeight(weight);
  this.setCurrentPosition(position);
}

Fish::Fish() {}

Fish::~Fish() {}

double Fish::getWeight(){
  return this.weight;
}

double Fish::getWeightVariation(){
  return this.weightVariation;
}

std::vector<double> Fish::getCurrentPosition(){
  return this.currentPosition;
}

std::vector<double> Fish::getPreviuosPosition(){
  return this.previuosPosition;
}

std::vector<double> Fish::getIndividualDisplacement(){
  return this.individualDisplacement;
}

double Fish::getFitnessVariation(){
  return this.fitnessVariation;
}

bool Fish::getImproved(){
  return this.improved;
}

void Fish::setWeight(double weight){
  this.weight = weight;
}

void Fish::setWeightVariation(double weightVariation){
  this.weightVariation = weightVariation;
}

void Fish::setCurrentPosition(std::vector<double> currentPosition){
  this.currentPosition = currentPosition;
}

void Fish::setPreviuosPosition(std::vector<double> previuosPosition){
  this.previuosPosition = previuosPosition;
}

void Fish::setIndividualDisplacement(std::vector<double> individualDisplacement){
  this.individualDisplacement = individualDisplacement;
}

void Fish::setFitnessVariation(double fitnessVariation){
  this.fitnessVariation = fitnessVariation;
}

void Fish::setImproved(bool improved){
  this.improved = improved;
}