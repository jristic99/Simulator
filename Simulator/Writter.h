#ifndef WRITTER_H
#define WRITTER_H

#include "Element.h"
#include "Exceptions.h"
#include <string>
#include <fstream>
#include <iomanip>

class Writter
{
public:
	void writeAllOutputs(const string& filepath, const vector<vector<bool>>& probes_output, 
		const vector<double>& simulation_time, const vector<Element*>& probes);

private:
	string outputFileName(const string& filepath, Element* probe); 
};
#endif