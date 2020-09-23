#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[])
{

  int max_number, i, j, counter, verbose;
  char *param1;

  errno = 0;
  long conv = strtol(argv[1], &param1, 10);

  if (errno != 0 || *param1 != '\0' || conv > INT_MAX - 1)
  {
    printf("You must type in a number smaller then %d!\n", INT_MAX);
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

  clock_t t = clock();
  int array_size = (max_number - 1) / 2;
  unsigned char *primes = malloc(sizeof(unsigned char) * (array_size + 1));

  //Initializing the array
  for (i = 0; i <= array_size; i++)
    primes[i] = 0x01;

  //Iterating over array
  for (i = 3; (i * i) <= max_number; i = i + 2)
  {
    if (primes[i / 2] != 0)
    {
      for (j = i * i; j <= max_number; j = j + 2 * i)
      {
        // Set non prime numbers as 0x00
        primes[j / 2] = 0x00;
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
  printf("\033[0;32m");
  printf("\n #################################################");
  printf("\n %-36s%11d%2s", "# Total of numbers evaluated: ", max_number, " #");
  printf("\n %-36s%11d%2s", "# Total of prime numbers found: ", counter, " #");
  printf("\n %-36s%10fs%2s", "# Elapsed time to evaluate numbers: ", time_taken, " #");
  printf("\n #################################################\n\n");
  printf("\033[0m");

  if (!verbose)
  {
    printf("Tip: Running this program using the flag -v will print all prime numbers found\n\n");
  }

  return 0;
}
