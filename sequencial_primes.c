#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <time.h>
#include <string.h>

int verbose, counter = 0;

void print_results(int max_number, double time_taken)
{
	printf("\033[0;32m");
	printf("\n\n #################################################");
	printf("\n %-15s%18s%16s", "# ", "Standard Algorithm", " #");
	printf("\n %-13s%23s%13s", "# ", "Sequencial Prime Finder", " #");
	printf("\n %-12s%26s%11s", "# ", "Gabriel Donadel Dall'Agnol", " #");
	printf("\n #################################################");
	printf("\n #%47s#", " ");
	printf("\n %-36s%11d%2s", "# Total of numbers evaluated: ", max_number, " #");
	printf("\n %-36s%11d%2s", "# Total of prime numbers found: ", counter, " #");
	printf("\n %-36s%10fs%2s", "# Elapsed time to evaluate numbers: ", time_taken, " #");
	printf("\n #%47s#", " ");
	printf("\n #################################################\n\n");
	printf("\033[0m");

	if (!verbose)
	{
		printf(" Tip: Running this program using the flag -v will print all prime numbers found\n\n");
	}
}

int is_prime(int n)
{
	int limit = n / 2;
	if (n % 2 == 0)
	{
		return 0;
	}
	for (int i = 3; i <= limit; i = i + 2)
	{
		if (n % i == 0)
		{
			return 0;
		}
	}

	return 1;
}

void evaluate_numbers(int max_number)
{
	if (max_number > 1)
	{
		counter++;
		if (verbose)
		{
			printf("\nNumbers: 2");
		}
	}

	if (verbose)
	{
		for (int i = 3; i <= max_number; i = i + 2)
		{
			if (is_prime(i))
			{
				printf(",%d", i);
				counter++;
			}
		}
	}
	else
	{

		for (int i = 3; i <= max_number; i = i + 2)
		{
			if (is_prime(i))
				counter++;
		}
	}
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

	if (argc >= 1)
	{
		for (int i = 1; i < argc; i++)
		{
			if (strcmp(argv[i], "-v") == 0)
			{
				verbose = 1;
			}
		}
	}

	t = clock();
	evaluate_numbers(max_number);
	t = clock() - t;
	double time_taken = ((double)t) / CLOCKS_PER_SEC;

	print_results(max_number, time_taken);

	return 0;
}
