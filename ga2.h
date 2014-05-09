/* 
 * File:   ga2.h
 * Author: victor
 *
 * Created on April 27, 2014, 12:47 PM
 */
void buscaLocalGA2(Array *individuo) {
	
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
			mach1 = getCmaxMachineGA2(individuo);//getRand(0, totalMachines-1);
			
			//Maybe after too many inserts, this machine got 0 jobs inside it. I need to avoid this selecting another machine
			mach2 = getRand(0, totalMachines-1);
			while(individuo[mach2].used == 0 || mach2 == mach1) // Not the same machine
				mach2 = getRand(0, totalMachines-1);
		
			// In these machines, get a random job
			job1 = getRand(0,individuo[mach1].used-1);
			job2 = getRand(0,individuo[mach2].used-1);
			// Now interchange them
			
			
			tmp = individuo[mach1].array[job1];
			individuo[mach1].array[job1] = individuo[mach2].array[job2];
			individuo[mach2].array[job2] = tmp;
			
			
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
				worst = makespanGA2(individuo);
			}
			// Rollback to generate another neighbor
			tmp = individuo[mach1].array[job1];
			individuo[mach1].array[job1] = individuo[mach2].array[job2];
			individuo[mach2].array[job2] = tmp;
		}
		else { // insertion
			// I need two machines, one to retrieve, one to receive. Find them randomly
			mach1 = getCmaxMachine();//getRand(0, totalMachines-1);
			//Maybe after too many inserts, this machine got 0 jobs inside it. I need to avoid this selecting another machine
			/*while(machines[mach1].used == 0)
				mach1 = getRand(0, totalMachines-1);*/

			// Machine2, where the job'll be placed
			mach2 = getNotACmaxMachineGA2(individuo);//getRand(0, totalMachines-1);
			while(mach2 == mach1) // Not the same machine
				mach2 = getRand(0, totalMachines-1);
				
			// One job from mach1
			job1 = getRand(0,individuo[mach1].used-1);
			// Insert this job in mach2
			insertArray(&individuo[mach2], individuo[mach1].array[job1]);
			
			// Remove this job from mach1
			removeArray(&individuo[mach1], job1);
			
			// I'll now check if this solution is better than the best neighbor so far.
			if(makespan() < worst) {
				
				// Ok, that's the best neighbor now. Let's store it details
				mov = 'i'; //insertion
				bestMach1 = mach1; // One which job1 is removed
				bestJob1 = job1;
				bestMach2 = mach2; // One which job1 is reallocated
				// To make the change, just remove used-1 pos from mach2 and allocate it's value to mach1
				
				// Update worst value to this neighbor makespan()
				worst = makespanGA2(individuo);
			}
			
			// Rollback to get another neighbor
			insertArray(&individuo[mach1], individuo[mach2].array[individuo[mach2].used-1]);
			removeArray(&individuo[mach2], individuo[mach2].used-1);
		
		}

		// Ok, after neighbor found, check if it's worse is better than actual makespan() accepting equal values (so we can run around neighborhood)
		if(worst < makespan()) {
			// The neighbor is better than actual solution
			// Make the change
			
			
			if(mov == 'c') {
				tmp = individuo[bestMach1].array[bestJob1];
				individuo[bestMach1].array[bestJob1] = individuo[bestMach2].array[bestJob2];
				individuo[bestMach2].array[bestJob2] = tmp;
				
			}
			else { // mov == 'i'
				insertArray(&individuo[mach2], individuo[mach1].array[individuo[mach1].used-1]);
				removeArray(&individuo[mach1], individuo[mach1].used-1);
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
int makespanGA2(Array *individuo) {
	
	int i = 0, worstTime = 0, worstMachine, tmp;
	for(;i<totalMachines;i++) {
		//printf("machine %d ",i);
		tmp = CiGA2(&individuo[i], i);
		
		if(tmp > worstTime) {
			worstTime = tmp;
			worstMachine = i;
		}
	}
	
	return worstTime;
}
int getCmaxMachineGA2(Array *individuo) {
	int Cmax = 0, ci, worstMachine, i,cWorstMachines = 0;
	int worstMachines[totalMachines];
	// Hint: Maybe there's two machines holding Cmax... after too many iterations, this could happen
	for(i=0;i < totalMachines;i++) {
		ci = CiGA2(&individuo[i], i);
		// Ok, that's the worse machine
		if(ci > Cmax) {
			Cmax = ci;
			// Resets worstMachines counter
			cWorstMachines = 0;
			// Reset worstMachines and add this machine to the array
			memset(worstMachines, 0, sizeof(worstMachines));
			worstMachines[cWorstMachines] = i;
		}
		// Check if we've another machine who also holds Cmax
		else if(ci == Cmax) {
			cWorstMachines++;
			worstMachines[cWorstMachines] = i;
		}
	}
	
	// Ok, now if I have worstMachines cWorstMachines > 0 I'll randomly select one of this elements (the machines who have the worst time Cmax)
	if(cWorstMachines != 0) {
		worstMachine = worstMachines[getRand(0,cWorstMachines)];
	}
	else {
		worstMachine = worstMachines[0];
	}
	return worstMachine;

}
// @todo worstMachine, change this name, it's best machine
int getNotACmaxMachineGA2(Array *individuo) {
	int Cmax = 0, ci, worstMachine, i,cWorstMachines = 0;
	int worstMachines[totalMachines];
	int achou;
	// Hint: Maybe there's two machines holding Cmax... after too many iterations, this could happen
	for(i=0;i < totalMachines;i++) {
		ci = CiGA2(&individuo[i], i);
		// Ok, that's the worse machine
		if(ci > Cmax) {
			Cmax = ci;
			// Resets worstMachines counter
			cWorstMachines = 0;
			// Reset worstMachines and add this machine to the array
			memset(worstMachines, 0, sizeof(worstMachines));
			worstMachines[cWorstMachines] = i;
		}
		// Check if we've another machine who also holds Cmax
		else if(ci == Cmax) {
			cWorstMachines++;
			worstMachines[cWorstMachines] = i;
		}
	}
	
	// Ok, now if I have cWorstMachines > 0 I'll randomly select one that is not of them
	if(cWorstMachines != 0) {
		achou =1;
		while(achou != 0) {
			achou = 0;
			worstMachine = getRand(0, totalMachines-1);
			for(i=0;i<=cWorstMachines;i++) {
				if(worstMachine == worstMachines[i])
					achou++;
			}
		}
		
	}
	// Get any machine except worstMachines[0], we've only one machine holding Cmax
	else {
		worstMachine = getRand(0, totalMachines-1);
		while(worstMachine == worstMachines[0])
			worstMachine = getRand(0, totalMachines-1);
	}
	return worstMachine;
}

int CiGA2(Array *maquina, int posicaoMaq) {
	int i, Ci=0;
	for(i=0;i<maquina->used;i++) {
		Ci += jobs[maquina->array[i]].job.tempo.array[posicaoMaq];
	}
	return Ci;
}

// @todo passar isto para o local.h/initialSolution()
void geraSolucaoInicial(Array *individuo) {
	int i,j;
	
	// Inicialmente vamos gerar apenas soluções aleatórias
	for(i=0;i<totalMachines;i++)
		initArray(&individuo[i], 1);
	
	// For each job...
	for(i=0;i<totalJobs;i++) {
		insertArray(&individuo[getRand(0,totalMachines-1)], i);
		//printf("Inseriu job %d na maquina\n", i);
	}
	
	// Just show the initial solution
	for(i=0;i<totalMachines;i++) {
		printf("\nMaquina %d: ", i);
		for(j=0;j<individuo[i].used;j++) {
			printf("%d ", individuo[i].array[j]);
		}
		
		//printf("\nTotal: %d Ci = %d\n\n", machines[i].used, Ci(&machines[i]));
		printf("\nTotal: %d Ci = %d", individuo[i].used, CiGA2(&individuo[i],i));
	}

	return;
}

// @todo localSearch()
void GA2() {
	int tamanhoPopulacao = 10,i,j,k,l,tmp,tmp2,duplicou,pai1C,pai2C,pCrossover,pMutacao,t,maqTarefa1,posTarefa1,maqTarefa2,posTarefa2, maqMutacao,tarefaMutacao,piorIndividuo,ciPiorIndividuo;
	Array populacao[tamanhoPopulacao][totalMachines];
	Array filhoGerado[totalMachines];
	Array tarefasNaoEncontradas, octaveX, octaveY;;
	bool haFilhos,houveMutacao, achou;
	
	initArray(&tarefasNaoEncontradas,1);
	printf("Iniciou populacao\n");
	// Cria tamanhoPopulacao inicial e a gera

	for(i=0;i<tamanhoPopulacao;i++) {
		geraSolucaoInicial(&populacao[i]); // Gera a maquina j do individuo i
		printf("\nGerou individuo %d\n", i);
	}
	
	for(i=0;i<tamanhoPopulacao;i++) {
		printf("\n\nINDIVIDUO %d\n", i);
		for(j=0;j<totalMachines;j++) {
			printf("Maquina %d: " , j);
			for(k=0;k<populacao[i][j].used;k++) {
				printf("%d ",populacao[i][j].array[k]);
			}
			printf("\n");
		}
	}
	printf("Primeiro job da primeira maquina do primeiro individuo: %d\n", populacao[0][0].array[0]);
	printf("Segundo job da segunda maquina do segundo individuo: %d\n", populacao[1][1].array[1]);
	
	// Octave (printar a evolucao do AG)
	initArray(&octaveX, 1); // Stores ASCII values to plot AG results
	initArray(&octaveY, 1); // Stores ASCII values to plot AG results
	
	// Probabilidade de crossover
	pCrossover = 80; // 80% (80)
	
	// Probabilidade pMutacao
	pMutacao = 10; // Aplicar radioatividade?
	
	// Tempo 0
	t = 0;
	
	while(t < 1000) {
		insertArray(&octaveX, t);
		t++;
		
		// Tell no second generation population exists
		haFilhos = false;
		houveMutacao = false;
		
		// CRUZAMENTO
		if(getRand(0,100) < pCrossover) {
			haFilhos = true;
			initArray(&tarefasNaoEncontradas,1);
			
			// Invididuos "pais": Os individuos da populacao que tem melhores Ci()
			pai1C = getRand(0,tamanhoPopulacao-1);
			pai2C = getRand(0,tamanhoPopulacao-1);
			while(pai2C == pai1C)
				pai2C = getRand(0,tamanhoPopulacao-1);
			//pai1C = 8;pai2C=9;
			// Iniciando as maquinas de filhoGerado
			for(i=0;i<totalMachines;i++)
				initArray(&filhoGerado[i], 1);
			
			// Gerar um filho com 50% de pai e 50% de pai2
			for(i=0;i<totalMachines/2;i++) //populacao[0][0].array[0]
				for(j=0;j<populacao[pai1C][i].used;j++) 
					insertArray(&filhoGerado[i], populacao[pai1C][i].array[j]);
			
			for(i=totalMachines/2;i<totalMachines;i++)
				for(j=0;j< populacao[pai2C][i].used;j++) 
					insertArray(&filhoGerado[i], populacao[pai2C][i].array[j]);
			
			printf("\nFilho gerado atraves das combinacoes dos pais %d e %d\n", pai1C, pai2C);
			for(i=0;i<totalMachines;i++) {
				printf("Maquina %d: ", i);
				for(j=0;j<filhoGerado[i].used;j++)
					printf("%d ", filhoGerado[i].array[j]);
				printf("\n");
			}
			
				
			printf("\nCi(Filho): %d\n", CiGA2(&filhoGerado[0],0));
			
			// Gera uma lista com jobs nao presentes no filho
			for(i=0;i<totalJobs;i++) {
				achou = false;
				//printf("Analisando job %d\n",i);
				for(j=0;j<totalMachines;j++) {
					//printf("Analisando a Maquina %d para o job %d\n", j, i);
					for(k=0;k<filhoGerado[j].used;k++) {
						if(filhoGerado[j].array[k] == i)
							achou = true;
					}
				}
				// Se nao achou, coloca na lista de nao achados, pois será inserido posteriormente
				if(!achou)
					insertArray(&tarefasNaoEncontradas, i);
			}
			
			printf("Criou a lista de tarefasNaoEncontradas com %d elementos\n", tarefasNaoEncontradas.used);
			for(i=0;i<tarefasNaoEncontradas.used;i++)
				printf("%d ", tarefasNaoEncontradas.array[i]);
			printf("\n");
			
			// Busco no indivíduo gerado, todas as tarefas duplicadas
			for(i=0;i<totalMachines;i++) {
				for(j=0;j<filhoGerado[i].used;j++) {
					if(filhoGerado[i].array[j] == -1) // Quer dizer que perdeu o torneio binário, nada a fazer por enquanto
						continue;
					// Para cada tarefa verifico se ela já foi alocada em outro lugar.
					duplicou = 0;
					maqTarefa1 = posTarefa1 = maqTarefa2 = posTarefa2 = -1;
					for(k=0;k<totalMachines;k++) {
						for(l=0;l<filhoGerado[k].used;l++) {
							if(filhoGerado[i].array[j] == filhoGerado[k].array[l]) {
								// Achou a tarefa, colocar na maqTarefa? posTarefa?
								duplicou++;
								if(duplicou == 1){
									maqTarefa1 = k;
									posTarefa1 = l;
								}
								else { // segunda localizacao
									maqTarefa2 = k;
									posTarefa2 = l;
								}	
							}
							
						}
					}
					// Ok, se existir maqTarefa2, então fazer torneio binário para ver quem vai ficar
					if(maqTarefa2 != -1) {
						// Torneio
						if(jobs[filhoGerado[maqTarefa1].array[posTarefa1]].job.tempo.array[maqTarefa1] > jobs[filhoGerado[maqTarefa2].array[posTarefa2]].job.tempo.array[maqTarefa2])
							// maqTarefa2 é melhor para sustentar aquele job, pega o job na maqTarefa1 e seta para -1 (substituir)
							removeArray(&filhoGerado[maqTarefa1], posTarefa1);
							//filhoGerado[maqTarefa1].array[posTarefa1] = -1;
						else
							// maqTarefa1 é melhor para sustentar aquele job, pega o job na maqTarefa2 e seta para -1 (substituir)
							removeArray(&filhoGerado[maqTarefa2], posTarefa2);
							//filhoGerado[maqTarefa2].array[posTarefa2] = -1;
					}
				}
			}
			
			// Agora corro com os jobs que não estão ali e preencho aleatoriamente os da tarefasNaoEncontradas
			for(i=0;i<tarefasNaoEncontradas.used;i++) {
				tmp = getRand(0,totalMachines-1);
				//tmp2 = getRand(0,filhoGerado[tmp].used);
				insertArray(&filhoGerado[tmp], tarefasNaoEncontradas.array[i]);
			}
			
			printf("FILHO DEPOIS DO CROSSOVER\n");
			for(i=0;i<totalMachines;i++) {
				printf("Maquina %d: ", i);
				for(j=0;j<filhoGerado[i].used;j++)
					printf("%d ", filhoGerado[i].array[j]);
				printf("\n");
			}
			
			// MUTAÇÃO
			if(getRand(0,100) < pMutacao) {
				//for(i=0;i<10;i++) {
					// Pega o filho gerado e troca uma tarefa da máquina de Cmax com outra qualquer que não Cmax
					maqMutacao = getCmaxMachineGA2(&filhoGerado);
					printf("maqMutacao: %d\n", maqMutacao);
					tarefaMutacao = getRand(0,filhoGerado[maqMutacao].used-1);
					printf("tarefaMutacao: %d\n", tarefaMutacao);
					tmp = getNotACmaxMachineGA2(&filhoGerado);
					printf("getNotACmaxMachineGA2: %d\n", tmp);
					insertArray(&filhoGerado[tmp], filhoGerado[maqMutacao].array[tarefaMutacao]);
					removeArray(&filhoGerado[maqMutacao], tarefaMutacao);
				//}
				
			}
			
			printf("FILHO DEPOIS DA MUTACAO\n");
			for(i=0;i<totalMachines;i++) {
				printf("Maquina %d: ", i);
				for(j=0;j<filhoGerado[i].used;j++)
					printf("%d ", filhoGerado[i].array[j]);
				printf("\n");
			}
			
			// VERIFICAÇÃO DA POPULAÇÃO SOBREVIVENTE
			piorIndividuo = 0;
			ciPiorIndividuo = 0;
			for(i=0;i<tamanhoPopulacao;i++) {
				if(makespanGA2(&populacao[i]) > ciPiorIndividuo) {
					piorIndividuo = i;
					ciPiorIndividuo = makespanGA2(&populacao[i]);
				}
				printf("makespan do individuo %d: %d\n", i, makespanGA2(&populacao[i]));
			}
			printf("pior individuo %d com Ci() de %d\n", piorIndividuo, ciPiorIndividuo);
			printf("makespan do filho: %d\n", makespanGA2(&filhoGerado));
			
			// Verificando se o Ci() do filho é pior que o pior individuo na populacao
			if(makespanGA2(&filhoGerado) < ciPiorIndividuo) {
				printf("\nFILHO TEM MELHORA\n\n");
				// O filho tem melhor Ci() que o pior indivíduo na população, trocar.
				for(i=0;i<totalMachines;i++) {
					copyArray(&filhoGerado[i], &populacao[piorIndividuo][i]);
				}
			}
			
			for(i=0;i<tamanhoPopulacao;i++) {
				printf("makespan do individuo %d: %d\n", i, makespanGA2(&populacao[i]));
			}
			
			for(i=0;i<totalMachines;i++)
				freeArray(&filhoGerado[i]);
		}
		
		insertArray(&octaveY, makespanGA2(&populacao));
		
	}
	
	printf("\n\nFIM GA2\nmakespan() sem buscaLocal: %d\n", makespanGA2(&populacao));
	
	// Executa busca local
	for(i=0;i<tamanhoPopulacao;i++) {
		buscaLocalGA2(&populacao[i]);
	}
	printf("\n\nFIM GA2\nmakespan() com buscaLocal: %d\n", makespanGA2(&populacao));
	// Armazena em arquivo resultado do octave
	report('G', &octaveX, &octaveY, totalJobs, totalMachines);
}



