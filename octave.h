/* 
 * File:   octave.h
 * Author: victor
 *
 * Created on April 6, 2014, 3:37 PM
 * 
 * Report charts
 * solucaoInicial = global
 */
void report(char algorithm, Array *octaveX, Array *octaveY, int jobs, int maquinas) {
	int i;
	FILE *fp;
	
	fp = fopen("octave.res", "w"); 
	char x[20] = "x = [";
	char z[1024];
	fwrite(x, sizeof(char), strlen(x), fp);
	for(i=0;i<octaveX->used;i++) {
		memset(x,0,sizeof(x));
		sprintf(x, "%d ", octaveX->array[i]); // 59 = ;
		fwrite(x, sizeof(char), strlen(x) ,fp);
	}
	strcpy(x,"];\n");
	fwrite(x, sizeof(char), strlen(x), fp);
	
	//y
	strcpy(x, "y = [");
	fwrite(x, sizeof(char), strlen(x), fp);
	for(i=0;i<octaveY->used;i++) {
		memset(x,0,sizeof(x));
		sprintf(x, "%d ", octaveY->array[i]); // 59 = ;
		fwrite(x, sizeof(char), strlen(x),fp);
	}
	memset(x,0,sizeof(x));
	strcpy(x, "];\n");
	fwrite(x,  sizeof(char), strlen(x),fp);
	
	
	memset(x,0,sizeof(x));
	strcpy(x,"plot(x,y);\n");
	fwrite(x, sizeof(char), strlen(x),fp);
	memset(x,0,sizeof(x));
	sprintf(z, "title(\"%s\\nConfiguracao: %d maquinas / %d tarefas. Solucao inicial: %s\");", (algorithm == 'G' ? "Algoritmo Genetico" : "Variable Neighborhood Search (VNS)"), maquinas, jobs, (solucaoInicial == 'G' ?  "Gulosa" : "Aleatoria"));
	//strcpy(x,"title(\"Algoritmo Genetico\");");
	fwrite(z, sizeof(char), strlen(z),fp);
	memset(x,0,sizeof(x));
	strcpy(x,"xlabel(\"Iteracao\");");
	fwrite(x, sizeof(char), strlen(x),fp);
	memset(x,0,sizeof(x));
	strcpy(x,"ylabel(\"C_{max}\");");
	fwrite(x, sizeof(char), strlen(x),fp);
	memset(x,0,sizeof(x));
	//strcpy(x,"text (pi, 0.7, \"Configuracao: x maquinas / n tarefas\");");
	
	
	
	strcpy(x,"grid on;");
	fwrite(x, sizeof(char), strlen(x), fp);
	memset(x,0,sizeof(x));
	strcpy(x,"saveas (1, \"figure1.png\");");
	fwrite(x, sizeof(char), strlen(x), fp);
	memset(x,0,sizeof(x));
	strcpy(x,"pause;\0");
	fwrite(x, sizeof(char), strlen(x), fp);
	
	
	
	/*fwrite(x, 1, sizeof(x), fp);
	for(i=0;i<octave.used;i++) {
		memset(x,0,sizeof(x));
		sprintf(x, (octave.array[i] != -1 ? "%d " : "%c "), (octave.array[i] != -1 ? octave.array[i] : ';')); // 59 = ;
		fwrite(x, 1, sizeof(x),fp);
	}
	memset(x,0,sizeof(x));
	strcpy(x, "])");*/
	
	
	fclose(fp);
	system("octave -qf octave.res");
}
