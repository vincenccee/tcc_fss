#include "moving_peaks.hpp"

MovingPeaks::MovingPeaks(int dimension){
    
}

MovingPeaks::MovingPeaks(){}
MovingPeaks::~MovingPeaks(){}

double MovingPeaks::getUpperBound(int pos){
	return 0.0;
}

double MovingPeaks::getLowerBound(int pos){
	return 0.0;
}

double MovingPeaks::evaluateFitness(std::vector<double> solution){
	return 0.0;
}

double MovingPeaks::getDimension(){
	return this->dimension;
}

std::vector<double> MovingPeaks::validatePosition(std::vector<double> position){
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

std::string MovingPeaks::getName(){
	return "MOVING_PEAKS";
}

bool MovingPeaks::fitnesIsBetter(double newFit, double oldFit){
	return newFit>oldFit;
}

bool MovingPeaks::isMinimization(){
	return false;
}