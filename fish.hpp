#ifndef _FISH_H_
#define _FISH_H_
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class Fish {
  private:
    double weight;
    double weightVariation;
    std::vector<double> currentPosition;
    std::vector<double> previuosPosition;
    std::vector<double> individualDisplacement;
    double fitness;
    double prevFitness;
    double fitnessVariation;
    bool improved;

  public:
    Fish(double weight, std::vector<double> position);
    ~Fish();

    double getWeight();
    double getWeightVariation();
    std::vector<double> getCurrentPosition();
    std::vector<double> getPreviuosPosition();
    std::vector<double> getIndividualDisplacement();
    double getFitnessVariation();
    double getFitness();
    double getPrevFitness();
    bool getImproved();
    double getPosition(int pos);

    void setWeight(double weight);
    void setWeightVariation(double weightVariation);
    void setCurrentPosition(std::vector<double> currentPosition);
    void setPreviuosPosition(std::vector<double> previuosPosition);
    void setIndividualDisplacement(std::vector<double> individualDisplacement);
    void setFitnessVariation(double fitnessVariation);
    void setFitness(double fitness);
    void setPrevFitness(double prevFitness);
    void setImproved(bool improved);
};

#endif
