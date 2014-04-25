/* 
 * File:   main.c
 * Author: victor
 *
 * Created on January 14, 2014, 10:23 AM
 * 
 * gcc -lm main.c
 * -lm to link math lib
 * 
 * octave teste.mat (with pause before end)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include "structs.h"
#include "util.h"
// Create the Job list w/ enough machine times. Need to be created here because needs structs.h file and it'll be used in menu.h
Jobs jobs[1000];
int totalJobs, totalMachines;
Array machines[50]; // Stores the job position, not job value!!
bool debug = false;
char solucaoInicial; // report
// Solution struct
#include "local.h" // localSearch, readFile, initialSolution...
#include "octave.h" // report
#include "vns.h"
#include "grasp.h"
#include "ga.h"
#include "menu.h"

int main(int argc, char** argv) {
	// Loads and set jobs time in each machine
	readFile();
	generateMenu();
	return (EXIT_SUCCESS);
}
