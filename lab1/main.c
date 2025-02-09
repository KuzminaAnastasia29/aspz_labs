#include<stdio.h>

int replace(char *str, char replacement){
	int count = 0;

	if(str == NULL) return 0;

	while(*str){
		if(*str == ' '){
			*str = replacement;
			count++;
		}
		str++;
	}

	return count;
}

int main(){
	char str[] = "The cat sat";
	char replacement = '-';

	int count = replace(str, replacement);

	printf("Modified string: \"%s\"\n", str);
	printf("Number of replacements: %d\n", count);

	return 0;
}
