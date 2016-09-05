#include "fish_school_search.hpp"

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
    cout << " - fitness: " << problem->evaluateFitness(position) << " - weight: " << school->getFish(i)->getWeight() << endl;
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
  // cout << "test: " << bestPopulationFitness.size() << " " << bestPopulationFitness[49] << endl;

  for(int j=0; j<this->runs; j++){
    this->school = new Population(tamPopulation, problem->getDimension(), problem->getLowerBound(j), problem->getUpperBound(j));
    school->initializePopulation();
    this->stepIndPercentage = this->stepIndInit;
    this->stepVolPercentage = this->stepVolInit;
    this->m_nmdf = 0;

    initializeBestPosition();
    // showPopulation();
    for(int i=0; i<this->iterations; i++){
      localSearch();
      setLocalSchoolNewWeight();
      // cout << "after localSearch: " << endl;
      // showPopulation();
      collectiveMovement();
      // cout << "after collectiveMovement: " << endl;
      // showPopulation();
      volitiveMovement();
      // setSchoolNewWeight();
      // cout << "after volitiveMovement: " << endl;
      // showPopulation();
      updateBestPosition(i);
      updatePlot(i);
      updateStepPercentage();
    }
    // showPopulation();
    cout << "position:  ";
    for(unsigned int i=0; i<bestPosition.size(); i++){
      cout << " * " << bestPosition[i];
    }
    result = getBestFitness();
    finalFitness.push_back(result);

    cout << "\nBest Finess = " << result << endl;
  }
  for(int j=0; j<this->iterations; j++){
    bestPopulationFitness[j] = bestPopulationFitness[j]/this->runs;
    bestIndividualFitness[j] = bestIndividualFitness[j]/this->runs;
    populationDiversity[j] = populationDiversity[j]/this->runs;
  }
  cout << "************** Final ****************" << endl;
  cout << "Media do Fitness: " << arithmeticAverage(finalFitness) << endl;
  cout << "Desvio Padrao: " << standardDeviation(finalFitness) << endl;

  gnu_plot_convergence_best_mean(bestIndividualFitness, bestPopulationFitness, iterations, "MelhoraFitness", "melhora_fit");
  gnu_plot_convergence(populationDiversity, iterations, "pop_diversity", "fitnessdaPopulacao", "Divesidade Genotipica", 1);
  popdata.close();
}

void FishSchoolSearch::localSearch(){
  std::vector<double> nextPosition;
  Fish *tmpFish;
  for(int i=0; i<tamPopulation; i++){
    tmpFish = school->getFish(i);
    nextPosition.clear();
    nextPosition = createNeighboorPosition(tmpFish->getCurrentPosition());
    if(problem->fitnesIsBetter(problem->evaluateFitness(nextPosition), problem->evaluateFitness(tmpFish->getCurrentPosition()))){
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
  for (unsigned int i = 0; i < problem->getDimension(); i++) {
    neighboorPosition.push_back(position[i] + stepIndPercentage*(problem->getUpperBound(i) - 
                                problem->getLowerBound(i)) * fRand(-1.0, 1.0));
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

void FishSchoolSearch::setLocalSchoolNewWeight(){
  double greaterFitnessGain = calculateLocalGreaterFitnessGain();
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

void FishSchoolSearch::setSchoolNewWeight(){
  double greaterFitnessGain = calculateGreaterFitnessGain();
  double newWeight;
  double fitnessGain;
  Fish *tmpFish;

  for(int i=0; i < tamPopulation; i++) {
    tmpFish = school->getFish(i);
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

double FishSchoolSearch::calculateLocalGreaterFitnessGain(){
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

double FishSchoolSearch::calculateGreaterFitnessGain(){
  double greaterFitnessGain = pow(10, -32);
  double fitnessGain;

  for(int i=1; i < tamPopulation; i++) {
    fitnessGain = calculateFitnessGain(school->getFish(i));
    if(fitnessGain > greaterFitnessGain){
      greaterFitnessGain = fitnessGain;
    }
  }
  return greaterFitnessGain;
}

double FishSchoolSearch::calculateFitnessGain(Fish *fish){
  double fitnessGain;
  fitnessGain = problem->evaluateFitness(fish->getCurrentPosition()) - 
                problem->evaluateFitness(fish->getPreviuosPosition());
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

void FishSchoolSearch::initializeBestPosition(){
  bestPosition = school->getFish(0)->getCurrentPosition();
}

void FishSchoolSearch::updateBestPosition(int pos){
  for(int i=0; i < tamPopulation; i++) {
    if(problem->fitnesIsBetter(problem->evaluateFitness(school->getFish(i)->getCurrentPosition()),
       problem->evaluateFitness(bestPosition))){
      bestPosition.clear();
      bestPosition = school->getFish(i)->getCurrentPosition();
    }
  }
  bestIndividualFitness[pos] += problem->evaluateFitness(bestPosition);
}

void FishSchoolSearch::updateStepPercentage(){
  stepIndPercentage -= (double)(stepIndInit - stepIndFinal)/(double)iterations;
  stepVolPercentage -= (double)(stepVolInit - stepVolFinal)/(double)iterations;
}

double FishSchoolSearch::getBestFitness(){
  return problem->evaluateFitness(bestPosition);  
}

double FishSchoolSearch::fRand(double fMin, double fMax){
  double f = (double)rand() / RAND_MAX;
  return fMin + f * (fMax - fMin);
}

void FishSchoolSearch::updatePlot(int pos){
  double totalFit = 0;
  double mediaFit = 0;
  Fish *tmpFish;

  for(int i=0; i < tamPopulation; i++) {
    tmpFish = school->getFish(i);
    totalFit += problem->evaluateFitness(tmpFish->getCurrentPosition());
  }
  mediaFit = totalFit/tamPopulation;
  bestPopulationFitness[pos] += mediaFit;
  populationDiversity[pos] += defaultGenotypicDiversityMeasure();
}

void FishSchoolSearch::gnu_plot_convergence(std::vector<double> mean_gen, int m_gen, std::string name, std::string title, std::string y_axis, double max_range){

  FILE *pipe = popen("gnuplot", "w");

  name = space2underscore(name);
  title = space2underscore(title);

  if (pipe != NULL) {
    fprintf(pipe,"set terminal postscript eps enhanced color colortext font 'Arial,22'\n");
    fprintf(pipe,"set key font 'Arial,18'\n");
    fprintf(pipe,"set encoding iso_8859_1\n");
    fprintf(pipe,"set xlabel 'Gera{/E \347}{/E \365}es'\n");
    string output_y_label("set ylabel '" + y_axis + "'\n");
    fprintf(pipe, "%s", output_y_label.c_str());
    string output("set output '" + OUTPUT_DIR + name + std::string(".eps") + "'\n");
    fprintf(pipe, "%s", output.c_str());

    if(max_range){
      //fprintf(pipe, "set xrange [-1:42]\n");
      string output_range("set yrange [0:" + to_string(max_range) + "]\n");
      fprintf(pipe, "%s", output_range.c_str());
    }
    fprintf(pipe, "set style line 1 lc rgb 'black' \n");
    
    ofstream color1(OUTPUT_DIR + name + ".dataset");
    for(int k=0; k<m_gen; k++) {
      color1 << k << " " << mean_gen[k] << endl; 
    }                  

    color1.close();
    std::string str_plot = "plot '" + OUTPUT_DIR + name + ".dataset' with lines ls 1 title '" + title + "'\n";
    fprintf(pipe, "%s", str_plot.c_str());

    fflush(pipe);
    pclose(pipe);
  } else{
    std::cout << "Could not open pipe" << std::endl;
  }
}

void FishSchoolSearch::gnu_plot_convergence_best_mean(std::vector<double> d_data1, std::vector<double> d_data2, int n_lines, std::string title, std::string filename){

  FILE *pipe = popen("gnuplot", "w");

  title = space2underscore(title);
  filename = space2underscore(filename);

  ofstream melhor_output(OUTPUT_DIR + filename + "_melhor.output");
  ofstream media_output(OUTPUT_DIR + filename + "_media.output");
  for(int k=0; k<n_lines; k++) {
    melhor_output << k << " " << d_data1[k] << endl; 
    media_output << k << " " << d_data2[k] << endl; 
  }
  melhor_output.close();
  media_output.close();

  if (pipe != NULL) {
    fprintf(pipe, "set bmargin 7\n");
    fprintf(pipe, "unset colorbox\n");
    fprintf(pipe, "set terminal postscript eps enhanced color colortext font 'Arial,22'\n");
    fprintf(pipe, "set key font 'Arial,18'\n");
    fprintf(pipe, "set encoding iso_8859_1\n");
    fprintf(pipe, "set xlabel 'Gera{/E \347}{/E \365}es'\n");
    fprintf(pipe, "set ylabel 'M{/E \351}dia Fitness'\n");
    string output("set output '" + OUTPUT_DIR + filename + std::string(".eps") + "'\n");
    fprintf(pipe, "%s", output.c_str());
    //fprintf(pipe, "set xrange [-1:42]\n"); // set the terminal
    //fprintf(pipe, "set yrange [-1:42]\n"); // set the terminal
    fprintf(pipe, "set style line 1  lc rgb '#B22C2C' dashtype 2 \n");
    fprintf(pipe, "set style line 2  lc rgb '#0404E9' lt 2 \n");
    std::string str_title = "set title '" + title + "'\n";
    fprintf(pipe, "%s", str_title.c_str());
    std::string str_plot1 = "plot '" + OUTPUT_DIR + filename + "_melhor.output' using 1:2 title 'Melhor' ls 1 lw 5 with lines, ";
    std::string str_plot2 = "'" + OUTPUT_DIR + filename + "_media.output' using 1:2 title 'M{/E \351}dia' ls 2  lw 3 with lines\n ";
    fprintf(pipe, "%s", str_plot1.c_str());
    fprintf(pipe, "%s", str_plot2.c_str());

    fflush(pipe);
    pclose(pipe);
  } else{
    std::cout << "Could not open pipe" << std::endl;
  }
}

std::string FishSchoolSearch::space2underscore(std::string text) {
  for(std::string::iterator it = text.begin(); it != text.end(); ++it) {
      if(*it == ' ') {
          *it = '_';
      }
  }
  return text;
}

double FishSchoolSearch::defaultGenotypicDiversityMeasure(){
  double diversity = 0;
  double aux_1 = 0;
  double aux_2 = 0;
  unsigned short int a = 0;
  unsigned short int b = 0;
  unsigned short int d = 0;
  for(a = 0; a < tamPopulation; a++)
  {
    for(b = (a+1); b < tamPopulation; b++)
    {
      aux_1 = 0;
      for(d = 0; d < problem->getDimension(); d++)
      {       
        aux_1 += pow(school->getFish(a)->getCurrentPosition()[d] - school->getFish(b)->getCurrentPosition()[d], 2);
      }
      if(b == (a+1) || aux_2 > aux_1)
      {
        aux_2 = aux_1;
      }
    }
    diversity += log((double)1.0 + aux_2);  
  } 
  if(m_nmdf < diversity)
  {
    m_nmdf = diversity;
  }
  return diversity / m_nmdf;
}

double FishSchoolSearch::standardDeviation(std::vector<double> data){
  double media = arithmeticAverage(data);
  double sum1 = 0;

  for (unsigned int i=0; i<data.size(); i++) {
      sum1 += pow((data[i] - media), 2);
  }
  return sqrt(sum1 / (double)(data.size() - 1));
}

double FishSchoolSearch::arithmeticAverage(std::vector<double> data){
  double sum = 0;
  for (unsigned int i=0; i<data.size(); i++) {
      sum += data[i];
  }
  return (sum / data.size());
}