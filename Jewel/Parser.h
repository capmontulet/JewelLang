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

#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef READER_H_
#include "Reader.h"
#endif
#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global vars */
static Token lookahead;
extern BufferReader* stringLiteralTable;
extern jewel_intg line;
extern Token tokenizer();
extern jewel_char* keywordTable[];
jewel_intg syntaxErrorNumber = 0;

#define STR_LANGNAME	"Jewel"
#define LANG_WRTE		"print$"
#define LANG_READ		"input$"
#define LANG_MAIN		"main$"

enum KEYWORDS {
    KW_data,      // 0 
    KW_code,      // 1
    KW_if,        // 2
    KW_else,      // 3
    KW_elif,      // 4
    KW_for,       // 5
    KW_foreach,   // 6
    KW_double,    // 7
    KW_int,       // 8
    KW_string,    // 9
    KW_var,       // 10
    KW_float,     // 11
    KW_boolean,   // 12
    KW_char,      // 13
    KW_true,      // 14
    KW_false,     // 15
    KW_return,    // 16
    KW_print,     // 17
    KW_or,        // 18
    NO_ATTR = -1  // -1
};


jewel_void startParser();
jewel_void matchToken(jewel_intg, jewel_intg);
jewel_void syncErrorHandler(jewel_intg);
jewel_void printError();


jewel_void codeSession();
jewel_void dataSession();
jewel_void optVarListDeclarations();
jewel_void optionalStatements();
jewel_void outputStatement();
jewel_void outputVariableList();
jewel_void program();
jewel_void statement();
jewel_void statements();
jewel_void statementsPrime();
jewel_void varlist_declarations();
jewel_void varlist_declarationsPrime();

#endif
