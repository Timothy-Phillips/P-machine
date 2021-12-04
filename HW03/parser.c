#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"

#define MAX_CODE_LENGTH 1000
#define MAX_SYMBOL_COUNT 100
#define UNMARKED -1
#define MARKED 1

instruction *code;
int cIndex;
symbol *table;
int tIndex;
int flag = 0;
char ident[12];
void printList(lexeme* list);
const char* getTokenType(enum token_type t);

void emit(int opname, int level, int mvalue);
void addToSymbolTable(int k, char n[], int v, int l, int a, int m);
void printparseerror(int err_code);
void printsymboltable();
void printassemblycode();
//custom globals
int lexLevel;
//custiom functions
void Program(lexeme* list);
void Block(lexeme* list);
void constant(lexeme* list);
int variable(lexeme* list);
void expression(lexeme* list);
void term(lexeme* list);
void factor(lexeme* list);
void procedure(lexeme* list);
void statement(lexeme* list);
void condition(lexeme* list);

int FINDSYMBOL(lexeme token, int kind);
void MARK();
int MULTIPLEDECLARATIONCHECK(lexeme token);


instruction *parse(lexeme *list, int printTable, int printCode)
{
	code = calloc(MAX_CODE_LENGTH, sizeof(instruction));
	table = calloc(MAX_SYMBOL_COUNT, sizeof(symbol));
	/* this line is EXTREMELY IMPORTANT, you MUST uncomment it
		when you test your code otherwise IT WILL SEGFAULT in 
		vm.o THIS LINE IS HOW THE VM KNOWS WHERE THE CODE ENDS
		WHEN COPYING IT TO THE PAS
	*/
	cIndex = 0;
	lexLevel = 0;
	// printf("START PARSE");
	Program(list);
	if(flag == 1)
	{
		code[cIndex].opcode = -1;
		return NULL;
	}
	code[cIndex].opcode = -1;

	return code;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//custom functions
void Program(lexeme* list)
{
	if(flag == 1)
	{
		return;
	}
	emit(7, 0, 0); // Emit JMP*//
	addToSymbolTable(3, "main", 0, lexLevel, 0, UNMARKED);
	lexLevel = -1;
	//adding this from discord
	// lexLevel++;
	lexeme curToken = list[lexLevel];

	Block(list);
	if(flag == 1)
	{
		return;
	}
	curToken = list[lexLevel];
	
	if(curToken.type != periodsym)
		printparseerror(1);
		flag = 1;
		return;

		
	emit(9, 0, 3); // Emit Halt
	
	for (int i = 0; i < cIndex; i++)
	{
		// if opcode is Call
		if (code[i].opcode == 5) 
			code[i].m = table[code[i].m].addr;
			
		code[0].m = table[0].addr;
	}
	
}
void Block(lexeme* list)
{
	if(flag == 1)
	{
		return;
	}
	//Increment level
	lexLevel++;
	//procedure_idx = current symbol table index - 1
	int procedure_idx = tIndex - 1;

	constant(list);
	if(flag == 1)
	{
		return;
	}
	int x = variable(list);//x = number of variables
	if(flag == 1)
	{
		return;
	}
	procedure(list);
	if(flag == 1)
	{
		return;
	}
	//table[procedure_idx].addr = current code index * 3
	table[procedure_idx].addr = cIndex * 3;
	//if level == 0
	if(lexLevel == 0)
	{
		//emit INC (M = x)
		emit(6, 0, x);
	}
	else
	{
		//emit INC (M = x + 3)
		emit(6, 0, x+3);
	}


	statement(list);
	if(flag == 1)
	{
		return;
	}
	MARK();

	//Decrement level 
	lexLevel--;
}
//CONST-Declaration
void constant(lexeme* list)
{
	if(flag == 1)
	{
		return;
	}
	lexeme curToken = list[lexLevel];
	if(curToken.type == constsym)
	{
		do
		{
			lexLevel++;
			curToken = list[lexLevel];
			if (curToken.type != identsym)
			{
				printparseerror(2);
				flag = 1;
				return;
			}
			int symidx = MULTIPLEDECLARATIONCHECK(curToken);
			if(symidx != -1)
			{
				printparseerror(18);
				flag = 1; 
				return;
			}
			//save ident name
			strcpy(ident, curToken.name);
			lexLevel++;
			curToken = list[lexLevel];
			if (curToken.type != assignsym)
			{
				printparseerror(6);
				flag = 1;
				return;
			}
			lexLevel++;
			curToken = list[lexLevel];
			if (curToken.type != numbersym)
			{
				printparseerror(11);
				flag = 1;
				return;
			}
			//add to symbol table (kind 1, saved name, number, level, 0, unmarked)
			addToSymbolTable(1, ident, curToken.value, lexLevel, 0, UNMARKED);
			lexLevel++;
			curToken = list[lexLevel];
		} while (curToken.type == commasym);
		  if (curToken.type != semicolonsym)
		  {
			  if (curToken.type == identsym)
			  {
				  printparseerror(14);
				  flag = 1;
				  return;
			  }
			  else
			  {
				  printparseerror(13);
				  flag = 1;
				  return;
			  }
		  }
		  lexLevel++;
		  curToken = list[lexLevel];
	}
		
}
//Variable-Declaration
int variable(lexeme* list)
{
	if(flag == 1)
	{
		return 0;
	}
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
				flag = 1;
				return 0;
			}//end if
			
			//symidx = MULTIPLEDECLARATIONCHECK(token)
			int symidx = MULTIPLEDECLARATIONCHECK(curToken);
			if(symidx != -1)
			{
				printparseerror(3);
				flag = 1;
				return 0;
			}//end if
			
			if(lexLevel == 0)
			{
				//add to symbol table (kind 2, ident, 0, level, numVars-1, unmarked)
				addToSymbolTable(2, ident, 0, lexLevel, numVars-1, UNMARKED);
			}
			else
			{
				//add to symbol table (kind 2, ident, 0, level, numVars+2, unmarked)
				addToSymbolTable(2, ident, 0, lexLevel, numVars+2, UNMARKED);
			}
			//get next token
			lexLevel++;
			curToken = list[lexLevel];
		} while (curToken.type == commasym);	//while token == commasym
		
		if(curToken.type != semicolonsym)
		{
			if(curToken.type == identsym)
			{
				printparseerror(14);
				flag = 1;
				return 0;
			}
			else
			{
				printparseerror(13);
				flag = 1;
				return 0;
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
	if(flag == 1)
	{
		return;
	}
	lexeme curToken = list[lexLevel];
	while (curToken.type == procsym)
	{
		lexLevel++;
		curToken = list[lexLevel];
		if (curToken.type != identsym)
		{
			printparseerror(7);
			flag = 1;
			return;
		}
		int symidx = MULTIPLEDECLARATIONCHECK(curToken);
		if (symidx != -1)
		{
			printparseerror(19);
			flag = 1;
			return;
		}
		//add to symbol table (kind 3, ident, 0, level, 0, unmarked)
		addToSymbolTable(3, ident, 0, lexLevel, 0, UNMARKED);
		lexLevel++;
		curToken = list[lexLevel];
		if (curToken.type != semicolonsym)
		{
			printparseerror(14);
			flag = 1;
			return;
		}
		lexLevel++;
		curToken = list[lexLevel];
		Block(list);
		if(flag == 1)
		{
			return;
		}
		if (curToken.type != semicolonsym)
		{
			printparseerror(14);
			flag = 1;
			return;
		}
		lexLevel++;
		curToken = list[lexLevel];
		emit(2, 0, 0);

	}
}

void statement(lexeme* list)
{
	if(flag == 1)
	{
		return;
	}
	lexeme curToken = list[lexLevel];
	int symIdx = 0;
	if(curToken.type == identsym)
	{
		symIdx = FINDSYMBOL(curToken, 2);
 		if (symIdx == -1)
		{
 			if (FINDSYMBOL(curToken, 1) != FINDSYMBOL (curToken, 3))
			{
				printparseerror(18);
				flag = 1;
				return;
			}
			else
			{
				printparseerror(19);
				flag = 1;
				return;
			}//end if else
		}//end if

		//get next token
		lexLevel++;
		curToken = list[lexLevel];

 		if (curToken.type != assignsym)
		{
			printList(list);
			// printf("\n\n***************************\n\tname:\t%svalue\t%d\ttype:\t\n***************************\n",curToken.name, curToken.value );
			printparseerror(5);
			flag = 1;
			return;
		}//end if
		//get next token
		lexLevel++;
		curToken = list[lexLevel];		
		
		expression(list);
		if(flag == 1)
		{
			return;
		}
		//emit STO (L = level – table[symIdx].level, M = table[symIdx].addr)
		// printf("L: %d, M: %d",(lexLevel - table[symIdx].level), table[symIdx].addr);
		emit(4, (lexLevel - table[symIdx].level), table[symIdx].addr);
		return;
	}//end outer if				 

	if(curToken.type == beginsym)
	{
		do
		{
			//get next token
			lexLevel++;
			curToken = list[lexLevel];
			statement(list);
			if(flag == 1)
			{
				return;
			}
		} while (curToken.type == semicolonsym);// while token == semicolonsym

		if(curToken.type != endsym)
		{
			//if token == identsym, beginsym, ifsym, whilesym, readsym, writesym,or callsym
			if(curToken.type == identsym || curToken.type == beginsym || curToken.type == ifsym || curToken.type == whilesym || curToken.type == readsym || curToken.type == writesym || curToken.type == callsym)
			{
				printList(list);
				printparseerror(15);
				flag = 1;
				return;
			}
			else
			{
				printparseerror(16);
				flag = 1;
				return;
			}//end if else			
		}//end if

		//get next token
		lexLevel++;
		curToken = list[lexLevel];		
		return;
	}//end if
	if(curToken.type == ifsym)
	{
		//get next token
		lexLevel++;
		curToken = list[lexLevel];	

		condition(list);

		int jpcIdx = cIndex;
		emit(8, 0, jpcIdx); //NOTSURE

		if(curToken.type != thensym)
		{
			printparseerror(8);
			flag = 1;
			return;
		}
		//get next token
		lexLevel++;
		curToken = list[lexLevel];	
		
		statement(list);
		
 		if(curToken.type == elsesym)
		{
			int jmpIdx = cIndex;
			emit(7, 0, jmpIdx); //NOTSURE
			code[jpcIdx].m = cIndex * 3;
			//missing from pseudo coe
			lexLevel++;
			curToken = list[lexLevel];

			statement(list);
			code[jmpIdx].m = cIndex * 3;
		}//end if
		else
		{
			code[jpcIdx].m = cIndex * 3;
		}
		return;
	}//end ifsym
	if(curToken.type == whilesym)
	{
		// get next token
		lexLevel++;
		curToken = list[lexLevel];

		int loopIdx = cIndex;
		condition(list);
		if(flag == 1)
		{
			return;
		}
		if(curToken.type != dosym)
		{
			printparseerror(9);
			flag = 1;
			return;
		}
		// get next token
		lexLevel++;
		curToken = list[lexLevel];

		int jpcIdx = cIndex;
		emit(8, 0, jpcIdx);
		statement(list);
		if(flag == 1)
		{
			return;
		}		
		emit(9, 0, loopIdx * 3);
		code[jpcIdx].m = cIndex * 3;
		return;		
	}//end whilesym

	if(curToken.type == readsym)
	{
		// get next token
		lexLevel++;
		curToken = list[lexLevel];	
		if(curToken.type != identsym)
		{
			printparseerror(6);
			flag = 1;
			return;
		}//end if
		int symIdx = FINDSYMBOL(curToken, 2);
		if(symIdx == -1)
		{
			if(FINDSYMBOL(curToken,1) != FINDSYMBOL(curToken,3))
			{
				printparseerror(18);
				flag = 1;
				return;
			}
			else
			{
				printparseerror(19);
				flag = 1;
				return;
			}//end if else
		}//end if
		
		// get next token
		lexLevel++;
		curToken = list[lexLevel];	

		emit(9, 0, 2);
	// 	emit STO (L = level – table[symIdx].level, M = table[symIdx].addr)
		emit(4, (lexLevel - table[symIdx].level), table[symIdx].addr);
		return;
	}//end readsym
	if(curToken.type == writesym)
	{
		// get next token
		lexLevel++;
		curToken = list[lexLevel];	

		expression(list);
		if(flag == 1)
		{
			return;
		}
		emit(9, 0, 1);
		return;
	// 	return

	}//end writesym
	if(curToken.type == callsym)
	{
		// get next token
		lexLevel++;
		curToken = list[lexLevel];	

		int symIdx = FINDSYMBOL(curToken, 3);
		if(symIdx == -1)
		{
			if(FINDSYMBOL(curToken, 1) != FINDSYMBOL(curToken, 2))
			{
				printparseerror(18);
				flag = 1;
				return;
			}
			else
			{
				printparseerror(19);
				flag = 1;
				return;
			}
		}//end if

		// get next token
		lexLevel++;
		curToken = list[lexLevel];	
		//emit CAL (L = level – table[symIdx].level, symIdx)
		emit(5, lexLevel- table[symIdx].level, symIdx);
		return;
	}//end callsym
	
}
void condition(lexeme* list)
{
	if(flag == 1)
	{
		return;
	}

	lexeme curToken = list[lexLevel];
	if (curToken.type == oddsym)
	{
		lexLevel++;
		curToken = list[lexLevel];
		expression(list);
		if(flag == 1)
		{
			return;
		}
		emit(2, 0, 6);
	}
	else
	{
		expression(list);
		if(flag == 1)
		{
			return;
		}
		if (curToken.type == eqlsym)
		{
			lexLevel++;
			curToken = list[lexLevel];
			expression(list);
			if(flag == 1)
			{
				return;
			}			
			emit(2, 0, 8);
		}
		else if (curToken.type == neqsym)
		{
			lexLevel++;
			curToken = list[lexLevel];
			expression(list);
			if(flag == 1)
			{
				return;
			}			
			emit(2, 0, 9);
		}
		else if (curToken.type == lsssym)
		{
			lexLevel++;
			curToken = list[lexLevel];
			expression(list);
			if(flag == 1)
			{
				return;
			}			
			emit(2, 0, 10);
		}
		else if (curToken.type == leqsym)
		{
			lexLevel++;
			curToken = list[lexLevel];
			expression(list);
			if(flag == 1)
			{
				return;
			}			
			emit(2, 0, 11);
		}
		else if (curToken.type == gtrsym)
		{
			lexLevel++;
			curToken = list[lexLevel];
			expression(list);
			if(flag == 1)
			{
				return;
			}			
			emit(2, 0, 12);
		}
		else if (curToken.type == geqsym)
		{
			lexLevel++;
			curToken = list[lexLevel];
			expression(list);
			if(flag == 1)
			{
				return;
			}			
			emit(2, 0, 13);
		}
		else
		{
			printparseerror(10);
			flag = 1;
			return;
		}
	}
}
void expression(lexeme* list)
{
	if(flag == 1)
	{
		return;
	}
	lexeme curToken = list[lexLevel];
	
	if (curToken.type == subsym)
	{
		lexLevel++;
		curToken = list[lexLevel];
		term(list);
		if(flag == 1)
		{
			return;
		}			
		emit(2, 0, 1); // Emit Neg

		while (curToken.type == addsym || curToken.type == subsym)
		{
			if (curToken.type == addsym)
			{
				lexLevel++;
				curToken = list[lexLevel];
				term(list);
		if(flag == 1)
		{
			return;
		}			
				emit(2, 0, 2);  // Emit Add
			}
			else
			{
				lexLevel++;
				curToken = list[lexLevel];
				term(list);
		if(flag == 1)
		{
			return;
		}			
				emit(2, 0, 3);  // Emit Sub
			}
		}
	}
	
	else
	{
		if (curToken.type == addsym)
		{
			lexLevel++;
			curToken = list[lexLevel];
		}
		
		term(list);
		while (curToken.type == addsym || curToken.type == subsym)
		{
			if (curToken.type == addsym)
			{
				lexLevel++;
				curToken = list[lexLevel];
				term(list);
		if(flag == 1)
		{
			return;
		}			
				emit(2, 0, 2); // Emit Add
			}
			else
			{
				lexLevel++;
				curToken = list[lexLevel];
				term(list);
		if(flag == 1)
		{
			return;
		}			
				emit(2, 0, 3); // Emit Sub
			}
		}
	}
	// FINISH THIS LINE vvv
	// if (curToken.type == rparensym || /* curToken.type == identsym || */ curToken.type == numbersym )
	// {
	// 	// printsymboltable();
	// 	// printf("\n\n***************************\n\tname:\t%svalue\t%d\ttype:\t",list[lexLevel].name, list[lexLevel].value );

	// 	printparseerror(17);
	// 	flag = 1;
	// 	return;
	// }
}
void term(lexeme* list)
{
	if(flag == 1)
	{
		return;
	}
	factor(list);
		if(flag == 1)
		{
			return;
		}			
	lexeme curToken = list[lexLevel];
	while (curToken.type == multsym || curToken.type == divsym || curToken.type == modsym)
	{
		if (curToken.type == multsym)
		{
			lexLevel++;
			curToken = list[lexLevel];
			factor(list);
		if(flag == 1)
		{
			return;
		}			
			emit(2, 0, 4); // Emit Mul
		}
		else if (curToken.type == divsym)
		{
			lexLevel++;
			curToken = list[lexLevel];
			factor(list);
		if(flag == 1)
		{
			return;
		}			
			emit(2, 0, 5); // Emit Div
		}
		else
		{
			lexLevel++;
			curToken = list[lexLevel];
			factor(list);
		if(flag == 1)
		{
			return;
		}			
			emit(2, 0, 7); // Emit Mod
		}
	}
}
void factor(lexeme* list)
{
	if(flag == 1)
	{
		return;
	}
	lexeme curToken = list[lexLevel];
	int symIdx_var = 0;
	int symIdx_const = 0;
	if (curToken.type == identsym)
	{
		symIdx_var = FINDSYMBOL(curToken, 2);
		symIdx_const = FINDSYMBOL(curToken, 1);
		if ((symIdx_var == -1) && (symIdx_const == -1))
		{
			if (FINDSYMBOL(curToken, 3) != -1)
			{
				printparseerror(14);
				flag = 1;
				return;
			}
			else
			{
				printparseerror(19);
				flag = 1;
				return;
			}
		if (symIdx_var == -1)
		{
			int m = table[symIdx_const].val;
			emit(1, 0, m);
		}
		else if ((symIdx_const == -1) || (table[symIdx_var].level) > table[symIdx_const].level)
		{
			int l = lexLevel - table[symIdx_var].level;
			int m = table[symIdx_var].addr;
			emit(3,l, m);
		}
		else
		{
			int m = table[symIdx_const].val;
			emit(1, 0, m);
		}
		lexLevel++;
		curToken = list[lexLevel];
	}
	else if (curToken.type == numbersym)
	{
		emit(1, 0, curToken.value);
		lexLevel++;
		curToken = list[lexLevel];
	}
	else if (curToken.type == lparensym)
	{
		lexLevel++;
		curToken = list[lexLevel];
		expression(list);
		if(flag == 1)
		{
			return;
		}			
		if (curToken.type != rparensym)
		{
			printparseerror(12);
				flag = 1;
				return;
		}
		lexLevel++;
		curToken = list[lexLevel];
	}
	else
	{
		printparseerror(18);
						flag = 1;
				return;
	}
}
}
int FINDSYMBOL(lexeme token, int kind)
{
	int result = 0;
	for(int x = 0; x < tIndex; x++)
	{
		if(strcmp(token.name, table[x].name) == 0 && table[x].kind == kind && table[x].mark == UNMARKED)
		{
			result = table[x].level;
		}//end if
	}//end for loop
	return result;
}//end findSymbol;
void MARK()
{
	for(int x = tIndex - 1; x >= 0; x--)
	{
		if(table[x].mark == UNMARKED)
		{
			if(table[x].level == lexLevel)
			{
				table[x].mark = MARKED;
			}//end if
			else if(table[x].level < lexLevel)
			{
				return;
			}
		}//end if
	}//end for loop
}//end MARK
int MULTIPLEDECLARATIONCHECK(lexeme token)
{
	for(int x  = 0; x < tIndex; x++)
	{
		if(strcmp(token.name, table[x].name) == 0)
		{
			if(table[x].mark != UNMARKED)
			{
				if(table[x].level == lexLevel)
				{
					return x;
				}//end inner if
			}//end middle if
		}//end outer if
	}//end for loop
	return -1;
}//end multippledeclationcheck
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//given functions
void emit(int opname, int level, int mvalue)
{
	code[cIndex].opcode = opname;
	code[cIndex].l = level;
	code[cIndex].m = mvalue;
	cIndex++;
	// printassemblycode();
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
void printList(lexeme* list)
{
	for(int i = 0; i < lexLevel; i++)
	{
		printf("\n\n***************************\n\tname:\t%s\tvalue:\t%dtype:\t%s\n***************************\n",list[i].name, list[i].value, getTokenType(list[i].type));
	}
}
const char* getTokenType(enum token_type t)
{
	switch( t)
	{
		case constsym:
			return "const";
		case varsym:
			return "var";
		case procsym:
			return "proc";
		case beginsym:
			return "begin";
		case endsym:
			return "end";
		case whilesym:
			return "while";
		case dosym:
			return "do";
		case ifsym:
			return "if";
		case thensym:
			return "then";
		case elsesym:
			return "else";
		case callsym:
			return "call";
		case writesym:
			return "write";
		case readsym:
			return "read";
		case identsym:
			return "ident";
		case numbersym:
			return "number";
		case assignsym:
			return "assign";
		case addsym:
			return "add";
		case subsym:
			return "sub";
		case multsym:
			return "mult";
		case divsym:
			return "div";
		case modsym:
			return "mod";
		case eqlsym:
			return "eql";
		case neqsym:
			return "neq";
		case lsssym:
			return "lss";
		case leqsym:
			return "leq";
		case gtrsym:
			return "gtr";
		case geqsym:
			return "geq";
		case oddsym:
			return "odd";
		case lparensym:
			return "lparen";
		case rparensym:
			return "rparen";
		case commasym:
			return "comma";
		case periodsym:
			return "period";
		case semicolonsym:
			return "semicolon";
		default:
			return"";
	}
}