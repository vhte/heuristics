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
void GA() {
	int t, worst, neighborhoodSize, pCrossover, pMutacao,tmp;
	int m1, m2, menor, maior, i,j,halfJobs;
	
	Array backup[totalMachines], maquinaFilho1, maquinaFilho2;
	
	// As I can't use memcpy because of pointers, just use a custom function that rebuilds an instance to another
	for(tmp=0;tmp<totalMachines;tmp++)
		initArray(&backup[tmp], totalJobs);
	// now it's good to copy
	for(tmp=0;tmp<totalMachines;tmp++) {
		copyArray(&machines[tmp], &backup[tmp]);
	}
	
	// Tempo 0
	t = 0;
	
	// 10% da populacao. Caso não tenha isto de rodadas sem melhora, fim.
	neighborhoodSize = 0.1*(totalJobs*(totalMachines-1));
	
	// Probabilidade de crossover
	pCrossover = 80;
	
	// Probabilidade pMutacao
	pMutacao = 2;
	
	// Gerando população inicial P(t) - Já é machines[]
	// Cada cromossomo {s1, s2, s3, s3...} será uma maquina, e é essa populacao que muda, pois vou selecionar 2 maquinas e trocar os jobs e avaliar os sobreviventes e substituir.
	// Dado m1 e m2, irei pegar 50% do maior e substituir no menor (porque m1 > m2)
	
	// Avaliando P(t)
	worst = makespan();
	
	// Enquanto critério de parada  não estiver satisfeito
	while(t < 1) {
		t++;
		// Seleção dos pais (binary tournament) - Motivos: Não pressionar tanto seleção (pressão de seleção) e não correr risco de convergência prematura
		// Escolher duas máquinas (cromossomos) aleatórios 2x, dessas 2x, pegar a que tem menor Ci()
		m1 = getRand(0,totalMachines);
		while(machines[m1].used == 0)
			m2 = getRand(0,totalMachines);
		tmp = getRand(0,totalMachines);
		
		// nao deixa pegar a mesma maquina NEM MAQUINAS VAZIAS
		while(tmp == m1 ||  machines[tmp].used == 0) {
			printf("tmp (%d) == m1 (%d)\n", tmp, m1);
			printf("machines[%d].used = %d |||| machines[%d].used %d\n", tmp, machines[tmp].used, m1, machines[m1].used);
			tmp = getRand(0,totalMachines);
		}
		
		if(Ci(m1) > Ci(tmp))
			// Primeiro pai
			m1 = tmp;
		
		// Segundo pai
		m2 = getRand(0,totalMachines);
		while(machines[m2].used == 0 || m1 == m2)
			m2 = getRand(0,totalMachines);
		
		tmp = getRand(0,totalMachines);
		
		// nao deixa pegar a mesma maquina nem a que já foi como primeiro pai NEM MAQUINAS VAZIAS
		while(tmp == m2 || m1 == tmp || machines[tmp].used == 0)
			tmp = getRand(0,totalMachines);
		
		
		if(Ci(m2) > Ci(tmp))
			// Primeiro pai
			m2 = tmp;
		
		// Com os dois pais setados, iniciar 2 filhos, cada qual com o tamanho de pai m1 e m2
		initArray(&maquinaFilho1, machines[m1].used);
		initArray(&maquinaFilho2, machines[m2].used);
		
		// Cruzamento (Crossover) (80%)
		//if(getRand(0,100) < pCrossover) {
			// Verificar qual das duas máquinas tem menos jobs, pois o corte será feito por ela
			if(machines[m1].used > machines[m2].used) {
				menor = m2;
				maior = m1;
			}
			else {
				menor = m1;
				maior = m2;
			}
			
			// Da máquina que tem menor jobs, pega 50% dela e fazer crossover com a outra maquina
			halfJobs = machines[menor].used/2;
			
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
				exit(-1);
		//}
		
		// Mutação (2%)
		if(getRand(0,100) < pMutacao) {
			//
		}
		
		// Avaliação
		
		// Sobreviventes (Roleta: Chance de sobrevivencia proporcional ao nível de aptidão C(i))
		
		// Reseto os filhos para serem reiniciados na próxima iteração
		freeArray(&maquinaFilho1);
		freeArray(&maquinaFilho2);
	 }
}