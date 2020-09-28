#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#include <unistd.h>

int i, max_number, n_threads;
unsigned char *primes = NULL;

struct thread_args
{
  int thread_i;
};

void print_results(int counter, double time_taken, int verbose)
{
  printf("\033[0;32m");
  printf("\n #################################################");
  printf("\n %-14s%21s%14s", "# ", "Sieve of Eratosthenes", " #");
  printf("\n %-14s%21s%14s", "# ", "Parallel Prime Finder", " #");
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

void *set_numbers_zero(void *args)
{
  struct thread_args *t_args = args;
  int thread_i = t_args->thread_i;

  for (int j = i * i + (thread_i * 2 * i); j <= max_number; j = j + 2 * i * (n_threads))
  {
    // Set non prime numbers as 0x00
    primes[j / 2] = 0x00;
  }
  return NULL;
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

  clock_t t = clock();
  pthread_t threads[n_threads];
  int array_size = (max_number - 1) / 2;
  primes = calloc(array_size + 1, sizeof(unsigned char)); //malloc(sizeof(unsigned char) * (array_size + 1));
  struct thread_args *t_args = malloc(sizeof(struct thread_args) * n_threads);
  for (int thread_i = 0; thread_i < n_threads; thread_i++)
  {
    t_args[thread_i].thread_i = thread_i;
  }

  //Initializing the array
  for (i = 0; i <= array_size; i++)
    primes[i] = 0x01;

  //Iterating over array
  for (i = 3; (i * i) <= max_number; i = i + 2)
  {
    if (primes[i / 2] != 0)
    {
      for (int thread_i = 0; thread_i < n_threads; thread_i++)
      {
        pthread_create(&threads[thread_i], NULL, set_numbers_zero, &t_args[thread_i]);
      }
      for (int thread_i = 0; thread_i < n_threads; thread_i++)
      {
        pthread_join(threads[thread_i], NULL);
      }
    }
  }

  if (max_number >= 2)
  {
    counter++;
  }

  // Print all prime numbers found
  if (verbose && max_number >= 2)
  {
    printf("Numbers: 2");

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
    for (i = 1; i <= array_size; i++)
      counter += primes[i];
  }

  t = clock() - t;
  double time_taken = ((double)t) / CLOCKS_PER_SEC;
  print_results(counter, time_taken, verbose);

  return 0;
}
