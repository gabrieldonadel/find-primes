#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include <unistd.h>
#include <math.h>

int i, max_number, n_threads;
unsigned char *primes = NULL;

void print_results(int counter, double time_taken, int verbose)
{
  printf("\033[0;32m");
  printf("\n #################################################");
  printf("\n %-14s%21s%14s", "# ", "Sieve of Eratosthenes", " #");
  printf("\n %-8s%34s%7s", "# ", "Parallel Prime Finder using OpenMP", " #");
  printf("\n %-12s%26s%11s", "# ", "Gabriel Donadel Dall'Agnol", " #");
  printf("\n #################################################");
  printf("\n #%47s#", " ");
  printf("\n %-36s%11d%2s", "# Number of threads used: ", n_threads, " #");
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

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    printf("The %s argument must be a number!\n", argc == 1 ? "first" : "second");
    return 0;
  }

  int j, counter, verbose;
  char *param1;
  char *param2;

  errno = 0;
  long conv1 = strtol(argv[1], &param1, 10);
  long conv2 = strtol(argv[2], &param2, 10);

  if (errno != 0 ||
      *param1 != '\0' ||
      *param2 != '\0' ||
      conv1 > INT_MAX - 1 ||
      conv2 > INT_MAX - 1)
  {
    printf("You must type in a number smaller then %d!\n", INT_MAX);
    return 0;
  }
  else
  {
    max_number = conv1;
    n_threads = conv2;
  }

  if (argc >= 2)
  {
    for (int i = 2; i < argc; i++)
    {
      if (strcmp(argv[i], "-v") == 0)
      {
        verbose = 1;
      }
    }
  }

  omp_set_num_threads(n_threads);

  struct timespec start, finish;
  clock_gettime(CLOCK_MONOTONIC, &start);

  int max_number_sqrt = (int)sqrt((double)max_number);
  int array_size = (max_number - 1) / 2;

  //Initializing the array
  primes = calloc(array_size + 1, sizeof(unsigned char));
#pragma omp parallel for
  for (i = 0; i <= array_size; i++)
    primes[i] = 0x01;

//Iterating over array
#pragma omp parallel for schedule(dynamic)
  for (i = 3; i <= max_number_sqrt; i = i + 2)
    if (primes[i / 2] != 0)
      for (int j = i * i; j <= max_number; j = j + 2 * i)
        primes[j / 2] = 0x00;

  if (max_number >= 2)
    counter++;

  // Print all prime numbers found
  if (verbose && max_number >= 2)
  {
    printf("Numbers: 2");

#pragma omp parallel for reduction(+ \
                                   : counter)
    for (i = 1; i <= array_size; i++)
    {
      if (primes[i] != 0x00)
      {
        counter++;
        printf(",%d", 2 * i + 1);
      }
    }
    printf("\n");
  }
  else
  {
#pragma omp parallel for reduction(+ \
                                   : counter)
    for (i = 1; i <= array_size; i++)
      counter += primes[i];
  }

  clock_gettime(CLOCK_MONOTONIC, &finish);
  double time_taken = (finish.tv_sec - start.tv_sec);
  time_taken += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
  print_results(counter, time_taken, verbose);

  return 0;
}
