#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "ga.h"

using namespace std;

int main(int argc, char** argv){
	if (argc<6){
		cout << "the numbers of argument are error\n plz enter 'num_run, population_num, genetic_len, mutation_rate, cross_rate'" << endl;
		exit(0);
	}
	else{
		// arg: num_run, genetic_len, mutation_rate, cross_rate
		ga* test = new ga(atoi(argv[1]), 
						atoi(argv[2]), 
						atoi(argv[3]), 
						atof(argv[4]), 
						atof(argv[5]));
		cout << "the number of runs: " << argv[1] << endl;
		cout << "the number of population: " << argv[2] << endl;
		cout << "the length of genes: " << argv[3] << endl;
		cout << "the rate of mutation: " << argv[4] << endl;
		cout << "the rate of cross: " << argv[5] << endl;

		test->run();
	}

	return 0;
}
