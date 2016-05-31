#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include "population.hpp"
// #include "parameters.cpp" 

#define LIM_IMF -600
#define LIM_SUP 600
#define TAM_POP 10
#define DIMENSION 5

using namespace std;

int main(int argc, char **argv) {
  Population *pop = new Population(TAM_POP, DIMENSION, LIM_IMF, LIM_SUP);

  pop->initializePipulation();

  pop->showPopulation();

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