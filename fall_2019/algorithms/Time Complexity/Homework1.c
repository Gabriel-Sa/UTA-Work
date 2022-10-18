//CSE 2320 Homework 1
#include <stdio.h>
#include <math.h>

/*
int main(int argc, char const *argv[]) {
  int N = 4;
  int S = 5;
  int count = 0;
  int countb = 0;
  int countc = 0;
  for (int i = 1; i <= N; i++)
  {
    printf("\ni = %d\n", i);
    count++;
    for(int k = 1; k <= S; k++)
    {
      countb++;
      for(int t = 1; t <= i; t++)
      {
        printf("%d ", t);
        countc++;
      }
      printf(" k = %d\n", k);
    }
  }
  printf(" %d %d %d\n", count, countb, countc);
  return 0;
}
*/

void runtime_increment(int N)
{
  int i, k, t, res = 0;
  for(i = 1; i <= N; i++)
    for(k = 1; k <= i; k++)
      for(t = 1; t <= N; t++)
        res++;
}

void runtime_print(int N)
{
  int i, k, t;
  for(i = 1; i <= N; i++)
    for(k = 1; k <= i; k++)
      for(t = 1; t <= N; t++)
        printf("A");
}

void runtime_pow(int N)
{
  int i, res = 0;
  for(i = 1; i <= pow(2.0, (double)N); i++)
    res++;
}

int main(int argc, char const *argv[]) {
  int N = 1-;
  //runtime_increment(N);
  runtime_print(N);
  //runtime_pow(N);

  return 0;
}
