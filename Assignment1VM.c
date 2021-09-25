/*
    Students:
     Timothy Phillips
     Kimberly Szekely
     Edgard Irineo
     Marcos Chacon
*/

#include <stdio.h>
#define PAS_SIZE 500

// Simple Placeholder Implementation
int main(int argc, char **argv)
{
  int pas[PAS_SIZE];

  FILE *fp;
  int temp, instructionCount, index = -1;

  // Read instructions from file
  if ((fp = fopen(argv[1], "r")) == NULL)
    return -1;

  while (!feof (fp) && fscanf (fp, "%d", &temp) && index++ < PAS_SIZE )
    pas[index] = temp;

  fclose(fp);

  instructionCount = index + 1;

  for(int i = 0; i < instructionCount; i = i + 3)
  {
     printf("%d %d %d\n", pas[i], pas[i+1], pas[i+2]);
  }
}
