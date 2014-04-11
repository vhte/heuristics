#include<stdlib.h>
#include<stdio.h>

int main() {
	
	FILE * fp;
	fp = fopen("res", "w+");
	char string[] = "Auei\nbr";
	fwrite(string, 1,sizeof(string), fp);
	return 1;
}
