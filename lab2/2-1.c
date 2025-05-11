#include<stdio.h>
#include<time.h>
#include<limits.h>

int main(){
	time_t max_time = (time_t)(~((time_t)1 << (sizeof(time_t)*8 -1)));

	printf("Розмір time_t: %zu байтів\n", sizeof(time_t));
	printf("Максимальне значення time_t: %ldd\n", (long long)max_time);
	
	char *time_str = ctime(&max_time);
	if(time_str)
		printf("Дата, яка відповідає максимальному значенню time_t: %s\n", time_str);
	else
		perror("ctime");

	return 0;
}