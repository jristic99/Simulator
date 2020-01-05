#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "DigitalCircuit.h"
#include "Element.h"
#include "Reader.h"
#include "Writter.h"
#include "Exceptions.h"
#include <algorithm>
#include <iostream>
#include <cmath>

using namespace std;

class Simulator
{
public:
	Simulator();
	~Simulator();

	Simulator(const Simulator&) = delete;
	Simulator(const Simulator&&) = delete;

	void loadCircuit(const string& filepath);
	void simulate(const string& filepath);

private:
	double sim_time_;
	int num_elements_;
	Reader* reader_;
	Writter* writter_; 
	DigitalCircuit* digital_circuit_;
	vector<double> simulation_time_;
	vector<Element*> elements_;
	vector<Element*> probes_; 
	vector<Element*> generators_;
	vector<vector<bool>> probes_output_; 

	vector<Element*> getProbes();
	vector<Element*> getGenerators();

	void findChangingMoments();
	void deletePrevious(); 
};


#endif