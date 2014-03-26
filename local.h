/* 
 * File:   local.h
 * Author: victor
 *
 * Created on March 25, 2014, 8:06 PM
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
	//exit(1);
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
		// Find best machine to put this job i
		for(j=0;j<jobs[i].job.tempo.used;j++) {
			if(jobs[i].job.tempo.array[j] < menor) {
				menor = jobs[i].job.tempo.array[j];
				melhorMaq = jobs[i].job.maquina.array[j];
			}
		}
		// allocate job i to its best machine...
		insertArray(&machines[melhorMaq], i);
	}
	
	printf("Solucao Inicial Gulosa gerada.\n\n");
	// Just show the initial solution
	for(i=0;i<totalMachines;i++) {
		printf("Maquina %d: ", i);
		for(j=0;j<machines[i].used;j++) {
			printf("%d ", machines[i].array[j]);
		}
		//printf("\nTotal: %d Ci = %d\n\n", machines[i].used, Ci(&machines[i]));
		printf("\nTotal: %d Ci = %d\n\n", machines[i].used, Ci(i));
	}
	
	//printf("\n%d\n", machines[0].jobs.used);
	//return;
	
	printf("\nGEROU SOLUCAO INICIAL\n");
	printf("Makespan: %d\n", makespan());
	
	return;
}

int Ci(int machine) {
	int i=0,sum=0;
	//printf("machine->used: %d\n",machine->used);
	for(;i < machines[machine].used;i++)
		sum += jobs[machines[machine].array[i]].job.tempo.array[machine];
	
	return sum;
}

// How much makespan C(max) is?
int makespan() {
	
	int i = 0, worstTime = 0, worstMachine, tmp;
	for(;i<totalMachines;i++) {
		//printf("machine %d ",i);
		tmp = Ci(i);
		
		if(tmp > worstTime) {
			worstTime = tmp;
			worstMachine = i;
		}
	}
	
	return worstTime;
}

// RVND
// Uses insertion and interchange movements and tries to find best neighbor.
// Moves: Interchange and Insertion 1x
// Stop criteria: 15 moves without get better (lesser) Cmax
// Neighborhood size: 0.1*(n*(m-1))
// RNVD?
// @todo If no parameter is passed, uses global (the answer to the problem)
void localSearch() {
	int iteration = 0, vizinhanca, mach1,mach2,job1,job2,tmp,worst,bestMach1,bestMach2,bestJob1,bestJob2,k,j;
	char mov; // inter'c'hange or 'i'nsertion. Need this to replace current solution to its best neighbor
	
	// How many iterations I'll make to get the best.  10% of totalJobs*(totalMachines-1) NEIGHBORHOOD
	vizinhanca = 0.1*(totalJobs*(totalMachines-1));
	//printf("TAMANHO VIZINHANCA: %d\n", vizinhanca);
	
	// The worst solution is INT_MAX just to tell the 1st neighbor is the best and initialize
	worst = INT_MAX;
	
	while(iteration < vizinhanca) {
		// Using DescidaRandomica
		// Choose 0 for interchange, 1 for insertion
		if(getRand(0,1) == 0) {
			// Interchange
			// I need two machines. Find them randomly
			mach1 = getRand(0, totalMachines-1);
			//Maybe after too many inserts, this machine got 0 jobs inside it. I need to avoid this selecting another machine
			while(machines[mach1].used == 0)
				mach1 = getRand(0, totalMachines-1);
			//Maybe after too many inserts, this machine got 0 jobs inside it. I need to avoid this selecting another machine
			mach2 = getRand(0, totalMachines-1);
			while(machines[mach2].used == 0) // Not the same machine
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
			
			
			// I'll now check if this solution is better than the best neighbor so far.
			if(makespan() < worst) {
				// Ok, that's the best neighbor now. Let's store it details
				mov = 'c'; //interchange
				bestMach1 = mach1;
				bestMach2 = mach2;
				bestJob1 = job1;
				bestJob2 = job2;
				// Just interchange positions, piece of cake.
				// Update worst value to this neighbor makespan()
				worst = makespan();
			}
			// Rollback to generate another neighbor
			tmp = machines[mach1].array[job1];
			machines[mach1].array[job1] = machines[mach2].array[job2];
			machines[mach2].array[job2] = tmp;
		}
		else { // insertion
			// I need two machines, one to retrieve, one to receive. Find them randomly
			mach1 = getRand(0, totalMachines-1);
			//Maybe after too many inserts, this machine got 0 jobs inside it. I need to avoid this selecting another machine
			while(machines[mach1].used == 0)
				mach1 = getRand(0, totalMachines-1);

			// Machine2, where the job'll be placed
			mach2 = getRand(0, totalMachines-1);

			// One job from mach1
			job1 = getRand(0,machines[mach1].used-1);
			// Insert this job in mach2
			insertArray(&machines[mach2], machines[mach1].array[job1]);
			
			// Remove this job from mach1
			removeArray(&machines[mach1], job1);
			
			// I'll now check if this solution is better than the best neighbor so far.
			if(makespan() < worst) {
				
				// Ok, that's the best neighbor now. Let's store it details
				mov = 'i'; //insertion
				bestMach1 = mach1; // One which job1 is removed
				bestJob1 = job1;
				bestMach2 = mach2; // One which job1 is reallocated
				// To make the change, just remove used-1 pos from mach2 and allocate it's value to mach1
				
				// Update worst value to this neighbor makespan()
				worst = makespan();
			}
			
			// Rollback to get another neighbor
			insertArray(&machines[mach1], machines[mach2].array[machines[mach2].used-1]);
			removeArray(&machines[mach2], machines[mach2].used-1);
		
		}

		// Ok, after neighbor found, check if it's worse is better than actual makespan() accepting equal values (so we can run around neighborhood)
		if(worst < makespan()) {
			// The neighbor is better than actual solution
			// Make the change
			
			
			if(mov == 'c') {
				tmp = machines[bestMach1].array[bestJob1];
				machines[bestMach1].array[bestJob1] = machines[bestMach2].array[bestJob2];
				machines[bestMach2].array[bestJob2] = tmp;
				
			}
			else { // mov == 'i'
				insertArray(&machines[mach2], machines[mach1].array[machines[mach1].used-1]);
				removeArray(&machines[mach1], machines[mach1].used-1);
			}
			
			// resets iteration counter
			iteration = 0; 
		}
		else
			iteration++;
	
		/*
		// Now let's check if makespan() = worst.
		if(makespan() == worst)
			equalRounds++;
		else
			equalRounds = 0;
		*/
		
	} // end equalRounds
	//printf("makespan after RVND: %d\n", makespan());
	// That's it, localSearch RVND is done
	return;
}

// Generate Random Solution for GA
// Just get the initial solution and generates a new one doing n (number of jobs) interchanges and insertions around it
randomSolution() {
	//
}