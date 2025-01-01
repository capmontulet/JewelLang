#define _CRT_SECURE_NO_WARNINGS

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

/* Check for ANSI C compliancy */
#define ANSI_C 0
#if defined(__STDC__)
#undef ANSI_C
#define ANSI_C 1
#endif

/* Global objects - variables */
static ReaderPointer sourceBuffer; /* pointer to input (source) buffer */
ReaderPointer stringLiteralTable; /* This buffer is used as a repository for string literals */
jewel_intg errorNumber;     /* Run-time error number = 0 by default (ANSI) */

/* External objects */
extern jewel_intg syntaxErrorNumber /* number of syntax errors reported by the parser */;
extern jewel_intg line; /* source code line number - defined in scanner.c */

/*
 * -------------------------------------------------------------
 *  Function declarations
 * -------------------------------------------------------------
 */

/* Function declarations (prototypes) */
extern jewel_void startParser(jewel_void);
extern jewel_intg startScanner(ReaderPointer sc_buf);

static jewel_void printParserError(jewel_char* fmt, ...);
static jewel_void displayParser(ReaderPointer ptrBuffer);
static jewel_long getParserFileSize(jewel_char* fname);
static jewel_void callGarbageCollector(jewel_void);


jewel_intg mainParser(jewel_intg argc, jewel_char** argv) {

	FILE* fi;       
	jewel_intg loadsize = 0; 


	if (argc <= 1) {
		printParserError("Date: %s  Time: %s", __DATE__, __TIME__);
		printParserError("Runtime error at line %d in file %s", __LINE__, __FILE__);
		printParserError("%s%s%s", argv[0], ": ", "Missing source file name.");
		printParserError("%s%s%s", "Usage: ", "parser", "  source_file_name");
		exit(EXIT_FAILURE);
	}

	/* create a source code input buffer - multiplicative mode */
	sourceBuffer = readerCreate(READER_DEFAULT_SIZE, READER_DEFAULT_INCREMENT, MODE_MULTI);
	if (sourceBuffer == NULL) {
		printParserError("%s%s%s", argv[0], ": ", "Could not create source buffer");
		exit(EXIT_FAILURE);
	}

	/*open source file */
	if ((fi = fopen(argv[2], "r")) == NULL) {
		printParserError("%s%s%s%s", argv[0], ": ", "Cannot open file: ", argv[2]);
		exit(EXIT_FAILURE);
	}

	/* load source file into input buffer  */
	printf("Reading file %s ....Please wait\n", argv[2]);
	loadsize = readerLoad(sourceBuffer, fi);
	if (loadsize == READER_ERROR)
		printParserError("%s%s%s", argv[0], ": ", "Error in loading buffer.");

	/* close source file */
	fclose(fi);
	/*find the size of the file  */
	if (loadsize == READER_ERROR) {
		printf("The input file %s %s\n", argv[2], "is not completely loaded.");
		printf("Input file size: %ld\n", getParserFileSize(argv[2]));
	}
	/* Add SEOF (EOF) to input buffer and display the source buffer */
	if ((loadsize != READER_ERROR) && (loadsize != 0)) {
		if (readerAddChar(sourceBuffer, READER_TERMINATOR)) {
			displayParser(sourceBuffer);
		}
	}
	/* create string Literal Table */
	stringLiteralTable = readerCreate(READER_DEFAULT_SIZE, READER_DEFAULT_INCREMENT, MODE_ADDIT);
	if (stringLiteralTable == NULL) {
		printParserError("%s%s%s", argv[0], ": ", "Could not create string literal buffer");
		exit(EXIT_FAILURE);
	}

	/* Registrer exit function */
	atexit(callGarbageCollector);

	/* Initialize scanner  */
	startScanner(sourceBuffer);

	/* Start parsing */
	printf("\nParsing the source file...\n\n");

	startParser();

	return (EXIT_SUCCESS); /* same effect as exit(0) */

}


jewel_void printParserError(jewel_char* fmt, ...) {

	va_list ap;
	va_start(ap, fmt);

	(jewel_void)vfprintf(stderr, fmt, ap);
	va_end(ap);

	/* Move to new line */
	if (strchr(fmt, '\n') == NULL)
		fprintf(stderr, "\n");
}


jewel_long getParserFileSize(jewel_char* fname) {
	FILE* input;
	jewel_long flength;
	input = fopen(fname, "r");
	if (input == NULL) {
		printParserError("%s%s", "Cannot open file: ", fname);
		return 0;
	}
	fseek(input, 0L, SEEK_END);
	flength = ftell(input);
	fclose(input);
	return flength;
}


jewel_void displayParser(ReaderPointer ptrBuffer) {
	printf("\nPrinting input buffer parameters:\n\n");
	printf("The capacity of the buffer is:  %d\n", readerGetSize(ptrBuffer));
	printf("The current size of the buffer is:  %d\n", readerGetPosWrte(ptrBuffer));
	printf("\nPrinting input buffer contents:\n\n");
	readerRecover(ptrBuffer);
	readerPrint(ptrBuffer);
}


jewel_void callGarbageCollector(julius_void) {
	if (syntaxErrorNumber)
		printf("\nSyntax errors: %d\n", syntaxErrorNumber);
	printf("\nCollecting garbage...\n");
	readerFree(sourceBuffer);
	readerFree(stringLiteralTable);
}
