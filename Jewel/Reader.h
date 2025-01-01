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

#ifndef READER_H_
#define READER_H_

#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

enum READER_MODE {
	MODE_FIXED = 'F', /* Fixed mode (constant size) */
	MODE_ADDIT = 'A', /* Additive mode (constant increment to be added) */
	MODE_MULTI = 'M'  /* Multiplicative mode (constant increment to be multiplied) */
};

#define READER_ERROR (-1)						/* General error message */
#define READER_TERMINATOR '\0'							/* General EOF */


#define READER_MAX_SIZE	INT_MAX-1	/* maximum capacity */ 

#define READER_DEFAULT_SIZE			250		/* default initial buffer reader capacity */
#define READER_DEFAULT_INCREMENT	10		/* default increment factor */


/* BITS                                (7654.3210) */
#define READER_DEFAULT_FLAG 0x00 	/* (0000.0000)_2 = (000)_10 */
/* BIT 7: FUL = Full */
#define SETFULBIT 0x80
#define RSTFULBIT 0x7F
#define CHKFULBIT SETFULBIT
/* BIT 6: EMP: Empty */
#define SETEMPBIT 0x40
#define RSTEMPBIT 0xBF
#define CHKEMPBIT SETEMPBIT
/* BIT 5: REL = Relocation */
#define SETRELBIT 0x20
#define RSTRELBIT 0xDF
#define CHKRELBIT SETRELBIT
/* BIT 4: END = EndOfBuffer */
#define SETENDBIT 0x10
#define RSTENDBIT 0xEF
#define CHKENDBIT SETENDBIT


#define NCHAR		128			/* Chars from 0 to 127 */

/* Offset declaration */
typedef struct position {
	jewel_intg mark;			/* the offset to the mark position (in chars) */
	jewel_intg read;			/* the offset to the get a char position (in chars) */
	jewel_intg wrte;			/* the offset to the add chars (in chars) */
} Position;

/* Buffer structure */
typedef struct bufferReader {
	jewel_char*		content;			/* pointer to the beginning of character array (character buffer) */
	jewel_intg		size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	jewel_intg		increment;			/* character array increment factor */
	jewel_intg		mode;				/* operational mode indicator */
	jewel_byte		flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position		position;			/* Offset / position field */
	jewel_intg		histogram[NCHAR];	/* Statistics of chars */
	jewel_intg		numReaderErrors;	/* Number of errors from Reader */
} BufferReader, * ReaderPointer;


/* General Operations */
ReaderPointer	readerCreate(jewel_intg, jewel_intg, jewel_intg);//Done 
ReaderPointer	readerAddChar(ReaderPointer const, jewel_char);//done
jewel_boln		readerClear(ReaderPointer const);//done, no defensive
jewel_boln		readerFree(ReaderPointer const);//done
jewel_boln		readerIsFull(ReaderPointer const);//done
jewel_boln		readerIsEmpty(ReaderPointer const);//done
jewel_boln		readerSetMark(ReaderPointer const, jewel_intg);//done
jewel_intg		readerPrint(ReaderPointer const);//done, no defensive
jewel_intg		readerLoad(ReaderPointer const, FILE* const);//done
jewel_boln		readerRecover(ReaderPointer const);//done
jewel_boln		readerRetract(ReaderPointer const);//done
jewel_boln		readerRestore(ReaderPointer const);//done
/* Getters */
jewel_char		readerGetChar(ReaderPointer const);//done
jewel_char*		readerGetContent(ReaderPointer const, jewel_intg);//done, no defensive
jewel_intg		readerGetPosRead(ReaderPointer const);//done
jewel_intg		readerGetPosWrte(ReaderPointer const);//done
jewel_intg		readerGetPosMark(ReaderPointer const);//done
jewel_intg		readerGetSize(ReaderPointer const);//done
jewel_intg		readerGetInc(ReaderPointer const);//done
jewel_intg		readerGetMode(ReaderPointer const);//done
jewel_byte		readerGetFlags(ReaderPointer const);//done
jewel_intg		readerShowStat(ReaderPointer const);//done
jewel_intg		readerNumErrors(ReaderPointer const);

#endif
