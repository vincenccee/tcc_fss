#include "fish_school_search.hpp"

#define INITIAL_WEIGHT 2500

using namespace std;

FishSchoolSearch::FishSchoolSearch(Problem *problem, int tamPopulation){
  this->problem = problem;
  this->tamPopulation = tamPopulation;
  this->stepIndInit = 0.01;
  this->stepIndFinal = 0.00001;
  this->stepVolInit = 0.01;
  this->stepVolFinal = 0.0001;
  this->minWeight = 1;
  this->maxWeight = 5000;
  this->tools = new GeneralTools();
}

FishSchoolSearch::FishSchoolSearch(){}

FishSchoolSearch::~FishSchoolSearch(){}

void FishSchoolSearch::showPopulation(){
  std::vector<double> position;
  for(int i=0; i<tamPopulation; i++){
    cout << "ind " << i << "\t";
    position.clear();
    position = school->getFish(i)->getCurrentPosition();
    for(unsigned int j=0; j<position.size() ; j++){
      cout << " * " << position[j];
    }
    cout << " - fitness: " << school->getFish(i)->getFitness() << " - weight: " << school->getFish(i)->getWeight() << endl;
  }
}

void FishSchoolSearch::evolutionaryCicle(int iterations, int runs){
  this->iterations = iterations;
  this->runs = runs;
  popdata.open("testdata1.txt");
  double result;

  for(int i=0; i<this->iterations; i++){
    bestPopulationFitness.push_back(0.0);
    bestIndividualFitness.push_back(0.0);
    populationDiversity.push_back(0.0);
  }

  for(int j=0; j<this->runs; j++){
    this->school = new Population(tamPopulation, problem->getDimension(), problem->getLowerBound(j), problem->getUpperBound(j), INITIAL_WEIGHT);
    school->initializePopulation();
    this->stepIndPercentage = this->stepIndInit;
    this->stepVolPercentage = this->stepVolInit;
    this->m_nmdf = 0;
    evaluatePopulationFitness(true);
    initializeBest();
    // showPopulation();
    for(int i=0; i<this->iterations; i++){
      localSearch();
      setSchoolNewWeight();
      // cout << "after localSearch: " << endl;
      // showPopulation();
      collectiveMovement();
      // cout << "after collectiveMovement: " << endl;
      // showPopulation();
      volitiveMovement();
      // cout << "after volitiveMovement: " << endl;
      // showPopulation();
      evaluatePopulationFitness();
      updateBest(i);
      updatePlot(i);
      updateStepPercentage();
    }
    // showPopulation();
    cout << "position:  ";
    for(unsigned int i=0; i<bestPosition.size(); i++){
      cout << " * " << bestPosition[i];
    }
    result = this->bestFitness;
    finalFitness.push_back(result);

    cout << "\nBest Finess = " << result << endl;
    problem->resetProblem();
  }
  for(int j=0; j<this->iterations; j++){
    bestPopulationFitness[j] = bestPopulationFitness[j]/this->runs;
    bestIndividualFitness[j] = bestIndividualFitness[j]/this->runs;
    populationDiversity[j] = populationDiversity[j]/this->runs;
  }
  cout << "************** Final ****************" << endl;
  cout << "Media do Fitness: " << tools->arithmeticAverage(finalFitness) << endl;
  cout << "Desvio Padrao: " << tools->standardDeviation(finalFitness) << endl;

  tools->gnu_plot_convergence_best_mean(bestIndividualFitness, bestPopulationFitness, iterations, "MelhoraFitness", "melhora_fit");
  tools->gnu_plot_convergence(populationDiversity, iterations, "pop_diversity", "fitnessdaPopulacao", "Divesidade Genotipica", 1);
  popdata.close();
}

void FishSchoolSearch::evaluatePopulationFitness(bool first){
  // For para ser paralizado (OPEN-MP)
  // #pragma omp parallel for
  for(int i=0; i<tamPopulation; i++)
    school->getFish(i)->setFitness(problem->evaluateFitness(school->getFish(i)->getCurrentPosition()));
  if(first){
    for(int i=0; i<tamPopulation; i++)
      school->getFish(i)->setPrevFitness(school->getFish(i)->getFitness());
  }
}


void FishSchoolSearch::localSearch(){
  double nextPosFitness;
  std::vector<double> nextPosition;
  Fish *tmpFish;
  // For para ser paralizado (OPEN-MP)
  // #pragma omp parallel for
  for(int i=0; i<tamPopulation; i++){
    tmpFish = school->getFish(i);
    nextPosition.clear();
    nextPosition = createNeighboorPosition(tmpFish->getCurrentPosition());
    nextPosFitness = problem->evaluateFitness(nextPosition);
    if(problem->fitnesIsBetter(nextPosFitness, tmpFish->getFitness())){
      tmpFish->setImproved(true);
      tmpFish->setCurrentPosition(nextPosition);
      tmpFish->setFitness(nextPosFitness);
    } else {
      tmpFish->setImproved(false);
    }
    school->updateFish(*tmpFish, i);
  }
  school->updatePopulationDisplacement();
}

std::vector<double> FishSchoolSearch::createNeighboorPosition(std::vector<double> position){
  std::vector<double> neighboorPosition;
  for (unsigned int i = 0; i < problem->getDimension(); i++) {
    neighboorPosition.push_back(position[i] + stepIndPercentage*(problem->getUpperBound(i) - 
                                problem->getLowerBound(i)) * tools->fRand(-1.0, 1.0));
  }
  return validatePosition(neighboorPosition);
}

std::vector<double> FishSchoolSearch::validatePosition(std::vector<double> position){
  for (unsigned int i = 0; i < position.size(); i++) {
    if(position[i] > problem->getUpperBound(i)){
      position[i] = problem->getUpperBound(i);
    }else if(position[i] < problem->getLowerBound(i)){
      position[i] = problem->getLowerBound(i);
    }
  }
  return position;
}

void FishSchoolSearch::setSchoolNewWeight(){
  double greaterFitnessGain = calculateGreaterFitnessGain();
  double newWeight;
  double fitnessGain;
  Fish *tmpFish;

  for(int i=0; i < tamPopulation; i++) {
    tmpFish = school->getFish(i);
    if(tmpFish->getImproved() and greaterFitnessGain != 0){
      fitnessGain = calculateFitnessGain(tmpFish);
      newWeight = tmpFish->getWeight() + fitnessGain/greaterFitnessGain;

      if(newWeight < minWeight){
        newWeight = minWeight;
      }else if(newWeight > maxWeight){
        newWeight = maxWeight;
      }
      tmpFish->setWeightVariation(newWeight - tmpFish->getWeight());
      tmpFish->setWeight(newWeight);
      school->updateFish(*tmpFish, i);
    }
  }
}

double FishSchoolSearch::calculateGreaterFitnessGain(){
  double greaterFitnessGain = 0;
  double fitnessGain;

  for(int i=1; i < tamPopulation; i++) {
    if(school->getFish(i)->getImproved()){
      fitnessGain = calculateFitnessGain(school->getFish(i));
      if(fitnessGain > greaterFitnessGain){
        greaterFitnessGain = fitnessGain;
      }
    }
  }
  return greaterFitnessGain;
}

double FishSchoolSearch::calculateFitnessGain(Fish *fish){
  double fitnessGain;
  fitnessGain = fish->getFitness() - fish->getPrevFitness();
  if(problem->isMinimization())
    fitnessGain *= -1;
  return fitnessGain;
}

void FishSchoolSearch::collectiveMovement(){
  Fish *tmpFish;
  std::vector<double> position;
  std::vector<double> coletiveSum = calculateCollectiveMovementSum();
  // cout << "coletive sum: ";
  // for(unsigned int i=0; i < coletiveSum.size(); i++) {
  //   cout << " * " << coletiveSum[i];
  // }
  // cout << endl;

  for(int i=0; i < tamPopulation; i++) {
    tmpFish = school->getFish(i);
    position.clear();
    for(unsigned int j=0; j < problem->getDimension(); j++) {
      position.push_back(tmpFish->getCurrentPosition()[j] + coletiveSum[j]);
    }
    tmpFish->setCurrentPosition(validatePosition(position));
    school->updateFish(*tmpFish, i);
  }
}

std::vector<double> FishSchoolSearch::calculateCollectiveMovementSum(){
  std::vector<double> colMovement;
  std::vector<double> fitnessGain = schoolFitnessGain();
  double fitnessGainSum = pow(10, -32);
  for(int i=0; i < tamPopulation; i++) {
    fitnessGainSum += fitnessGain[i];
  }
  double currentMoviment;

  for(unsigned int i=0; i < problem->getDimension(); i++) {
    currentMoviment = 0;
    if(fitnessGainSum == 0){
      colMovement.push_back(0.0);
    } else {
      for(int j=0; j < tamPopulation; j++) {
        if(school->getFish(j)->getImproved()){
          currentMoviment += fitnessGain[j] * school->getFish(j)->getIndividualDisplacement()[i];
        }
      }
      colMovement.push_back(currentMoviment/fitnessGainSum);
    }
  }
  return colMovement;
}

std::vector<double> FishSchoolSearch::schoolFitnessGain(){
  std::vector<double> schoolFitnessGain;
  for(int i=0; i < tamPopulation; i++) {
    if(school->getFish(i)->getImproved()){
      schoolFitnessGain.push_back(calculateFitnessGain(school->getFish(i)));
    }
    else{
      schoolFitnessGain.push_back(0.0);
    }
  }
  return schoolFitnessGain;
}

void FishSchoolSearch::volitiveMovement(){
  std::vector<double> position;
  std::vector<double> barycenter = calculateBarycenter();
  double sign = weightVariationSignal();
  double distanceToBarycenter;
  Fish *tmpFish;

  // cout << "baricentro: ";
  // for(unsigned int i=0; i < barycenter.size(); i++) {
  //   cout << " * "<< barycenter[i];
  // }
  // cout << endl;

  for(int i=0; i < tamPopulation; i++) {
    position.clear();
    tmpFish = school->getFish(i);
    distanceToBarycenter = euclidianDistance(tmpFish->getCurrentPosition(), barycenter);
    for(unsigned int j=0; j<problem->getDimension(); j++){
      position.push_back(tmpFish->getCurrentPosition()[j] + sign*stepVolPercentage*
                         (problem->getUpperBound(j) - problem->getLowerBound(j)) *
                         ((tmpFish->getCurrentPosition()[j] - barycenter[j]) / distanceToBarycenter));
    }
    tmpFish->setCurrentPosition(validatePosition(position));
    school->updateFish(*tmpFish, i);
  }
}

double FishSchoolSearch::weightVariationSignal(){
  double weightVariation = 0;
  for(int i=0; i < tamPopulation; i++) {
    weightVariation += school->getFish(i)->getWeightVariation();
  }
  // if(weightVariation < 0) cout << "Dilatação!!!!" << endl;
  return (weightVariation >= 0 ? -1.0 : 1.0);
}

double FishSchoolSearch::euclidianDistance(std::vector<double> a, std::vector<double> b){
  double sum = 0;
  for(int i=0; i < tamPopulation; i++) {
    sum += pow(a[i]-b[i], 2);
  }
  return sqrt(sum);
}

std::vector<double> FishSchoolSearch::calculateBarycenter(){
  std::vector<double> barycenter;
  double weightSum = calculateWeightSum();
  double aux;

  for(unsigned int i=0; i < problem->getDimension(); i++) {
    aux = 0;
    for(int j=0; j < tamPopulation; j++) {
      aux += school->getFish(j)->getWeight() * school->getFish(j)->getCurrentPosition()[i];
    }
    barycenter.push_back(aux/weightSum);
  }
  return barycenter;
}

double FishSchoolSearch::calculateWeightSum(){
  double weightSum = 0;

  for (int i=0; i < tamPopulation; i++) {
    weightSum += school->getFish(i)->getWeight();
  }
  return weightSum;
}

void FishSchoolSearch::initializeBest(){
  this->bestFitness = school->getFish(0)->getFitness();
  this->bestPosition = school->getFish(0)->getCurrentPosition();
}

void FishSchoolSearch::updateBest(int pos){
  Fish *tmpFish;
  for(int i=0; i < tamPopulation; i++) {
    tmpFish = school->getFish(i);
    if(problem->fitnesIsBetter(tmpFish->getFitness(), this->bestFitness)){
      bestPosition.clear();
      bestPosition = tmpFish->getCurrentPosition();
      this->bestFitness = tmpFish->getFitness();
    }
  }
  bestIndividualFitness[pos] += this->bestFitness;
}

void FishSchoolSearch::updateStepPercentage(){
  stepIndPercentage -= (double)(stepIndInit - stepIndFinal)/(double)iterations;
  stepVolPercentage -= (double)(stepVolInit - stepVolFinal)/(double)iterations;
}

void FishSchoolSearch::updatePlot(int pos){
  double totalFit = 0;
  double mediaFit = 0;

  for(int i=0; i < tamPopulation; i++) {
    totalFit += school->getFish(i)->getFitness();
  }
  mediaFit = totalFit/tamPopulation;
  bestPopulationFitness[pos] += mediaFit;
  populationDiversity[pos] += defaultGenotypicDiversityMeasure();
}

double FishSchoolSearch::defaultGenotypicDiversityMeasure(){
  //VARIÁVEIS LOCAIS
  double diversity = 0;
  double aux_1 = 0;
  double aux_2 = 0;
  unsigned short int a = 0;
  unsigned short int b = 0;
  unsigned short int d = 0;
  for(a = 0; a < tamPopulation; a++){
    for(b = (a+1); b < tamPopulation; b++){
      aux_1 = 0;
      for(d = 0; d < problem->getDimension(); d++){ //SOMATÓRIO VARIANDO OS GENES ATÉ O NUMERO DE DIMENSÕES
        aux_1 += pow(school->getFish(a)->getCurrentPosition()[d] - school->getFish(b)->getCurrentPosition()[d], 2);
      }
      aux_1 = sqrt(aux_1); //RAIZ QUADRADA DO RESULTADO DO SOMATÓRIO
      aux_1 = aux_1 / problem->getDimension(); //DIVIDE RESULTADO PELO NUMERO DE DIMENSÕES
      if(b == (a+1) || aux_2 > aux_1){
        aux_2 = aux_1; //ATRIBUI O MENOR RESULTADO A AUX_2
      }
    }
    diversity += log((double)1.0 + aux_2); //CALCULA O LOGARITMO NATURAL DO RESULTADO SOMADO DE UM
  }
  //ATRIBUI O RESULTADO A N_NMDF
  if(m_nmdf < diversity){
    m_nmdf = diversity;
  }
  return diversity / m_nmdf;
}