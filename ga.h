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
	int t, worst, neighborhoodSize, pCrossover, pMutacao,tmp,tmp2;
	int m1, m2, menor, maior, i,j,k,maqCorrente,halfJobs,job1,job2,ciPais,ciFilhos = INT_MAX,qte1,qte2;
	int roleta[2]; // 0 - Pais 1 - Filhos (talvez mutados)
	int totalMaq1 = 0, totalMaq2 = 0, relativoFilho1, relativoFilho2;
	char menorC;
	
	Array maquinaFilho1, maquinaFilho2, jobsPais; // backup[totalMachines], 
	
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
	while(t < 100) {
		printf("\n\nINICIO ITERACAO (%d) AG\n", t);
		// Resets ciFilhos (inform that they doesn't exists)
		ciFilhos = INT_MAX;
		totalMaq1 = 0;
		totalMaq2 = 0;
	
		t++;
		// Seleção dos pais (binary tournament no 2o pai) - Motivos: Não pressionar tanto seleção (pressão de seleção) e não correr risco de convergência prematura
		// Escolher duas máquinas (cromossomos) aleatórios 2x, dessas 2x, pegar a que tem menor Ci()
		// A primeira maquina nao será aleatória. Será a pior máquina (que segura o makespan()). Não adianta eu pegar duas aleatórias sendo que o makespan ficará inalterado
		
		// Primeiro pai (quem segura o makespan())
		for(i=0;i<totalMachines;i++) {
			if(Ci(i) == makespan()) {
				m1 = i;
				break;
			}
		}
		
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
			// Com os dois pais setados, iniciar 2 filhos, cada qual com o tamanho 1 (insertArray() tratará de aumentar o tamanho deles automaticamente a  medida do necessário)
			initArray(&maquinaFilho1, 1);
			initArray(&maquinaFilho2, 1);
printf("Iniciou maquinaFilho1 com %d e maquinaFilho2 com %d de tamanho\n", 1,1);//machines[m1].used, machines[m2].used);
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
			// Vai adicionando os jobs de todos os pais no vetor para ficar mais fácil saber quem está ou não no vetor depois quando for adicionar os jobs nos filhos
			initArray(&jobsPais, 1);
			printf("Pai1 - Maquina %d (total: %d)\n", m1, machines[m1].used);
			for(i=0;i<machines[m1].used;i++) {
				printf("%d ", machines[m1].array[i]);
				insertArray(&jobsPais, machines[m1].array[i]);
			}
			printf("\n\nPai2 - Maquina %d (total: %d)\n", m2, machines[m2].used);
			for(i=0;i<machines[m2].used;i++) {
				printf("%d ", machines[m2].array[i]);
				insertArray(&jobsPais, machines[m2].array[i]);
			}
			printf("\n");
			
			// Serão gerados 2 filhos, cruzando os pais
			// A escolha de quais genes (jobs) e quais posições será completamente aleatória, permitindo que o crossover nao se vicie nas metades exatas de cada cromossomo e possa mudar o tamho dos filhos. A ordem dos jobs (genes) nao importa dentro da maquina. Os filhos terão pelo menos 1 gene do pai1 e 1 gene do pai2
			// Gerando o 1o filho
			// Se nao tem pelo menos 1 em m1 e 1 em m2, reseta tudo e faz outro filho
			while(totalMaq1 == 0 || totalMaq2 == 0) {
				qte1 = getRand(2,(machines[m1].used + machines[m2].used)-2);
				initArray(&maquinaFilho1, qte1);
				for(i = 0;i < qte1;i++) {
					// Vai pegar da m1(0) ou m2(1)
					maqCorrente = getRand(0,1);
					
					// Gera tmp único em maquinaFilho1
					tmp = machines[(maqCorrente == 0 ? m1 : m2)].array[getRand(0,machines[(maqCorrente == 0 ? m1 : m2)].used-1)];
					// É único dentro do array?
					if(inArray(tmp, &maquinaFilho1)) {
						i--; // Roda de novo o loop
						continue;
					}
					
					if(maqCorrente == 0)
						totalMaq1++;
					else
						totalMaq2++;
					insertArray(&maquinaFilho1, tmp);
				}
			}
			// Pequena correção para maquinaFilho1 
			// maquinaFilho1 nao pode pegar todos os genes de uma maquina pai. Pelo menos 1 não deve entrar
			// Se eu achar, faço interchange com a maquina que ele nao pegou todas
			// Ok, agora verifico se algum deles (j,k) bateu com os respectivos used de m1 m2
			if(totalMaq1 == machines[m1].used) {
				// Interchange de um job aleatório pavra m2
				tmp = getRand(0,maquinaFilho1.used-1);
				while(!inArray(maquinaFilho1.array[tmp], &machines[m1])) {
					tmp = getRand(0,maquinaFilho1.used-1);
				}
				printf("saiu1\n");
				// Ok, agora pego esta posicao tmp que esta dentro de m1 e substituo pela primeira posicao de m2 que nao tenha ainda no maquinaFilho1
				//tmp2 = getRand(0,machines[m2].used-1);
				for(i=0;i<jobsPais.used;i++) {
					if(!inArray(machines[m2].array[i], &maquinaFilho1)) {
						tmp2 = i;
						break;
					}
				}
				/*while(inArray(machines[m2].array[tmp2], &maquinaFilho1)) {
						tmp2 = getRand(0,machines[m2].used-1);
				}*/
				printf("saiu2\n");
				// Ok, trocá-los....
				j = tmp;
				maquinaFilho1.array[tmp] = machines[m2].array[tmp2];
				machines[m2].array[tmp2] = maquinaFilho1.array[j];
			}
			else if(totalMaq2 == machines[m2].used) {
				// Interchange de um job aleatório para m1
				tmp = getRand(0,maquinaFilho1.used-1);
				while(!inArray(maquinaFilho1.array[tmp], &machines[m2])) {
					tmp = getRand(0,maquinaFilho1.used-1);
				}
				printf("saiu3\n");
				// Ok, agora pego esta posicao tmp que esta dentro de m1 e substituo por uma posicao de m2 que nao tenha ainda no maquinaFilho1
				tmp2 = getRand(0,machines[m1].used-1);
				while(inArray(machines[m1].array[tmp2], &maquinaFilho1)) {
						tmp2 = getRand(0,machines[m1].used-1);
				}
				printf("saiu4\n");
				// Ok, trocá-los....
				j = tmp;
				maquinaFilho1.array[tmp] = machines[m1].array[tmp2];
				machines[m1].array[tmp2] = maquinaFilho1.array[j];
			}
			
			// Última característica de maquinaFilho1, será contabilizado o Ci() na m1 ou m2? Simples, a que tiver mais jobs daquela m*
			if(totalMaq1 > totalMaq2) {
				relativoFilho1 = m1;
				relativoFilho2 = m2;
			}
			else {
				relativoFilho1 = m2;
				relativoFilho2 = m1;
			}
			printf("Filho1 será relativo à máquina %d pois tem mais jobs dela em seus cromossomos\n", relativoFilho1);
			printf("Filho2 será relativo à máquina %d pois tem mais jobs dela em seus cromossomos\n", relativoFilho2);
			
			printf("\n\nFilho1 (total: %d)\n", maquinaFilho1.used);
			for(i=0;i<maquinaFilho1.used;i++) {
				printf("%d ", maquinaFilho1.array[i]);
			}
			printf("\n");
			
			// Gerando o 2o filho, ja tenho garantia de que terá pelo menos 1 de m1 ou 1 de m2. Simplesmente vou inserindo todo mundo de jobsPais que não esteja em maquinaFilho1
			/*passouMaq1 = false;
			passouMaq2 = false;
			while(!passouMaq1 || !passouMaq2) {*/
				qte2 = (machines[m1].used + machines[m2].used) - qte1;
				printf("qte1: %d qte2: %d\n", qte1, qte2);
				//exit(-1);
				initArray(&maquinaFilho2, qte2);
				for(i=0;i<jobsPais.used;i++) {
					if(!inArray(jobsPais.array[i], &maquinaFilho1))
						insertArray(&maquinaFilho2, jobsPais.array[i]);
				}
				/*for(i = 0;i < qte2;i++) {
					// Vai pegar da m1(0) ou m2(1)
					j = getRand(0,1);
					
					// Gera tmp único em maquinaFilho1
					tmp = machines[(j == 0 ? m1 : m2)].array[getRand(0,machines[(j == 0 ? m1 : m2)].used-1)];
					// É único dentro do array?
					if(inArray(tmp, &maquinaFilho1) || inArray(tmp, &maquinaFilho2)) {
						i--; // Roda de novo o loop
						printf("tmp(%d) está dentro de maquinaFilho1 (0) ou maquinaFilho2(1). j: %d. Nao posso duplicar jobs\n", tmp, j);
						printf("maquinaFilho2 esta assim ate agora: \n");
						for(j=0;j<maquinaFilho2.used;j++) {
							printf("%d ", maquinaFilho2.array[j]);
						}
						printf("\n");
						continue;
					}
					
					insertArray(&maquinaFilho2, tmp);
				}*/
				
			//}

			
			
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
		if(getRand(0,100) < pMutacao && maquinaFilho1.size != 0) {
printf("\n\nENTROU MUTACAO FILHOS\n");
			// Muta os filhos. Se não houver filhos, void
			
			// Há filhos? Mute-os
			
				// In these machines, get a random job
				job1 = getRand(0,maquinaFilho1.used-1);
				job2 = getRand(0,maquinaFilho2.used-1);

				// Now interchange them
				tmp = maquinaFilho1.array[job1];
				maquinaFilho1.array[job1] = maquinaFilho2.array[job2];
				maquinaFilho2.array[job2] = tmp;
			
			
		}
		
		// Avaliação (pais e filhos[se existirem])
		// 3 Avaliações (com os pais [nao mutados talvez com os mutados] e com os filhos [talvez mutados])
		// Feita da seguinte forma: Soma dos tempos das duas máquinas. Ci(pai1)+Ci(pai2), Ci(filho1)+Ci(filho2)
		ciPais = (Ci(m1) > Ci(m2) ? Ci(m1) : Ci(m2));
printf("ciPais: %d\n", ciPais);
		// Se tem filho, soma tambem
		// @todo Fazer Ci receber um Array e somar por ele
		if(maquinaFilho1.size != 0) {
			ciFilhos = 0; // Reseto, pois sei que existem
printf("Ha filhos, vou fazer ciFilhos que por enquanto é 0\n");
printf("Tamanho maquinaFilho1: %d maquinaFilho2: %d\n", maquinaFilho1.used, maquinaFilho2.used);
			// O filho1 corresponde OU a maquina m1 OU m2
			for(i=0;i<maquinaFilho1.used;i++)
				ciFilhos += jobs[maquinaFilho1.array[i]].job.tempo.array[relativoFilho1];
tmp = ciFilhos;
ciFilhos = 0;
			for(i=0;i<maquinaFilho2.used;i++)
				ciFilhos += jobs[maquinaFilho2.array[i]].job.tempo.array[relativoFilho2];
if(tmp > ciFilhos)
	ciFilhos = tmp;
printf("ciFilhos: %d\n", ciFilhos);
		}
			
printf("\n\nSOBREVIVENTES\n");
		// Sobreviventes (Roleta: Chance de sobrevivencia proporcional ao nível de aptidão C(i) das duas piores máquinas [pais e filhos])
		// Se filhos, então analisar de acordo com pais
		if(ciFilhos != INT_MAX) {
printf("Ha ciFilhos\n");
			// Roleta nos pais e nos filhos
			// Pega o maior, será a referência 100%
		
			// Regra de três para porcentagem. Aptidão relativa
			// 0 - Chance Pais Mutados
			roleta[0] = 100 - (ciPais * 100 / (ciPais+ciFilhos));
			// 1 - Chance Filhos
			roleta[1] = 100 -  (ciFilhos * 100 / (ciPais+ciFilhos));
printf("Chance de escolher os pais[0]: %d com os filhos[1]: %d\n", roleta[0], roleta[1]);
			// Preciso saber do	 menor, pois como é uma roleta de 2, se for menor que menor, então ele é escolhido, caso contrário o maior
			menor = (roleta[0] < roleta[1] ? 0 : 1);
			menorC = (menor == 0 ? 'p' : 'f');
			tmp = getRand(0,100);
			printf("getRand() selecao: %d menor: %d\n", tmp, menor);
			
			if(tmp < roleta[menor]) { // Como menor Ci é referencia, inverte-se a condição. Quem for maior na porcentagem tem menos chance de ser escolhido.
				// O que tinha menos chances foi escolhido, mudar no machines[]
				// Independente se os pais foram mutados ou não, já está dentro de machines[], tenho que ver se foram os filhos para assim substituir no lugar dos pais
				if(menorC == 'f') {
					initArray(&machines[m1], 1);
					initArray(&machines[m2], 1);
					
					for(i=0;i<maquinaFilho1.used;i++)
						insertArray(&machines[relativoFilho1], maquinaFilho1.array[i]);
					for(i=0;i<maquinaFilho2.used;i++)
						insertArray(&machines[relativoFilho2], maquinaFilho2.array[i]);
					// TROCAR OS FILHOS, ATENCAO POIS M1.SIZE PODE SER DIFERENTE PAI1.SIZE
					/*if(maquinaFilho1.used == machines[m1].used) {
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
					}*/
				}
			}
			else {
				// O que tinha maiores foi escolhido, mudar no machines[]
				// Mesma explicação acima, troca os filhos se foram mutados
				

				
				if(menorC == 'f') {
					initArray(&machines[m1], 1);
					initArray(&machines[m2], 1);
					for(i=0;i<maquinaFilho1.used;i++)
						insertArray(&machines[relativoFilho1], maquinaFilho1.array[i]);
					for(i=0;i<maquinaFilho2.used;i++)
						insertArray(&machines[relativoFilho2], maquinaFilho2.array[i]);
					/*
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
					}*/
				}
				
			}
			
			
		}
		else {
			// Nada, pois só há pais e estes não são mutados, nada mudou.
			/*
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
				
			}*/
		}
		
		freeArray(&maquinaFilho1);
		freeArray(&maquinaFilho2);
		

		printf("\n\nFIM ITERACAO GA\nm1: ");
		for(i=0;i<machines[m1].used;i++)
			printf("%d ", machines[m1].array[i]);
		printf(" Ci(): %d\nm2: ", Ci(m1));
		for(i=0;i<machines[m2].used;i++)
			printf("%d ", machines[m2].array[i]);
		printf("Ci(): %d\n", Ci(m2));
		
		printf("MAKESPAN ITERACAO %d: %d\n", t, makespan());
		
		
	}
	
	
	printf("\n\nFim AG. Makespan: %d\n", makespan());
	printf("Executou busca local\n");
	localSearch();
	printf("Fim AG. Makespan: %d\n", makespan());
}