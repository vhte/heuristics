/* 
 * File:   main.c
 * Author: victor
 *
 * Created on January 14, 2014, 10:23 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>
#include "util.h"
#include "structs.h"
// Create the Job list w/ enough machine times. Need to be created here because needs structs.h file and it'll be used in menu.h
Jobs jobs[1000];
int totalJobs, totalMachines;
Array machines[10]; // Stores the job position, not job value!!
bool debug = true;
// Solution struct
#include "menu.h"

int main(int argc, char** argv) {
	// Loads and set jobs time in each machine
	readFile();
	generateMenu();
	return (EXIT_SUCCESS);
}