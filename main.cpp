#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>

#define TAM_POP 10
#define DIMENSION 10
#define ITERATIONS 5000
#define RUNS 1

#include <string>
#include <cmath>
#include "fish_school_search.hpp"
#include "benchmarcks/problem.hpp"
#include "benchmarcks/problem_factory.hpp"
// #include "parameters.cpp" 

using namespace std;

string getProblem();

int main(int argc, char **argv) {
  string value;

  cout << "test: " << fabs(-2) << endl;

  value = getProblem();
  cout << "opcao escolhida foi: " << value << endl;
  ProblemFactory *factory = new ProblemFactory();
  Problem *problem = factory->get(value, DIMENSION);

  FishSchoolSearch *fss = new FishSchoolSearch(problem, TAM_POP);
  fss->evolutionaryCicle(ITERATIONS, RUNS);

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
  cout << "6. Moving Peaks" << endl;
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
      answer = "MOVING_PEAKS";
      break;
    default:
      cout << "Opcao Invalida" << endl;
      getProblem();
  }
  return answer;
}