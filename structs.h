/**
 * Interface of program structs
 */
// http://stackoverflow.com/questions/3536153/c-dynamically-growing-array
typedef struct {
	int *array;
	size_t used;
	size_t size;
} Array;

void initArray(Array *a, size_t initialSize) {
  a->array = (int *)malloc(initialSize * sizeof(int));
  a->used = 0;
  a->size = initialSize;
}

void insertArray(Array *a, int element) {
  if (a->used == a->size) {
    a->size *= 2;
    a->array = (int *)realloc(a->array, a->size * sizeof(int));
  }
  //printf("***Colocando na posicao %d o valor %d. O tamanho usado e: %d, o tamanho total e: %zd***\n", a->used, element, a->used, a->size);
  a->array[a->used++] = element;
}

void freeArray(Array *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}

typedef struct {
	Array maquina;
	Array tempo;
} Job;

typedef struct {
	Job job;
} Jobs;

// Solution
/*typedef struct {
	Array jobs; // Each job has a time 
} Machine;*/
typedef struct {
	Array jobs;
} Machines;