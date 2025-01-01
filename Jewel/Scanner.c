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

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

/* #define NDEBUG        to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern ReaderPointer stringLiteralTable;	/* String literal table */
jewel_intg line;							/* Current line number of the source code */
extern jewel_intg errorNumber;				/* Defined in platy_st.c - run-time error number */

extern jewel_intg stateType[];
extern jewel_char* keywordTable[];
extern PTR_ACCFUN finalStateTable[];
extern jewel_intg transitionTable[][TABLE_COLUMNS];

/* Local(file) global objects - variables */
static ReaderPointer lexemeBuffer;			/* Pointer to temporary lexeme buffer */
static ReaderPointer sourceBuffer;			/* Pointer to input source buffer */

/*
 ************************************************************
 * Intitializes scanner
 ***********************************************************
 */

jewel_intg startScanner(ReaderPointer psc_buf) {
	readerRecover(psc_buf);
	readerClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;

	return EXIT_SUCCESS;
}

/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states 
 *		in the Transition Diagram).
 ***********************************************************
 */

Token tokenizer(jewel_void) {

	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	jewel_char c;				/* input symbol */
	jewel_intg state = 0;		/* initial state of the FSM */
	jewel_intg lexStart;		/* start offset of a lexeme in the input char buffer (array) */
	jewel_intg lexEnd;			/* end offset of a lexeme in the input char buffer (array)*/
	jewel_intg lexLength;		/* token length */
	jewel_intg i;				/* counter */
	jewel_char newc;			/* new char */
	
	while (1) { /* endless loop broken by token returns it will generate a warning */
		c = readerGetChar(sourceBuffer);

		switch (c) {
		case ' ':
			break;
		case '\t':
		case '\f':
		case ',' :
		case ':' :
			break;
		case '\n':
			line++;
			break;

		//making single quotes work the same as double quotes
		case '\'':
			currentToken.code = STR_T;
			break;
		case ';':
			currentToken.code = EOS_T;
			return currentToken;
		case '(':
			currentToken.code = LPR_T;
			return currentToken;
		case ')':
			currentToken.code = RPR_T;
			return currentToken;
		case '{':
			currentToken.code = LBR_T;
			return currentToken;
		case '}':
			currentToken.code = RBR_T;
			return currentToken;
		case '+':
			currentToken.code = ARI_T;
			currentToken.attribute.arithmeticOperator = OP_ADD;
			return currentToken;
		case '-':
			currentToken.code = ARI_T;
			currentToken.attribute.arithmeticOperator = OP_SUB;
			return currentToken;
		case '*':
			currentToken.code = ARI_T;
			currentToken.attribute.arithmeticOperator = OP_MUL;
			return currentToken;
		case '/':
			currentToken.code = ARI_T;
			currentToken.attribute.arithmeticOperator = OP_DIV;
			return currentToken;
		case '=':
			newc = readerGetChar(sourceBuffer);
			if (newc == '=') {

				currentToken.code = LOG_T;
				currentToken.attribute.relationalOperator = OP_EQ;
				return currentToken;
			}
			else {
				readerRetract(sourceBuffer);
				currentToken.code = EQU_T;
				return currentToken;
			}
		case '!':
			newc = readerGetChar(sourceBuffer);
			if (newc == '=') { //if its != 
				currentToken.code = LOG_T;
				currentToken.attribute.relationalOperator = OP_NE;
				return currentToken;
			}
			else {   //otherwise its only !
				readerRetract(sourceBuffer);
				currentToken.code = LOG_T;
				currentToken.attribute.logicalOperator = OP_NOT;
				return currentToken;
			}
		case '<':
			currentToken.code = LOG_T;
			currentToken.attribute.relationalOperator = OP_LT;
			return currentToken;
		case '>':
			currentToken.code = LOG_T;
			currentToken.attribute.relationalOperator = OP_GT;
			return currentToken;
		/* Comments */
		case '#':
			newc = readerGetChar(sourceBuffer);
			do {
				c = readerGetChar(sourceBuffer);
				if (c == CHARSEOF0 || c == CHARSEOF255) {
					readerRetract(sourceBuffer);
					//return currentToken;
				}
				else if (c == '\n') {
					line++;
				}
			} while (c != '#' && c != CHARSEOF0 && c != CHARSEOF255);
			break;
		/* Cases for END OF FILE */
		case CHARSEOF0:
			currentToken.code = SEOF_T;
			currentToken.attribute.seofType = SEOF_0;
			return currentToken;
		case CHARSEOF255:
			currentToken.code = SEOF_T;
			currentToken.attribute.seofType = SEOF_255;
			return currentToken;


		default: 
			state = nextState(state, c);
			lexStart = readerGetPosRead(sourceBuffer) - 1;
			readerSetMark(sourceBuffer, lexStart);
			int pos = 0;
			while (stateType[state] == NOAS) {
				c = readerGetChar(sourceBuffer);
				state = nextState(state, c);
				pos++;
			}
			if (c == ' ' || c == ';' || c == '(' || c == ')') {
				readerRetract(sourceBuffer);
			}
			if (stateType[state] == ASWR)
				readerRetract(sourceBuffer);
			lexEnd = readerGetPosRead(sourceBuffer);
			lexLength = lexEnd - lexStart;
			
			lexemeBuffer = readerCreate((jewel_intg)lexLength + 2, 0, MODE_FIXED);
			if (!lexemeBuffer) {
				fprintf(stderr, "Scanner error: Can not create buffer\n");
				exit(1);
			}
			readerRestore(sourceBuffer);
			for (i = 0; i < lexLength; i++)
				readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
			readerAddChar(lexemeBuffer, READER_TERMINATOR);

			currentToken = (*finalStateTable[state])(readerGetContent(lexemeBuffer, 0));
			readerRestore(lexemeBuffer);
			return currentToken;
		}

	}

}


jewel_intg nextState(jewel_intg state, jewel_char c) {
	jewel_intg col;
	jewel_intg next;
	col = nextClass(c);
	next = transitionTable[state][col];
	if (DEBUG)
		printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
	assert(next != FS);
	if (DEBUG)
		if (next == FS) {
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
			exit(1);
		}
	return next;
}


jewel_intg nextClass(jewel_char c) {
	jewel_intg val = -1;
	switch (c) {
	case CHRCOL2:
		val = 2;
		break;
	case CHRCOL3:
		val = 3;
		break;
	case CHRCOL5:
		val = 5;
		break;
	case CHRCOL6:
		val = 6;
		break;
	case CHRCOL7:
		val = 7;
		break;
	case CHARSEOF0:
	case CHARSEOF255:
		val = 8;
		break;
	default:
		if (isalpha(c))
			val = 0;
		else if (isdigit(c))
			val = 1;
		else
			val = 4;
	}
	return val;
}


 /*
  ************************************************************
  * Acceptance State Function IL
  *		Function responsible to identify IL (integer literals).
  * - It is necessary respect the limit (ex: 2-byte integer in C).
  * - In the case of larger lexemes, error shoul be returned.
  * - Only first ERR_LEN characters are accepted and eventually,
  *   additional three dots (...) should be put in the output.
  ***********************************************************
  */

Token funcIL(jewel_char lexeme[]) {
	Token currentToken = { 0 };
	jewel_long tlong;
	if (lexeme[0] != '\0' && strlen(lexeme) > NUM_LEN) {
		currentToken = funcErr(lexeme);
	}
	else {
		tlong = atol(lexeme);
		if (tlong >= 0 && tlong <= SHRT_MAX) {
			currentToken.code = INT_T;
			currentToken.attribute.intValue = (jewel_intg)tlong;
		}
		else {
			currentToken = funcErr(lexeme);
		}
	}
	return currentToken;
}


/*
 ************************************************************
 * Acceptance State Function ID
 *		In this function, the pattern for IDs must be recognized.
 *		Since keywords obey the same pattern, is required to test if
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *  - Suggestion: Use "strncpy" function.
 ***********************************************************
 */

Token funcID(jewel_char lexeme[]) {
	Token currentToken = { 0 };
	size_t length = strlen(lexeme);
	jewel_char lastch = lexeme[length - 1];
	jewel_intg isID = jewel_FALSE;
	switch (lastch) {
		case MNIDPREFIX:
			currentToken.code = MNID_T;
			isID = jewel_TRUE;
			break;
		default:
			// Test Keyword
			currentToken = funcKEY(lexeme);
			break;
	}
	if (isID == jewel_TRUE) {
		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
		currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF0;
	}
	return currentToken;
}


/*
************************************************************
 * Acceptance State Function SL
 *		Function responsible to identify SL (string literals).
 * - The lexeme must be stored in the String Literal Table 
 *   (stringLiteralTable). You need to include the literals in 
 *   this structure, using offsets. Remember to include \0 to
 *   separate the lexemes. Remember also to incremente the line.
 ***********************************************************
 */

Token funcSL(jewel_char lexeme[]) {
	Token currentToken = { 0 };
	jewel_intg i = 0, len = (jewel_intg)strlen(lexeme);
	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == '\n')
			line++;
		if (!readerAddChar(stringLiteralTable, lexeme[i])) {
			currentToken.code = RTE_T;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
	}
	if (!readerAddChar(stringLiteralTable, CHARSEOF0)) {
		currentToken.code = RTE_T;
		strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
		errorNumber = RTE_CODE;
		return currentToken;
	}
	currentToken.code = STR_T;
	return currentToken;
}


/*
************************************************************
 * This function checks if one specific lexeme is a keyword.
 * - Tip: Remember to use the keywordTable to check the keywords.
 ***********************************************************
 */

Token funcKEY(jewel_char lexeme[]) {
	Token currentToken = { 0 };
	jewel_intg kwindex = -1, j = 0;
	for (j = 0; j < KWT_SIZE; j++)
		if (!strcmp(lexeme, &keywordTable[j][0]))
			kwindex = j;
	if (kwindex != -1) {
		currentToken.code = KW_T;
		currentToken.attribute.codeType = kwindex;
	}
	else {
		currentToken = funcVar(lexeme);
	}
	return currentToken;
}


/*
************************************************************
 * Acceptance State Function Error
 *		Function responsible to deal with ERR token.
 * - This function uses the errLexeme, respecting the limit given
 *   by ERR_LEN. If necessary, use three dots (...) to use the
 *   limit defined. The error lexeme contains line terminators,
 *   so remember to increment line.
 ***********************************************************
 */

Token funcErr(jewel_char lexeme[]) {
	Token currentToken = { 0 };
	jewel_intg i = 0, len = (jewel_intg)strlen(lexeme);
	if (len > ERR_LEN) {
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = CHARSEOF0;
		strcat(currentToken.attribute.errLexeme, "...");
	}
	else {
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}
	for (i = 0; i < len; i++)
		if (lexeme[i] == '\n')
			line++;
	currentToken.code = ERR_T;
	return currentToken;
}

/*
 ************************************************************
 * The function prints the token returned by the scanner
 ***********************************************************
 */

jewel_void printToken(Token t) {

	/* External variables */
	extern jewel_char* keywordTable[];  /* link to keyword table in */
	extern numScannerErrors;			/* link to number of errors (defined in Scanner.h) */

	switch (t.code) {
	case RTE_T:
		//numScannerErrors++;
		printf("RTE_T\t\t%s", t.attribute.errLexeme);
		/* Call here run-time error handling component */
		if (errorNumber) {
			printf("%d", errorNumber);
			exit(errorNumber);
		}
		printf("\n");
		break;
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
		printf("Scanner error: invalid token code: %d\n", t.code);
	}
}

Token funcDBL(jewel_char lexeme[]) {

	Token currentToken = { 0 };
	jewel_doub floatNumber;
	if (lexeme[strlen(lexeme) - 1] == '\0') {
		currentToken = funcErr(lexeme);
	}
	else {
		floatNumber = atof(lexeme);
		if (floatNumber >= 0.0 && floatNumber <= FLT_MAX) {
			currentToken.code = DBL_T;
			currentToken.attribute.doubleValue = (jewel_real)floatNumber;
		}
		else {
			currentToken = funcErr(lexeme);
		}
	}
	return currentToken;
}


Token funcVar(jewel_char lexeme[])
{
	Token currentToken = { 0 };
	currentToken.code = VAR_T;
	strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
	currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF0;
	return currentToken;
}

