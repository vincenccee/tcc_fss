#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include "population.hpp"
#include "problem.hpp"
// #include "parameters.cpp" 

#define TAM_POP 10
#define DIMENSION 5

using namespace std;

int main(int argc, char **argv) {
  Problem *problem = new Problem(DIMENSION);
  Population *pop = new Population(TAM_POP, problem->dimension, problem->getLowerBound(), problem->getUpperBound());

  pop->initializePipulation();

  pop->showPopulation();

  for(unsigned int i=0; i<pop->getTamPopulation(); i++){
    cout << "fitness " << i << " =\t"; 
    cout << problem->evaluateFitness(pop->getPopulation()[i].getCurrentPosition()) << endl;
  }

  // for(unsigned int i=0; i < 10; i++){
  //   cout << "Escolha: ";
  //   cin >> opcao;
  //   pop[i] = opcao;
  //   v.push_back(i);  
  // }
  
  // for(unsigned int j=0; j < 10; j++){
  //   cout << "array: " << "\t";
  //   cout << pop[j] << "\t";
  // }
  // for(unsigned int j=0; j < 10; j++){
  //   cout << "vector: " << "\t";
  //   cout << v[j] << "\t";
  // }
	return 0;
}