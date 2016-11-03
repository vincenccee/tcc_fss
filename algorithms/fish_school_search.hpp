#ifndef _FISH_SCHOOL_SEARCH_H_
#define _FISH_SCHOOL_SEARCH_H_
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
// #include <omp.h>
#include "../population.hpp"
#include "../fish.hpp"
#include "../benchmarcks/problem.hpp"
#include "../general_tools.hpp"

class FishSchoolSearch {
  private:
    Problem *problem;
    Population *school;
    GeneralTools *tools;
    std::vector<double> bestPosition;
    std::vector<double> bestPopulationFitness;
    std::vector<double> bestIndividualFitness;
    std::vector<double> populationDiversity;
    std::vector<double> finalFitness;
    std::string OUTPUT_DIR;
    std::ofstream popdata;

    double bestFitness;
    double stepIndPercentage;
    double stepIndInit;
    double stepIndFinal;
    double stepVolPercentage;
    double stepVolInit;
    double stepVolFinal;
    double minWeight;
    double maxWeight;
    double m_nmdf;
    int runs;
    int iterations;
    int tamPopulation;

  public:
    FishSchoolSearch(Problem *problem, int tamPopulation);
    FishSchoolSearch();
    ~FishSchoolSearch();

    void showPopulation();
    void evolutionaryCicle(int iterations, int runs);
    void evaluatePopulationFitness(bool first = false);
    void localSearch();
    std::vector<double> createNeighboorPosition(std::vector<double> position);
    std::vector<double> validatePosition(std::vector<double> position);
    void setSchoolNewWeight();
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
    void initializeBest();
    void updateBest(int pos);
    void updateStepPercentage();
    double getBestFitness();
    void updatePlot(int pos);
    double defaultGenotypicDiversityMeasure();
};

#endif