void GRASP_construction() {
	Array RCL; // Restricted Candidate List
	float total, alpha = 20;
	int i,j,k,tmp,choosedMach,time;
	
	total = round(totalMachines*(alpha/100));
	if(total == 0)
		total = 1;
	
	// For each job, get the alpha% array of best machines
	// O(nm)
	for(i=0;i<totalJobs;i++) {
		initArray(&RCL, total);
		
		// Walks around all machines and check one by one
		for(j=0;j<totalMachines;j++) {
			// get pij
			time = jobs[i].job.tempo.array[j];
			
			// First, if RCL.used != total, just add this machine to RCL because RCL still not full of values
			if(RCL.used < total)
				insertArray(&RCL, j);
			// Check if he's one of the alpha best solutions
			else { // RCL is full, need to check if the could enter inside it
				// Check if this time is better than the worst one (last inside RCL)
				if(time < jobs[i].job.tempo.array[RCL.array[RCL.used-1]])
					RCL.array[RCL.used-1] = j;
			}
			// Reorganize ASC RCL
			for(k=0;k<RCL.used-1;k++) {
				if(RCL.array[k] > RCL.array[k+1]) {
					tmp = RCL.array[k+1];
					RCL.array[k+1] = RCL.array[k];
					RCL.array[k] = tmp;
				}
			}
		}
		// Select one machine from RCL
		choosedMach = RCL.array[getRand(0,RCL.used-1)];
		insertArray(&machines[choosedMach], i);
	}
}

void GRASP() {
	int bestSolution = INT_MAX, i,j;
	Array backup[totalMachines];
	
	
		
	// As I can't use memcpy because of pointers, just use a custom function that rebuilds an instance to another
	for(i=0;i<totalMachines;i++)
		initArray(&backup[i], totalJobs);
	// now it's good to copy
	for(i=0;i<totalMachines;i++) 
		copyArray(&machines[i], &backup[i]);	
	
	// Main loop
	for(i=0;i<10;i++) {
			printf("Melhor solucao inicial: %d\nIteracao: %d\n\n", bestSolution, i);
			// Initialize number of machines.
			for(j=0;j<totalMachines;j++)
				initArray(&machines[j], totalJobs);
			GRASP_construction();
			//localSearch();
			VNS();
			
			if(makespan() < bestSolution) {
				bestSolution = makespan();
			}
			/*else {
				// Rollback
				for(j=0;j<totalMachines;j++)
						copyArray(&backup[j], &machines[j]);	
			}*/
	}
	
	printf("\n\nGRASP FINAL SOLUTION: %d\n", bestSolution);
}


