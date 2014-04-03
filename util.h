/* 
 * File:   util.h
 * Author: victor
 *
 * Created on February 18, 2014, 12:43 PM
 */
// Generates random number between specified interval (l <= x <= u)

// I need this to be global, so srand() can generate different number each iteration because we'll always have a different nRand
unsigned int getRand_n = 0;
int getRand(int min_num, int max_num) {
	int result=0,low_num=0,hi_num=0;
	if(min_num<max_num) {
		low_num=min_num;
		hi_num=max_num+1; // this is done to include max_num in output.
	}else{
		low_num=max_num+1;// this is done to include max_num in output.
		hi_num=min_num;
	}
	srand(time(NULL) - ++getRand_n*2);
	//srand(time(NULL) ^ (getpid()<<16));
	//srand(time(NULL));
	result = (rand()%(hi_num-low_num))+low_num;
	return result;
}

bool inArray(int num, Array *arr) {
	int i;
	bool res = false;
	
	for(i=0;i<arr->used;i++) {
		if(num == arr->array[i]) {
			res = true;
			break;
		}
	}
	
	return res;
}