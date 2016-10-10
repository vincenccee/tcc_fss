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

class FishSchoolSearch2 {
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

    double alpha;
    double beta;
    double gamma;
    double min_gamma;
    double max_gamma;
    double bestFitness;
    double minWeight;
    double maxWeight;
    double m_nmdf;
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
    void updateFishPosition();
    std::vector<double> individualMoviment(Fish *fish);
    std::vector<double> colletiveMoviment();
    std::vector<double> volitiveMoviment(Fish *fish);
    double weightVariationSignal();
    std::vector<double> calculateBarycenter();
    double calculateWeightSum();
    void initializeBest();
    void updateBest(int pos);
    double getBestFitness();
    void updateGamma();
    bool increasedSignal();

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