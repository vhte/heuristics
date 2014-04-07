/* 
 * File:   ga.h
 * Author: victor
 *
 * Created on March 25, 2014, 8:00 PM
 */
// Genetic Algorithm
// Possible representation: n-ary. <3,7,1,2> means job 0 is in machine 3, job 2 is in machine 7...
// Cromossomo = Maquina
// Gene = Job
// @todo Melhorar a verificacao se filhos foram gerados pelo cruzamento na hora de mutar
void GA() {
	int t, worst, neighborhoodSize, pCrossover, pMutacao,tmp,tmp2;
	float pGenesAfetados,pTrocaGenes,pAtual;
	int m1, m2, menor, maior, i,j,k,maqCorrente,halfJobs,job1,job2,ciPais,ciFilhos = INT_MAX,qte1,qte2;
	int roleta[2]; // 0 - Pais 1 - Filhos (talvez mutados)
	int totalMaq1 = 0, totalMaq2 = 0, relativoFilho1, relativoFilho2;
	char menorC;
	bool haFilhos;
	
	Array maquinaFilho1, maquinaFilho2, listaTrocarFilho1, listaTrocarFilho2, listaMutarFilho1, listaMutarFilho2, octaveX, octaveY;

	// Tempo 0
	t = 0;
	
	// 10% da populacao. Caso não tenha isto de rodadas sem melhora, fim.
	neighborhoodSize = 0.1*(totalJobs*(totalMachines-1));
	
	// Probabilidade de crossover
	pCrossover = 80; // 80% (80)
	
	// Probabilidade pMutacao
	pMutacao = 10; // Aplicar radioatividade?
	
	// Porcentagem de cada cromossomo de genes a serem mutados (entre 2 genes, pegará o de menor qte de cromossomos)
	// Trabalha com CROSSOVER
	pGenesAfetados = 10;
	
	// Porcentagem de genes do menor cromossomo entre 2 cromossomos que irá fazer interchange com genes 
	// TRABALHA COM MUTACAO
	pTrocaGenes = 10;
	
	// Gerando população inicial P(t) - Já é machines[]
	// Cada cromossomo {s1, s2, s3, s3...} será uma maquina, e é essa populacao que muda, pois vou selecionar 2 maquinas e trocar os jobs e avaliar os sobreviventes e substituir.
	// Dado m1 e m2, irei pegar 50% do maior e substituir no menor (porque m1 > m2)
	
	// Avaliando P(t)
	worst = makespan();
	
	// Octave (printar a evolucao do AG)
	initArray(&octaveX, 1); // Stores ASCII values to plot AG results
	initArray(&octaveY, 1); // Stores ASCII values to plot AG results
	
	// Enquanto critério de parada  não estiver satisfeito
	while(t < 1000) {
		insertArray(&octaveX, t);
		printf("\n\nINICIO ITERACAO (%d) AG\n", t);
		// Resets ciFilhos (inform that they doesn't exists)
		ciFilhos = INT_MAX;
		totalMaq1 = 0;
		totalMaq2 = 0;
		haFilhos = false;
	
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
		
		printf("Pais selecionados. m1: %d e m2: %d\n", m1, 2);
		// Dois filhos com tamanho 0, ou seja, nenhum filho gerado até agora (caso exista apenas mutação)
		initArray(&maquinaFilho1, 1);
		initArray(&maquinaFilho2, 1);

		// Cruzamento (Crossover) (80%)
		if(getRand(0,100) < pCrossover) {
			printf("\n\nENTROU CROSSOVER\n");
			haFilhos = true;
			// Com os dois pais setados, iniciar 2 filhos, cada qual com o tamanho 1 (insertArray() tratará de aumentar o tamanho deles automaticamente a  medida do necessário)
			initArray(&maquinaFilho1, 1);
			initArray(&maquinaFilho2, 1);

			// Showing parents
			printf("Pai1 - Maquina %d (total: %d)\n", m1, machines[m1].used);
			for(i=0;i<machines[m1].used;i++)
				printf("%d ", machines[m1].array[i]);
			
			printf("\n\nPai2 - Maquina %d (total: %d)\n", m2, machines[m2].used);
			for(i=0;i<machines[m2].used;i++)
				printf("%d ", machines[m2].array[i]);
			printf("\n");
			
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
			
			// Referencia para pGenesAfetados sera baseado em maquinaFilho1
			// listaTrocarFilho1 e 2 tem indices de machine1 e 2
			for(i=0;i<pAtual;i++) {
				j = getRand(0,maquinaFilho1.used-1);
				while(inArray(j, &listaTrocarFilho1))
					j = getRand(0,maquinaFilho1.used-1);
				insertArray(&listaTrocarFilho1, j);
			}


			for(i=0;i<pAtual;i++) {
				j = getRand(0,maquinaFilho2.used-1);
				while(inArray(j, &listaTrocarFilho2))
					j = getRand(0,maquinaFilho2.used-1);
				insertArray(&listaTrocarFilho2, j);
			}
			/*}
			else {
				pAtual = round(maquinaFilho2.used*(pGenesAfetados/100));
				if(pAtual < 1)
					pAtual = 1;
				// Referencia para pGenesAfetados sera baseado em maquinaFilho2
				for(i=0;i<pAtual;i++) {
					j = getRand(0,maquinaFilho1.used-1);
					while(inArray(j, &listaTrocarFilho1))
						j = getRand(0,maquinaFilho1.used-1);
					insertArray(&listaTrocarFilho1, j);
				}
				
				pAtual = round(maquinaFilho2.used*(pGenesAfetados/100));
				if(pAtual < 1)
					pAtual = 1;
				for(i=0;i<pAtual;i++) {
					j = getRand(0,maquinaFilho2.used-1);
					while(inArray(j, &listaTrocarFilho2))
						j = getRand(0,maquinaFilho2.used-1);
					insertArray(&listaTrocarFilho2, j);
				}
			}*/
			
			// Agora, pego a listaTrocarFilho1 e troco as posicoes na listaTrocarFilho2
			for(i=0;i<listaTrocarFilho1.used;i++) {
				tmp = maquinaFilho1.array[listaTrocarFilho1.array[i]];
				maquinaFilho1.array[listaTrocarFilho1.array[i]] = maquinaFilho2.array[listaTrocarFilho2.array[i]];
				maquinaFilho2.array[listaTrocarFilho2.array[i]] = tmp;
			}
			
			printf("CROSSOVER. FILHOS:\n");
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
		}
		
		// Mutação - Remove/Insert
		if(getRand(0,100) < pMutacao && haFilhos) {
			printf("\n\nENTROU MUTACAO FILHOS\n");
			// Muta os filhos com remove/insert aleatório de acordo com pTrocaGenes (cromossomos mutados: 1 à pTrocaGenes%). Pelo menos 1 cromossomo é mutado (duh!)
			initArray(&listaMutarFilho1, 1);  
			pAtual = maquinaFilho1.used*(pTrocaGenes/100);
			if(pAtual < 1)
				pAtual = 1;
			for(i=0;i</*getRand(1,*/pAtual/*)*/;i++) {
				j = getRand(0,maquinaFilho1.used-1);
				while(inArray(maquinaFilho1.array[j], &listaMutarFilho1))
					j = getRand(0,maquinaFilho1.used-1);
				insertArray(&listaMutarFilho1, maquinaFilho1.array[j]); // Insere o VALOR (job)
			}
			printf("listaMutarFilho1:\n");
			for(i=0;i<listaMutarFilho1.used;i++) {
				printf("%d ", listaMutarFilho1.array[i]);
			}
			printf("\n");
			
			// Possiveis mutacoes de filho2
			initArray(&listaMutarFilho2, 1);
			pAtual = maquinaFilho2.used*(pTrocaGenes/100);
			if(pAtual <1) {
				printf("pAtual meno que 1: %d\n", pAtual);
				printf("getRand(1,1) = %d\n", getRand(1,1));
				//exit(-1);
			}
			if(pAtual < 1)
				pAtual = 1;
			for(i=0;i</*getRand(1,*/pAtual/*)*/;i++) {
				j = getRand(0,maquinaFilho2.used-1);
				while(inArray(maquinaFilho2.array[j], &listaMutarFilho2)) {
					printf("-- maquinaFilho2.used: %d, pTrocaGenes: %d, pAtual: %d\n", maquinaFilho2.used, pTrocaGenes, pAtual);
					for(tmp = 0;tmp<listaMutarFilho2.used;tmp++)
						printf("%d ", listaMutarFilho2.array[tmp]);
					printf("\n--\n");
					printf("Ha %d na listaMutarFilho2\n", maquinaFilho2.array[j]);
					j = getRand(0,maquinaFilho2.used-1);
				}
				insertArray(&listaMutarFilho2, maquinaFilho2.array[j]);
			}
			
			
			printf("listaMutarFilho2:\n");
			for(i=0;i<listaMutarFilho2.used;i++) {
				printf("%d ", listaMutarFilho2.array[i]);
			}
			printf("\n");
			
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
				
				insertArray(&maquinaFilho2, tmp);
			}
			
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
				
				insertArray(&maquinaFilho1, tmp);
			}
			
			printf("APOS MUTACAO. FILHOS ATUALIZADOS\n");
			printf("Filho1 (total: %d) sem crossover/mutacao\n", maquinaFilho1.used);
			for(i=0;i<maquinaFilho1.used;i++) {
				printf("%d ", maquinaFilho1.array[i]);
			}
			printf("\n\nFilho2 (total: %d) sem crossover/mutacao\n", maquinaFilho2.used);
			for(i=0;i<maquinaFilho2.used;i++) {
				printf("%d ", maquinaFilho2.array[i]);
			}
			printf("\n");
			
		}
		
		// Avaliação (pais e filhos[se existirem])
		// 3 Avaliações (com os pais [nao mutados talvez com os mutados] e com os filhos [talvez mutados])
		// Feita da seguinte forma: Soma dos tempos das duas máquinas. Ci(pai1)+Ci(pai2), Ci(filho1)+Ci(filho2)
		ciPais = Ci(m1) + Ci(m2);
printf("ciPais: %d\n", ciPais);
		// Se tem filho, soma tambem
		// @todo Fazer Ci receber um Array e somar por ele
		if(haFilhos) {
			ciFilhos = 0; // Reseto, pois sei que existem
printf("Ha filhos, vou fazer ciFilhos que por enquanto é 0\n");
printf("Tamanho maquinaFilho1: %d maquinaFilho2: %d\n", maquinaFilho1.used, maquinaFilho2.used);
			// O filho1 corresponde OU a maquina m1 OU m2

			for(i=0;i<maquinaFilho1.used;i++) {
				//printf("ADD Jobs: %d Maquina: %d Tempo: %d\n", maquinaFilho1.array[i], m1, jobs[maquinaFilho1.array[i]].job.tempo.array[m1]);
				ciFilhos += jobs[maquinaFilho1.array[i]].job.tempo.array[m1];
			}

			for(i=0;i<maquinaFilho2.used;i++) {
				ciFilhos += jobs[maquinaFilho2.array[i]].job.tempo.array[m2];
			}

printf("ciFilhos: %d\n", ciFilhos);
		}
			
printf("\n\nSOBREVIVENTES\n");
		// Sobreviventes (Roleta: Chance de sobrevivencia proporcional ao nível de aptidão C(i) das duas piores máquinas [pais e filhos])
		// Se filhos, então analisar de acordo com pais
		if(haFilhos) {
printf("Ha ciFilhos\n");
			// Roleta nos pais e nos filhos
			// Pega o maior, será a referência 100%
		
			// Regra de três para porcentagem. Aptidão relativa
			// 0 - Chance Pais Mutados
			roleta[0] = 100 - (ciPais * 100 / (ciPais+ciFilhos));
			// 1 - Chance Filhos
			roleta[1] = 100 - (ciFilhos * 100 / (ciPais+ciFilhos));
printf("Chance de escolher os pais[0]: %d com os filhos[1]: %d\n", roleta[0], roleta[1]);
			// Preciso saber do	 menor, pois como é uma roleta de 2, se for menor que menor, então ele é escolhido, caso contrário o maior
			menor = (roleta[0] < roleta[1] ? 0 : 1);
			menorC = (menor == 0 ? 'p' : 'f');
			tmp = getRand(0,100);
			/*if(menorC == 'p') {
				roleta[0] = 95;
				roleta[1] = 5;
			}
			else {
				roleta[0] = 5;
				roleta[1] = 95;
			}*/
			
			printf("getRand() selecao: %d menor: %d\n", tmp, menor);
			
			if(roleta[0] > roleta[1]) { // A chance de pais ser escolhido é maior
				if(tmp < roleta[1]) { // se a roleta for menor que o menor (filhos), trocar por filhos.
					initArray(&machines[m1], 1);
					initArray(&machines[m2], 1);
					
					for(i=0;i<maquinaFilho1.used;i++)
						insertArray(&machines[m1], maquinaFilho1.array[i]);
					for(i=0;i<maquinaFilho2.used;i++)
						insertArray(&machines[m2], maquinaFilho2.array[i]);
				}
			}
			else { // pais <= filhos
				if(tmp >= roleta[0]) { // se a roleta for maior que o menor (pais), trocar por filhos
					initArray(&machines[m1], 1);
					initArray(&machines[m2], 1);
					
					for(i=0;i<maquinaFilho1.used;i++)
						insertArray(&machines[m1], maquinaFilho1.array[i]);
					for(i=0;i<maquinaFilho2.used;i++)
						insertArray(&machines[m2], maquinaFilho2.array[i]);
				}
			}

		}
		else {
			// Nada, pois só há pais e estes não são mutados, nada mudou.
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
		
		insertArray(&octaveY, makespan());
		
	}
	
	
	printf("\n\nFim AG. Makespan: %d\n", makespan());
	printf("Executou busca local\n");
	localSearch();
	printf("Fim AG. Makespan: %d\n", makespan());
	
	// Armazena em arquivo resultado do octave
	report('G', &octaveX, &octaveY, totalJobs, totalMachines);
}