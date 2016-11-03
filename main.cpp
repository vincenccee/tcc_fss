#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>

#define TAM_POP 10
#define DIMENSION 10
#define ITERATIONS 5000
#define RUNS 2

#include <string>
#include <cmath>
#include "algorithms/fish_school_search.hpp"
#include "algorithms/fish_school_search_2.hpp"
#include "benchmarcks/problem.hpp"
#include "benchmarcks/problem_factory.hpp"
// #include "parameters.cpp" 

using namespace std;

string getProblem();
int getAlgorithm();

int main(int argc, char **argv) {
  string value;
  int algorithm;
  FishSchoolSearch *fss;
  FishSchoolSearch2 *fss2;

  value = getProblem();
  cout << "opcao escolhida foi: " << value << endl;
  algorithm = getAlgorithm();
  ProblemFactory *factory = new ProblemFactory();
  Problem *problem = factory->get(value, DIMENSION);

  switch (algorithm){
    case 1:
      fss = new FishSchoolSearch(problem, TAM_POP);
      fss->evolutionaryCicle(ITERATIONS, RUNS);
      break;
    case 2:
      fss2 = new FishSchoolSearch2(problem, TAM_POP);
      fss2->evolutionaryCicle(ITERATIONS*2, RUNS);
      break;
    default:
      cout << "Opcao Invalida" << endl;
      getAlgorithm();
  }

	return 0;
}

string getProblem(){
  int index;
  string answer;
  cout << "Select the problem:" << endl;
  cout << "1. Ackley" << endl;
  cout << "2. Rastring" << endl;
  cout << "3. Rosembrock" << endl;
  cout << "4. Griewank" << endl;
  cout << "5. Schwefel 1.2" << endl;
  cout << "6. Sphere" << endl;
  cout << "7. Moving Peaks" << endl;
  cin >> index;
  switch (index){
    case 1:
      answer = "ACKLEY";
      break;
    case 2:
      answer = "RASTRING";
      break;
    case 3:
      answer = "ROSEMBROCK";
      break;
    case 4:
      answer = "GRIEWANK";
      break;
    case 5:
      answer = "SCHWEFEL";
      break;
    case 6:
      answer = "SPHERE";
      break;
    case 7:
      answer = "MOVING_PEAKS";
      break;
    default:
      cout << "Opcao Invalida" << endl;
      getProblem();
  }
  return answer;
}

int getAlgorithm(){
  int index;
  cout << "Select the algorithm:" << endl;
  cout << "1. FSS" << endl;
  cout << "2. FSS2" << endl;
  cin >> index;
  return index;
}