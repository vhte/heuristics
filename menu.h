
/*
 * File:   menu.h
 * Author: victor
 *
 * Created on January 14, 2014, 11:12 AM
 */
// @todo WTF is the 1 at the end of the 1st line?
// @todo Maybe a first read to know how much jobs and machines we have and put it in Jobs jobs[num]
void readFile() {
	// Declares file pointer
	FILE *file;
	char line[180];
	int i = 0;
	
	int j = 0, k = 0, tmpJ[20]; // count for totalJobs
	// Create the Job list w/ enough machine times
	//Jobs jobs[1000];

	// Get the file and read it
	file = fopen("111.txt", "r"); // open for reading
	
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
			if(debug)
				printf("LINHA 2\n");
			// Creates the jobs array and each position being a machine time
			for(j=0;j<totalJobs;j++) {
				initArray(&jobs[j].job.maquina, totalMachines);
				initArray(&jobs[j].job.tempo, totalMachines);
			}
			continue;
		}
		// 3rd line to 10002nd line - <tab>MACHINENR<tab>TIMEOFJOBLINE
		k=0;

		if(debug)
			printf("\n\nJOB %d\n\n", i-3);
		sscanf(line,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
				&tmpJ[0], &tmpJ[1], &tmpJ[2], &tmpJ[3], &tmpJ[4], &tmpJ[5], &tmpJ[6], &tmpJ[7], &tmpJ[8], &tmpJ[9],
				&tmpJ[10], &tmpJ[11], &tmpJ[12], &tmpJ[13], &tmpJ[14], &tmpJ[15], &tmpJ[16], &tmpJ[17], &tmpJ[18], &tmpJ[19]);
		k = 0;

		// Adding the relationship MACHINE->TIME to this job
		for(j=0;j<totalMachines*2;j++) {
			//printf("Maq %d Tem %d\n", jobs[i-3].job.maquina.array[0], jobs[i-3].job.tempo.array[0]);
			insertArray(&jobs[i-3].job.maquina, tmpJ[j]);
			insertArray(&jobs[i-3].job.tempo, tmpJ[j+1]);
			j++;
		}
		
		if(debug)
			for(j=0;j<totalMachines;j=j++) {
				printf("Maquina %d Tempo %d\n", jobs[i-3].job.maquina.array[j], jobs[i-3].job.tempo.array[j]);
				j++;
			}
		//freeArray(&jobs[i-3].job.maquina);
		//exit(1);
			
			
		
		//exit(EXIT_SUCCESS);
		// Saiu da linha
		if(debug) {
			printf("\n\ntotalMachines: %zd | total maquinas: %zd", totalMachines, jobs[i-3].job.maquina.used);
		//exit(1);
		
			for(j=0;j<totalMachines;j++)
				printf("Job %d Maquina %d: Tempo: %d \n", i-3,jobs[i-3].job.maquina.array[j], jobs[i-3].job.tempo.array[j]);
		}
		//exit(EXIT_SUCCESS);
		
		//fputs(line,stdout);
		// For each line, I need to create all of the jobs with respective machine times
		//printf("%c - %c - %c", line[0], line[1], line[2]);
		// Print everything
		if(debug)
			printf("Jobs %d\n", i-3);
	}
	fclose(file);
	
	return;
}

// Initial solution = For each job, gets the shortest time and put it in the machine attached to this time
// @todo free(&jobs). We don't need it anymore
void initialSolution() {
	int i=0,j,menor,melhorMaq;
	// Initialize number of machines
	for(;i<totalMachines;i++)
		initArray(&machines[i], totalJobs);
	
	// For each job...
	for(i=0;i<totalJobs;i++) {
		// ... allocate the shortest time to selected machine
		menor = INT_MAX;
		for(j=0;j<jobs[i].job.tempo.used;j++) {
			if(jobs[i].job.tempo.array[j] < menor) {
				menor = jobs[i].job.tempo.array[j];
				melhorMaq = jobs[i].job.maquina.array[j];
			}
		}
		// allocate...
		insertArray(&machines[melhorMaq], menor);
	}
	
	printf("Solucao Inicial Gulosa gerada.\n\n");
	// Just show the initial solution
	for(i=0;i<totalMachines;i++) {
		printf("Maquina %d: ", i);
		for(j=0;j<machines[i].used;j++) {
			printf("%d ", machines[i].array[j]);
		}
		printf("\nTotal: %d Ci = %d\n\n", machines[i].used, Ci(&machines[i]));
	}
	
	//printf("\n%d\n", machines[0].jobs.used);
	//return;
	
	printf("\nGEROU SOLUCAO INICIAL\n");
	printf("Makespan: %d\n", makespan());
	
	return;
}

int Ci(Array *machine) {
	int i=0,sum=0;
	for(;i < machine->used;i++)
		sum += machine->array[i];
	
	return sum;
}

// How much makespan C(max) is?
int makespan() {
	int i = 0, worstTime = 0, worstMachine, tmp;
	for(;i<totalMachines;i++) {
		tmp = Ci(&machines[i]);
		if(tmp > worstTime) {
			worstTime = tmp;
			worstMachine = i;
		}
	}
	return worstTime;
}

// Uses insertion and interchange movements and tries to find best neighboor.
// Moves: Interchange and Insertion 1x
// Stop criteria: 15 moves without get better (lesser) Cmax
// Neighboorhood size: 0.1*(n*(m-1))
// RNVD?
void localSearch() {
	int iteration = 0, equalRounds = 0, mach1,mach2,job1,job2,tmp,worst;
	// How many iterations I'll make to get the best.  10% of totalJobs*(totalMachines-1)
	int iterations = 0.1*(totalJobs*(totalMachines-1));
	if(debug)
		printf("TOTAL ITERATIONS: %d\n", iterations);
	
	while(equalRounds < 15) {
		// Let's get the worst solution so far
		worst = makespan();

		// Find best interchange
		while(iteration < iterations) {
			// I need two machines. Find them randomly
			mach1 = getRand(0, totalMachines-1);
			mach2 = getRand(0, totalMachines-1);

			// In these machines, get a random job
			job1 = getRand(0,machines[mach1].used-1);
			job2 = getRand(0,machines[mach2].used-1);

			// Now interchange them
			//printf("TESTE: %d %d\n", machines[0].array[0], machines[0].array[1]);
			//printf("BEFORE:\nMaq %d Job %d: %d\nMaq %d Job %d: %d\n",mach1, job1, machines[mach1].array[job1], mach2,job2,machines[mach2].array[job2]);
			//printf("makespan: %d\n\n", makespan());
			tmp = machines[mach1].array[job1];
			machines[mach1].array[job1] = machines[mach2].array[job2];
			machines[mach2].array[job2] = tmp;
			//printf("AFTER:\nMaq %d Job %d: %d\nMaq %d Job %d: %d\n\n",mach1, job1, machines[mach1].array[job1], mach2,job2,machines[mach2].array[job2]);
			//printf("makespan: %d\n\n", makespan());
			//return;

			// I'll now check if this solution is better than the best one.
			// I'll allow movements without makespan time changes just to do the movement
			if(makespan() > worst) {
				// That's not a good option, let's rollback
				tmp = machines[mach1].array[job1];
				machines[mach1].array[job1] = machines[mach2].array[job2];
				machines[mach2].array[job2] = tmp;
			}

			// Ok, that was one iteration
			iteration++;
		}
		if(debug)
			printf("\nmakespan final interchange: %d\n", makespan());

		// Reset iteration
		iteration = 0;
		// Get new makespan
		worst = makespan();

		// Ok, now that I've the best Interchange, let's get this solution and make Insertion mods
		while(iteration < iterations) {
			// I need two machines, one to retrieve, one to receive. Find them randomly
			mach1 = getRand(0, totalMachines-1);
			//Maybe after too many inserts, this machine got 0 jobs inside it. I need to avoid this selecting another machine
			while(machines[mach1].used-1 == 0)
				mach1 = getRand(0, totalMachines-1);

			// Machine2, where the job'll be placed
			mach2 = getRand(0, totalMachines-1);

			// One job from mach1
			job1 = getRand(0,machines[mach1].used-1);

			// Insert this job in mach2
			insertArray(&machines[mach2], machines[mach1].array[job1]);

			// Remove this job from mach1
			removeArray(&machines[mach1], job1);

			// Let's check if this is a good solution (better than the last one)
			if(makespan() > worst) {
				// Rollback
				insertArray(&machines[mach1], machines[mach2].array[machines[mach2].used-1]);
				removeArray(&machines[mach2], machines[mach2].used-1);
			}

			// Ok, that was an iteration
			iteration++;
		}
		if(debug)
			printf("\nmakespan final insertion: %d\n", makespan());
	
		// Now let's check if makespan() = worst.
		if(makespan() == worst)
			equalRounds++;
		else
			equalRounds = 0;
	} // end equalRounds
	
	// That's it, localSearch is done
	return;
}

void GA() {
	//
}

void generateMenu() {
	// Imprime o menu para o usuario escolher
	printf("########## HEURISTICS ARTICLE ##########\n\n");
	printf("1 - Generates initial solution\n");
	printf("2 - Local Search\n");
	printf("3 - Genetic Algorithm (GA)\n");
	printf("4 - Iterated Greedy\n");
	printf("5 - VNS\n");
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