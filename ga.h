/* 
 * File:   ga.h
 * Author: victor
 *
 * Created on March 25, 2014, 8:00 PM
 */
// Genetic Algorithm
// n-ary representation. <3,7,1,2> means job 0 is in machine 3, job 2 is in machine 7...
// Cromossomo = Maquina
// Gene = Job
// @todo Melhorar a verificacao se filhos foram gerados pelo cruzamento na hora de mutar
void GA() {
	int t, worst, neighborhoodSize, pCrossover, pMutacao,tmp;
	int m1, m2, menor, maior, i,j,halfJobs,job1,job2,ciPais,ciFilhos = INT_MAX;
	int roleta[2];
	
	Array backup[totalMachines], maquinaFilho1, maquinaFilho2;
	
printf("Array backup criado\n");
	
	// Tempo 0
	t = 0;
	
	// 10% da populacao. Caso não tenha isto de rodadas sem melhora, fim.
	neighborhoodSize = 0.1*(totalJobs*(totalMachines-1));
	
	// Probabilidade de crossover
	pCrossover = 80; // 80% (80)
	
	// Probabilidade pMutacao
	pMutacao = 2;
	
	// Gerando população inicial P(t) - Já é machines[]
	// Cada cromossomo {s1, s2, s3, s3...} será uma maquina, e é essa populacao que muda, pois vou selecionar 2 maquinas e trocar os jobs e avaliar os sobreviventes e substituir.
	// Dado m1 e m2, irei pegar 50% do maior e substituir no menor (porque m1 > m2)
	
	// Avaliando P(t)
	worst = makespan();
	
	// Enquanto critério de parada  não estiver satisfeito
	while(t < 1) {
		// Resets ciFilhos (inform that they doesn't exists)
		ciFilhos = INT_MAX;
		
		// Copies to check if has or not mutation
		for(tmp=0;tmp<totalMachines;tmp++)
			initArray(&backup[tmp], totalJobs);
		// now it's good to copy
		for(tmp=0;tmp<totalMachines;tmp++) {
			copyArray(&machines[tmp], &backup[tmp]);
		}
		t++;
		// Seleção dos pais (binary tournament) - Motivos: Não pressionar tanto seleção (pressão de seleção) e não correr risco de convergência prematura
		// Escolher duas máquinas (cromossomos) aleatórios 2x, dessas 2x, pegar a que tem menor Ci()
		m1 = getRand(0,totalMachines);
		while(machines[m1].used == 0)
			m1 = getRand(0,totalMachines);
		tmp = getRand(0,totalMachines);
		
		// nao deixa pegar a mesma maquina NEM MAQUINAS VAZIAS
		while(tmp == m1 ||  machines[tmp].used == 0) {
			printf("tmp (%d) == m1 (%d)\n", tmp, m1);
			printf("machines[%d].used = %d\nmachines[%d].used = %d\n\n", tmp, machines[tmp].used, m1, machines[m1].used);
			tmp = getRand(0,totalMachines);
		}
		
		if(Ci(m1) > Ci(tmp))
			// Primeiro pai
			m1 = tmp;
		
		// Segundo pai
		m2 = getRand(0,totalMachines);
		while(machines[m2].used == 0 || m1 == m2) {
			printf("(m1)machines[%d].used = %d\n(m2)machines[%d].used = %d\n\n",  m1, machines[m1].used,m2, machines[m2].used);
			m2 = getRand(0,totalMachines);
		}
		
		tmp = getRand(0,totalMachines);
		
		// nao deixa pegar a mesma maquina nem a que já foi como primeiro pai NEM MAQUINAS VAZIAS
		while(tmp == m2 || tmp == m1 || machines[tmp].used == 0) {
			printf("(tmp) machines[%d].used = %d\n(m1) machines[%d].used = %d\n(m2) machines[%d].used = %d\n\n", tmp, machines[tmp].used, m1, machines[m1].used, m2, machines[m2].used);
			tmp = getRand(0,totalMachines);
		}

		if(Ci(m2) > Ci(tmp))
			// Primeiro pai
			m2 = tmp;
printf("Pais selecionados. m1: %d e m2: %d\n", m1, 2);
		// Dois filhos com tamanho 0, ou seja, nenhum filho gerado até agora (caso exista apenas mutação)
		initArray(&maquinaFilho1, 0);
		initArray(&maquinaFilho2, 0);
printf("Iniciou maquinaFilho1 e maquinaFilho2 com 0 de tamanho\n");
		// Cruzamento (Crossover) (80%)
		if(getRand(0,100) < pCrossover) {
printf("\n\nENTROU CROSSOVER\n");		
			// Com os dois pais setados, iniciar 2 filhos, cada qual com o tamanho de pai m1 e m2
			initArray(&maquinaFilho1, machines[m1].used);
			initArray(&maquinaFilho2, machines[m2].used);
printf("Iniciou maquinaFilho1 com %d e maquinaFilho2 com %d de tamanho\n", machines[m1].used, machines[m2].used);
			// Verificar qual das duas máquinas tem menos jobs, pois o corte será feito por ela
		
			if(machines[m1].used > machines[m2].used) {
				menor = m2;
				maior = m1;
			}
			else {
				menor = m1;
				maior = m2;
			}
printf("m1.used: %d m2.used: %d\nmaior:%d menor:%d\n", machines[m1].used, machines[m2].used, maior, menor);
			// Da máquina que tem menor jobs, pega 50% dela e fazer crossover com a outra maquina
			halfJobs = machines[menor].used/2;
printf("halfJobs: %d\n", halfJobs);
				// verificando se copiou certo
				printf("Pai1 - Maquina %d (total: %d)\n", m1, machines[m1].used);
				for(i=0;i<machines[m1].used;i++) {
					printf("%d ", machines[m1].array[i]);
				}
				printf("\n\nPai2 - Maquina %d (total: %d)\n", m2, machines[m2].used);
				for(i=0;i<machines[m2].used;i++) {
					printf("%d ", machines[m2].array[i]);
				}
			
			// Serão gerados 2 filhos, cruzando os pais
			// Gero o 1o filho. 1 metade do menor, 2a metade do maior
			for(i=0;i<halfJobs;i++) {
				insertArray(&maquinaFilho1, machines[menor].array[i]);
			}
			// Insiro o restante de maior a partir de halfJobs
			for(i=halfJobs;i<machines[maior].used;i++) {
				insertArray(&maquinaFilho1, machines[maior].array[i]);
			}
			
			// Gero o 2o filho. 1a metade do maior, 2a metade do menor
			for(i=0;i<machines[menor].used - halfJobs-1;i++) { // -1
				insertArray(&maquinaFilho2, machines[maior].array[i]);
			}
			// Insiro o restante de maior a partir de halfJobs // -1
			for(i=machines[menor].used - halfJobs-1;i<machines[menor].used;i++) {
				insertArray(&maquinaFilho2, machines[menor].array[i]);
			}
			
			
				printf("\n\nFilho1 (total: %d)\n", maquinaFilho1.used);
				for(i=0;i<maquinaFilho1.used;i++) {
					printf("%d ", maquinaFilho1.array[i]);
				}
				printf("\n\nFilho2 (total: %d)\n", maquinaFilho2.used);
				for(i=0;i<maquinaFilho2.used;i++) {
					printf("%d ", maquinaFilho2.array[i]);
				}
				printf("\n");
				//exit(-1);
		}
		
		// Mutação (2%) - Interchange 1x
		if(getRand(0,100) < pMutacao) {
printf("\n\nENTROU MUTACAO\n");
			// Muta pais e filhos. Se não houver filhos, apenas os pais
			// verificando se copiou certo
			printf("Pai1 - Maquina %d (total: %d)\n", m1, machines[m1].used);
			for(i=0;i<machines[m1].used;i++) {
				printf("%d ", machines[m1].array[i]);
			}
			printf("\n\nPai2 - Maquina %d (total: %d)\n", m2, machines[m2].used);
			for(i=0;i<machines[m2].used;i++) {
				printf("%d ", machines[m2].array[i]);
			}
			// Mutando os pais
			// In these machines, get a random job (position)
			job1 = getRand(0,machines[m1].used-1);
			job2 = getRand(0,machines[m2].used-1);

			// Now interchange them
			tmp = machines[m1].array[job1];
			machines[m1].array[job1] = machines[m2].array[job2];
			machines[m2].array[job2] = tmp;
			printf("\nMUTOU!\n");
			printf("Pai1 - Maquina %d (total: %d)\n", m1, machines[m1].used);
			for(i=0;i<machines[m1].used;i++) {
				printf("%d ", machines[m1].array[i]);
			}
			printf("\n");
			printf("\n\nPai2 - Maquina %d (total: %d)\n", m2, machines[m2].used);
			for(i=0;i<machines[m2].used;i++) {
				printf("%d ", machines[m2].array[i]);
			}
			printf("\n");
			
			// Há filhos? Mute-os
			if(maquinaFilho1.size != 0) {
				// In these machines, get a random job
				job1 = getRand(0,maquinaFilho1.used-1);
				job2 = getRand(0,maquinaFilho2.used-1);

				// Now interchange them
				tmp = maquinaFilho1.array[job1];
				maquinaFilho1.array[job1] = maquinaFilho2.array[job2];
				maquinaFilho2.array[job2] = tmp;
			}
			
		}
		
		// Avaliação (pais e filhos[se existirem])
		// 2 Avaliações (com os pais [talvez mutados] e com os filhos [talvez mutados])
		// Feita da seguinte forma: Soma dos tempos das duas máquinas. Ci(pai1)+Ci(pai2), Ci(filho1)+Ci(filho2)
		ciPais = Ci(m1) + Ci(m2);
printf("ciPais: %d\n", ciPais);		
		// Se tem filho, soma tambem
		// @todo Fazer Ci receber um Array e somar por ele
		if(maquinaFilho1.size != 0) {
			ciFilhos = 0; // Reseto, pois sei que existem
printf("Ha filhos, vou fazer ciFilhos que por enquanto é 0\n");
			// O filho1 corresponde OU a maquina m1 OU m2
			// ATENCAO!!!!!!!!!!!!!!!!!!!!!!! TEM QUE VER QUAL MAQUINA FILHO1 SERA, PODEM SER IGUAIS!
			for(i=0;i<maquinaFilho1.used;i++)
				ciFilhos += jobs[maquinaFilho1.array[i]].job.tempo.array[(machines[m1].used == maquinaFilho1.used ? m1 : m2)];
			for(i=0;i<maquinaFilho2.used;i++)
				ciFilhos += jobs[maquinaFilho1.array[i]].job.tempo.array[(machines[m2].used == maquinaFilho2.used ? m2 : m1)];
printf("ciFilhos: %d\n", ciFilhos);			
		}
			
printf("SOBREVIVENTES\n");
		// Sobreviventes (Roleta: Chance de sobrevivencia proporcional ao nível de aptidão C(i))
		// Se filhos, então analisar de acordo com pais
		if(ciFilhos != INT_MAX) {
printf("Ha ciFilhos\n");
			// Roleta nos pais e nos filhos
			// Pega o maior, será a referência 100%
		
			// Regra de três para porcentagem. Aptidão relativa
			// 0 - Chance Pais
			roleta[0] = ciPais * 100 / (ciPais+ciFilhos);
			// 1 - Chance Filhos
			roleta[1] = ciFilhos * 100 / (ciPais+ciFilhos);
printf("Chance de escolher os pais[0]: %d com os filhos[1]: %d (SEM REVERSO)\n", roleta[0], roleta[1]);
			// Preciso saber do	 menor, pois como é uma roleta de 2, se for menor que menor, então ele é escolhido, caso contrário o maior
			menor = (roleta[0] < roleta[1] ? 0 : 1);
			if(getRand(0,100) > roleta[menor]) { // Como menor Ci é referencia, inverte-se a condição. Quem for maior na porcentagem tem menos chance de ser escolhido.
				// O que tinha menos chances foi escolhido, mudar no machines[]
				// Independente se os pais foram mutados ou não, já está dentro de machines[], tenho que ver se foram os filhos para assim substituir no lugar dos pais
				if(menor == 1) {
					// TROCAR OS FILHOS, ATENCAO POIS M1.SIZE PODE SER DIFERENTE PAI1.SIZE
					if(maquinaFilho1.used == machines[m1].used) {
						// Trocar maquinaFilho1 na m1 e maquinaFilho2 na m2
						for(i=0;i<machines[m1].used;i++)
							machines[m1].array[i] = maquinaFilho1.array[i];
						for(i=0;i<machines[m2].used;i++) 
							machines[m2].array[i] = maquinaFilho2.array[i];
					}
					else {
						// trocar maquinaFilho1 na m2 e maquinaFilho2 na m1
						for(i=0;i<machines[m1].used;i++)
							machines[m1].array[i] = maquinaFilho2.array[i];
						for(i=0;i<machines[m2].used;i++) 
							machines[m2].array[i] = maquinaFilho1.array[i];
					}
				}
			}
			else {
				// O que tinha maiores foi escolhido, mudar no machines[]
				// Mesma explicação acima, troca os filhos se foram mutados
				if(menor == 1) {
					// TROCAR OS FILHOS, ATENCAO POIS M1.SIZE PODE SER DIFERENTE PAI1.SIZE
					if(maquinaFilho1.used == machines[m1].used) {
						// Trocar maquinaFilho1 na m1 e maquinaFilho2 na m2
						for(i=0;i<machines[m1].used;i++)
							machines[m1].array[i] = maquinaFilho1.array[i];
						for(i=0;i<machines[m2].used;i++) 
							machines[m2].array[i] = maquinaFilho2.array[i];
					}
					else {
						// trocar maquinaFilho1 na m2 e maquinaFilho2 na m1
						for(i=0;i<machines[m1].used;i++)
							machines[m1].array[i] = maquinaFilho2.array[i];
						for(i=0;i<machines[m2].used;i++) 
							machines[m2].array[i] = maquinaFilho1.array[i];
					}
				}
				
			}
			
			
		}
		else {
			// Avaliar só os pais. Verificar se foram mutados, e se foram, roleta!
			// Se Ci de machines[] != Ci de backup, houve mutação.
			printf("Só vai avaliar os pais pois ciFilhos == INT_MAX\n");
			// Pego do backup pra saber os originais
			ciPais = 0;
			for(i=0;i<backup[m1].used;i++)
				ciPais += backup[m1].array[i];
			for(i=0;i<backup[m2].used;i++)
				ciPais += backup[m2].array[i];
			if(Ci(m1)+Ci(m2) >= ciPais) { // >= para incentivar a mutação
				// Backup é melhor
				for(i=0;i<machines[m1].used;i++)
					machines[m1].array[i] = backup[m1].array[i];
				for(i=0;i<machines[m2].used;i++) 
					machines[m2].array[i] = backup[m2].array[i];
				
			}
		}
		
		freeArray(&maquinaFilho1);
		freeArray(&maquinaFilho2);
		
		// limpa backup
		for(tmp=0;tmp<totalMachines;tmp++)
			freeArray(&backup[tmp]);
		
		printf("\n\nFIM ITERACAO GA%d - %dA\nm1: ",machines[m1].used,machines[m2].used);
		for(i=0;i<machines[m1].used;i++)
			printf("%d ", machines[m1].array[i]);
		printf(" Ci(): %d\nm2: ", Ci(m1));
		for(i=0;i<machines[m2].used;i++)
			printf("%d ", machines[m2].array[i]);
		printf("Ci(): %d\n", Ci(m2));
		
		
	}
	
	
	printf("\n\nFim AG. Makespan: %d\n", makespan());
}