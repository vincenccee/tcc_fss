#ifndef _FISH_SCHOOL_SEARCH_2_H_
#define _FISH_SCHOOL_SEARCH_2_H_
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

class FishSchoolSearch2 {
  private:
    Problem *problem;
    Population *school;
    GeneralTools *tools;
    std::vector<double> bestPosition;
    std::vector<double> bestPopulationFitness;
    std::vector<double> bestIndividualFitness;
    std::vector<double> populationDiversity;
    std::vector<double> finalFitness;
    std::ofstream popdata;

    double alpha;
    double beta;
    double gamma;
    double minGamma;
    double maxGamma;
    double bestFitness;
    double minWeight;
    double maxWeight;
    double m_nmdf;
    int prevCountFish;
    int currCountFish;
    int betterNumber;
    int runs;
    int iterations;
    int tamPopulation;

  public:
    FishSchoolSearch2(Problem *problem, int tamPopulation);
    FishSchoolSearch2();
    ~FishSchoolSearch2();

    void showPopulation();
    void evolutionaryCicle(int iterations, int runs);
    void evaluatePopulationFitness(bool first = false);
    void localSearch();
    std::vector<double> createNeighboorPosition(std::vector<double> position);
    std::vector<double> validatePosition(std::vector<double> position);
    void setSchoolNewWeight();
    double calculateFitnessGain(Fish *fish);
    double calculateGreaterFitnessGain();
    void updateFishPosition();
    std::vector<double> individualMoviment(Fish *fish);
    std::vector<double> colletiveMoviment();
    std::vector<double> volitiveMoviment(Fish *fish, std::vector<double> barycenter);
    double weightVariationSignal();
    std::vector<double> calculateBarycenter();
    double calculateWeightSum();
    void initializeBest();
    void updateBest(int pos);
    double getBestFitness();
    void updateGamma();
    void updatePlot(int pos);
    double defaultGenotypicDiversityMeasure();
};

#endif