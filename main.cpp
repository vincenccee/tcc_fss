#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>

#define TAM_POP 30
#define DIMENSION 30
#define ITERATIONS 5000
#define RUNS 10

#include "fish_school_search.hpp"
#include "problem.hpp"
// #include "parameters.cpp" 

using namespace std;

int main(int argc, char **argv) {
  Problem *problem = new Problem(DIMENSION);
  // std::vector<double> test;
  // for(unsigned int i=0; i<DIMENSION; i++) test.push_back(0);
  // cout << "tudo zero: " << problem->evaluateFitness(test) << endl;
  // test.clear();
  // for(unsigned int i=0; i<DIMENSION; i++) test.push_back(-600);
  // cout << "tudo 600: " << problem->evaluateFitness(test) << endl;

  FishSchoolSearch *fss = new FishSchoolSearch(*problem, TAM_POP);
  fss->evolutionaryCicle(ITERATIONS, RUNS);

	return 0;
}