/* 
 * File:   vns.h
 * Author: victor
 *
 * Created on March 25, 2014, 7:59 PM
 */

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