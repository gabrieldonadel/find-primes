#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <time.h>

int is_prime(int n)
{
	int limit = n/2;
	for(int i = 2; i <= limit; i++){
		if(n%i == 0){
			return 0;
		}
	}

	return 1;
}

void evaluate_numbers(int max_number){
	int counter = 0;

	for(int i = 2; i <= max_number; i++){
		if(is_prime(i)){
			if(counter){
				printf(",%d", i);
			}else{
				printf("Numbers: %d", i);
			}
			counter++;
		}
	}

	if(counter){
		printf("\n\nTotal of prime numbers found: %d\n", counter);
	}
}

int main(int argc, char *argv[])
{
	clock_t t;
	int max_number;
	char *p;

	errno = 0;
	long conv = strtol(argv[1], &p, 10);

	// Check for errors: e.g., the string does not represent an integer
	// or the integer is larger than int
	if (errno != 0 || *p != '\0' || conv > INT_MAX) {
		// Put here the handling of the error, like exiting the program with
		// an error message
		printf("Digite um numero valido!");
		return 0;
	} else {
		max_number = conv;
	}

	t = clock();
	evaluate_numbers(max_number);
	t = clock() - t;
	double time_taken = ((double)t)/CLOCKS_PER_SEC;
	printf("Elapsed time to evaluate %d numbers: %fs\n", max_number,time_taken);

	return 0;
}
