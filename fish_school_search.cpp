#include "fish_school_search.hpp"

using namespace std;

FishSchoolSearch::FishSchoolSearch(Problem problem, int tamPopulation){
  this->problem = problem;
  this->tamPopulation = tamPopulation;
  this->stepIndInit = 0.01;
  this->stepIndFinal = 0.00001;
  this->stepVolInit = 0.01;
  this->stepVolFinal = 0.0001;
  this->minWeight = 1;
  this->maxWeight = 5000;
}

FishSchoolSearch::FishSchoolSearch(){}

FishSchoolSearch::~FishSchoolSearch(){}

void FishSchoolSearch::showPopulation(){
  std::vector<double> position;
  for(unsigned int i=0; i<tamPopulation; i++){
    cout << "ind " << i << "\t";
    position.clear();
    position = school->getFish(i)->getCurrentPosition();
    for(unsigned int j=0; j<position.size() ; j++){
      cout << " * " << position[j];
    }
    cout << " - fitness: " << problem.evaluateFitness(position) << " - weight: " << school->getFish(i)->getWeight() << endl;
  }
}

void FishSchoolSearch::evolutionaryCicle(int iterations){
  this->iterations = iterations;
  this->school = new Population(tamPopulation, problem.dimension, problem.getLowerBound(), problem.getUpperBound());
  school->initializePopulation();
  this->stepIndPercentage = this->stepIndInit;
  this->stepVolPercentage = this->stepVolInit;
  initializeBestPosition();
  // showPopulation();
  for(unsigned int i=0; i<this->iterations; i++){
    localSearch();
    setLocalSchoolNewWeight();
    // cout << "after localSearch: " << endl;
    // showPopulation();
    collectiveMovement();
    // cout << "after collectiveMovement: " << endl;
    // showPopulation();
    volitiveMovement();
    setSchoolNewWeight();
    // cout << "after volitiveMovement: " << endl;
    // showPopulation();
    updateBestPosition();
    updateStepPercentage();
  }
  showPopulation();
  cout << "position:  ";
  for(unsigned int i=0; i<bestPosition.size(); i++){
    cout << " * " << bestPosition[i];
  }
  cout << "\nBest Finess = " << getBestFitness() << endl;
}

void FishSchoolSearch::localSearch(){
  std::vector<double> nextPosition;
  Fish *tmpFish;
  for(unsigned int i=0; i<tamPopulation; i++){
    tmpFish = school->getFish(i);
    nextPosition.clear();
    nextPosition = createNeighboorPosition(tmpFish->getCurrentPosition());
    if(problem.evaluateFitness(nextPosition) > problem.evaluateFitness(tmpFish->getCurrentPosition())){
      tmpFish->setImproved(true);
      tmpFish->setCurrentPosition(nextPosition);
    } else {
      tmpFish->setImproved(false);
    }
    school->updateFish(*tmpFish, i);
  }
  school->updatePopulationDisplacement();
}

std::vector<double> FishSchoolSearch::createNeighboorPosition(std::vector<double> position){
  std::vector<double> neighboorPosition;
  for (unsigned int i = 0; i < problem.dimension; i++) {
    neighboorPosition.push_back(position[i] + stepIndPercentage*(problem.getUpperBound() - 
                                problem.getLowerBound()) * fRand(-1.0, 1.0));
  }
  return validatePosition(neighboorPosition);
}

std::vector<double> FishSchoolSearch::validatePosition(std::vector<double> position){
  for (unsigned int i = 0; i < position.size(); i++) {
    if(position[i] > problem.getUpperBound()){
      position[i] = problem.getUpperBound();
    }else if(position[i] < problem.getLowerBound()){
      position[i] = problem.getLowerBound();
    }
  }
  return position;
}

void FishSchoolSearch::setLocalSchoolNewWeight(){
  double greaterFitnessGain = calculateLocalGreaterFitnessGain();
  double newWeight;
  double fitnessGain;
  Fish *tmpFish;

  for(unsigned int i=0; i < tamPopulation; i++) {
    tmpFish = school->getFish(i);
    if(tmpFish->getImproved()){
      fitnessGain = calculateFitnessGain(tmpFish);
      newWeight = tmpFish->getWeight() + fitnessGain/greaterFitnessGain;
      if(newWeight < 10 or newWeight > 4500){
        cout << "new weight: " << newWeight << endl; 
        cout << "fit gain: " << fitnessGain << endl; 
        cout << "great fit gain: " << greaterFitnessGain << endl; 
        cout << "local: " << endl; 
        getchar();
      }
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

void FishSchoolSearch::setSchoolNewWeight(){
  double greaterFitnessGain = calculateGreaterFitnessGain();
  double newWeight;
  double fitnessGain;
  Fish *tmpFish;

  for(unsigned int i=0; i < tamPopulation; i++) {
    tmpFish = school->getFish(i);
    fitnessGain = calculateFitnessGain(tmpFish);
    newWeight = tmpFish->getWeight() + fitnessGain/greaterFitnessGain;
    if(newWeight < 10 or newWeight > 4500){
      cout << "new weight: " << newWeight << endl; 
      cout << "fit gain: " << fitnessGain << endl; 
      cout << "great fit gain: " << greaterFitnessGain << endl; 
      cout << "Search: " << endl; 
      getchar();
    }
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

double FishSchoolSearch::calculateLocalGreaterFitnessGain(){
  double greaterFitnessGain = pow(10, -32);
  double fitnessGain;

  for(unsigned int i=1; i < tamPopulation; i++) {
    if(school->getFish(i)->getImproved()){
      fitnessGain = calculateFitnessGain(school->getFish(i));
      if(fitnessGain > greaterFitnessGain){
        greaterFitnessGain = fitnessGain;
      }
    }
  }
  return greaterFitnessGain;
}

double FishSchoolSearch::calculateGreaterFitnessGain(){
  double greaterFitnessGain = pow(10, -32);
  double fitnessGain;
  // cout << "greater fit gain: " << greaterFitnessGain << endl;

  for(unsigned int i=1; i < tamPopulation; i++) {
    fitnessGain = calculateFitnessGain(school->getFish(i));
    if(fitnessGain > greaterFitnessGain){
      greaterFitnessGain = fitnessGain;
    }
  }
  return greaterFitnessGain;
}

double FishSchoolSearch::calculateFitnessGain(Fish *fish){
  double fitnessGain;
  fitnessGain = problem.evaluateFitness(fish->getCurrentPosition()) - 
                problem.evaluateFitness(fish->getPreviuosPosition());
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

  for(unsigned int i=0; i < tamPopulation; i++) {
    tmpFish = school->getFish(i);
    position.clear();
    for(unsigned int j=0; j < problem.dimension; j++) {
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
  // cout << "star gain: " << fitnessGainSum << endl;
  for(unsigned int i=0; i < tamPopulation; i++) {
    fitnessGainSum += fitnessGain[i];
  }
  double currentMoviment;

  for(unsigned int i=0; i < problem.dimension; i++) {
    currentMoviment = 0;
    if(fitnessGainSum == 0){
      colMovement.push_back(0.0);
    } else {
      for(unsigned int j=0; j < tamPopulation; j++) {
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
  for(unsigned int i=0; i < tamPopulation; i++) {
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
  double aux;
  Fish *tmpFish;

  // cout << "baricentro: ";
  // for(unsigned int i=0; i < barycenter.size(); i++) {
  //   cout << " * "<< barycenter[i];
  // }
  // cout << endl;

  for(unsigned int i=0; i < tamPopulation; i++) {
    position.clear();
    tmpFish = school->getFish(i);
    distanceToBarycenter = euclidianDistance(tmpFish->getCurrentPosition(), barycenter);
    for(unsigned int j=0; j<problem.dimension; j++){
      position.push_back(tmpFish->getCurrentPosition()[j] + sign*stepVolPercentage*
                         (problem.getUpperBound() - problem.getLowerBound()) *
                         ((tmpFish->getCurrentPosition()[j] - barycenter[j]) / distanceToBarycenter));
    }
    tmpFish->setCurrentPosition(validatePosition(position));
    school->updateFish(*tmpFish, i);
  }
}

double FishSchoolSearch::weightVariationSignal(){
  double weightVariation = 0;
  for(unsigned int i=0; i < tamPopulation; i++) {
    weightVariation += school->getFish(i)->getWeightVariation();
  }
  if(weightVariation < 0) cout << "Dilatação porraaaaa!!!!" << endl;
  return (weightVariation >= 0 ? -1.0 : 1.0);
}

double FishSchoolSearch::euclidianDistance(std::vector<double> a, std::vector<double> b){
  double sum = 0;
  for(unsigned int i=0; i < tamPopulation; i++) {
    sum += pow(a[i]-b[i], 2);
  }
  return sqrt(sum);
}

std::vector<double> FishSchoolSearch::calculateBarycenter(){
  std::vector<double> barycenter;
  double weightSum = calculateWeightSum();
  // cout << "weightSum: " << weightSum << endl;
  double aux;

  for(unsigned int i=0; i < problem.dimension; i++) {
    aux = 0;
    for(unsigned int j=0; j < tamPopulation; j++) {
      aux += school->getFish(j)->getWeight() * school->getFish(j)->getCurrentPosition()[i];
    }
    barycenter.push_back(aux/weightSum);
  }
  return barycenter;
}

double FishSchoolSearch::calculateWeightSum(){
  double weightSum = 0;

  for (unsigned int i=0; i < tamPopulation; i++) {
    weightSum += school->getFish(i)->getWeight();
  }
  return weightSum;
}

void FishSchoolSearch::initializeBestPosition(){
  bestPosition = school->getFish(0)->getCurrentPosition();
}

void FishSchoolSearch::updateBestPosition(){
  for(unsigned int i=0; i < tamPopulation; i++) {
    if(problem.evaluateFitness(school->getFish(i)->getCurrentPosition()) > problem.evaluateFitness(bestPosition)){
      bestPosition.clear();
      bestPosition = school->getFish(i)->getCurrentPosition();
    }
  }
}

void FishSchoolSearch::updateStepPercentage(){
  stepIndPercentage -= (double)(stepIndInit - stepIndFinal)/(double)iterations;
  stepVolPercentage -= (double)(stepVolInit - stepVolFinal)/(double)iterations;
}

double FishSchoolSearch::getBestFitness(){
  return problem.evaluateFitness(bestPosition);  
}

double FishSchoolSearch::fRand(double fMin, double fMax){
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}
