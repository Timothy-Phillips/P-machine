/*
    Students:
     Timothy Phillips
     Kimberly Szekely
     Edgard Irineo
     Marcos Chacon
*/

#include <stdio.h>
#define PAS_SIZE 500

// Functional Prototypes
int getInstructions(char *input, int *pas);

// Simple Placeholder Implementation
int main(int argc, char **argv)
{
  int pas[PAS_SIZE];

  // Read instructions from file
  getInstructions(argv[1], pas);

  // Test File IO
  printf("%d\n" pas[0]);
}


int getInstructions(char *input, int *pas)
{
  FILE *fp;
  int temp;
  int i = -1;

  if ((fp = fopen(input, "r")) == NULL)
    return -1;

  while (!feof (fp) && fscanf (fp, "%d", &temp) && i++ < PAS_SIZE )
    pas[i] = temp;

  fclose(fp);

  // change to return instruction length
  return 0;
}
