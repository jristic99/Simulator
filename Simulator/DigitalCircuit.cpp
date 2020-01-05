#include "DigitalCircuit.h"

//DFS for N-ary tree
bool DigitalCircuit::findProbeOutput(const vector<Element*> &elements_, Element* probe, double time) const
{
	Element* current = probe;
	stack<Element*> unvisited = {};
	set<Element*> visited = {}; 

	unvisited.push(current); 
	while (!unvisited.empty()) 
	{
		current = unvisited.top();

		if (current->isGenerator())
		{
			current->calculateOutput(time);
			unvisited.pop();
			visited.insert(current);
		}
		else
		{
			bool allChildrenVisited = true;
			for (int i = 0; i < current->getNumPins(); i++)
			{
				if (visited.find(current->getChild(i)) == visited.end()) 
				{
					unvisited.push(current->getChild(i));
					allChildrenVisited = false;
				}
			}
			if (allChildrenVisited)
			{
				current->calculateOutput(time);
				unvisited.pop();
				visited.insert(current);
			}
		}
	}
	return probe->getOutput(time);
}
