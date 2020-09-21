#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{

  int max_number, i, j, counter;
  char *p;

  errno = 0;
  long conv = strtol(argv[1], &p, 10);

  if (errno != 0 || *p != '\0' || conv > INT_MAX)
  {
    printf("Digite um numero menor que %d!/n", INT_MAX);
    return 0;
  }
  else
  {
    max_number = conv;
  }

  clock_t t = clock();
  int *primes;
  primes = (int *)malloc(sizeof(int) * max_number);

  //populating array with naturals numbers
  primes[2] = 2;
  for (i = 3; i <= max_number; i = i + 2)
    primes[i] = i;

  i = 2;
  while ((i * i) <= max_number)
  {
    if (primes[i] != 0)
    {
      for (j = 2; j < max_number; j++)
      {
        if (primes[i] * j > max_number)
          break;
        else
          // Instead of deleteing , making elemnets 0
          primes[primes[i] * j] = 0;
      }
    }
    i++;
  }

  for (i = 2; i <= max_number; i++)
  {
    if (primes[i] != 0)
    {
      counter++;
      printf(",%d", primes[i]);
    }
  }

  printf("\n\nTotal of prime numbers found: %d\n", counter);

  t = clock() - t;
  double time_taken = ((double)t) / CLOCKS_PER_SEC;
  printf("Elapsed time to evaluate %d numbers: %fs\n", max_number, time_taken);

  return 0;
}
