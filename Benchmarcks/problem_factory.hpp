#ifndef PROBLEM_FACTORY_H
#define PROBLEM_FACTORY_H

#include <string>

#include "problem.hpp"
#include "statics/ackley.hpp"
#include "statics/rastring.hpp"
#include "statics/griewank.hpp"
#include "statics/rosembrock.hpp"
#include "statics/schafel12.hpp"

class ProblemFactory{
public:
  Problem * get(std::string name, int dimension);
};

Problem * ProblemFactory::get(std::string name, int dimension)
{
  if(name == "RASTRIGIN")
  {
    return new Rastring(dimension);
  }
  else if(name == "ROSEMBROCK")
  {
    return new Rosembrock(dimension);
  }
  else if(name == "SCHAFEL12")
  {
    return new Schafel12(dimension);
  }
  else if(name == "GRIEWANK")
  {
    return new Griewank(dimension);
  }
  else if(name == "ACKLEY")
  {
    return new Ackley(dimension);
  }
  throw std::invalid_argument( "received invalid Objective Function name" );
}

#endif