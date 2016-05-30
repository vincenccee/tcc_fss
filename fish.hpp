#ifndef _FISH_H_
#define _FISH_H_

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

class Fish {
private:
  double weight;
  double weightVariation;
  std::vector<double> currentPosition;
  std::vector<double> previuosPosition;
  std::vector<double> individualDisplacement;
  double fitnessVariation;
  bool improved;


public:
  Fish(double weight, std::vector<double> position);
  Fish();
  ~Fish();

  double getWeight();
  double getWeightVariation();
  std::vector<double> getCurrentPosition();
  std::vector<double> getPreviuosPosition();
  std::vector<double> getIndividualDisplacement();
  double getFitnessVariation();
  bool getImproved();

  void setWeight(double weight);
  void setWeightVariation(double weightVariation);
  void setCurrentPosition(std::vector<double> currentPosition);
  void setPreviuosPosition(std::vector<double> previuosPosition);
  void setIndividualDisplacement(std::vector<double> individualDisplacement);
  void setFitnessVariation(double fitnessVariation);
  void setImproved(bool improved);
};

#endif
