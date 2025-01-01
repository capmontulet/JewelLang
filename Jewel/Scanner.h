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

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> 
#endif

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 10   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

enum TOKENS {
	ERR_T,		/*  0: Error token */
	MNID_T,		/*  1: Method name identifier token (start: $) */
	STR_T,		/*  2: String literal token */
	LPR_T,		/*  3: Left parenthesis token */
	RPR_T,		/*  4: Right parenthesis token */
	LBR_T,		/*  5: Left brace token */
	RBR_T,		/*  6: Right brace token */
	KW_T,		/*  7: Keyword token */
	INT_T,		/*  8: Integer token */
	RTE_T,		/*  9: Run-time error token */
	INL_T,		/* 10: Run-time error token */
	SEOF_T, 	/* 11: Source end-of-file token */

	//doing from here - tom

	EQU_T,	    /* 12 Equals operator */
	EOS_T,		/* 13 End of Statement */
	VAR_T,		/* 14 Variable literal token */
	FS_T,		/* 15 Full sTop token*/
	ARI_T,		/* 16 Arithmetic token*/
	OPR_T,		/* 17 Operator token*/
	LOG_T,		/* 18 Logic operator token*/
	DBL_T		/* 19 Double value token*/
};


typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;


typedef union TokenAttribute {
	jewel_intg codeType;				/* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	jewel_intg intValue;				/* integer literal attribute (value) */
	jewel_intg keywordIndex;			/* keyword index in the keyword table */
	jewel_intg contentString;			/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	jewel_real doubleValue;				/* floating-point literal attribute (value) */
	jewel_char idLexeme[VID_LEN + 1];	/* variable identifier token attribute */
	jewel_char errLexeme[ERR_LEN + 1];	/* error token attribite */
} TokenAttribute;


typedef struct idAttibutes {
	jewel_byte flags;					/* Flags information */
	union {
		jewel_intg intValue;			/* Integer value */
		jewel_real floatValue;			/* Float value */
		jewel_char* stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	jewel_intg code;					/* token code */
	TokenAttribute attribute;			/* token attribute */
	IdAttibutes   idAttribute;			/* not used in this scanner implementation - for further use */
} Token;


/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

/*  Special case tokens processed separately one by one in the token-driven part of the scanner
 *  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' , ';', 
 *  white space, #comment\n , ',' , ';' , '-' , '+' , '*' , '/', # , 
 *  .&., .|. , .!. , SEOF.
 */


#define CHRCOL2 '$'
#define CHRCOL3 '.'
#define CHRCOL4 '('
#define CHRCOL5 '\n'
#define CHRCOL6 '"'
#define CHRCOL7 '#'

#define MNIDPREFIX '$'

#define FS		100		/* Illegal state */
#define ESNR	101		/* Error state with no retract */
#define ESWR	102		/* Error state with retract */

#define TABLE_COLUMNS 9

static jewel_intg transitionTable[][TABLE_COLUMNS] = {
/*   [A-z] , [0-9],    $,  .,      (,    \n,   ",    #    other
	L(0),  D(1), U(2),  P(3)  M(4), N(5), Q(6), C(7),  O(8) */
{     1,     8, ESNR, ESNR, ESNR, ESNR,     4,     6,   ESNR}, // S0:  NOAS
{     1,     1,    1,    3,    2,    3,     3,     3,      3}, // S1:  NOAS
{    FS,    FS,   FS,   FS,   FS,   FS,    FS,    FS,     FS}, // S2:  ASNR (MVID)
{    FS,    FS,   FS,   FS,   FS,   FS,    FS,    FS,     FS}, // S3:  ASWR (VARIABLE)
{     4,     4,    4,    4,    4,    4,     5,     4,	   4}, // S4:  NOAS 
{    FS,    FS,   FS,   FS,   FS,   FS,    FS,	  FS,  	  FS}, // S5:  ASNR (STRING)
{     6,     6,    6,    6,    6,    7,     6,	   6,	   6}, // S6:  NOAS
{    FS,    FS,   FS,   FS,   FS,   FS,    FS,	  FS,	  FS}, // S7:  ASNR (COMMENTS)
{     9,     8,    9,   10,    9,	 9,     9,	   9,	  9,}, // S8:  NOAS 
{    FS,    FS,   FS,   FS,   FS,   FS,    FS,	  FS,	  FS}, // S9:  ASWR (INTEGER)
{    11,    10,   11,   11,   11,   11,    11,	  11,	  11}, // S10: NOAS 
{    FS,    FS,   FS,   FS,   FS,   FS,    FS,	  FS,	  FS}, // S11: ASWR (DOUBLE)
};


#define NOAS	0		/* not accepting state */
#define ASNR	1		/* accepting state with no retract */
#define ASWR	2		/* accepting state with retract */


static jewel_intg stateType[] = {
	NOAS, /* 00 */
	NOAS, /* 01 */
	ASNR, /* 02 - Method Identifier */
	ASWR, /* 03 - Variable */
	NOAS, /* 04 */
	ASNR, /* 05 - String */
	NOAS, /* 06 */
	ASNR, /* 07 - Comment */
	NOAS, /* 08 */
	ASNR, /* 09 - Integer */
	NOAS, /* 10 */
	ASNR, /* 11 - Double */
	ASNR, /* 12 - Error 1 (no retraction) */
	ASWR  /* 13 - Error 2 (retract) */
};

/* Static (local) function  prototypes */
jewel_intg startScanner(ReaderPointer psc_buf);
Token tokenizer(jewel_void);
static jewel_intg nextClass(jewel_char c);					/* character class function */
static jewel_intg nextState(jewel_intg, jewel_char);		/* state machine function */
jewel_void printToken(Token t);

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

typedef Token(*PTR_ACCFUN)(jewel_char* lexeme);

/* Declare accepting states functions */
Token funcSL	(jewel_char lexeme[]);
Token funcID	(jewel_char lexeme[]);
Token funcKEY	(jewel_char lexeme[]);
Token funcErr	(jewel_char lexeme[]);
Token funcIL	(jewel_char lexeme[]);
Token funcDBL	(jewel_char lexeme[]);
Token funcVar	(jewel_char lexeme[]);


static PTR_ACCFUN finalStateTable[] = {
	NULL,       /* -           [00] */
	NULL,       /* -           [01] */
	funcID,     /* Method ID   [02] */
	funcID,     /* Variable    [03] */
	NULL,       /* -           [04] */
	funcSL,     /* String      [05] */
	NULL,       /* -           [06] */
	NULL,       /* Comment     [07] */
	NULL,       /* -           [08] */
	funcIL,     /* Integer     [09] */
	NULL,       /* -           [10] */
	funcDBL,    /* Double      [11] */
	funcErr,    /* Error 1     [12] */
	funcErr     /* Error 2     [13] - without retraction */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

#define KWT_SIZE 19

static jewel_char* keywordTable[KWT_SIZE] = {
	"data",
	"code",
	"if",
	"else",
	"else if",
	"for",
	"foreach",
	"var",
	"int",
	"string",
	"double",
	"float",
	"boolean",
	"char",
	"true",
	"false",
	"return",
	"print",
	"or"
};

#define INDENT '\t'  /* Tabulation */


typedef struct languageAttributes {
	jewel_char indentationCharType;
	jewel_intg indentationCurrentPos;
} LanguageAttributes;

jewel_intg numScannerErrors;

#endif
