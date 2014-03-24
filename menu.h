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
// Genetic Algorithm
// n-ary representation. <3,7,1,2> means job 0 is in machine 3, job 2 is in machine 7...
// Cromossomo = Maquina
// Gene = Job
void GA() {
	int t, worst, neighborhoodSize, pCrossover, pMutacao,tmp;
	int m1, m2, menor, i,j,halfJobs;
	
	Array backup[totalMachines];
	
	// As I can't use memcpy because of pointers, just use a custom function that rebuilds an instance to another
	for(tmp=0;tmp<totalMachines;tmp++)
		initArray(&backup[tmp], totalJobs);
	// now it's good to copy
	for(tmp=0;tmp<totalMachines;tmp++) {
		copyArray(&machines[tmp], &backup[tmp]);	
	}
	
	// Tempo 0
	t = 0;
	
	// 10% da populacao. Caso não tenha isto de rodadas sem melhora, fim.
	neighborhoodSize = 0.1*(totalJobs*(totalMachines-1));
	
	// Probabilidade de crossover
	pCrossover = 80;
	
	// Probabilidade pMutacao
	pMutacao = 2;
	
	// Gerando população inicial P(t) - Já é machines[]
	// Cada cromossomo {s1, s2, s3, s3...} será uma maquina, e é essa populacao que muda, pois vou selecionar 2 maquinas e trocar os jobs e avaliar os sobreviventes e substituir.
	// Dado m1 e m2, irei pegar 50% do maior e substituir no menor (porque m1 > m2)
	
	// Avaliando P(t)
	worst = makespan();
	
	// Enquanto critério de parada  não estiver satisfeito
	while(t < 1) {
		t++;
		// Seleção dos pais (binary tournament) - Motivos: Não pressionar tanto seleção (pressão de seleção) e não correr risco de convergência prematura
		// Escolher duas máquinas (cromossomos) aleatórios 2x, dessas 2x, pegar a que tem menor Ci()
		m1 = getRand(0,totalMachines);
		tmp = getRand(0,totalMachines);
		
		// nao deixa pegar a mesma maquina
		while(tmp == m1)
			tmp = getRand(0,totalMachines);
		
		if(Ci(m1) > Ci(tmp))
			// Primeiro pai
			m1 = tmp;
		
		// Segundo pai
		m2 = getRand(0,totalMachines);
		tmp = getRand(0,totalMachines);
		
		// nao deixa pegar a mesma maquina nem a que já foi como primeiro pai
		while(tmp == m2|| m1 == m2)
			tmp = getRand(0,totalMachines);
		
		if(Ci(m2) > Ci(tmp))
			// Primeiro pai
			m2 = tmp;
		
		// Cruzamento (Crossover) (80%)
		if(getRand(0,100) < pCrossover) {
			// Verificar qual das duas máquinas tem menos jobs, pois o corte será feito por ela
			if(machines[m1].used > machines[m2].used)
				menor = m2;
			else
				menor = m1;
			
			// Da máquina que tem menor jobs, pega 50% dela e fazer crossover com a outra maquina
			halfJobs = machines[menor].used/2;
			
			// Serão gerados 2 filhos, obedecendo a ordem dos jobs
			for(i=0;i<halfJobs;i++) {
				// Gero o 1o filho, 
			}
		}
		
		// Mutação (2%)
		if(getRand(0,100) < pMutacao) {
			//
		}
		
		// Avaliação
		
		// Sobreviventes (Roleta: Chance de sobrevivencia proporcional ao nível de aptidão C(i))
	 }
}

void VNS() {
	// Set how many different neighbors we'll get
	int neighborhoodSize = 0.1*(totalJobs*(totalMachines-1));
	int countNaoTeveMelhora = 0, k, mach1, mach2,job1,job2,tmp,cmax;
	bool teveMelhora;

	Array beforeLocalsearch[totalMachines];
	
	// As I can't use memcpy because of pointers, just use a custom function that rebuilds an instance to another
	for(tmp=0;tmp<totalMachines;tmp++)
		initArray(&beforeLocalsearch[tmp], totalJobs);
	// now it's good to copy
	for(tmp=0;tmp<totalMachines;tmp++) {
		copyArray(&machines[tmp], &beforeLocalsearch[tmp]);	
	}
	
	//memcpy(&beforeLocalsearch, &machines, sizeof machines);
	// Same as localSearch, if 15 makespan() did not change, countNaoTeveMelhora.
	while(countNaoTeveMelhora < neighborhoodSize) {
		// Get current cmax
		cmax = makespan();
		k = 0;
		teveMelhora = false;
		// Do first neighbor. If not better than current solution, 2nd neighbor... if better, resets, 1st neighbor again
		while(k < 5) {
			// 1st neighbor (1 interchange)
			if(k == 0) {
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
				tmp = machines[mach1].array[job1];
				machines[mach1].array[job1] = machines[mach2].array[job2];
				machines[mach2].array[job2] = tmp;

				// Do local search
				localSearch();
				
				//printf("interchange happened. makespan(): %d\n", makespan());
				if(makespan() < cmax) {
					//printf("A new makespan is generated in interchange: %d. Better than %d\n", makespan(),cmax);
					// No rollback, that's a good solution. Update cmax
					cmax = makespan();
					k = 0;

					for(tmp=0;tmp<totalMachines;tmp++)
						copyArray(&machines[tmp], &beforeLocalsearch[tmp]);	
					printf("Melhora. countNaoTeveMelhora= %d  k = %d makespan =  %d\n", countNaoTeveMelhora, k, makespan());
					teveMelhora = true;
					continue; // Resets while
				}
				else {
					// Rollback. This isn't a good solution. Get next neighbor
					for(tmp=0;tmp<totalMachines;tmp++)
						copyArray(&beforeLocalsearch[tmp], &machines[tmp]);	
					// end if-else structure and adds 1 in k
				}
			}
			// 2nd neighbor (1 insertion)
			else if(k == 1) {
				// Let's insert
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

				// Do local search
				localSearch();
				
				if(makespan() < cmax) {
					// No rollback, that's a good solution. Update cmax and return to the first neighbor
					cmax = makespan();
					k = 0;
					
					for(tmp=0;tmp<totalMachines;tmp++)
						copyArray(&machines[tmp], &beforeLocalsearch[tmp]);
					printf("Melhora. countNaoTeveMelhora= %d  k = %d makespan =  %d\n", countNaoTeveMelhora, k, makespan());
					teveMelhora = true;
					continue; // Resets while
				}
				else {
					// Rollback. This isn't a good solution. Get next neighbor
					for(tmp=0;tmp<totalMachines;tmp++)
						copyArray(&beforeLocalsearch[tmp], &machines[tmp]);	
					// end if-else structure and adds 1 in k
				}
			}
			// 3rd neighbor (2 interchanges)
			else if(k == 2) {
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
				tmp = machines[mach1].array[job1];
				machines[mach1].array[job1] = machines[mach2].array[job2];
				machines[mach2].array[job2] = tmp;
				
				// Interchange again
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
				tmp = machines[mach1].array[job1];
				machines[mach1].array[job1] = machines[mach2].array[job2];
				machines[mach2].array[job2] = tmp;
				
				// Do local search
				localSearch();
				
				
				//printf("interchange happened. makespan(): %d\n", makespan());
				if(makespan() < cmax) {
					//printf("A new makespan is generated in interchange: %d. Better than %d\n", makespan(),cmax);
					// No rollback, that's a good solution. Update cmax
					cmax = makespan();
					k = 0;

					for(tmp=0;tmp<totalMachines;tmp++)
						copyArray(&machines[tmp], &beforeLocalsearch[tmp]);	
					printf("Melhora. countNaoTeveMelhora= %d  k = %d makespan =  %d\n", countNaoTeveMelhora, k, makespan());
					teveMelhora = true;
					continue; // Resets while
				}
				else {
					// Rollback. This isn't a good solution. Get next neighbor
					for(tmp=0;tmp<totalMachines;tmp++)
						copyArray(&beforeLocalsearch[tmp], &machines[tmp]);	
					// end if-else structure and adds 1 in k
				}
			}
			// 4th neighbor (2 insertions)
			else if(k == 3) {
				// Let's insert
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
				
				// Do local search
				localSearch();
				
				if(makespan() < cmax) {
					// No rollback, that's a good solution. Update cmax and return to the first neighbor
					cmax = makespan();
					k = 0;
					
					for(tmp=0;tmp<totalMachines;tmp++)
						copyArray(&machines[tmp], &beforeLocalsearch[tmp]);
					printf("Melhora. countNaoTeveMelhora= %d  k = %d makespan =  %d\n", countNaoTeveMelhora, k, makespan());
					teveMelhora = true;
					continue; // Resets while
				}
				else {
					// Rollback. This isn't a good solution. Get next neighbor
					for(tmp=0;tmp<totalMachines;tmp++)
						copyArray(&beforeLocalsearch[tmp], &machines[tmp]);	
					// end if-else structure and adds 1 in k
				}
			}
			// 5th neighbor (2 interchanges before 2 insertions)
			else if(k == 4) {
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
				tmp = machines[mach1].array[job1];
				machines[mach1].array[job1] = machines[mach2].array[job2];
				machines[mach2].array[job2] = tmp;
				
				// Interchange again
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
				tmp = machines[mach1].array[job1];
				machines[mach1].array[job1] = machines[mach2].array[job2];
				machines[mach2].array[job2] = tmp;
				
				// Let's insert
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
				
				// Do local search
				localSearch();
				
				if(makespan() < cmax) {
					// No rollback, that's a good solution. Update cmax and return to the first neighbor
					cmax = makespan();
					k = 0;
					
					for(tmp=0;tmp<totalMachines;tmp++)
						copyArray(&machines[tmp], &beforeLocalsearch[tmp]);
					printf("Melhora. countNaoTeveMelhora= %d  k = %d makespan =  %d\n", countNaoTeveMelhora, k, makespan());
					teveMelhora = true;
					continue; // Resets while
				}
				else {
					// Rollback. This isn't a good solution. Get next neighbor
					for(tmp=0;tmp<totalMachines;tmp++)
						copyArray(&beforeLocalsearch[tmp], &machines[tmp]);	
					// end if-else structure and adds 1 in k
				}
			}
			
			k++;
		}
		
		if(teveMelhora)
			countNaoTeveMelhora = 0;
		else
			countNaoTeveMelhora++;
		//for(tmp=0;tmp<totalMachines;tmp++)
			//copyArray(&machines[tmp], &beforeLocalsearch[tmp]);
		// Copy machines to beforeLocalSearch
		printf("===== AFTER ALL NEIGHBORHOOD countNaoTeveMelhora: %d  VNS k = %d: %d\n", countNaoTeveMelhora, k, makespan());
	}
	printf("VNS: %d\n", makespan());
	return;
}

void generateMenu() {
	time_t start,countNaoTeveMelhora;
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
			time(&countNaoTeveMelhora);
			printf("Finished in about %.0f seconds. \n", difftime(countNaoTeveMelhora, start));
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