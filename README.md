# find-primes

A simple C project with diffrent implementations of prime numbers finders

## How to compile

### Compiling the sequenial finder

```c
gcc sequencial_primes.c -o binaries/sequencial_primes
```

### Compiling the parallel finder using pthreads

```c
gcc pthread_parallel_primes.c -o binaries/pthread_parallel_primes
```

### Compiling the sequenial Sieve of Eratosthenes finder

```c
gcc sieve_of_eratosthenes.c -o binaries/sieve_of_eratosthenes
```

### Compiling the parallel Sieve of Eratosthenes finder using pthreads

```c
gcc pthread_parallel_sieve_of_eratosthenes.c -o binaries/pthread_parallel_sieve_of_eratosthenes -pthread
```

### Compiling the parallel Sieve of Eratosthenes finder using OpenMP

on OSX

```c
clang -Xpreprocessor -fopenmp open_mp_parallel_sieve_of_eratosthenes.c -o binaries/open_mp_parallel_sieve_of_eratosthenes  -lomp
```
