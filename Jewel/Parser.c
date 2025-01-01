/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2023
* Author: Thomas Stanley, Noah Horner
* Professors: Paulo Sousa
************************************************************
				  , gaaaaaaaagaaaaaaaaaaaaagaaaaaaaag,
				  , aP8b    _, dYba,    , adPb, _    d8Ya,
				, aP   Yb_,dP     Yba, ,adP     Yb,_dP   Ya,
			  , aP     _88         )888(          88_      Ya,
		   , aP    _,dP Yb,     adP  8  Yba,     dP Yb,_     Ya,
		 , aPYb _, dP8    Yb, adP    8    Yba, dP    8Yb, _ dPYa,
	   , aP   YdP  dP     YbdP       8       YbdP     Yb  YbP   Ya,
	   I8aaaaaa8aaa8baaaaaa88aaaaaaaa8aaaaaaaa88aaaaaad8aaa8aaaaaa8I
	   `Yb,   d8a, Ya      d8b,      8, d8b    aP,    a8b,      dP'
		  Yb,dP  Ya  8,   dI  Yb,    8    ,dP  Ib   ,8  aP  Yb,dP
			Y8,    YaI8, ,8'    Yb,  8  ,dP    `8, ,8IaP    ,8P
			  Yb,   ` Y8ad'       Yb,8,dP       `ba8P '   ,dP
				Yb,    ` 8,         Y8P         ,8 '    ,dP
				  Yb,    `8,         8         ,8'    ,dP
					Yb,   `Ya     -JEWEL-     aP'   ,dP
					  Yb,    8,      8      ,8    ,dP
						Yb,  `8,     8     ,8'  ,dP
						  Yb, `Ya    8    aP' ,dP
							Yb,  8,  8  ,8 , dP
							  Yb,`8, 8 ,8',dP
								Yb,Ya8aP,dP
								  Y88888P
									Y8P
							 ALGONQUIN COLLEGE

*/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef PARSER_H_
#include "Parser.h"
#endif

jewel_void startParser() {
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


jewel_void matchToken(jewel_intg tokenCode, jewel_intg tokenAttribute) {
	jewel_intg matchFlag = 1;
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType != tokenAttribute) {
			matchFlag = 0;
		}
		if (tokenCode == DBL_T) {
			lookahead = tokenizer();
			matchFlag = 1;
		}
		while (lookahead.code == VAR_T && lookahead.attribute.codeType == tokenAttribute) {
			statement();
			lookahead = tokenizer();
		}
		break;
	case EOS_T:
		if (tokenCode != EOS_T) {
			matchFlag = 0;
		}
		break;
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode);
}


jewel_void syncErrorHandler(jewel_intg syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}


jewel_void printError() {
	Token t = lookahead;
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("MNID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case LBR_T:
		printf("LBR_T\t\t%s\n", "{");
		break;
	case VAR_T:
		printf("VAR_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("STR_T\t\t%d\t ", (jewel_intg)t.attribute.codeType);
		printf("%s\n", readerGetContent(stringLiteralTable, (jewel_intg)t.attribute.codeType));
		break;
	case LPR_T:
		printf("LPR_T\t\t%s\n", "(");
		break;
	case DBL_T:
		printf("DBL_T\t\t%.4f\n", t.attribute.doubleValue);
		break;
	case RPR_T:
		printf("RPR_T\t\t%s\n", ")");
		break;
	case INL_T:
		printf("INL_T\t\t%d\n", t.attribute.intValue);
		break;
	case ARI_T:
		printf("ARI_T\t\t%d\n", t.attribute.arithmeticOperator);
		break;
	case RBR_T:
		printf("RBR_T\t\t%s\n", "}");
		break;
	case KW_T:
		printf("KW_T\t\t%s\n", keywordTable[t.attribute.codeType]);
		break;
	case EOS_T:
		printf("EOS_T\t\t%s\n", ";");
		break;
	case EQU_T:
		printf("EQU_T\t\t%s\n", "=");
		break;
	case LOG_T:
		printf("LOG_T\n");
		break;
	case INT_T:
		printf("INT_T\t\t%d\n", t.attribute.intValue);
		break;
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
	}
}

jewel_void program() {
	switch (lookahead.code) {
	case MNID_T:
		if (strncmp(lookahead.attribute.idLexeme, LANG_MAIN, 5) == 0) {
			matchToken(MNID_T, NO_ATTR);
			matchToken(LBR_T, NO_ATTR);
			dataSession();
			codeSession();
			matchToken(RBR_T, NO_ATTR);
			break;
		}
		else {
			printError();
		}
	case SEOF_T:
		; // Empty
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}


jewel_void dataSession() {
	matchToken(KW_T, KW_data);
	matchToken(LBR_T, NO_ATTR);
	optVarListDeclarations();
	matchToken(EOS_T, NO_ATTR);
	matchToken(RBR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Data Session parsed");
}


jewel_void optVarListDeclarations() {
	while (lookahead.code == KW_double || lookahead.code == KW_int || lookahead.code == KW_float) {
		switch (lookahead.code) {
		case KW_double:
			matchToken(DBL_T, KW_double);
			// handle double variable declaration
			printf("Double Parsed\n");
			break;
		case KW_int:
			matchToken(KW_int, NO_ATTR);
			// handle int variable declaration
			printf("Int Parsed\n");
			break;
		case KW_float:
			matchToken(KW_string, NO_ATTR);
			// handle float variable declaration
			printf("String Parsed\n");
			break;
		default:
			// handle syntax error or unexpected token
			printf("Unexpected token\n");
			syncErrorHandler(VAR_T); // or whatever error handler is appropriate
		}
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional Variable List Declarations parsed");
}


jewel_void codeSession() {
	matchToken(KW_T, KW_code);
	matchToken(LBR_T, NO_ATTR);
	optionalStatements();
	matchToken(RBR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Code Session parsed");
}

jewel_void optionalStatements() {
	switch (lookahead.code) {
	case MNID_T:
		if ((strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) ||
			(strncmp(lookahead.attribute.idLexeme, LANG_READ, 6) == 0)) {
				statements();
				break;
		}
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}


jewel_void statements() {
	statement();
	statementsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}


jewel_void statementsPrime() {
	switch (lookahead.code) {
	case MNID_T:
		if (strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) {
			statements();
			break;
		}
	default:
		; //empty string
	}
}


jewel_void statement() {
	switch (lookahead.code) {
	case KW_T:
		switch (lookahead.attribute.codeType) {
		default:
			printError();
		}
		break;
	case MNID_T:
		if (strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) {
			outputStatement();
		}
		break;
	case VAR_T:
		matchToken(VAR_T, NO_ATTR);
		matchToken(EQU_T, NO_ATTR);
		if (lookahead.code == STR_T || lookahead.code == INT_T || lookahead.code == DBL_T) {
			outputVariableList();
			if (lookahead.code == ARI_T) {
				
			}
		}
		else if (lookahead.code == VAR_T) {
			matchToken(VAR_T, NO_ATTR);
			if (lookahead.code == ARI_T) {
				
			}
		}
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

jewel_void outputStatement() {
	matchToken(MNID_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	outputVariableList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}


jewel_void outputVariableList() {
	switch (lookahead.code) {
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	case DBL_T:
		matchToken(DBL_T, NO_ATTR);
		break;
	default:
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": Output variable list parsed");
}


jewel_void varlist_declarations()
{
	
}

jewel_void varlist_declarationsPrime()
{
	
}
