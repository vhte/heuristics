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
#include "structs.h"
// Create the Job list w/ enough machine times. Need to be created here because needs structs.h file and it'll be used in menu.h
Jobs jobs[1000];
int totalJobs, totalMachines;

// Solution struct
#include "menu.h";
/*
 * 
 */


int main(int argc, char** argv) {
	// Loads and set jobs time in each machine
	bool debug = true;
	readFile(debug);
	generateMenu();
	return (EXIT_SUCCESS);
}