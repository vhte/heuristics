/* 
 * File:   main.c
 * Author: victor
 *
 * Created on January 14, 2014, 10:23 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "menu.h"
/*
 * 
 */
int main(int argc, char** argv) {
	// Loads and set jobs time in each machine
	readFile();
	generateMenu();
	return (EXIT_SUCCESS);
}