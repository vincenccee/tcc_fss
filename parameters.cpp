#include <iostream>

class Parameters {
  public:
    static int POPULATION_SIZE = 30;
    static final int MIN_WEIGHT = 1;
    static final int INIT_WEIGHT = 2500;
    static final int MAX_WEIGHT = 5000;
    static int DIMENSION = 30; 
    static double STEP_IND_INIT = 0.01;
    static double STEP_IND_FINAL = 0.00001;
    static double STEP_VOL_INIT = 0.01;
    static double STEP_VOL_FINAL = 0.0001;
}