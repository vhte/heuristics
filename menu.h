/*
 * File:   menu.h
 * Author: victor
 *
 * Created on January 14, 2014, 11:12 AM
 */

void generateMenu() {
	time_t start,tempo;
	// Imprime o menu para o usuario escolher
	printf("########## HEURISTICS ARTICLE ##########\n\n");
	printf("1 - Generates initial solution\n");
	printf("2 - Local Search\n");
	printf("3 - Genetic Algorithm (GA)\n");
	printf("4 - VNS\n");
	printf("5 - Iterated Greedy\n");
	printf("6 - Exit\n");
	
	/*
	 The most common error when using getchar() is to try and use a char variable to store the result. You need to use an int variable, since the range of values getchar() returns is "a value in the range of unsigned char, plus the single negative value EOF". A char variable doesn't have sufficient range for this, which can mean that you can confuse a completely valid character return with EOF. The same applies to getc().
	 */
	int opcao;
	opcao = getchar();
	getchar(); // For the \n (this or use scanf())
	
	// Clear screen and show up actions to select
	system("clear");
	switch(opcao) {
		// Generates a new initial solution
		case '1':
			initialSolution();
			generateMenu();
			break;
		// Do a local search in a solution set
		case '2':
			localSearch();
			generateMenu();
			break;
		case '3':
			GA();
			generateMenu();
			break;
		case '4':
			printf("VNS0\n");
			time(&start);
			VNS();
			time(&tempo);
			printf("Finished in about %.0f seconds. \n", difftime(tempo, start));
			generateMenu();
			break;
		// Exits the program
		case '6':
			// Clean dynamic arrays
			//freeArray(&jobs[i-3].job.maquina);
			exit(EXIT_SUCCESS); // EXIT_FAILURE
			break;
		// Warning about invalid option, need to choose again
		default:
			printf("Invalid \"%c\" option\n", opcao);
			generateMenu();
			break;
	}
}