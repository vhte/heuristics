#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int main() {
	float alpha = 20;int totalMachines = 10;
	
	float total;
	
	total = round(totalMachines*(alpha/100));
	printf("total antes validacao: %f\n", total);
	if(total == 0)
		total = 1;
	printf("total: %f\n", total);
	return -1;
}
