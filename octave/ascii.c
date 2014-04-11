#include<stdlib.h>
#include<stdio.h>

int main() {
	char str[] = "Victor";
	int i=0;
	while(str[i])
		printf("%c = %d\n", str[i++], str[i]);
	printf("\n");
	return -1;
}
