#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>

struct set_of_numbers
{
  int min;
  int max;
};

struct thread_custom_set
{
  int total_threads;
  int t_index;
  int max;
  int primes_found;
};

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

void *custom_evaluate_numbers(void *args)
{
  struct thread_custom_set *set = (struct thread_custom_set *)args;
  int total_threads = set->total_threads;
  int t_index = set->t_index;
  int max = set->max;
  int counter = 0;

  if (t_index == 1)
  {
    t_index += total_threads;
  }

  for (int i = t_index; i <= max; i = i + total_threads)
  {
    if (is_prime(i))
    {
      printf(",%d", i);

      counter++;
    }
  }

  set->primes_found = counter;
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  clock_t t;

  int max_number, n_threads, counter, i;

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

  t = clock();

  pthread_t threads[n_threads];
  struct thread_custom_set *sets = malloc(sizeof(struct thread_custom_set) * n_threads);

  if (max_number > 1)
  {
    printf("Numbers: ");
  }

  for (i = 0; i < n_threads; i++)
  {

    sets[i].t_index = i + 1;
    sets[i].max = max_number;
    sets[i].total_threads = n_threads;
    pthread_create(&threads[i], NULL, custom_evaluate_numbers, (void *)&sets[i]);
  }

  for (i = 0; i < n_threads; i++)
    pthread_join(threads[i], NULL);

  for (i = 0; i < n_threads; i++)
  {
    counter += sets[i].primes_found;
  }

  t = clock() - t;
  double time_taken = ((double)t) / CLOCKS_PER_SEC;
  printf("\n\nTotal of prime numbers found: %d", counter);
  printf("\nElapsed time to evaluate %d numbers: %fs\n", max_number, time_taken);

  return 0;
}
