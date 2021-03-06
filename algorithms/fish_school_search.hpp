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

class FishSchoolSearch {
  private:
    Problem *problem;
    Population *school;
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

    double fRand(double fMin, double fMax);

    void updatePlot(int pos);
    void gnu_plot_convergence(std::vector<double> mean_gen, int m_gen, std::string name, std::string title, std::string y_axis, double max_range);
    void gnu_plot_convergence_best_mean(std::vector<double> d_data1, std::vector<double> d_data2, int n_lines, std::string title, std::string filename);
    std::string space2underscore(std::string text);
    double defaultGenotypicDiversityMeasure();
    double standardDeviation(std::vector<double> data);
    double arithmeticAverage(std::vector<double> data);
};

#endif