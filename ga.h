/* 
 * File:   ga.h
 * Author: victor
 *
 * Created on March 25, 2014, 8:00 PM
 */
// Genetic Algorithm
// Another possible representation: n-ary. <3,7,1,2> means job 0 is in machine 3, job 2 is in machine 7...
// Actual representation: chromosome = machine AND gene = job
// Gene = Job
void GA() {
	int t, worst, neighborhoodSize, pCrossover, pMutacao,tmp,tmp2,maqMutacaoFilho1,maqMutacaoFilho2;
	float pGenesAfetados,pTrocaGenes,pAtual;
	int m1, m2, menor, maior, i,j,k,maqCorrente,halfJobs,job1,job2,ciPais,ciFilhos,qte1,qte2;
	int roleta[2]; // 0 - Pais 1 - Filhos (talvez mutados)
	char menorC;
	bool haFilhos, houveMutacao;
	
	Array maquinaFilho1, maquinaFilho2, listaTrocarFilho1, listaTrocarFilho2, listaMutarFilho1, listaMutarFilho2, octaveX, octaveY;
	
	// Tempo 0
	t = 0;
	
	// 10% da populacao. Caso não tenha isto de rodadas sem melhora, fim.
	neighborhoodSize = 0.1*(totalJobs*(totalMachines-1));
	
	// Probabilidade de crossover
	pCrossover = 80; // 80% (80)
	
	// Probabilidade pMutacao
	pMutacao = 7; // Aplicar radioatividade?
	
	// Porcentagem de cada cromossomo de genes a serem mutados (entre 2 genes, pegará o de menor qte de cromossomos)
	// Trabalha com CROSSOVER
	pGenesAfetados = 1;
	
	// Porcentagem de genes do menor cromossomo entre 2 cromossomos que irá fazer interchange com genes 
	// TRABALHA COM MUTACAO
	pTrocaGenes = 1;
	
	// Gerando população inicial P(t) - Já é machines[]
	// Cada cromossomo {s1, s2, s3, s3...} será uma maquina, e é essa populacao que muda, pois vou selecionar 2 maquinas e trocar os jobs e avaliar os sobreviventes e substituir.
	// Dado m1 e m2, irei pegar 50% do maior e substituir no menor (porque m1 > m2)
	
	// Avaliando P(t)
	worst = makespan();
	
	if(debug) {
		printf("Configurações:\nPorcentagem de entrar em Crossover: %d%\nPorcentagem de entrar na Mutação: %d%\nPorcentagem de genes afetados no Crossover:%.0f%\nPorcentagem de genes afetados na Mutação:%.0f%\n", pCrossover, pMutacao,pGenesAfetados,pTrocaGenes);
		system("read -p \"Aperte qualquer tecla para prosseguir...\"");
	}
	
	// Octave (printar a evolucao do AG)
	initArray(&octaveX, 1); // Stores ASCII values to plot AG results
	initArray(&octaveY, 1); // Stores ASCII values to plot AG results
	
	// Enquanto critério de parada  não estiver satisfeito
	while(t < 10000) {
		insertArray(&octaveX, t);

		// Tell no second generation population exists
		haFilhos = false;
		houveMutacao = false;
	
		if(debug)
			printf("\n\nIteração %d\n", t);
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
		if(debug) {
			printf("Escolheu o primeiro pai (pior máquina. A que gera o makespan). Máquina %d\n", m1);
			for(i=0;i<machines[m1].used;i++)
				printf("%d ", machines[m1].array[i]);
			printf("\nCi(%d): %d\n", m1, Ci(m1));
		}
		
		// Segundo pai
		m2 = getRand(0,totalMachines-1);
		while(machines[m2].used == 0 || m1 == m2) {
			//printf("(m1)machines[%d].used = %d\n(m2)machines[%d].used = %d\n\n",  m1, machines[m1].used,m2, machines[m2].used);
			m2 = getRand(0,totalMachines-1);
		}
		
		tmp = getRand(0,totalMachines-1);
		
		// nao deixa pegar a mesma maquina nem a que já foi como primeiro pai NEM MAQUINAS VAZIAS
		while(tmp == m2 || tmp == m1 || machines[tmp].used == 0) {
			//printf("(tmp) machines[%d].used = %d\n(m1) machines[%d].used = %d\n(m2) machines[%d].used = %d\n\n", tmp, machines[tmp].used, m1, machines[m1].used, m2, machines[m2].used);
			tmp = getRand(0,totalMachines-1);
		}

		if(Ci(m2) > Ci(tmp))
			// Primeiro pai
			m2 = tmp;
		if(debug) {
			printf("Realizou torneio binário entre 2 possíveis pais e escolheu Máquina %d por ter menor Ci (o que nao ajuda muito)\n", m2);
			for(i=0;i<machines[m2].used;i++) 
				printf("%d ", machines[m2].array[i]);
			printf("\nCi(%d): %d\n", m2, Ci(m2));
			system("read -p \"Aperte qualquer tecla para prosseguir...\"");
		}
		
		// Dois filhos com tamanho 1, ou seja, nenhum filho gerado até agora (caso exista apenas mutação).
		// Cria-os apenas para não dar core dumped caso não entre no crossover (e mutação). Quem controla se os filhos existem ou não é haFilhos
		initArray(&maquinaFilho1, 1);
		initArray(&maquinaFilho2, 1);

		// Cruzamento (Crossover) (80%)
		if(getRand(0,100) < pCrossover) {
			haFilhos = true;
			// Com os dois pais setados, iniciar 2 filhos, cada qual com o tamanho 1 (insertArray() tratará de aumentar o tamanho deles automaticamente a  medida do necessário)
			initArray(&maquinaFilho1, 1);
			initArray(&maquinaFilho2, 1);

			// Serão gerados 2 filhos, cruzando os pais
			// Os filhos serão cópias dos pais m1 e m2, adicinados interchange de pGenesAfetados% entre eles aleatoriamente se entrou no crossover. E pTrocaGenes% (remover/inserir) no caso da mutacao
			// Gerando o 1o filho - Cópia de m1 + pGenesAfetados% de interchange com m2
			copyArray(&machines[m1], &maquinaFilho1);
			// Inicio o array que vai ter a lista de POSICOES de maquinaFilho1 a trocar com maquinaFilho2
			initArray(&listaTrocarFilho1, 1);
			
			// Gerando o 2o filho, cópia de m2
			copyArray(&machines[m2], &maquinaFilho2);
			// Inicio o array que vai ter a lista de POSICOES de maquinaFilho1 a trocar com maquinaFilho2
			initArray(&listaTrocarFilho2, 1);
			
			// Agora adicionado os jobs a fazerem interchange
			if(maquinaFilho1.used < maquinaFilho2.used)
				pAtual = round(maquinaFilho1.used*(pGenesAfetados/100));
			else
				pAtual = round(maquinaFilho2.used*(pGenesAfetados/100));
			if(pAtual < 1)
				pAtual = 1;
			
			if(debug) {
				printf("\n\nIniciou maquinaFilho1 como cópia da máquina %d\n", m1);
				for(i=0;i<maquinaFilho1.used;i++)
					printf("%d ", maquinaFilho1.array[i]);
				printf("\nIniciou maquinaFilho2 como cópia da máquina %d\n", m2);
				for(i=0;i<maquinaFilho2.used;i++)
					printf("%d ", maquinaFilho2.array[i]);
				printf("\nInicou as listasTrocaFilho* e a qte de jobs de crossover será baseada na menor maquina (que tem menos jobs): %.0f\n", pAtual);
				system("read -p \"Aperte qualquer tecla para prosseguir...\"");
			}
			
			// Referencia para pGenesAfetados sera baseado em maquinaFilho1
			// listaTrocarFilho1 e 2 tem indices de machine1 e 2
			tmp2 = 0;
			for(i=0;i<pAtual;i++) {
				j = getRand(0,maquinaFilho1.used-1);
				while(inArray(j, &listaTrocarFilho1))
					j = getRand(0,maquinaFilho1.used-1);
				
				// Esse job é bom para a troca?
				if((jobs[maquinaFilho1.array[j]].job.tempo.array[m1] > jobs[maquinaFilho1.array[j]].job.tempo.array[m2]) || tmp2 > pAtual)
					// É bom, insere
					insertArray(&listaTrocarFilho1, j);
				else {
					//printf("i: %d - O job %d na maquinaFilho1 tem tempo em m1 de %d e m2 de %d. Nao trocou. tmp2: %d\n", i,maquinaFilho1.array[j], jobs[maquinaFilho1.array[j]].job.tempo.array[m1], jobs[maquinaFilho1.array[j]].job.tempo.array[m2],tmp2);
					tmp2++;
					i--;
				}
			}

			tmp2 = 0;
			for(i=0;i<pAtual;i++) {
				j = getRand(0,maquinaFilho2.used-1);
				while(inArray(j, &listaTrocarFilho2))
					j = getRand(0,maquinaFilho2.used-1);
				
				// Esse job é bom para a troca?
				if((jobs[maquinaFilho2.array[j]].job.tempo.array[m2] > jobs[maquinaFilho2.array[j]].job.tempo.array[m1]) || tmp2 > pAtual)
					// É bom, insere
					insertArray(&listaTrocarFilho2, j);
				else {
					//printf("i: %d - O job %d na maquinaFilho2 tem tempo em m2 de %d e m1 de %d. Nao trocou. tmp2: %d\n", i, maquinaFilho2.array[j], jobs[maquinaFilho2.array[j]].job.tempo.array[m2], jobs[maquinaFilho2.array[j]].job.tempo.array[m1],tmp2);
					tmp2++;
					i--;
				}
			}
			
			if(debug) {
				printf("\n\nAs listas listaTrocarFilho foram geradas.\nlistaTrocarFilho1: ");
				for(i=0;i<listaTrocarFilho1.used;i++)
					printf("%d ", listaTrocarFilho1.array[i]);
				printf("\nlistaTrocarFilho2: ");
				for(i=0;i<listaTrocarFilho2.used;i++)
					printf("%d ", listaTrocarFilho2.array[i]);
				printf("\n");
				system("read -p \"Aperte qualquer tecla para prosseguir...\"");
			}
			
			
			// Agora, pego a listaTrocarFilho1 e troco as posicoes na listaTrocarFilho2
			for(i=0;i<listaTrocarFilho1.used;i++) {
				tmp = maquinaFilho1.array[listaTrocarFilho1.array[i]];
				maquinaFilho1.array[listaTrocarFilho1.array[i]] = maquinaFilho2.array[listaTrocarFilho2.array[i]];
				maquinaFilho2.array[listaTrocarFilho2.array[i]] = tmp;
			}
			
			if(debug) {
				printf("\n\nFez o crossover entre maquinaFilho1 e maquinaFilho2\nmaquinaFilho1: ");
				for(i=0;i<maquinaFilho1.used;i++)
					printf("%d ", maquinaFilho1.array[i]);
				printf("\nCi(%d): ", m1);
				ciFilhos = 0;
				for(i=0;i<maquinaFilho1.used;i++) {
					ciFilhos += jobs[maquinaFilho1.array[i]].job.tempo.array[m1];
				}
				printf("%d", ciFilhos);
				printf("\nmaquinaFilho2: ", m2);
				for(i=0;i<maquinaFilho2.used;i++)
					printf("%d ", maquinaFilho2.array[i]);
				printf("\nCi(%d): ", m2);
				ciFilhos = 0;
				for(i=0;i<maquinaFilho2.used;i++) {
					ciFilhos += jobs[maquinaFilho2.array[i]].job.tempo.array[m2];
				}
				printf("%d", ciFilhos);
				printf("\n");
				system("read -p \"Aperte qualquer tecla para prosseguir...\"");
			}
			/*
			printf("Filho1 (total: %d) sem crossover/mutacao\n", maquinaFilho1.used);
			for(i=0;i<maquinaFilho1.used;i++) {
				printf("%d ", maquinaFilho1.array[i]);
			}
			printf("\n\nFilho2 (total: %d) sem crossover/mutacao\n", maquinaFilho2.used);
			for(i=0;i<maquinaFilho2.used;i++) {
				printf("%d ", maquinaFilho2.array[i]);
			}
			printf("\n");
			//exit(-1);
			*/
		}
		
		// Mutação - Remove/Insert
		if(getRand(0,100) < pMutacao && haFilhos) {
			houveMutacao = true;
			
			// Muta os filhos com remove/insert aleatório de acordo com pTrocaGenes (cromossomos mutados: 1 à pTrocaGenes%). Pelo menos 1 cromossomo é mutado (duh!)
			initArray(&listaMutarFilho1, 1);  
			pAtual = maquinaFilho1.used*(pTrocaGenes/100);
			if(pAtual < 1)
				pAtual = 1;
			
			if(debug)
				printf("\n\nIniciou Mutacao.\nTotal de jobs que serao retirados da maquinaFilho1 e inseridos em maquinaFilho2: %.0f\n", pAtual);
			for(i=0;i</*getRand(1,*/pAtual/*)*/;i++) {
				j = getRand(0,maquinaFilho1.used-1);
				while(inArray(maquinaFilho1.array[j], &listaMutarFilho1))
					j = getRand(0,maquinaFilho1.used-1);
				insertArray(&listaMutarFilho1, maquinaFilho1.array[j]); // Insere o VALOR (job)
			}
			if(debug) {
				printf("A listaMutarFilho1 é: ");
				for(i=0;i<listaMutarFilho1.used;i++)
					printf("%d ", maquinaFilho1.array[i]);
				printf("\n");
			}
			
			
			// Possiveis mutacoes de filho2
			initArray(&listaMutarFilho2, 1);
			pAtual = maquinaFilho2.used*(pTrocaGenes/100);
			
			if(pAtual < 1)
				pAtual = 1;
			
			if(debug)
				printf("Total de jobs que serao retirados da maquinaFilho2 e inseridos em maquinaFilho1: %.0f\n",pAtual);
			for(i=0;i</*getRand(1,*/pAtual/*)*/;i++) {
				j = getRand(0,maquinaFilho2.used-1);
				while(inArray(maquinaFilho2.array[j], &listaMutarFilho2)) {
					j = getRand(0,maquinaFilho2.used-1);
				}
				insertArray(&listaMutarFilho2, maquinaFilho2.array[j]);
			}
			if(debug) {
				printf("A listaMutarFilho2 é: ");
				for(i=0;i<listaMutarFilho2.used;i++)
					printf("%d ", maquinaFilho2.array[i]);
				printf("\n");
				system("read -p \"Aperte qualquer tecla para prosseguir...\"");
			}
			
			// Ok, agora todo mundo que é de listaMutarFilho1 vai pra maquinaFilho2 e todo mundo de listaMutarFilho2 vai para maquinaFilho1
			for(i=0;i<listaMutarFilho1.used;i++) {
				// Retirar de maquinaFilho1 e adicionar em maquinaFilho2
				tmp = listaMutarFilho1.array[i];
				for(j=0;j<maquinaFilho1.used;j++) {
					if(maquinaFilho1.array[j] == tmp) {
						removeArray(&maquinaFilho1, j);
						break;
					}
				}
				//removeArray(&maquinaFilho1, listaMutarFilho1.array[i]);
				
				//insertArray(&maquinaFilho2, tmp);
				
			}
			// Vai inserir numa maquina que nao seja m1 nem m2. Isso só vai ser executado se filhos melhor que pais la na seleção de individuos aptos
			maqMutacaoFilho1 = getRand(0,totalMachines-1);
			while(maqMutacaoFilho1 == m1 || maqMutacaoFilho1 == m2)
				maqMutacaoFilho1 = getRand(0,totalMachines-1);
			
			for(i=0;i<listaMutarFilho2.used;i++) {
				// Retirar de maquinaFilho2 e adicionar em maquinaFilho1
				tmp = listaMutarFilho2.array[i];
				for(j=0;j<maquinaFilho2.used;j++) {
					if(maquinaFilho2.array[j] == tmp) {
						removeArray(&maquinaFilho2, j);
						break;
					}
				}
				//removeArray(&maquinaFilho2, listaMutarFilho2.array[i]);
				
				//insertArray(&maquinaFilho1, tmp);
				
			}
			maqMutacaoFilho2 = getRand(0,totalMachines-1);
			while(maqMutacaoFilho2 == m1 || maqMutacaoFilho2 == m2 || maqMutacaoFilho2 == maqMutacaoFilho1)
				maqMutacaoFilho2 = getRand(0,totalMachines-1);
			
			if(debug) {
				printf("\n\nE assim ficou apos a mutacao\nmaquinaFilho1: ");
				for(i=0;i<maquinaFilho1.used;i++)
					printf("%d ", maquinaFilho1.array[i]);
				printf("\nCi(%d): ", m1);
				ciFilhos = 0;
				for(i=0;i<maquinaFilho1.used;i++) {
					ciFilhos += jobs[maquinaFilho1.array[i]].job.tempo.array[m1];
				}
				printf("%d", ciFilhos);
				printf("\nmaquinaFilho2: ");
				for(i=0;i<maquinaFilho2.used;i++)
					printf("%d ", maquinaFilho2.array[i]);
				printf("\nCi(%d): ", m2);
				ciFilhos = 0;
				for(i=0;i<maquinaFilho2.used;i++) {
					ciFilhos += jobs[maquinaFilho2.array[i]].job.tempo.array[m2];
				}
				printf("%d", ciFilhos);
				printf("\n");
				system("read -p \"Aperte qualquer tecla para prosseguir...\"");
			}
			
		}
		
		// Avaliação (pais e filhos[se existirem])
		// 3 Avaliações (com os pais [nao mutados talvez com os mutados] e com os filhos [talvez mutados])
		// Feita da seguinte forma: Soma dos tempos das duas máquinas. Ci(pai1)+Ci(pai2), Ci(filho1)+Ci(filho2)
		//ciPais = Ci(m1) + Ci(m2);
		ciPais = Ci(m1); // maquina mais cara

		// Se tem filho, soma tambem
		if(haFilhos) {
			ciFilhos = 0; // Reseto, pois sei que existem

			// O filho1 corresponde a maquina m1

			for(i=0;i<maquinaFilho1.used;i++) {
				ciFilhos += jobs[maquinaFilho1.array[i]].job.tempo.array[m1];
			}
			tmp = ciFilhos;
			ciFilhos = 0;
			for(i=0;i<maquinaFilho2.used;i++) {
				ciFilhos += jobs[maquinaFilho2.array[i]].job.tempo.array[m2];
			}
			if(ciFilhos < tmp)
				ciFilhos = tmp; // Máquina mais cara dos filhos


		}
			

		// Sobreviventes (Roleta: Chance de sobrevivencia proporcional ao nível de aptidão C(i) das duas piores máquinas [pais e filhos])
		// Se filhos, então analisar de acordo com pais
		if(haFilhos) {

			// Roleta nos pais e nos filhos
			// Pega o maior, será a referência 100%
		
			// Regra de três para porcentagem. Aptidão relativa
			// 0 - Chance Pais Mutados
			roleta[0] = 100 - (ciPais * 100 / (ciPais+ciFilhos));
			// 1 - Chance Filhos
			roleta[1] = 100 - (ciFilhos * 100 / (ciPais+ciFilhos));

			// Preciso saber do	 menor, pois como é uma roleta de 2, se for menor que menor, então ele é escolhido, caso contrário o maior
			menor = (roleta[0] < roleta[1] ? 0 : 1);
			menorC = (menor == 0 ? 'p' : 'f');
			tmp = getRand(0,100);
			
			// Roubando na roleta (forçando resultados)
			/*if(menorC == 'p') {
				roleta[0] = 0;
				roleta[1] = 100;
			}
			else {
				roleta[0] = 100;
				roleta[1] = 0;
			}*/
			if(debug) {
				printf("\n\nRoleta\nChance de aceitar os pais: %d%\nChance de aceitar os filhos: %d%\nValor da roleta: %d\n", roleta[0], roleta[1], tmp);
				system("read -p \"Aperte qualquer tecla para prosseguir...\"");
			}
			if(roleta[0] > roleta[1]) { // A chance de pais ser escolhido é maior
				if(tmp < roleta[1]) { // se a roleta for menor que o menor (filhos), trocar por filhos.
					//freeArray(&machines[m1]);
					//freeArray(&machines[m2]);
					initArray(&machines[m1], 1);
					initArray(&machines[m2], 1);
					
					for(i=0;i<maquinaFilho1.used;i++)
						insertArray(&machines[m1], maquinaFilho1.array[i]);
					for(i=0;i<maquinaFilho2.used;i++)
						insertArray(&machines[m2], maquinaFilho2.array[i]);
					
					// Se houve mutacao, realizar a insercao nas maquinas
					if(houveMutacao) {
						for(i=0;i<listaMutarFilho1.used;i++)
							insertArray(&machines[maqMutacaoFilho1], listaMutarFilho1.array[i]);
						
						for(i=0;i<listaMutarFilho2.used;i++)
							insertArray(&machines[maqMutacaoFilho2], listaMutarFilho2.array[i]);
					}
				}
			}
			else { // pais <= filhos
				if(tmp >= roleta[0]) { // se a roleta for maior que o menor (pais), trocar por filhos
					//freeArray(&machines[m1]);
					//freeArray(&machines[m2]);
					initArray(&machines[m1], 1);
					initArray(&machines[m2], 1);
					
					for(i=0;i<maquinaFilho1.used;i++)
						insertArray(&machines[m1], maquinaFilho1.array[i]);
					for(i=0;i<maquinaFilho2.used;i++)
						insertArray(&machines[m2], maquinaFilho2.array[i]);
					
					// Se houve mutacao, realizar a insercao nas maquinas
					if(houveMutacao) {
						for(i=0;i<listaMutarFilho1.used;i++)
							insertArray(&machines[maqMutacaoFilho1], listaMutarFilho1.array[i]);
						
						for(i=0;i<listaMutarFilho2.used;i++)
							insertArray(&machines[maqMutacaoFilho2], listaMutarFilho2.array[i]);
					}
				}
			}

		}
		else {
			// Nada, pois só há pais e estes não são mutados, nada mudou.
		}
		
		freeArray(&maquinaFilho1);
		freeArray(&maquinaFilho2);
		

		/*printf("\n\nFIM ITERACAO GA\nm1: ");
		for(i=0;i<machines[m1].used;i++)
			printf("%d ", machines[m1].array[i]);
		printf(" Ci(): %d\nm2: ", Ci(m1));
		for(i=0;i<machines[m2].used;i++)
			printf("%d ", machines[m2].array[i]);
		printf("Ci(): %d\n", Ci(m2));
		
		printf("MAKESPAN ITERACAO %d: %d\n", t, makespan());
		*/
		insertArray(&octaveY, makespan());
		
	}
	
	
	printf("\n\nFim AG. Makespan: %d\n", makespan());
	printf("Executou busca local\n");
	localSearch();
	printf("Fim AG. Makespan: %d\n", makespan());
	
	int worstTime = 0, worstMachine;
	for(;i<totalMachines;i++) {
		//printf("machine %d ",i);
		tmp = Ci(i);
		
		if(tmp > worstTime) {
			worstTime = tmp;
			worstMachine = i;
		}
	}
	
	
	printf("Maquina que detem o makespan: %d com tempo: %d\n", worstMachine, worstTime);
	printf("jobs: ");
	for(i=0;i<machines[worstMachine].used;i++)
		printf("%d ", machines[worstMachine].array[i]);
	printf("\n");
	
	// Armazena em arquivo resultado do octave
	report('G', &octaveX, &octaveY, totalJobs, totalMachines);
}