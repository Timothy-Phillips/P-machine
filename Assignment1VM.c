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
int BP = 0;
int gp = 0;
int dp = 0;
int m = 0;
int op = 0;
int pas[PAS_SIZE];
int sp = 0;
int pc = 0;
int L = 0;
int MAX_PAS_LENGTH = 500;

int base(int L);
void print_execution(int line, char *opname, int *IR, int PC, int BP, int SP, int DP, int *pas, int GP);

int main(int argc, char **argv)
{

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
  //start execute cycle
  switch(op)
  {
    //LIT
    case 1:
      if(BP == gp)
      {
        dp++;
        pas[dp] = m;
      }
      else
      {
        sp--;
        pas[sp] = m;
      }
      break;
    //OPR
    case 2:
      switch(m)
      {
        //RTN
        case 0:
          sp = BP + 1;
          BP = pas[sp - 2];
          pc = pas[sp - 3];
          break;
        //NEG
        case 1:
          if(BP == gp)
          {
            pas[dp] = (-1 * pas[dp]);
          }
          else
          {
            pas[sp] = (-1 * pas[sp]);
          }
          break;
        //ADD
        case 2:
          if(BP == gp)
          {
            dp--;
            pas[dp] += pas[dp + 1];
          }
          else
          {
            sp++;
            pas[sp] += pas[sp - 1];
          }
          break;
        //SUB      
        case 3:
          if(BP == gp)
          {
            dp--;
            pas[dp] = pas[dp] - pas[dp + 1];
          }
          else
          {
            sp++;
            pas[sp] = pas[sp] - pas[sp - 1];
          }
          break;
        //MUL
        case 4:
          if(BP == gp)
          {
            dp--;
            pas[dp] *= pas[dp + 1];
          }
          else
          {
            sp++;
            pas[sp] *= pas[sp - 1];
          }
          break;
        //DIV
        case 5:
          if(BP == gp)
          {
            dp--;
            pas[dp] = pas[dp] / pas[dp + 1];
          }
          else
          {
            sp++;
            pas[sp] = pas[sp] / pas[sp - 1];
          }
          break;
        //ODD
        case 6:
          if(BP == gp)
          {
            pas[dp] = pas[dp] % 2;
          }
          else
          {
            pas[sp] *= pas[sp] % 2;
          }          
          break;
        //MOD
        case 7:
          if(BP == gp)
          {
            dp--;
            pas[dp] = pas[dp] % pas[dp + 1];
          }
          else
          {
            sp++;
            pas[sp] = pas[sp] % pas[sp - 1];
          }                    
          break;
        //EQL
        case 8:
          if(BP == gp)
          {
            dp--;
            pas[dp] = pas[dp] == pas[dp + 1];
          }
          else
          {
            sp++;
            pas[sp] = pas[sp] == pas[sp - 1];
          }          
          break;
        //NEQ
        case 9:
          if(BP == gp)
          {
            dp--;
            pas[dp] = pas[dp] != pas[dp + 1];
          }
          else
          {
            sp++;
            pas[sp] = pas[sp] != pas[sp - 1];
          }  
          break;
        //LSS
        case 10:
          if(BP == gp)
          {
            dp--;
            pas[dp] = pas[dp] < pas[dp + 1];
          }
          else
          {
            sp++;
            pas[sp] = pas[sp] < pas[sp - 1];
          }
          break;
        //LEQ
        case 11:
          if(BP == gp)
          {
            dp--;
            pas[dp] = pas[dp] <= pas[dp + 1];
          }
          else
          {
            sp++;
            pas[sp] = pas[sp] <= pas[sp - 1];
          }
          break;
        //GTR
        case 12:
          if(BP == gp)
          {
            dp--;
            pas[dp] = pas[dp] > pas[dp + 1];
          }
          else
          {
            sp++;
            pas[sp] = pas[sp] > pas[sp - 1];
          }
          break;
        //GEQ
        case 13:
          if(BP == gp)
          {
            dp--;
            pas[dp] = pas[dp] >= pas[dp + 1];
          }
          else
          {
            sp++;
            pas[sp] = pas[sp] >= pas[sp - 1];
          }
          break;
        default:
          break;       
      }
      break;
    //LOD
    case 3:
      if(BP == gp)
      {
        dp++;
        pas[dp] = pas[gp + m];
      }
      else
      {
        sp--;
        if(base(L)== gp)
        {
          pas[sp] = pas[gp + m];          
        }
        else
        {
          pas[sp] = pas[base(L) - m];
        }
      }      
      break;
    //STO
    case 4:
      if(BP == gp)
      {
        pas[gp + m] = pas[dp];
        dp--;
      }
      else
      {
        if(base(L)== gp)
        {
          pas[sp] = pas[gp + m];          
        }
        else
        {
          pas[sp] = pas[base(L) - m];
        }
        sp++;
      }   
      break;
    //CAL
    case 5:
      pas[sp - 1] = base(L);
      pas[sp - 2] = BP;
      pas[sp - 3] = pc;
      BP = sp - 1;
      pc = m;
      break;
    //INC
    case 6:
      if(BP == gp)
      {
        dp += m;
      }
      else
      {
        sp -= m;
      }
      break;
    //JMP
    case 7:
      pc = m;
      break;
    //JPC
    case 8:
      if(BP == gp)
      {
        if(pas[dp] == 0)
        {
          pc = m;
        }
          dp--;
      }
      else
      {
        if(pas[sp] == 0)
        {
          pc = m;
        }
        sp++;
      }
      break;
    //SYS
    case 9:
      switch(m)
      {
        case 1:
          printf("Top of Stack Value:");
          if(BP == gp)
          {
            printf("%d", pas[dp]);
            dp--;
          }
          else
          {
            printf("%d", pas[sp]);
            sp++;
          }
          break;
        case 2:
          printf("Please Enter an Integer:");
          if(BP == gp)
          {
            dp++;
            scanf("%d", pas[dp]);
          }
          else
          {
            sp--;
            scanf("%d", pas[sp]);
          }    
          break;
        case 3:
          //set halt flag to zero
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }//end outer switch

  printf("\t\t\t\tPC\tBP\tSP\tDP\tdata\n");
  printf("Initial values:\t%d\t%d\t%d\t%d\n", pc, BP, sp, dp);
}//end main

/**********************************************/
/*		Find base L levels down		 */
/*							 */
/**********************************************/
 
int base(int L)
{
	int arb = BP;	// arb = activation record base
	while ( L > 0)     //find base L levels down
	{
		arb = pas[arb];
		L--;
	}
	return arb;
}

void print_execution(int line, char *opname, int *IR, int PC, int BP, int SP, int DP, int *pas, int GP)
{
	int i;
	// print out instruction and registers
	printf("%2d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t", line, opname, IR[1], IR[2], PC, BP, SP, DP);
	
	// print data section
	for (i = GP; i <= DP; i++)
		printf("%d ", pas[i]);
	printf("\n");
	
	// print stack
	printf("\tstack : ");
	for (i = MAX_PAS_LENGTH - 1; i >= SP; i--)
		printf("%d ", pas[i]);
	printf("\n");
}
