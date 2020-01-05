#include "Reader.h"

vector<Element*> Reader::readCircuit(const string& filepath)
{
	fstream input_file(filepath, ios::in);

	readSimulationParam(input_file);
	vector<Element*> elements = readElements(input_file);
	readConnections(elements, input_file);

	input_file.close();

	return elements;
}

double Reader::getSimTime() const
{
	return sim_time_;
}

int Reader::getNumElements() const
{
	return num_elements_;
}

void Reader::readSimulationParam(fstream & input_file)
{
	input_file >> sim_time_;
	input_file >> num_elements_;

	if (sim_time_ < 0) throw NegativeInput("Simulation time::must be positive");
	if (num_elements_ < 0) throw NegativeInput("Number of elements::must be positive");
}

vector<Element*> Reader::readElements(fstream& input_file)
{
	vector<Element*> elements;
	set<int> ids = {};

	for (int i = 0; i < num_elements_; i++)
	{
		int id, element_type;
		input_file >> id;
		input_file >> element_type;

		if (ids.find(id) == ids.end()) ids.insert(id);
		else throw WrongIdentifier("Identifier value::cannot support two identifiers with same value");

		Element* new_element = nullptr;
		string freq_tmp;
		double frequency;
		double tmp;
		vector<double> changes;
		int num_pins;
		double sum = 0;

		switch (element_type)
		{
		case PROBE:
			new_element = new Probe(id);
			break;
		case CLOCK:
			input_file >> frequency;
			new_element = new ClockSignal(id, frequency);
			break;
		case MANUAL:
			while (input_file.peek() != '\n')
			{
				input_file >> tmp;
				changes.push_back(tmp);
			}
			new_element = new ManualSignal(id, changes);
			break;
		case NOT:
			new_element = new NotCircuit(id);
			break;
		case AND:
			while (input_file.peek() != '\n' && !isDigit(input_file.peek())) input_file.get();

			if (isDigit(input_file.peek()))
				input_file >> num_pins; 
			else num_pins = 2; 

			new_element = new AndCircuit(id, num_pins);
			break;
		case OR:
			while (input_file.peek() != '\n' && !isDigit(input_file.peek())) input_file.get();

			if (isDigit(input_file.peek()))
				input_file >> num_pins;
			else num_pins = 2;

			new_element = new OrCircuit(id, num_pins);
			break;
		default:
			throw WrongElementType("Element type::out of range");
			break;
		}

		elements.push_back(new_element);
	}
	return elements;
}

void Reader::readConnections(vector<Element*> &elements, fstream& input_file)
{
	while (input_file.peek() != EOF)
	{
		int first, second, pin;
		input_file >> first >> second >> pin;

		Element* parent = nullptr, *child = nullptr;

		for (int i = 0; i < elements.size(); i++)
		{
			if (elements[i]->getId() == first) child = elements[i];
			else if (elements[i]->getId() == second) parent = elements[i];
		}

		if (!child || !parent || child == parent) 
			throw WrongInput("Connections of a digital circuit not valid");

		if(pin >= parent->getNumPins())
			throw WrongInput("Connections of a digital circuit not valid");

		parent->setChild(child, pin);
	}
}

inline bool Reader::isDigit(char c)
{
	return ('0' <= c && c <= '9');
}
