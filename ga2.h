/* 
 * File:   ga2.h
 * Author: victor
 *
 * Created on April 27, 2014, 12:47 PM
 */
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

void GA2() {
	int tamanhoPopulacao = 10,i,j,k,l,tmp,tmp2,duplicou,pai1C,pai2C,pCrossover,pMutacao,t,maqTarefa1,posTarefa1,maqTarefa2,posTarefa2, maqMutacao,tarefaMutacao,piorIndividuo,ciPiorIndividuo;
	Array populacao[tamanhoPopulacao][totalMachines];
	Array pai1,pai2,filhoGerado[totalMachines];
	Array tarefasNaoEncontradas;
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
	
	// Probabilidade de crossover
	pCrossover = 80; // 80% (80)
	
	// Probabilidade pMutacao
	pMutacao = 5; // Aplicar radioatividade?
	
	// Tempo 0
	t = 0;
	
	while(t < 30) {
		t++;
		
		// Tell no second generation population exists
		haFilhos = false;
		houveMutacao = false;
		
		// CRUZAMENTO
		if(getRand(0,100) < pCrossover) {
			haFilhos = true;
			initArray(&tarefasNaoEncontradas,1);
			
			// Invididuos "pais"
			pai1C = getRand(0,tamanhoPopulacao-1);
			pai2C = getRand(0,tamanhoPopulacao-1);
			while(pai2C == pai1C)
				pai2C = getRand(0,tamanhoPopulacao-1);
			pai1C = 8;pai2C=9;
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
				// Pega o filho gerado e troca uma tarefa da máquina de Cmax com outra qualquer que não Cmax
				maqMutacao = getCmaxMachineGA2(&filhoGerado);
				printf("maqMutacao: %d\n", maqMutacao);
				tarefaMutacao = getRand(0,filhoGerado[maqMutacao].used-1);
				printf("tarefaMutacao: %d\n", tarefaMutacao);
				tmp = getNotACmaxMachineGA2(&filhoGerado);
				printf("getNotACmaxMachineGA2: %d\n", tmp);
				insertArray(&filhoGerado[tmp], filhoGerado[maqMutacao].array[tarefaMutacao]);
				removeArray(&filhoGerado[maqMutacao], tarefaMutacao);
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
		
	}
	
	printf("\n\nFIM GA2\nmakespan(): %d\n", makespanGA2(&populacao));
}



