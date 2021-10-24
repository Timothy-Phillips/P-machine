/* 
	Group Members:
	Timothy Phillips
	Marcos Chacon
	Kimberly Szekely
	Edgard Irineo

	This is the lex.c file for the UCF Fall 2021 Systems Software Project.
	For HW2, you must implement the function lexeme *lexanalyzer(char *input).
	You may add as many constants, global variables, and support functions
	as you desire.
	
	If you choose to alter the printing functions or delete list or lex_index, 
	you MUST make a note of that in you readme file, otherwise you will lose 
	5 points.
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "compiler.h"
#define MAX_NUMBER_TOKENS 500
#define MAX_IDENT_LEN 11
#define MAX_NUMBER_LEN 5

lexeme *list;
//create global integer to track list index
int lex_index;

int lookahead(char *input, char *target);
lexeme* constructLex(char *name, int value, token_type type);
void printlexerror(int type);
void printtokens();
char token;


lexeme *lexanalyzer(char *input)
{
	//allocate memory for the global list of lexems
	//lexeme* result;
	int length = strlen(input);
	list = calloc(length, sizeof(lexeme));
	lex_index = 0;
	
	//step through the text character by character
	for(int i = 0; i < length; i++)
	{
		token = input[i];
		
		
		// White Space
		if (iscntrl(token)||isspace(token))
		{
			continue;
		}	
		//special symbols
		//== !=  <  <=  >  >=  %  *  /  +  -  (  )  ,  .  ;  :=			
		// 33 Special Symbols 
		else if (lookahead(&input[i], "const"))
		{
			list[lex_index] = *(constructLex("constsym", 1, constsym));
			lex_index++;
			i += 4;
			continue;
		}
		else if (lookahead(&input[i], "var"))
		{
			list[lex_index] = *(constructLex("varsym", 2, varsym));
			lex_index++;
			i += 2;
			continue;
		}
		else if (lookahead(&input[i], "procedure"))
		{
			list[lex_index] = *(constructLex("procsym", 3, procsym));
			lex_index++;
			i += 8;
			continue;
		}
		else if (lookahead(&input[i], "begin\n"))
		{
			list[lex_index] = *(constructLex("beginsym", 4, beginsym));
			lex_index++;
			i += 4;
			continue;
		}
		else if (lookahead(&input[i], "end"))
		{
			list[lex_index] = *(constructLex("endsym", 5, endsym));
			lex_index++;
			i += 2;
			continue;
		}
		else if (lookahead(&input[i], "while"))
		{
			list[lex_index] = *(constructLex("whilesym", 6, whilesym));
			lex_index++;
			i += 4;
			continue;
		}
		else if (lookahead(&input[i], "do"))
		{
			list[lex_index] = *(constructLex("dosym", 7, dosym));
			lex_index++;
			i += 1;
			continue;
		}
		else if (lookahead(&input[i], "if"))
		{
			list[lex_index] = *(constructLex("ifsym", 8, ifsym));
			lex_index++;
			i += 1;
			continue;
		}
		else if (lookahead(&input[i], "then"))
		{
			list[lex_index] = *(constructLex("thensym", 9, thensym));
			lex_index++;
			i += 3;
			continue;
		}
		else if (lookahead(&input[i], "else"))
		{
			list[lex_index] = *(constructLex("elsesym", 10, elsesym));
			lex_index++;
			i += 3;
			continue;
		}
		else if (lookahead(&input[i], "call"))
		{
			list[lex_index] = *(constructLex("callsym", 11, callsym));
			lex_index++;
			i += 3;
			continue;
		}
		else if (lookahead(&input[i], "write"))
		{
			list[lex_index] = *(constructLex("writesym", 12, writesym));
			lex_index++;
			i += 4;
			continue;
		}
		else if (lookahead(&input[i], "read"))
		{
			list[lex_index] = *(constructLex("readsym", 13, readsym));
			lex_index++;
			i += 3;
			continue;
		}
		// check for ":=", throw error if ":x"
		else if (input[i] == ':')
		{
			if (input[i+1] == '=')
			{
				list[lex_index] = *(constructLex("assignsym", 16, assignsym));
				lex_index++;
				i++;
				continue;
			}
			else
			{
				printlexerror(1);
				return NULL;
			}
		}
		else if (token == '+')
		{
			list[lex_index] = *(constructLex("addsym", 17, addsym));
			lex_index++;
		}
		else if (token == '-')
		{
			list[lex_index] = *(constructLex("subsym", 18, subsym));
			lex_index++;
		}
		else if (token == '*')
		{
			list[lex_index] = *(constructLex("multsym", 19, multsym));
			lex_index++;
		}
		else if (token == '/' && input[i+1] != '/')
		{
			list[lex_index] = *(constructLex("divsym", 20, divsym));
			lex_index++;
			continue;
		}
		else if (token == '%')
		{
			list[lex_index] = *(constructLex("modsym", 21, modsym));
			lex_index++;
		}
		else if (lookahead(&input[i], "=="))
		{
			list[lex_index] = *(constructLex("eqlsym", 22, eqlsym));
			lex_index++;
			i++;
		}
		else if (lookahead(&input[i], "!="))
		{
			list[lex_index] = *(constructLex("", 23, eqlsym));
			lex_index++;
			i++;
		}
		else if (token == '<')
		{
			if (input[i+1] == '=')
			{
				list[lex_index] = *(constructLex("leqsym", 25, leqsym));
				lex_index++;
				i++;
			}
			else
			{
				list[lex_index] = *(constructLex("lsssym", 24, lsssym));
				lex_index++;
			}
			
		}

		else if (token == '>')
		{
			if (input[i+1] == '=')
			{
				list[lex_index] = *(constructLex("geqsym", 27, geqsym));
				lex_index++;
				i++;
			}
			else
			{
				list[lex_index] = *(constructLex("gtrsym", 26, gtrsym));
				lex_index++;	
			}

		}

		else if (lookahead(&input[i], "odd"))
		{
			list[lex_index] = *(constructLex("oddsym", 28, oddsym));
			lex_index++;
			i += 2;
		}
		else if (token == '(')
		{
			list[lex_index] = *(constructLex("lparensym", 29, lparensym));
			lex_index++;
		}
		else if (token == ')')
		{
			list[lex_index] = *(constructLex("rparensym", 30, rparensym));
			lex_index++;
		}
		else if (token == ',')
		{
			list[lex_index] = *(constructLex("commasym", 31, commasym));
			lex_index++;
		}
		else if (token == '.')
		{
			list[lex_index] = *(constructLex("periodsym", 32, periodsym));
			lex_index++;
		}
		else if (token == ';')
		{
			list[lex_index] = *(constructLex("semicolonsym", 33, semicolonsym));
			lex_index++;
		}	
		else if(isalpha(token) != 0)
		{
			int identLength = 0;
			char *ident = calloc(12, sizeof(char));
			ident[identLength] = token;
			identLength++;
			
			for(int j = i + 1; j < length;j++)
			{
				
				if(isalpha(input[j]) != 0 || isdigit(input[j]) != 0)	
				{
					if(identLength >= MAX_IDENT_LEN)	
					{
						printlexerror(4);
						return NULL;
					}
					// if(input[j] >=  'A')
					// {
					// 	ident[identLength] = input[j];
					// }
					// else
					// {
					// }

					ident[identLength] = input[j];
					identLength++;
				}
				else
				{
					// i = j;
					break;
				}//end if else
				i = j;

			}//end for loop
			list[lex_index] = *(constructLex(ident, 0, identsym));
			lex_index++;
			// i += identLength;
		}//end identifiers
		else if(isdigit(token) != 0)
		{
			int numLength = 1;
			int num = token - '0';
			
			for(int j = i + 1; j < length;j++ )
			{
				if(isdigit(input[j]) != 0)
				{
					num *= 10;
					num += (input[j] - '0');
					numLength++;
				}
				else if(isalpha(input[j]) != 0)
				{
					printlexerror(2);	
					return NULL;
				}
				else
				{
					break;
				}//end if else
				
				if(numLength >= MAX_NUMBER_LEN)
				{
					printlexerror(3);
					return NULL;
				}
				i++;
			}
			//end for loop
			
			list[lex_index] = *(constructLex("number", num, numbersym));
			lex_index++;
		}
		//end numbers
		//comments
		//
		else if(token == '/' && (i < length - 1) && input[i + 1] == '/') 
		{
			int j = i +1;
			while(input[j] != '\n')
				j++;
			i = j;	
		}//end comments
		else
		{
			printlexerror(1);
			return NULL;
		}
	}//end for loop
	printtokens();
	return list;
}

// Takes input string and a target string, determines if the next sequence in
// the input string matches the target. Returns 1 for match, 0 for failure.
int lookahead(char *input, char *target)
{
	int targetLength = strlen(target);
	int inputLength = strlen(input);
	if (inputLength < targetLength)
		return 0;

	for(int i = 0; i < targetLength; i++)
	{
		if(input[i] != target[i])
			return 0;
	}
	if(isalpha(input[targetLength]) != 0 || isdigit(input[targetLength] != 0))
		return 0;

	return 1;
}

// lexeme contructor
lexeme* constructLex(char *name, int value, token_type type)
{
	lexeme* output = malloc(sizeof(struct lexeme));
	strcpy(output->name, name);
	output->value = value;
	output->type = type;
	return output;
}

void printtokens()
{
	int i;
	printf("Lexeme Table:\n");
	printf("lexeme\t\ttoken type\n");
	for (i = 0; i < lex_index; i++)
	{
		switch (list[i].type)
		{
			case oddsym:
				printf("%11s\t%d", "odd", oddsym);
				break;
			case eqlsym:
				printf("%11s\t%d", "==", eqlsym);
				break;
			case neqsym:
				printf("%11s\t%d", "!=", neqsym);
				break;
			case lsssym:
				printf("%11s\t%d", "<", lsssym);
				break;
			case leqsym:
				printf("%11s\t%d", "<=", leqsym);
				break;
			case gtrsym:
				printf("%11s\t%d", ">", gtrsym);
				break;
			case geqsym:
				printf("%11s\t%d", ">=", geqsym);
				break;
			case modsym:
				printf("%11s\t%d", "%", modsym);
				break;
			case multsym:
				printf("%11s\t%d", "*", multsym);
				break;
			case divsym:
				printf("%11s\t%d", "/", divsym);
				break;
			case addsym:
				printf("%11s\t%d", "+", addsym);
				break;
			case subsym:
				printf("%11s\t%d", "-", subsym);
				break;
			case lparensym:
				printf("%11s\t%d", "(", lparensym);
				break;
			case rparensym:
				printf("%11s\t%d", ")", rparensym);
				break;
			case commasym:
				printf("%11s\t%d", ",", commasym);
				break;
			case periodsym:
				printf("%11s\t%d", ".", periodsym);
				break;
			case semicolonsym:
				printf("%11s\t%d", ";", semicolonsym);
				break;
			case assignsym:
				printf("%11s\t%d", ":=", assignsym);
				break;
			case beginsym:
				printf("%11s\t%d", "begin", beginsym);
				break;
			case endsym:
				printf("%11s\t%d", "end", endsym);
				break;
			case ifsym:
				printf("%11s\t%d", "if", ifsym);
				break;
			case thensym:
				printf("%11s\t%d", "then", thensym);
				break;
			case elsesym:
				printf("%11s\t%d", "else", elsesym);
				break;
			case whilesym:
				printf("%11s\t%d", "while", whilesym);
				break;
			case dosym:
				printf("%11s\t%d", "do", dosym);
				break;
			case callsym:
				printf("%11s\t%d", "call", callsym);
				break;
			case writesym:
				printf("%11s\t%d", "write", writesym);
				break;
			case readsym:
				printf("%11s\t%d", "read", readsym);
				break;
			case constsym:
				printf("%11s\t%d", "const", constsym);
				break;
			case varsym:
				printf("%11s\t%d", "var", varsym);
				break;
			case procsym:
				printf("%11s\t%d", "procedure", procsym);
				break;
			case identsym:
				printf("%11s\t%d", list[i].name, identsym);
				break;
			case numbersym:
				printf("%11d\t%d", list[i].value, numbersym);
				break;
		}
		printf("\n");
	}
	printf("\n");
	printf("Token List:\n");
	for (i = 0; i < lex_index; i++)
	{
		if (list[i].type == numbersym)
			printf("%d %d ", numbersym, list[i].value);
		else if (list[i].type == identsym)
			printf("%d %s ", identsym, list[i].name);
		else
			printf("%d ", list[i].type);
	}
	printf("\n");
	list[lex_index++].type = -1;
}

void printlexerror(int type)
{
	if (type == 1)
		printf("Lexical Analyzer Error: Invalid Symbol\n");
	else if (type == 2)
		printf("Lexical Analyzer Error: Invalid Identifier\n");
	else if (type == 3)
		printf("Lexical Analyzer Error: Excessive Number Length\n");
	else if (type == 4)
		printf("Lexical Analyzer Error: Excessive Identifier Length\n");
	else
		printf("Implementation Error: Unrecognized Error Type\n");
	
	free(list);
	return;
}