#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

int n_threads, verbose;

struct thread_custom_set
{
  int t_index;
  int max;
  int primes_found;
};

void print_results(int max_number, int counter, double time_taken)
{
  printf("\033[0;32m");
  printf("\n\n #################################################");
  printf("\n %-15s%18s%16s", "# ", "Standard Algorithm", " #");
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

void *custom_evaluate_numbers(void *args)
{
  struct thread_custom_set *set = (struct thread_custom_set *)args;
  int t_index = set->t_index;
  int max = set->max;
  int t_counter = 0;

  if (verbose)
  {
    for (int i = t_index; i <= max; i = i + (n_threads * 2))
    {
      if (is_prime(i))
      {
        printf(",%d", i);
        t_counter++;
      }
    }
  }
  else
  {
    for (int i = t_index; i <= max; i = i + (n_threads * 2))
    {
      if (is_prime(i))
      {
        t_counter++;
      }
    }
  }

  set->primes_found = t_counter;
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  struct timespec start, finish;

  int max_number, counter, i;

  char *p1;
  char *p2;

  errno = 0;
  long conv1 = strtol(argv[1], &p1, 10);
  long conv2 = strtol(argv[2], &p2, 10);

  if (errno != 0 || *p1 != '\0' || conv1 > INT_MAX)
  {
    printf("Type in a valid param!");
    return 0;
  }
  else
  {
    max_number = conv1;
    n_threads = conv2;
  }

  if (argc >= 3)
  {
    for (int i = 2; i < argc; i++)
    {
      if (strcmp(argv[i], "-v") == 0)
      {
        verbose = 1;
      }
    }
  }

  clock_gettime(CLOCK_MONOTONIC, &start);

  pthread_t threads[n_threads];
  struct thread_custom_set *sets = malloc(sizeof(struct thread_custom_set) * n_threads);

  if (max_number > 1)
  {
    counter++;
    if (verbose)
    {
      printf("Numbers: 2");
    }
  }

  for (i = 1; i <= n_threads; i++)
  {
    sets[i - 1].t_index = (i * 2) + 1;
    sets[i - 1].max = max_number;
    pthread_create(&threads[i - 1], NULL, custom_evaluate_numbers, (void *)&sets[i - 1]);
  }

  for (i = 0; i < n_threads; i++)
    pthread_join(threads[i], NULL);

  for (i = 0; i < n_threads; i++)
  {
    counter += sets[i].primes_found;
  }

  free(sets);

  clock_gettime(CLOCK_MONOTONIC, &finish);
  double time_taken = (finish.tv_sec - start.tv_sec);
  time_taken += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

  print_results(max_number, counter, time_taken);

  return 0;
}
