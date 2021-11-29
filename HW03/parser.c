#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"

#define MAX_CODE_LENGTH 1000
#define MAX_SYMBOL_COUNT 100
#define UNMARKED -1


instruction *code;
int cIndex;
symbol *table;
int tIndex;

void emit(int opname, int level, int mvalue);
void addToSymbolTable(int k, char n[], int v, int l, int a, int m);
void printparseerror(int err_code);
void printsymboltable();
void printassemblycode();

//custom globals
int lexLevel;
//custiom functions
void Block(lexeme* list);
void constant();
int variable(lexeme* list);
void Program();

instruction *parse(lexeme *list, int printTable, int printCode)
{
	code = NULL;
	/* this line is EXTREMELY IMPORTANT, you MUST uncomment it
		when you test your code otherwise IT WILL SEGFAULT in 
		vm.o THIS LINE IS HOW THE VM KNOWS WHERE THE CODE ENDS
		WHEN COPYING IT TO THE PAS
	code[cIndex].opcode = -1;
	*/
	//TODO Program
	Program();
	//TODO Block
	Block();

	//TODO Const-Declaration

	//TODO Var-Declaration

	//TODO Procedure-Declaration

	//TODO Statement

	//TODO Condition

	//TODO Expression

	//TODO Term

	//TODO Factor

	

	return code;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//custom functions
void Program()
{
	emit(7, lexLevel, 0) // Not sure if this is right, refer to assignmnet FAQ
	addToSymbolTable(3, "main", 0, 0, 0, UNMARKED);
	lexLevel = -1;
	Block()
	int token
	if()
}
void Block(lexeme* list)
{
	//Increment level
	lexLevel++;
	//procedure_idx = current symbol table index - 1
	int procedure_idx = tIndex - 1;

	//	TODO CONST-DECLARATION			not sure about these
	//	x = VAR-DECLARATION
	int x = variable(list);//x = number of variables
	//	TODO PROCEDURE-DECLARATION

	//table[procedure_idx].addr = current code index * 3
	table[procedure_idx].addr = cIndex * 3;
	//if level == 0
	if(lexLevel == 0)
	{
		//emit INC (M = x)
		emit(6, lexLevel, x);
	}
	else
	{
		//emit INC (M = x + 3)
		emit(6, lexLevel, x+3);
	}

	/* 	TODO
	STATEMENT
	MARK
	*/	

	//Decrement level 
	lexLevel--;
}
//CONST-Declaration
void constant(lexeme* list)
{
	lexeme curToken = list[lexLevel];
	if(curToken.type == constsym)
	{
		do
		{
			lexLevel++;
			curToken = list[lexLevel];
			if (curToken.type != identsym)
			{
				printparseerror(7);
			}
			int symidx = MULTIPLEDECLARATIONCHECK(curToken.type);
			if(symidx != -1)
			{
				printparseerror(19);
			}
			//save ident name

			lexLevel++;
			curToken = list[lexLevel];
			if (curToken.type != assignsym)
			{
				printparseerror(6);
			}
			lexLevel++;
			curToken = list[lexLevel];
			if (curToken.type != numbersym)
			{
				printparseerror(11);
			}
			//add to symbol table (kind 1, saved name, number, level, 0, unmarked)
			addToSymbolTable(1, curToken.name, curToken.value, lexLevel, 0, UNMARKED);
			lexLevel++;
			curToken = list[lexLevel];
		} while (curToken.type == commasym);
		  if (curToken.type != semicolonsym)
		  {
			  if (curToken.type == identsym)
			  {
				  printparseerror(13);
			  }
			  else
			  {
				  printparseerror(14);
			  }
		  }
		  lexLevel++;
		  curToken = list[lexLevel];
		}
		
}
//Variable-Declaration
int variable(lexeme* list)
{
	lexeme curToken = list[lexLevel];

	int numVars = 0;

	//if token == varsym
	if(curToken.type == varsym)
	{
		do
		{
			numVars++;
			lexLevel++;
			curToken = list[lexLevel];

			//if token != identsym
			if(curToken.type != identsym)
			{
				printparseerror(3);
			}//end if
			
			//symidx = MULTIPLEDECLARATIONCHECK(token)
			int symidx = MULTIPLEDECLARATIONCHECK(curToken);
			if(symidx != -1)
			{
				printparseerror(19)
			}//end if
			
			if(lexLevel == 0)
			{
				//add to symbol table (kind 2, ident, 0, level, numVars-1, unmarked)
				addToSymbolTable(2, curToken.name, 0, lexLevel, numVars-1, UNMARKED);
			}
			else
			{
				//add to symbol table (kind 2, ident, 0, level, numVars+2, unmarked)
				addToSymbolTable(2, curToken.name, 0, lexLevel, numVars+2, UNMARKED);
			}
			//get next token
			lexLevel++;
			curToken = list[lexLevel];
		} while (curToken.type == commasym);	//while token == commasym
		
		if(curToken.type != semicolonsym)
		{
			if(curToken.type == identsym)
			{
				printparseerror(13);
			}
			else
			{
				printparseerror(14);
			}//end inner if else
		}//end middle if

		//get next token NOTSURE
		lexLevel++;

	}//end if
	return numVars;
}
//Procedure-Declaration
void procedure(lexeme* list)
{
	lexeme curToken = list[lexLevel];
	while (curToken.type == procsym)
	{
		lexLevel++;
		curToken = list[lexLevel];
		if (curToken.type != identsym)
		{
			printparseerror(7);
		}
		int symidx = MULTIPLEDECLARATIONCHECK(curToken);
		if (symidx != -1)
		{
			printparseerror(19);
		}
		//add to symbol table (kind 3, ident, 0, level, 0, unmarked)
		addToSymbolTable(3, curToken.name, 0, lexLevel, 0, UNMARKED);
		lexLevel++;
		curToken = list[lexLevel];
		if (curToken.type != semicolonsym)
		{
			printparseerror(14);
		}
		lexLevel++;
		curToken = list[lexLevel];
		Block();
		if (curToken.type != semicolonsym)
		{
			printparseerror(14);
		}
		lexLevel++;
		curToken = list[lexLevel];
		emit();

	}
}

void statement()
{
	
}
void condition()
{

}
void expression()
{
	
}
void term()
{
	
}
void factor()
{
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//given functions


void emit(int opname, int level, int mvalue)
{
	code[cIndex].opcode = opname;
	code[cIndex].l = level;
	code[cIndex].m = mvalue;
	cIndex++;
}

void addToSymbolTable(int k, char n[], int v, int l, int a, int m)
{
	table[tIndex].kind = k;
	strcpy(table[tIndex].name, n);
	table[tIndex].val = v;
	table[tIndex].level = l;
	table[tIndex].addr = a;
	table[tIndex].mark = m;
	tIndex++;
}


void printparseerror(int err_code)
{
	switch (err_code)
	{
		case 1:
			printf("Parser Error: Program must be closed by a period\n");
			break;
		case 2:
			printf("Parser Error: Constant declarations should follow the pattern 'ident := number {, ident := number}'\n");
			break;
		case 3:
			printf("Parser Error: Variable declarations should follow the pattern 'ident {, ident}'\n");
			break;
		case 4:
			printf("Parser Error: Procedure declarations should follow the pattern 'ident ;'\n");
			break;
		case 5:
			printf("Parser Error: Variables must be assigned using :=\n");
			break;
		case 6:
			printf("Parser Error: Only variables may be assigned to or read\n");
			break;
		case 7:
			printf("Parser Error: call must be followed by a procedure identifier\n");
			break;
		case 8:
			printf("Parser Error: if must be followed by then\n");
			break;
		case 9:
			printf("Parser Error: while must be followed by do\n");
			break;
		case 10:
			printf("Parser Error: Relational operator missing from condition\n");
			break;
		case 11:
			printf("Parser Error: Arithmetic expressions may only contain arithmetic operators, numbers, parentheses, constants, and variables\n");
			break;
		case 12:
			printf("Parser Error: ( must be followed by )\n");
			break;
		case 13:
			printf("Parser Error: Multiple symbols in variable and constant declarations must be separated by commas\n");
			break;
		case 14:
			printf("Parser Error: Symbol declarations should close with a semicolon\n");
			break;
		case 15:
			printf("Parser Error: Statements within begin-end must be separated by a semicolon\n");
			break;
		case 16:
			printf("Parser Error: begin must be followed by end\n");
			break;
		case 17:
			printf("Parser Error: Bad arithmetic\n");
			break;
		case 18:
			printf("Parser Error: Confliciting symbol declarations\n");
			break;
		case 19:
			printf("Parser Error: Undeclared identifier\n");
			break;
		default:
			printf("Implementation Error: unrecognized error code\n");
			break;
	}
	
	free(code);
	free(table);
}

void printsymboltable()
{
	int i;
	printf("Symbol Table:\n");
	printf("Kind | Name        | Value | Level | Address | Mark\n");
	printf("---------------------------------------------------\n");
	for (i = 0; i < tIndex; i++)
		printf("%4d | %11s | %5d | %5d | %5d | %5d\n", table[i].kind, table[i].name, table[i].val, table[i].level, table[i].addr, table[i].mark); 
	
	free(table);
	table = NULL;
}

void printassemblycode()
{
	int i;
	printf("Line\tOP Code\tOP Name\tL\tM\n");
	for (i = 0; i < cIndex; i++)
	{
		printf("%d\t", i);
		printf("%d\t", code[i].opcode);
		switch (code[i].opcode)
		{
			case 1:
				printf("LIT\t");
				break;
			case 2:
				switch (code[i].m)
				{
					case 0:
						printf("RTN\t");
						break;
					case 1:
						printf("NEG\t");
						break;
					case 2:
						printf("ADD\t");
						break;
					case 3:
						printf("SUB\t");
						break;
					case 4:
						printf("MUL\t");
						break;
					case 5:
						printf("DIV\t");
						break;
					case 6:
						printf("ODD\t");
						break;
					case 7:
						printf("MOD\t");
						break;
					case 8:
						printf("EQL\t");
						break;
					case 9:
						printf("NEQ\t");
						break;
					case 10:
						printf("LSS\t");
						break;
					case 11:
						printf("LEQ\t");
						break;
					case 12:
						printf("GTR\t");
						break;
					case 13:
						printf("GEQ\t");
						break;
					default:
						printf("err\t");
						break;
				}
				break;
			case 3:
				printf("LOD\t");
				break;
			case 4:
				printf("STO\t");
				break;
			case 5:
				printf("CAL\t");
				break;
			case 6:
				printf("INC\t");
				break;
			case 7:
				printf("JMP\t");
				break;
			case 8:
				printf("JPC\t");
				break;
			case 9:
				switch (code[i].m)
				{
					case 1:
						printf("WRT\t");
						break;
					case 2:
						printf("RED\t");
						break;
					case 3:
						printf("HAL\t");
						break;
					default:
						printf("err\t");
						break;
				}
				break;
			default:
				printf("err\t");
				break;
		}
		printf("%d\t%d\n", code[i].l, code[i].m);
	}
	if (table != NULL)
		free(table);
}