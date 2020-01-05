#include "Simulator.h"
#include <iostream>
#include <vector>

void test() {
	// IZMENITI SADRZAJ UNUTAR '{' i '}' DA BISTE POKRETALI ZELJENI SKUP TESTOVA
	int test_numbers[] = { 0, 1, 2, 3, 4, 5};
	std::vector<int> test_vector(test_numbers, test_numbers + sizeof(test_numbers) / sizeof(int));
	try {
		Simulator simulator;
		for (int test_number : test_vector) {
			simulator.loadCircuit("test_" + to_string(test_number) + ".txt");
			simulator.simulate("test_" + to_string(test_number) + "_output.txt");
		}
	}
	catch (exception& e) {
		std::cout << e.what() << std::endl;
	}
}