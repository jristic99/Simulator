#include "Simulator.h"

Simulator::Simulator() : sim_time_(0), num_elements_(0), 
	reader_(new Reader()), writter_(new Writter()), digital_circuit_(nullptr) {}

Simulator::~Simulator()
{
	for (auto element : elements_)
		delete element;

	delete reader_;
	reader_ = nullptr;

	delete writter_;
	writter_ = nullptr;

	delete digital_circuit_;
	digital_circuit_ = nullptr;
}

void Simulator::loadCircuit(const string& filepath)
{
	if (digital_circuit_) deletePrevious();

	elements_ = reader_->readCircuit(filepath);
	sim_time_ = reader_->getSimTime();
	num_elements_ = reader_->getNumElements();

	generators_ = getGenerators();
	probes_ = getProbes();

	digital_circuit_ = new DigitalCircuit();
	
	findChangingMoments();
}

void Simulator::simulate(const string& filepath)
{
	int num_probes = probes_.size();
	probes_output_.assign(num_probes, {});


	for (int i = 0; i < num_probes; i++)
	{
		for (auto time : simulation_time_) 
		{
			bool result = digital_circuit_->findProbeOutput(elements_, probes_[i], time);
			probes_output_[i].push_back(result);
		}
	}
	writter_->writeAllOutputs(filepath, probes_output_, simulation_time_, probes_);
}

vector<Element*> Simulator::getProbes()
{
	vector<Element*> probes = {};
	for (auto element : elements_)
		if (element->isProbe())
			probes.push_back(element);
	return probes;
}

vector<Element*> Simulator::getGenerators()
{
	vector<Element*> generators = {};
	for (auto element : elements_)
	{
		if (element->isGenerator())
			generators.push_back(element);
	}
	return generators;
}

void Simulator::findChangingMoments()
{
	for (auto gen : generators_)
	{
		vector<double> new_times = gen->getChangingMoments(sim_time_);

		for (auto t1 : new_times)
		{
			bool is_there = false;
			for (auto t2 : simulation_time_)
			{

				if (abs(t1 - t2) < pow(10, -6)) 
				{
					is_there = true;
					break;
				}
			}
			if (!is_there)
				simulation_time_.push_back(t1);
		}
	}

	sort(simulation_time_.begin(), simulation_time_.end());
}

void Simulator::deletePrevious()
{
	for (auto element : elements_)
		delete element;

	delete digital_circuit_;

	simulation_time_.clear();
	elements_.clear();
	probes_.clear();
	generators_.clear();
	probes_output_.clear();
}

