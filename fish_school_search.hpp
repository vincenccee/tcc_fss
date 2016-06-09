#ifndef _FISH_SCHOOL_SEARCH_H_
#define _FISH_SCHOOL_SEARCH_H_
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include "population.hpp"
#include "problem.hpp"
#include "fish.hpp"

class FishSchoolSearch {
  private:
    Problem problem;
    Population *school;
    std::vector<double> bestPosition;

    double stepIndPercentage;
    double stepIndInit;
    double stepIndFinal;
    double stepVolPercentage;
    double stepVolInit;
    double stepVolFinal;
    double minWeight;
    double maxWeight;
    int iterations;
    int tamPopulation;

  public:
    FishSchoolSearch(Problem problem, int tamPopulation);
    FishSchoolSearch();
    ~FishSchoolSearch();

    void showPopulation();
    void evolutionaryCicle(int iterations);
    void localSearch();
    std::vector<double> createNeighboorPosition(std::vector<double> position);
    std::vector<double> validatePosition(std::vector<double> position);
    void setLocalSchoolNewWeight();
    void setSchoolNewWeight();
    double calculateLocalGreaterFitnessGain();
    double calculateGreaterFitnessGain();
    double calculateFitnessGain(Fish *fish);
    void collectiveMovement();
    std::vector<double> calculateCollectiveMovementSum();
    std::vector<double> schoolFitnessGain();
    double calculateFitnessGainSum();
    void volitiveMovement();
    double weightVariationSignal();
    double euclidianDistance(std::vector<double> a, std::vector<double> b);
    std::vector<double> calculateBarycenter();
    double calculateWeightSum();
    void initializeBestPosition();
    void updateBestPosition();
    void updateStepPercentage();
    double getBestFitness();

    double fRand(double fMin, double fMax);
};

#endif