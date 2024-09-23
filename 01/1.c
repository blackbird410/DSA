#include <stdio.h>
#include <stdlib.h>

void allocArray(int ***p, int m, int n)
{
  int i;
  *p = (int **)malloc(m * sizeof(int *));  
  (*p)[0] = (int *)malloc(m * n * sizeof(int));  

  for (i = 1; i < m; i++)
    (*p)[i] = (*p)[0] + i * n; 
}


int main()
{
  int **array, *a;
  int j, k;
  allocArray(&array, 5, 10);
  for(j = 0;j < 5;j ++)
    for(k = 0;k < 10;k ++)
      array[j][k] = j * 10 + k;
  for(j = 0;j < 5;j ++)
    for(k = 0;k < 10;k ++)
      printf("%p ", &(array[j][k]));

  // Free memory
  a = *array;
  free(a);
  free(array);
  return 0;
}
