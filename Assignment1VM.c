/*
    Students:
     Timothy Phillips
     Kimberly Szekely
     Edgard Irineo
     Marcos Chacon
*/
#include <stdio.h>
#include <string.h>
#define PAS_SIZE 500
int MAX_PAS_LENGTH = 500;

// IR struct declaration due to need for both string and char in same var
// struct IR
// {
//   int op;
//   int l;
//   int m;
// };

// Simple Placeholder Implementation
// int m = 0;
// int op = 0;
// int L = 0;

// Kims register variables, use or dont use them.
//•	Create PAS and Registers
int GP = 0;
int IC = 0;
int DP = 0;
int FREE = 0;
int BP = 0;
int PC = 0;
int SP = 0;
int PAS[PAS_SIZE];

int base(int L);
void print_execution(int line, char *opname, int *IR, int PC, int BP, int SP, int DP, int *PAS, int GP);

int main(int argc, char **argv)
{
  int halt = 1;
  char opname[4];
  
  // Initializing the PAS array to 0
  for (int i = 0; i < PAS_SIZE; i++)
  {
    PAS[i] = 0;
  }

  // Initializing an instance of the IR struct and then setting each internal variable
  // struct IR IR1;
  // IR1.op = 0;
  // IR1.l = 0;
  // IR1.m = 0;
  int IR[3];
  FILE *fp;
  int temp, instructionCount, index = -1;
  //•	Read input file name from command line and open file
  // Read instructions from file
  if ((fp = fopen(argv[1], "r")) == NULL)
    return -1;
  //•	Read in all program instructions from file to text section
  while (!feof (fp) && fscanf (fp, "%d", &temp) && index++ < PAS_SIZE )
    PAS[index] = temp;

  fclose(fp);

  instructionCount = index + 1;

  for(int i = 0; i < instructionCount; i = i + 3)
  {
    printf("%d %d %d\n", PAS[i], PAS[i + 1], PAS[i + 2]);
    IC += 3;
  }

  //•	Setup registers based on location of instruction counter 
  GP = IC;
  DP = IC - 1;
  FREE = IC + 40;
  BP = IC;
  PC = 0;
  SP = MAX_PAS_LENGTH;

  //•	Print Column Headers to Console
  printf("PC BP SP DP data\n");;

  //•	Print Initial Values for each Register to Console
  printf("Initial values: %d %d %d %d", PC, BP, SP, DP);
  for(int i = GP; i < DP; i++)
  {
    printf(" %d", PAS[i]);
  }

  printf("\n");
 //Fetch Cycle, fetches the next instruction stored in PC and puts it in the IR, then updates the PC
  while (PC != IC && halt != 0) {
  // IR = PAS[PC];
	// PC += 3;
    IR[0] = PAS[PC++];
    IR[1] = PAS[PC++];
    IR[2] = PAS[PC++];

    //•	Enter Execute Cycle						
    switch(IR[0])
    {
      //LIT
      case 1:
        strcpy(opname, "LIT");
        if(BP == GP)
        {
          DP++;
          PAS[DP] = IR[2];
        }
        else
        {
          SP--;
          PAS[SP] = IR[2];
        }
        break;
      //OPR
      case 2:
        switch(IR[2])
        {
          //RTN
          case 0:
            strcpy(opname, "RTN");
            SP = BP + 1;
            BP = PAS[SP - 2];
            PC = PAS[SP - 3];
            break;
          //NEG
          case 1:
            strcpy (opname, "NEG");
            if(BP == GP)
            {
              PAS[DP] = (-1 * PAS[DP]);
            }
            else
            {
              PAS[SP] = (-1 * PAS[SP]);
            }
            break;
          //ADD
          case 2:
            strcpy(opname, "ADD");
            if(BP == GP)
            {
              DP--;
              PAS[DP] += PAS[DP + 1];
            }
            else
            {
              SP++;
              PAS[SP] += PAS[SP - 1];
            }
            break;
          //SUB      
          case 3:
            strcpy(opname, "SUB");
            if(BP == GP)
            {
              DP--;
              PAS[DP] = PAS[DP] - PAS[DP + 1];
            }
            else
            {
              SP++;
              PAS[SP] = PAS[SP] - PAS[SP - 1];
            }
            break;
          //MUL
          case 4:
            strcpy(opname, "MUL");
            if(BP == GP)
            {
              DP--;
              PAS[DP] *= PAS[DP + 1];
            }
            else
            {
              SP++;
              PAS[SP] *= PAS[SP - 1];
            }
            break;
          //DIV
          case 5:
            strcpy(opname, "DIV");
            if(BP == GP)
            {
              DP--;
              PAS[DP] = PAS[DP] / PAS[DP + 1];
            }
            else
            {
              SP++;
              PAS[SP] = PAS[SP] / PAS[SP - 1];
            }
            break;
          //ODD
          case 6:
            strcpy(opname, "ODD");
            if(BP == GP)
            {
              PAS[DP] = PAS[DP] % 2;
            }
            else
            {
              PAS[SP] *= PAS[SP] % 2;
            }          
            break;
          //MOD
          case 7:
            strcpy(opname, "MOD");
            if(BP == GP)
            {
              DP--;
              PAS[DP] = PAS[DP] % PAS[DP + 1];
            }
            else
            {
              SP++;
              PAS[SP] = PAS[SP] % PAS[SP - 1];
            }                    
            break;
          //EQL
          case 8:
            strcpy(opname, "EQL");
            if(BP == GP)
            {
              DP--;
              PAS[DP] = PAS[DP] == PAS[DP + 1];
            }
            else
            {
              SP++;
              PAS[SP] = PAS[SP] == PAS[SP - 1];
            }          
            break;
          //NEQ
          case 9:
            strcpy(opname, "NEQ");
            if(BP == GP)
            {
              DP--;
              PAS[DP] = PAS[DP] != PAS[DP + 1];
            }
            else
            {
              SP++;
              PAS[SP] = PAS[SP] != PAS[SP - 1];
            }  
            break;
          //LSS
          case 10:
            strcpy(opname, "LSS");
            if(BP == GP)
            {
              DP--;
              PAS[DP] = PAS[DP] < PAS[DP + 1];
            }
            else
            {
              SP++;
              PAS[SP] = PAS[SP] < PAS[SP - 1];
            }
            break;
          //LEQ
          case 11:
            strcpy(opname, "LEQ");
            if(BP == GP)
            {
              DP--;
              PAS[DP] = PAS[DP] <= PAS[DP + 1];
            }
            else
            {
              SP++;
              PAS[SP] = PAS[SP] <= PAS[SP - 1];
            }
            break;
          //GTR
          case 12:
            strcpy(opname, "GTR");
            if(BP == GP)
            {
              DP--;
              PAS[DP] = PAS[DP] > PAS[DP + 1];
            }
            else
            {
              SP++;
              PAS[SP] = PAS[SP] > PAS[SP - 1];
            }
            break;
          //GEQ
          case 13:
            strcpy(opname, "GEQ");
            if(BP == GP)
            {
              DP--;
              PAS[DP] = PAS[DP] >= PAS[DP + 1];
            }
            else
            {
              SP++;
              PAS[SP] = PAS[SP] >= PAS[SP - 1];
            }
            break;
          default:
            break;       
        }
        break;
      //LOD
      case 3:
        strcpy(opname, "LOD");
        if(BP == GP)
        {
          DP++;
          PAS[DP] = PAS[GP + IR[2]];
        }
        else
        {
          SP--;
          if(base(IR[1])== GP)
          {
            PAS[SP] = PAS[GP + IR[2]];
          }
          else
          {
            PAS[SP] = PAS[base(IR[1]) - IR[2]];
          }
        }      
        break;
      //STO
      case 4:
        strcpy(opname, "STO");
        if(BP == GP)
        {
          PAS[GP + IR[2]] = PAS[DP];
          DP--;
        }
        else
        {
          if(base(IR[1])== GP)
          {
            PAS[SP] = PAS[GP + IR[2]];          
          }
          else
          {
            PAS[SP] = PAS[base(IR[1]) - IR[2]];
          }
          SP++;
        }   
        break;
      //CAL
      case 5:
        strcpy(opname, "CAL");
        PAS[SP - 1] = base(IR[1]);
        PAS[SP - 2] = BP;
        PAS[SP - 3] = PC;
        BP = SP - 1;
        PC = IR[2];
        break;
      //INC
      case 6:
        strcpy(opname, "INC");
        if(BP == GP)
        {
          DP += IR[2];
        }
        else
        {
          SP -= IR[2];
        }
        break;
      //JMP
      case 7:
        strcpy(opname, "JMP");
        PC = IR[2];
        break;
      //JPC
      case 8:
        strcpy(opname, "JPC");
        if(BP == GP)
        {
          if(PAS[DP] == 0)
          {
            PC = IR[2];
          }
            DP--;
        }
        else
        {
          if(PAS[SP] == 0)
          {
            PC = IR[2];
          }
          SP++;
        }
        break;
      //SYS
      case 9:
        strcpy(opname, "SYS");
        switch(IR[2])
        {
          case 1:
            printf("Top of Stack Value:");
            if(BP == GP)
            {
              printf("%d", PAS[DP]);
              DP--;
            }
            else
            {
              printf("%d", PAS[SP]);
              SP++;
            }
            break;
          case 2:
            printf("Please Enter an Integer:");
            if(BP == GP)
            {
              DP++;
              scanf("%d", &PAS[DP]);
            }
            else
            {
              SP--;
              scanf("%d", &PAS[SP]);
            }    
            break;
          case 3:
            halt = 0;
            break;
          default:
            break;
        }
        break;
      default:
        break;
    }//end outer switch
  
    //•	Call print function with appropriate values (register values, oPCode name, line number, etc.)
    print_execution( (PC / 3), opname, IR, PC, BP, SP, DP, PAS, GP);
  }//end loop

 



  // printf("\t\t\t\tPC\tBP\tSP\tDP\tdata\n");
  // printf("Initial values:\t%d\t%d\t%d\t%d\n", PC, BP, SP, DP);
  return 0;
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
    //  POSSIBLE EROR ALERT: Changed PAS var to PAS to match rest of doc, if its wrong can be changed back
		arb = PAS[arb];
		L--;
	}
	return arb;
}

void print_execution(int line, char *opname, int *IR, int PC, int BP, int SP, int DP, int *PAS, int GP)
{
	int i;
	// print out instruction and registers
	printf("%2d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t", line, opname, IR[1], IR[2], PC, BP, SP, DP);
	
	// print data section
	for (i = GP; i <= DP; i++)
		printf("%d ", PAS[i]);
	printf("\n");
	
	// print stack
	printf("\tstack : ");
	for (i = MAX_PAS_LENGTH - 1; i >= SP; i--)
		printf("%d ", PAS[i]);
	printf("\n");

	for (i = MAX_PAS_LENGTH - 1; i >= SP; i--)
		printf("%d ", PAS[i]);
	printf("\n");

	for (i = MAX_PAS_LENGTH - 1; i >= SP; i--)
		printf("%d ", PAS[i]);
	printf("\n");
}
