#ifndef READER_H
#define READER_H

#include "Element.h"
#include "Exceptions.h"
#include <string>
#include <fstream>
#include <iomanip>
#include <set>

class Reader
{
public:
	vector<Element*> readCircuit(const string& filepath);

	double getSimTime() const;
	int getNumElements() const;

private:
	double sim_time_; 
	int num_elements_;
	enum ElementType { PROBE, CLOCK, MANUAL, NOT, AND, OR };

	// Pomocne funkcije
	void readSimulationParam(fstream& input_file);
	vector<Element*> readElements(fstream& input_file);
	void readConnections(vector<Element*> &elements, fstream& input_file);

	inline bool isDigit(char c);
};

#endif
