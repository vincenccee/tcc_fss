#include "moving_peaks.hpp"

MovingPeaks::MovingPeaks(int dimension, int scenario){
  this->scen = new Scenario(scenario);
  this->count = 0;
  // set initial height for all peaks 
  if(scen->start_height != 0){
    for(int i=0; i<scen->npeaks; i++)
      peaks_height.push_back(scen->start_height);
  }else{
    for(int i=0; i<scen->npeaks; i++)
      peaks_height.push_back(fRand(scen->min_height, scen->max_height));
  }

  // set initial width for all peaks 
  if(scen->start_width != 0){
    for(int i=0; i<scen->npeaks; i++)
      peaks_width.push_back(scen->start_width);
  }else{
    for(int i=0; i<scen->npeaks; i++)
      peaks_width.push_back(fRand(scen->min_width, scen->max_width));
  }

  // set initial position for all peaks
  std::vector<double> tmp_pos;
  for(int i=0; i<scen->npeaks; i++){
    tmp_pos.clear();
    for(int j=0; j<this->dimension; j++){
      tmp_pos.push_back(fRand(scen->min_coord, scen->max_coord));
      peaks_position.push_back(tmp_pos);
    }
  }
}

MovingPeaks::MovingPeaks(){}
MovingPeaks::~MovingPeaks(){}

double MovingPeaks::getUpperBound(int pos){
	return scen->max_coord;
}

double MovingPeaks::getLowerBound(int pos){
	return scen->min_coord;
}

double MovingPeaks::evaluateFitness(std::vector<double> solution){
  std::vector<double> possible_values;
  this->count++;
  for(int i=0; i<scen->npeaks; i++)
    possible_values.push_back(callFunction(solution, peaks_position[i], peaks_height[i], peaks_width[i]));

  if(this->count % scen->period == 0){
    changePeaks();
  }

  return maxValue(possible_values);
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

double MovingPeaks::cone(std::vector<double> individual, std::vector<double> position, double height, double width){
  double value = 0.0;
  for(int i=0; i<this->dimension; i++)
    value += pow(individual[i] - position[i], 2);
  return height - width * sqrt(value);
}

double MovingPeaks::sphere(std::vector<double> individual, std::vector<double> position, double height, double width){
  double value = 0.0;
  for(int i=0; i<this->dimension; i++)
    value += pow(individual[i] - position[i], 2);
  return height  * value;
}

double MovingPeaks::function1(std::vector<double> individual, std::vector<double> position, double height, double width){
  double value = 0.0;
  for(int i=0; i<this->dimension; i++)
    value += pow(individual[i] - position[i], 2);
  return height / (1 + width * value);
}

double MovingPeaks::callFunction(std::vector<double> individual, std::vector<double> position, double height, double width){
  double result;
  switch(scen->pfunc){
    case 1:
      result = cone(individual, position, height, width);
      break;
    case 2:
      result = sphere(individual, position, height, width);
      break;
    case 3:
      result = function1(individual, position, height, width);
      break;
  }
  return result;
}

void MovingPeaks::changePeaks(){

}

double MovingPeaks::maxValue(std::vector<double> values){
  double result = 0.0;
  for(int i=0; i<scen->npeaks; i++){
    if(values[i]>result)
      result = values[i];
  }
  return result;
}

double MovingPeaks::fRand(double fMin, double fMax){
  double f = (double)rand() / RAND_MAX;
  return fMin + f * (fMax - fMin);
}