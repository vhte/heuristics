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
	
	int j = 0, k = 0, l=0; // count for totalJobs
	char totalJobsC[10], totalMachinesC[5], tmp[5]; // tmp to get strings
	int totalJobs, totalMachines; // after atoi(totalJobsC)
	// Create the Job list w/ enough machine times
	Jobs jobs[totalJobs];
	
	// Defines the final relation job-machine
	//JobList jobs;
	// Get the file and read it
	file = fopen("1011.txt", "r"); // open for reading
	
	while(fgets(line, sizeof line,file) != NULL) {
		i++;
		// 1st line - nr of jobs<space>number of machines<space>1
		if(i == 1) {
			// Keep running until we reach the end of line
			while(1) {
				// Until reach the first space, get the number of jobs			
				while(line[j] != ' ') {
					totalJobsC[k++] = line[j];
					j++;
				}
				j++; // so we pass the ' '
				totalJobs = atoi(totalJobsC);
				printf("Total de jobs: %d\n", totalJobs);
				//printf("Na posicao %d da linha %d achou o caractere '%c'\n", j, i, line[j]);
				
				// Until reach the second space, get the number of machines
				k=0;
				while(line[j] != ' ') {
					totalMachinesC[k++] = line[j++];
				}
				totalMachines = atoi(totalMachinesC);
				printf("Total de machines: %d\n", totalMachines);
				
				// Until reach the line break, do nothing (?)
				//exit(EXIT_SUCCESS);
				
				// Done with the first line
				break;
			}
			
			// goto 2nd line
			continue;
			//printf("%s", temp);
			
		}
		// 2nd line - number of machines
		else if(i == 2) {
			printf("LINHA 2\n");
			
			// Creates the jobs array and each position being a machine time
			
			continue;
		}
		// 3rd line to 10002nd line - <tab>MACHINENR<tab>TIMEOFJOBLINE
		j=0;
		k=0;
		
		while(line[j] != '\0' && line[j] != '\n') {
			// k = 1 machine 0
			// k = 2 job time in machine 0
			// k = 3 machine 1...
			if(line[j] == '\t') {
				j++;
				k++;
				continue;
			}
			// Get whatever it is
			l=0;
			//tmp[0] = '\0'; // reset
			memset( tmp, '\0', sizeof(tmp) );
			//strcpy(tmp, "");
			while(line[j] != '\t') {
				//printf("Para k=%d joguei dentro da string '%c'\n", k, line[j]);
				tmp[l] = line[j];
				l++;
				j++;
			}
			// Closes the string
			tmp[l+1] = '\0';
			
			printf("k: %d. String: %d\n",k,atoi(tmp));
			
			
			// new machine
			if(k%2 == 1)
				jobs[i].maquina = atoi(tmp);
			else
				jobs[i].tempo = atoi(tmp);
			k++;
			//printf("LINHA %d machine %d job %d: \"%d\"\n", i, jobs[i].maquina, i, jobs[i].tempo);
			j++;
		}
		//exit(EXIT_SUCCESS);
		
		//fputs(line,stdout);
		// For each line, I need to create all of the jobs with respective machine times
		//printf("%c - %c - %c", line[0], line[1], line[2]);
		// Print everything
		
		// @todo A logica de jobs[i].maquina esta errada
		printf("Job %i\n", j);
		for(j=0;j<totalMachines;j++) {
			//printf("Maquina %d: %d", jobs[i].)
		}
		printf("")
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
	printf("3 - ILS\n");
	printf("4 - GRASP\n");
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