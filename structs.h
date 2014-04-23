/**
 * Interface of program structs.
 * Also check http://happybearsoftware.com/implementing-a-dynamic-array.html
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
  a->array[a->used++] = element;
}

void removeArray(Array *a, int position) {
	int i;
	for(i = position;i < a->used;i++) {
		// exception for the last i
		if(i != a->used - 1)
			a->array[i] = a->array[i+1];
	}
	// Just update used var
	a->used = a->used - 1;
}

void freeArray(Array *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}

void copyArray(Array *from, Array *to) {
	int i;
	// Clear before all
	to->used=0;
	
	for(i=0;i < from->used;i++) {
		insertArray(to, from->array[i]);
	}
}

typedef struct {
	Array maquina;
	Array tempo;
} Job;

typedef struct {
	Job job;
} Jobs;