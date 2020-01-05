#include "Element.h"

Element::Element(int id) : id_(id) {}

Element::~Element() {}

Element::Element(const Element & other) 
{
	id_ = other.id_;
}

Element::Element(const Element && other)
{
	id_ = other.id_;

	for (auto child : other.children_)
		children_.push_back(child);

	for (auto child : other.children_)
		child = nullptr;
}

int Element::getId() const
{
	return id_;
}

Element * Element::getChild(int pin) const
{
	return children_[pin];
}

bool Element::isGenerator() const 
{
	return false;
}

bool Element::isProbe() const
{
	return false;
}

vector<double> Element::getChangingMoments(double final_moment)
{
	return {};
}

void Element::setChild(Element* child, int pin)
{
	if(pin < getNumPins())
		children_[pin] = child;
	else throw WrongPin("Pin value::out of range");
}

bool Element::getOutput(double time)
{
	return output_;
}

DigitalSource::DigitalSource(int id) : Element(id) {}

DigitalSource::DigitalSource(const DigitalSource& other) : Element(other) {}

DigitalSource::DigitalSource(const DigitalSource && other) : Element(other) {}

int DigitalSource::getNumPins() const
{
	return 0;
}

bool DigitalSource::isGenerator() const
{
	return true;
}

Probe::Probe(int id) : Element(id) 
{
	children_.push_back(nullptr);
}

Probe::Probe(const Probe& other) : Element(other) {}

Probe::Probe(const Probe && other) : Element(other) {}

Probe* Probe::clone() const
{
	return new Probe(*this);
}

int Probe::getNumPins() const
{
	return 1;
}

bool Probe::isProbe() const
{
	return true;
}

bool Probe::calculateOutput(double time)
{
	output_ = (children_[0])->getOutput(time);
	return output_;
}

ClockSignal::ClockSignal(int id, double frequency) : 
	DigitalSource(id), frequency_(frequency) {}

ClockSignal::ClockSignal(const ClockSignal& other) : DigitalSource(other)
{
	frequency_ = other.frequency_;
}

ClockSignal::ClockSignal(const ClockSignal && other) : DigitalSource(other) 
{
	frequency_ = other.frequency_;
}

ClockSignal* ClockSignal::clone() const
{
	return new ClockSignal(*this);
}

vector<double> ClockSignal::getChangingMoments(double final_moment)
{
	vector<double> changes = {};
	double curr_moment = 0;
	double half_period = 1 / frequency_ / 2;

	while (curr_moment < final_moment) 
	{
		changes.push_back(curr_moment);
		curr_moment += half_period;
	}
	return changes;
}

bool ClockSignal::calculateOutput(double time)
{
	bool output = false;
	double time_tmp = time;
	double period = 1 / frequency_;
	while (time_tmp - period >= 0) time_tmp -= period; 

	if ((abs(time_tmp - period) < pow(10, -6)))
	{
		output_ = false;
		return false;
	} 

	if((abs(time_tmp - period / 2) < pow(10, -6)) || time_tmp > period / 2)
		output = true;
	else output = false;

	output_ = output;
	return output;
}

ManualSignal::ManualSignal(int id, vector<double> changes) : 
	DigitalSource(id), changes_(changes) {}

ManualSignal::ManualSignal(const ManualSignal & other) : DigitalSource(other)
{
	changes_ = other.changes_;
}

ManualSignal::ManualSignal(const ManualSignal && other) : DigitalSource(other)
{
	changes_ = other.changes_;
}

ManualSignal* ManualSignal::clone() const
{
	return new ManualSignal(*this);
}

vector<double> ManualSignal::getChangingMoments(double final_moment)
{
	double current = 0;
	int i;
	for (i = 0; i < changes_.size(); i++)
		if(current + changes_[i] < final_moment)
		{
			current += changes_[i];
			changes_[i] = current;
		}
		else break;
	changes_.resize(i);
	return changes_;
}

bool ManualSignal::calculateOutput(double time)
{
	bool output = false;
	for (int i = 0; i < changes_.size(); i++)
		if (changes_[i] <= time) output = !output;
		else break;
	output_ = output;
	return output;
}

LogicCircuit::LogicCircuit(int id, int num_pins) : 
	Element(id), num_pins_(num_pins)
{
	if (num_pins > 0)
		children_.assign(num_pins, nullptr);
}

LogicCircuit::LogicCircuit(const LogicCircuit & other) : Element(other)
{
	children_.assign(other.num_pins_, nullptr);
}

LogicCircuit::LogicCircuit(const LogicCircuit && other) : Element(other) {}

int LogicCircuit::getNumPins() const
{
	return num_pins_;
}

NotCircuit::NotCircuit(int id) : LogicCircuit(id, 1) {}

NotCircuit* NotCircuit::clone() const
{
	return new NotCircuit(*this);
}

NotCircuit::NotCircuit(const NotCircuit & other) : LogicCircuit(other) {}

NotCircuit::NotCircuit(const NotCircuit && other) : LogicCircuit(other) {}

bool NotCircuit::calculateOutput(double time)
{
	output_ = !(children_[0]->getOutput(time));
	return output_;
}

AndCircuit::AndCircuit(int id, int num_pins) : LogicCircuit(id, num_pins) {}

AndCircuit::AndCircuit(const AndCircuit & other) : LogicCircuit(other) {}

AndCircuit::AndCircuit(const AndCircuit && other) : LogicCircuit(other) {}

AndCircuit * AndCircuit::clone() const
{
	return new AndCircuit(*this);
}

bool AndCircuit::calculateOutput(double time)
{
	bool output = true;
	for (auto child : children_)
		output = output && child->getOutput(time);
	output_ = output;
	return output;
}

OrCircuit::OrCircuit(int id, int num_pins) : LogicCircuit(id, num_pins) {}

OrCircuit::OrCircuit(const OrCircuit & other) : LogicCircuit(other) {}

OrCircuit::OrCircuit(const OrCircuit && other) : LogicCircuit(other) {}

OrCircuit* OrCircuit::clone() const
{
	return new OrCircuit(*this);
}

bool OrCircuit::calculateOutput(double time)
{
	bool output = false;
	for (auto child : children_)
		output = output || child->getOutput(time);
	output_ = output;
	return output;
}
