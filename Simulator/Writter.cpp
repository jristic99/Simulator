#include "Writter.h"

void Writter::writeAllOutputs(const string& filepath, const vector<vector<bool>> &probes_output,
	const vector<double> &simulation_time, const vector<Element*> &probes)
{
	for (int i = 0; i < probes.size(); i++)
	{
		string output_file_name = outputFileName(filepath, probes[i]);

		fstream output_file(output_file_name, ios::out);

		int prev = 0;
		int curr = 0;
		if (probes_output[i][0])
		{
			output_file << false << " -> " << true << ": ";
			output_file << simulation_time[0] << "us" << endl;
		}

		while (curr < simulation_time.size())
		{
			while (probes_output[i][prev] == probes_output[i][curr] && curr < simulation_time.size() - 1) curr++;
			
			if (curr == simulation_time.size() - 1 && probes_output[i][prev] == probes_output[i][curr]) break;

			output_file << probes_output[i][prev] << " -> " << probes_output[i][curr] << ": ";
			output_file  << simulation_time[curr] << "us" << endl;
			prev = curr;
			curr++;
		}
		output_file.close();
	}
}

string Writter::outputFileName(const string& filepath, Element* probe)
{
	string output_file_name = filepath.substr(0, filepath.length() - 4);
	output_file_name = output_file_name + "_" + to_string(probe->getId()) + ".txt";

	return output_file_name;
}
