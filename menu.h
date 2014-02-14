
/*
 * File:   menu.h
 * Author: victor
 *
 * Created on January 14, 2014, 11:12 AM
 */
void localSearch() {
	printf("\nEXECUTOU BUSCA LOCAL\n");
}

// @todo WTF is the 1 at the end of the 1st line?
void readFile() {
	// Declares file pointer
	FILE *file;
	char line[180];
	int i = 0;
	
	int j = 0, k = 0, l=0, tmpJ[20]; // count for totalJobs
	char tmp[5]; // tmp to get strings
	int totalJobs, totalMachines, maq=0,tempo=0; // after atoi(totalJobsC)
	// Create the Job list w/ enough machine times
	Jobs jobs[1000];

	// Get the file and read it
	file = fopen("1011.txt", "r"); // open for reading
	
	while(fgets(line, sizeof line,file) != NULL) {
		i++;
		// 1st line - nr of jobs<space>number of machines<space>1
		if(i == 1) {
			
			sscanf(line,"%d %d %d", &totalJobs, &totalMachines, &j);
			//printf("Total de jobs: %d. Total de maquinas: %d", totalJobs,totalMachines);
			//exit(1);
			
			// goto 2nd line
			continue;
			//printf("%s", temp);
			
		}
		// 2nd line - number of machines
		else if(i == 2) {
			printf("LINHA 2\n");
			
			// Creates the jobs array and each position being a machine time
			for(j=0;j<totalJobs;j++) {
				initArray(&jobs[i].job.maquina, totalMachines);
				initArray(&jobs[i].job.tempo, totalMachines);
			}
			continue;
		}
		// 3rd line to 10002nd line - <tab>MACHINENR<tab>TIMEOFJOBLINE
		j=0;
		k=0;
		
		while(line[j] != '\0' ) {
			
			sscanf(line,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
					&tmpJ[0], &tmpJ[1], &tmpJ[2], &tmpJ[3], &tmpJ[4], &tmpJ[5], &tmpJ[6], &tmpJ[7], &tmpJ[8], &tmpJ[9],
					&tmpJ[10], &tmpJ[11], &tmpJ[12], &tmpJ[13], &tmpJ[14], &tmpJ[15], &tmpJ[16], &tmpJ[17], &tmpJ[18], &tmpJ[19]);
			k = 0;
			
			for(j=0;j<totalMachines*2;j++) {
				//printf("Maq %d Tem %d\n", jobs[i-3].job.maquina.array[0], jobs[i-3].job.tempo.array[0]);
				insertArray(&jobs[i-3].job.maquina, tmpJ[j]);
				insertArray(&jobs[i-3].job.tempo, tmpJ[j+1]);
				printf("Maq %d Tem %d\n", jobs[i-3].job.maquina.array[0], jobs[i-3].job.tempo.array[0]);
				printf("Add maq %d com tem %d\n", tmpJ[j], tmpJ[j+1]);
				j++;
			}
			printf("1o Job\n\n");
			for(j=0;j<totalMachines;j=j++) {
				printf("Maquina %d Tempo %d\n", jobs[i-3].job.maquina.array[j], jobs[i-3].job.tempo.array[j]);
				j++;
			}
			//freeArray(&jobs);
			exit(1);
			printf("FIM DA LINHA 2");
			
			
		}
		//exit(EXIT_SUCCESS);
		// Saiu da linha
		printf("\n\ntotalMachines: %zd | total maquinas: %zd", totalMachines, jobs[i-3].job.maquina.used);
		exit(1);
		for(j=0;j<totalMachines;j++)
			printf("Job %d Maquina %d: Tempo: %d \n", i,jobs[i-3].job.maquina.array[j], jobs[i-3].job.tempo.array[j]);
		exit(EXIT_SUCCESS);
		
		//fputs(line,stdout);
		// For each line, I need to create all of the jobs with respective machine times
		//printf("%c - %c - %c", line[0], line[1], line[2]);
		// Print everything
		
		printf("Jobs %d\n", i);
		
		
		exit(1);
	}
	fclose(file);
}

void initialSolution() {
	printf("\nGEROU SOLUCAO INICIAL\n");
}

void generateMenu() {
	// Imprime o menu para o usuario escolher
	printf("########## HEURISTICS ARTICLE ##########\n\n");
	printf("1 - Generates initial solution\n");
	printf("2 - Local Search\n");
	printf("3 - Genetic Algorithm\n");
	printf("4 - VNS\n");
	printf("5 - Exit\n");
	
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
		// Exits the program
		case '5':
			exit(EXIT_SUCCESS); // EXIT_FAILURE
			break;
		// Warning about invalid option, need to choose again
		default:
			printf("Invalid \"%c\" option\n", opcao);
			generateMenu();
			break;
	}
}