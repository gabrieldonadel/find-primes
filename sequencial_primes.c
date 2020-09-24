#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <time.h>

int verbose = 0;

int is_prime(int n)
{
	int limit = n / 2;
	for (int i = 2; i <= limit; i++)
	{
		if (n % i == 0)
		{
			return 0;
		}
	}

	return 1;
}

void verbosely_evaluate_numbers(int max_number)
{
	int counter = 0;

	if (max_number > 1)
	{
		printf("Numbers: 2");
		counter++;
	}

	for (int i = 3; i <= max_number; i = i + 2)
	{
		if (is_prime(i))
		{
			printf(",%d", i);
			counter++;
		}
	}

	printf("\n\nTotal of prime numbers found: %d\n", counter);
}

void evaluate_numbers(int max_number)
{
	int counter = 0;

	if (max_number > 1)
		counter++;

	for (int i = 3; i <= max_number; i = i + 2)
	{
		if (is_prime(i))
			counter++;
	}

	printf("\n\nTotal of prime numbers found: %d\n", counter);
}

int main(int argc, char *argv[])
{
	clock_t t;
	int max_number;
	char *p;

	errno = 0;
	long conv = strtol(argv[1], &p, 10);

	if (errno != 0 || *p != '\0' || conv > INT_MAX)
	{
		printf("Digite um numero valido!");
		return 0;
	}
	else
	{
		max_number = conv;
	}

	t = clock();
	if (verbose)
	{

		evaluate_numbers(max_number);
	}
	else
	{
		verbosely_evaluate_numbers(max_number);
	}
	t = clock() - t;
	double time_taken = ((double)t) / CLOCKS_PER_SEC;
	printf("Elapsed time to evaluate %d numbers: %fs\n", max_number, time_taken);

	return 0;
}
