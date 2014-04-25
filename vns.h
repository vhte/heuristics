/* 
 * File:   vns.h
 * Author: victor
 *
 * Created on March 25, 2014, 7:59 PM
 */


// VNS neighbors are more "aggressive" because normal moves (insertion/interchange) are inside localSearch. Let this method do the simple steps
void VNS() {
	// Set how many different neighbors we'll get
	int neighborhoodSize = 0.1*(totalJobs*(totalMachines-1));
	int countNaoTeveMelhora = 0,i, k, mach1, mach2,job1,job2,tmp,cmax,contadorOctave;
	bool teveMelhora;
	//teste
	int agressividade = 1,countAgressividade=0;

	Array beforeLocalsearch[totalMachines], octaveX, octaveY;
	
	// As I can't use memcpy because of pointers, just use a custom function that rebuilds an instance to another
	for(tmp=0;tmp<totalMachines;tmp++)
		initArray(&beforeLocalsearch[tmp], totalJobs);
	// now it's good to copy
	for(tmp=0;tmp<totalMachines;tmp++) {
		copyArray(&machines[tmp], &beforeLocalsearch[tmp]);	
	}
	
	// Octave (printar a evolucao do AG)
	initArray(&octaveX, 1); // Stores ASCII values to plot AG results
	initArray(&octaveY, 1); // Stores ASCII values to plot AG results
	contadorOctave = 0;
	
	//memcpy(&beforeLocalsearch, &machines, sizeof machines);
	// Same as localSearch, if 15 makespan() did not change, countNaoTeveMelhora.
	while(countNaoTeveMelhora < 500) { //neighborhoodSize
		// Get current cmax
		cmax = makespan();
		k = 0;
		teveMelhora = false;
		countAgressividade++;
		if(countAgressividade < 250)
			agressividade = 1;
		else if(countAgressividade < 350)
			agressividade = 2;
		else
			agressividade = 3;
		
		
		// Do first neighbor. If not better than current solution, 2nd neighbor... if better, resets, 1st neighbor again
		// HINT: mach1 in a insertion movement is always the machine who holds Cmax, so we always remove one job from it and Cmax'll be lesser than before :D
		// Leave small moves (agressividade interchange and 1 insertion to localSearch)
		while(k < 6) {
			// 1st neighbor (agressividade interchange 2 insertions)
			if(k == 0) {
				moveInterchange(agressividade);
				moveInsertion(agressividade);
				moveInsertion(agressividade);

				// Do local search
				localSearch();
				
				//printf("interchange happened. makespan(): %d\n", makespan());
				if(makespan() < cmax) {
					//printf("A new makespan is generated in interchange: %d. Better than %d\n", makespan(),cmax);
					// No rollback, that's a good solution. Update cmax
					cmax = makespan();

					for(tmp=0;tmp<totalMachines;tmp++)
						copyArray(&machines[tmp], &beforeLocalsearch[tmp]);	
					printf("Melhora. countNaoTeveMelhora= %d  k = %d makespan =  %d\n", countNaoTeveMelhora, k, makespan());
					teveMelhora = true;
					
					// Returns to the first neighbor
					k = 0;
					countAgressividade = 0;
					continue; // Resets while
				}
				else {
					// Rollback. This isn't a good solution. Get next neighbor
					for(tmp=0;tmp<totalMachines;tmp++)
						copyArray(&beforeLocalsearch[tmp], &machines[tmp]);	
					// end if-else structure and adds 1 in k
				}
			}
			// 2nd neighbor (agressividade insertion 2 interchanges)
			else if(k == 1) {
				moveInsertion(agressividade);
				moveInterchange(agressividade);
				moveInterchange(agressividade);

				// Do local search
				localSearch();
				
				if(makespan() < cmax) {
					// No rollback, that's a good solution. Update cmax
					cmax = makespan();
					
					for(tmp=0;tmp<totalMachines;tmp++)
						copyArray(&machines[tmp], &beforeLocalsearch[tmp]);
					printf("Melhora. countNaoTeveMelhora= %d  k = %d makespan =  %d\n", countNaoTeveMelhora, k, makespan());
					teveMelhora = true;
					
					// Return to the first neighbor
					k = 0;
					countAgressividade = 0;
					continue; // Resets while
				}
				else {
					// Rollback. This isn't a good solution. Get next neighbor
					for(tmp=0;tmp<totalMachines;tmp++)
						copyArray(&beforeLocalsearch[tmp], &machines[tmp]);	
					// end if-else structure and adds 1 in k
				}
			}
			// 3rd neighbor (2 interchanges after 2 insertions)
			else if(k == 2) {
				moveInsertion(agressividade);
				moveInsertion(agressividade);
				moveInterchange(agressividade);
				moveInterchange(agressividade);
				
				// Do local search
				localSearch();
				
				
				//printf("interchange happened. makespan(): %d\n", makespan());
				if(makespan() < cmax) {
					//printf("A new makespan is generated in interchange: %d. Better than %d\n", makespan(),cmax);
					// No rollback, that's a good solution. Update cmax
					cmax = makespan();

					for(tmp=0;tmp<totalMachines;tmp++)
						copyArray(&machines[tmp], &beforeLocalsearch[tmp]);	
					printf("Melhora. countNaoTeveMelhora= %d  k = %d makespan =  %d\n", countNaoTeveMelhora, k, makespan());
					teveMelhora = true;
					
					// Return to the first neighbor
					k = 0;
					countAgressividade = 0;
					continue; // Resets while
				}
				else {
					// Rollback. This isn't a good solution. Get next neighbor
					for(tmp=0;tmp<totalMachines;tmp++)
						copyArray(&beforeLocalsearch[tmp], &machines[tmp]);	
					// end if-else structure and adds 1 in k
				}
			}
			// 4th neighbor (3 insertions)
			else if(k == 3) {
				moveInsertion(agressividade);
				moveInsertion(agressividade);
				moveInsertion(agressividade);
				
				// Do local search
				localSearch();
				
				if(makespan() < cmax) {
					// No rollback, that's a good solution. Update cmax
					cmax = makespan();
					
					for(tmp=0;tmp<totalMachines;tmp++)
						copyArray(&machines[tmp], &beforeLocalsearch[tmp]);
					printf("Melhora. countNaoTeveMelhora= %d  k = %d makespan =  %d\n", countNaoTeveMelhora, k, makespan());
					teveMelhora = true;
					
					// Return to the first neighbor*
					k = 0;
					countAgressividade = 0;
					continue; // Resets while
				}
				else {
					// Rollback. This isn't a good solution. Get next neighbor
					for(tmp=0;tmp<totalMachines;tmp++)
						copyArray(&beforeLocalsearch[tmp], &machines[tmp]);	
					// end if-else structure and adds 1 in k
				}
			}
			// 5th neighbor (3 interchanges)
			else if(k == 4) {
				moveInterchange(agressividade);
				moveInterchange(agressividade);
				moveInterchange(agressividade);
				
				// Do local search
				localSearch();
				
				if(makespan() < cmax) {
					// No rollback, that's a good solution. Update cmax
					cmax = makespan();
					
					for(tmp=0;tmp<totalMachines;tmp++)
						copyArray(&machines[tmp], &beforeLocalsearch[tmp]);
					printf("Melhora. countNaoTeveMelhora= %d  k = %d makespan =  %d\n", countNaoTeveMelhora, k, makespan());
					teveMelhora = true;
					
					// Return to the first neighbor
					k = 0;
					countAgressividade = 0;
					continue; // Resets while
				}
				else {
					// Rollback. This isn't a good solution. Get next neighbor
					for(tmp=0;tmp<totalMachines;tmp++)
						copyArray(&beforeLocalsearch[tmp], &machines[tmp]);	
					// end if-else structure and adds 1 in k
				}
			}
			// 5th neighbor (3 insertions interspersed by 3 interchanges)
			else if(k == 5) {
				moveInsertion(agressividade);
				moveInterchange(agressividade);
				moveInsertion(agressividade);
				moveInterchange(agressividade);
				moveInsertion(agressividade);
				moveInterchange(agressividade);
				
				// Do local search
				localSearch();
				
				if(makespan() < cmax) {
					// No rollback, that's a good solution. Update cmax
					cmax = makespan();
					
					
					for(tmp=0;tmp<totalMachines;tmp++)
						copyArray(&machines[tmp], &beforeLocalsearch[tmp]);
					printf("Melhora. countNaoTeveMelhora= %d  k = %d makespan =  %d\n", countNaoTeveMelhora, k, makespan());
					teveMelhora = true;
					
					// Return to the first neighbor
					k = 0;
					countAgressividade = 0;
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
		// After movement, insert result in report
		insertArray(&octaveX, contadorOctave++);
		insertArray(&octaveY, makespan());
		
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
	// Armazena em arquivo resultado do octave
	//report('V', &octaveX, &octaveY, totalJobs, totalMachines);
	
	
	
	return;
}
