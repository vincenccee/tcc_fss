#include <iostream>
#include <cstdlib>

#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char **argv) {
	std::vector<std::vector<int> > pop;

  for(unsigned int i=0; i < 10; i++){
    cout << "Escolha: ";
    cin >> opcao;
    pop.push(opcao);  
  }
  
  for(unsigned int j=0; j < 10; j++){
    cout << pop[j] << "\n";
  }
	return 0;
}