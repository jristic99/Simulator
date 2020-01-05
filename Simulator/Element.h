#ifndef ELEMENT_H
#define ELEMENT_H

#include "Exceptions.h"
#include <vector>
using namespace std;

class Element
{
public:
	Element(int id);
	virtual ~Element();

	Element(const Element& other);
	Element(const Element&& other);
	
	virtual Element* clone() const = 0;

	virtual int getNumPins() const = 0;
	int getId() const;
	Element* getChild(int pin) const;

	void setChild(Element* child, int pin);

	virtual bool isGenerator() const;
	virtual bool isProbe() const;

	virtual vector<double> getChangingMoments(double final_moment);
	virtual bool calculateOutput(double time) = 0;
	virtual bool getOutput(double time);

protected:
	vector<Element*> children_; 
	bool output_;

private:
	int id_;
};

class DigitalSource : public Element
{
public:
	virtual int getNumPins() const override;
	virtual bool isGenerator() const override;

protected: 
	DigitalSource(int id);
	DigitalSource(const DigitalSource& other);
	DigitalSource(const DigitalSource&& other);
};


class ClockSignal : public DigitalSource
{
public:
	ClockSignal(int id, double frequency);

	ClockSignal(const ClockSignal& other);
	ClockSignal(const ClockSignal&& other);

	virtual ClockSignal* clone() const override;

	virtual vector<double> getChangingMoments(double final_moment) override;
	virtual bool calculateOutput(double time) override;
private:
	double frequency_;
};

class ManualSignal : public DigitalSource
{
public:
	ManualSignal(int id, vector<double> changes);

	ManualSignal(const ManualSignal& other);
	ManualSignal(const ManualSignal&& other);

	virtual ManualSignal* clone() const override;

	virtual vector<double> getChangingMoments(double final_moment) override;
	virtual bool calculateOutput(double time) override;

private:
	vector<double> changes_;
};

class LogicCircuit : public Element 
{
public:
	virtual int getNumPins() const override;

protected:
	LogicCircuit(int id, int num_pins);

	LogicCircuit(const LogicCircuit& other);
	LogicCircuit(const LogicCircuit&& other);

private:
	int num_pins_;
};

class NotCircuit : public LogicCircuit
{
public:
	NotCircuit(int id);

	NotCircuit(const NotCircuit& other);
	NotCircuit(const NotCircuit&& other);

	virtual NotCircuit* clone() const override;

	virtual bool calculateOutput(double time) override;
};

class AndCircuit : public LogicCircuit
{
public:
	AndCircuit(int id, int num_pins);

	AndCircuit(const AndCircuit& other);
	AndCircuit(const AndCircuit&& other);

	virtual AndCircuit* clone() const override;

	virtual bool calculateOutput(double time) override;
};

class OrCircuit : public LogicCircuit
{
public:
	OrCircuit(int id, int num_pins);

	OrCircuit(const OrCircuit& other);
	OrCircuit(const OrCircuit&& other);

	virtual OrCircuit* clone() const override;

	virtual bool calculateOutput(double time) override;
};

class Probe : public Element
{
public:
	Probe(int id);

	Probe(const Probe& other);
	Probe(const Probe&& other);

	virtual Probe* clone() const override;

	virtual int getNumPins() const override;
	virtual bool isProbe() const override;
	virtual bool calculateOutput(double time) override;

};

#endif