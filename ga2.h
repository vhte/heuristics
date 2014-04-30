/* 
 * File:   ga2.h
 * Author: victor
 *
 * Created on April 27, 2014, 12:47 PM
 */


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
	int tamanhoPopulacao = 10,i,j,k,l,tmp,duplicou,pai1C,pai2C,pCrossover,pMutacao,t,maqTarefa1,posTarefa1,maqTarefa2,posTarefa2;
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
	pMutacao = 7; // Aplicar radioatividade?
	
	// Tempo 0
	t = 0;
	
	while(t < 1) {
		t++;
		
		// Tell no second generation population exists
		haFilhos = false;
		houveMutacao = false;
		
		// CRUZAMENTO
		if(getRand(0,100) < pCrossover) {
			haFilhos = true;
			
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
				printf("Analisando job %d\n",i);
				for(j=0;j<totalMachines;j++) {
					printf("Analisando a Maquina %d para o job %d\n", j, i);
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
							filhoGerado[maqTarefa1].array[posTarefa1] = -1;
						else
							// maqTarefa1 é melhor para sustentar aquele job, pega o job na maqTarefa2 e seta para -1 (substituir)
							filhoGerado[maqTarefa2].array[posTarefa2] = -1;
					}
				}
			}
			
			// Agora corro com os jobs que não estão ali e preencho aleatoriamente os "-1"
			for(i=0;i<totalMachines;i++) {
				for(j=0;j<filhoGerado[i].used;j++) {
					if(filhoGerado[i].array[j] == -1) {
						printf("HEY1 %d\n",tarefasNaoEncontradas.used-1);
						tmp = getRand(0,tarefasNaoEncontradas.used-1);
						printf("HEY2\n");
						//filhoGerado[i].array[j] = tarefasNaoEncontradas.array[tmp];
						printf("HEY3\n");
						
						//removeArray(&tarefasNaoEncontradas, tmp);
						printf("HEY4\n");
					}
				}
			}
			
			printf("FILHO DEPOIS DO CROSSOVER\n");
			for(i=0;i<totalMachines;i++) {
				printf("Maquina %d: ", i);
				for(j=0;j<filhoGerado[i].used;j++)
					printf("%d ", filhoGerado[i].array[j]);
				printf("\n");
			}
		}
	}
}



