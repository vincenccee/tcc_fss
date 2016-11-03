#ifndef _GENERAL_TOOLS_H_
#define _GENERAL_TOOLS_H_
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <string>
#include <fstream>

class GeneralTools{
	private:
		std::string OUTPUT_DIR;

		std::string space2underscore(std::string text);

  public:
    double standardDeviation(std::vector<double> data);

		double arithmeticAverage(std::vector<double> data);

		int generateIntegerRandomNumber(int lowerBound, int upperBound);

		double generateRealRandomNumber(double lowerBound, double upperBound);

		double fRand(double fMin, double fMax);

		void gnu_plot_convergence(std::vector<double> mean_gen, int m_gen, std::string name, std::string title, std::string y_axis, double max_range);

		void gnu_plot_convergence_best_mean(std::vector<double> d_data1, std::vector<double> d_data2, int n_lines, std::string title, std::string filename);
};

#endif