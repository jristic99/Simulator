#ifndef DIGITAL_CIRCUIT_H
#define DIGITAL_CIRCUIT_H

#include "Element.h"
#include <stack>
#include <set>

class DigitalCircuit
{
public:
	bool findProbeOutput(const vector<Element*> &elements_, Element* probe, double time) const;
};

#endif