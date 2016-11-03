#include "fish_school_search_2.hpp"

#define INITIAL_WEIGHT 500

using namespace std;

FishSchoolSearch2::FishSchoolSearch2(Problem *problem, int tamPopulation){
  this->problem = problem;
  this->tamPopulation = tamPopulation;
  this->minWeight = 1;
  this->maxWeight = 1000;
  this->alpha = -0.01;
  this->beta = 0.4;
  this->gamma = 0.1;
  this->minGamma = 0.001;
  this->maxGamma = 0.999;
  this->betterNumber = 0;
  this->tools = new GeneralTools();
}

FishSchoolSearch2::FishSchoolSearch2(){}

FishSchoolSearch2::~FishSchoolSearch2(){}

void FishSchoolSearch2::showPopulation(){
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

void FishSchoolSearch2::evolutionaryCicle(int iterations, int runs){
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
    this->m_nmdf = 0;
    prevCountFish = currCountFish = 0;
    evaluatePopulationFitness(true);
    localSearch();
    initializeBest();
    setSchoolNewWeight();
    prevCountFish = currCountFish;
    for(int i=1; i<this->iterations; i++){
      updateGamma();
      currCountFish = 0;
      school->updatePopulationDisplacement();
      setSchoolNewWeight();
      
      // main function of update position (union of the three previus moviments)
      updateFishPosition();
      evaluatePopulationFitness();

      updateBest(i);
      updatePlot(i);
    }
    // showPopulation();
    cout << "position:  ";
    for(unsigned int i=0; i<bestPosition.size(); i++){
      cout << " * " << bestPosition[i];
    }
    result = this->bestFitness;
    finalFitness.push_back(result);

    cout << "\nBest Finess = " << result << endl;
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

void FishSchoolSearch2::evaluatePopulationFitness(bool first){
  // For para ser paralizado (OPEN-MP)
  // #pragma omp parallel for
  for(int i=0; i<tamPopulation; i++)
    school->getFish(i)->setFitness(problem->evaluateFitness(school->getFish(i)->getCurrentPosition()));
  if(first){
    for(int i=0; i<tamPopulation; i++)
      school->getFish(i)->setPrevFitness(school->getFish(i)->getFitness());
  }
}

void FishSchoolSearch2::localSearch(){
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
    tmpFish->setCurrentPosition(nextPosition);
    tmpFish->setFitness(nextPosFitness);
    tmpFish->setImproved(true);
    school->updateFish(*tmpFish, i);
  }
}

std::vector<double> FishSchoolSearch2::createNeighboorPosition(std::vector<double> position){
  std::vector<double> neighboorPosition;
  for (unsigned int i = 0; i < problem->getDimension(); i++) {
    neighboorPosition.push_back(position[i] + 0.01*(problem->getUpperBound(i) - 
                                problem->getLowerBound(i)) * tools->fRand(-1.0, 1.0));
  }
  return validatePosition(neighboorPosition);
}

std::vector<double> FishSchoolSearch2::validatePosition(std::vector<double> position){
  for (unsigned int i = 0; i < position.size(); i++) {
    if(position[i] > problem->getUpperBound(i)){
      position[i] = problem->getUpperBound(i);
    }else if(position[i] < problem->getLowerBound(i)){
      position[i] = problem->getLowerBound(i);
    }
  }
  return position;
}

void FishSchoolSearch2::setSchoolNewWeight(){
  double newWeight;
  Fish *tmpFish;
  double greaterFitnessGain = calculateGreaterFitnessGain();
  if(greaterFitnessGain == 0)
    greaterFitnessGain = 1;

  for(int i=0; i < tamPopulation; i++) {
    tmpFish = school->getFish(i);
    newWeight = tmpFish->getWeight() + (calculateFitnessGain(tmpFish)/greaterFitnessGain);

    if(newWeight < minWeight){
      newWeight = minWeight;
    }else if(newWeight > maxWeight){
      newWeight = maxWeight;
    }
    tmpFish->setWeightVariation(newWeight - tmpFish->getWeight());
    if(tmpFish->getWeightVariation() > 0)
      currCountFish++;
    tmpFish->setWeight(newWeight);
    school->updateFish(*tmpFish, i);
  }
}

double FishSchoolSearch2::calculateFitnessGain(Fish *fish){
  double fitnessGain;
  fitnessGain = fish->getFitness() - fish->getPrevFitness();
  if(problem->isMinimization())
    fitnessGain *= -1;
  return fitnessGain;
}

double FishSchoolSearch2::calculateGreaterFitnessGain(){
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

void FishSchoolSearch2::updateFishPosition(){
  Fish *tmpFish;
  std::vector<double> position;
  std::vector<double> newPosition;
  std::vector<double> individualVet;
  std::vector<double> colletiveVet = colletiveMoviment();
  std::vector<double> barycenter = calculateBarycenter();
  std::vector<double> volitiveVet;

  for (int i = 0; i < tamPopulation; ++i){
    tmpFish = school->getFish(i);
    newPosition.clear();
    position = tmpFish->getCurrentPosition();
    individualVet = individualMoviment(tmpFish);
    volitiveVet = volitiveMoviment(tmpFish, barycenter);
    for(unsigned int j=0; j < problem->getDimension(); j++){
      newPosition.push_back((-1.0) * position[j] + individualVet[j] + colletiveVet[j] + volitiveVet[j]);
    }
    tmpFish->setCurrentPosition(validatePosition(newPosition));
    school->updateFish(*tmpFish, i);
  }
}

std::vector<double> FishSchoolSearch2::individualMoviment(Fish *fish){
  std::vector<double> position;
  std::vector<double> displacement = fish->getIndividualDisplacement();
  for (int i = 0; i < problem->getDimension(); ++i){
    position.push_back(this->beta * this->gamma * displacement[i]);
  }
  return position;
}

std::vector<double> FishSchoolSearch2::colletiveMoviment(){
  std::vector<double> colMovement;
  double weightSum = calculateWeightSum();
  double aux;
  double factor = this->gamma * tools->fRand(0, 1);

  for(unsigned int i=0; i < problem->getDimension(); i++) {
    aux = 0;
    for(int j=0; j < tamPopulation; j++) {
      aux += school->getFish(j)->getWeight() * school->getFish(j)->getIndividualDisplacement()[i];
    }
    colMovement.push_back(factor * aux/weightSum);
  }
  return colMovement;
}

std::vector<double> FishSchoolSearch2::volitiveMoviment(Fish *fish, std::vector<double> barycenter){
  std::vector<double> volitiveMoviment;
  std::vector<double> position = fish->getCurrentPosition();
  bool sign = weightVariationSignal();
  double factor = (this->gamma * tools->fRand(0, 1)) * sign;

  for (int i = 0; i < problem->getDimension(); ++i){
    volitiveMoviment.push_back(factor * (position[i] - barycenter[i]));
  }
  return volitiveMoviment;
}

double FishSchoolSearch2::weightVariationSignal(){
  double weightVariation = 0;
  for(int i=0; i < tamPopulation; i++) {
    weightVariation += school->getFish(i)->getWeightVariation();
  }
  // if(weightVariation < 0) cout << "Dilatação!!!!" << endl;
  return (weightVariation >= 0 ? 1.0 : -1.0);
}

std::vector<double> FishSchoolSearch2::calculateBarycenter(){
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

double FishSchoolSearch2::calculateWeightSum(){
  double weightSum = 0;

  for (int i=0; i < tamPopulation; i++) {
    weightSum += school->getFish(i)->getWeight();
  }
  return weightSum;
}

void FishSchoolSearch2::initializeBest(){
  this->bestFitness = school->getFish(0)->getFitness();
  this->bestPosition = school->getFish(0)->getCurrentPosition();
}

void FishSchoolSearch2::updateBest(int pos){
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

void FishSchoolSearch2::updateGamma(){
  this->gamma *= this->alpha;
  if(this->gamma < minGamma)
    this->gamma = minGamma;
  else if(this->gamma > maxGamma)
    this->gamma = maxGamma;
  // update alpha
  if(currCountFish < prevCountFish)
    alpha *= -1.0;
  prevCountFish = currCountFish;
}

void FishSchoolSearch2::updatePlot(int pos){
  double totalFit = 0;
  double mediaFit = 0;

  for(int i=0; i < tamPopulation; i++) {
    totalFit += school->getFish(i)->getFitness();
  }
  mediaFit = totalFit/tamPopulation;
  bestPopulationFitness[pos] += mediaFit;
  populationDiversity[pos] += defaultGenotypicDiversityMeasure();
}

double FishSchoolSearch2::defaultGenotypicDiversityMeasure(){
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