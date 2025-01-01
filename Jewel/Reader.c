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
#include "Reader.h"
#endif


ReaderPointer readerCreate(jewel_intg size, jewel_intg increment, jewel_intg mode) {
	ReaderPointer readerPointer;

	if (size == NULL) {
		size = READER_DEFAULT_SIZE;
		increment = READER_DEFAULT_INCREMENT;
	}
	if (increment == NULL) {
		mode = MODE_FIXED;
	}
	if (mode != 'F' && mode != 'A' && mode != 'M') {
		return NULL;
	}

	readerPointer = (ReaderPointer)calloc(1, sizeof(BufferReader));

	if (!readerPointer)
		return NULL;
	readerPointer->content = (jewel_char*)malloc(size);
	if (!readerPointer->content) {
		free(readerPointer);
		return NULL;
	}

	readerPointer->size = size;
	readerPointer->increment = increment;
	readerPointer->mode = mode;

	for (int i = 0; i <= 127; i++) {
		readerPointer->histogram[i] = 0;
	}

	readerPointer->flags = READER_DEFAULT_FLAG | SETEMPBIT;

	return readerPointer;
}

ReaderPointer readerAddChar(ReaderPointer const readerPointer, jewel_char ch) {
	jewel_char* tempReader = NULL;
	jewel_intg newSize = 0;
	jewel_intg originalAddress = &(readerPointer->content);
	if (ch < 0 || ch > 127) {
		readerPointer->numReaderErrors++;
		return NULL;
	}

	if (readerPointer->position.wrte * (jewel_intg)sizeof(jewel_char) < readerPointer->size) {

	}else{
		switch (readerPointer->mode) {
		case MODE_FIXED:
			return READER_ERROR;
		case MODE_ADDIT:
			newSize = readerPointer->size + readerPointer->increment;
			//it is required to check if the result is valid (not negative and lower than READER_MAX_SIZE
			if (newSize < 0 || newSize > READER_MAX_SIZE) {
				readerPointer->flags = SETFULBIT;
			}
			if (newSize > readerPointer->size) {
					tempReader = (char*)realloc(readerPointer->content, newSize);
			}
			break;
		case MODE_MULTI:
			newSize = readerPointer->size * readerPointer->increment;
			if (newSize < 0 || newSize > READER_MAX_SIZE) {
				readerPointer->flags = SETFULBIT;
			}
			if (newSize > readerPointer->size) {
				tempReader = (char*)realloc(readerPointer->content, newSize);
			}
			break;
		default:
			return NULL;
		}
		if (!tempReader) {
			return NULL;
		}else {
			readerPointer->content = tempReader;
			if (readerPointer)	readerPointer->size = newSize;
		}
	}
	if ((readerPointer) && (&(readerPointer->content) != originalAddress)) readerPointer->flags = SETRELBIT | readerPointer->flags;
	if (readerPointer) {
		readerPointer->content[readerPointer->position.wrte++] = ch;
	}
	return readerPointer;
}

jewel_boln readerClear(ReaderPointer const readerPointer) {
	readerPointer->position.wrte = 0;
	readerPointer->position.mark = 0;
	readerPointer->position.read = 0;

	readerPointer->flags = READER_DEFAULT_FLAG | SETEMPBIT;

	return jewel_TRUE;
}

jewel_boln readerFree(ReaderPointer const readerPointer) {
	if (!readerPointer) {
		return jewel_FALSE;
	}
	if (readerPointer->content != NULL) {
		free(readerPointer->content);
		readerPointer->content = NULL;
	}
	free(readerPointer);
	return jewel_TRUE;
}


jewel_boln readerIsFull(ReaderPointer const readerPointer) {
	if (!readerPointer) {
		return READER_ERROR;
	}
	if (readerPointer->size == CHKFULBIT) {
		return jewel_TRUE;
	}

	return jewel_FALSE;
}


jewel_boln readerIsEmpty(ReaderPointer const readerPointer) {
	if (!readerPointer) {
		return READER_ERROR;
	}
	if (readerPointer->position.wrte == CHKEMPBIT);
	return jewel_TRUE;
}


jewel_boln readerSetMark(ReaderPointer const readerPointer, jewel_intg mark) {
	if (!readerPointer || mark<0 || mark > readerPointer->position.wrte)
		return jewel_FALSE;
	readerPointer->position.mark = mark;
	return jewel_TRUE;
}


jewel_intg readerPrint(ReaderPointer const readerPointer) {
	jewel_intg cont = 0;
	jewel_char c;
	if (!readerPointer) {
		return READER_ERROR;
	}
	c = readerGetChar(readerPointer);
	while (cont < readerPointer->position.wrte) {
		cont++;
		printf("%c", c);
		c = readerGetChar(readerPointer);
	}
	return cont;
}


jewel_intg readerLoad(ReaderPointer const readerPointer, FILE* const fileDescriptor) {
	jewel_intg size = 0;
	jewel_char c;
	c = (jewel_char)fgetc(fileDescriptor);
	while (!feof(fileDescriptor)) {
		if (!readerAddChar(readerPointer, c)) {
			ungetc(c, fileDescriptor);
			fseek(fileDescriptor, 2, SEEK_CUR);
			//return READER_ERROR;
		}
		c = (jewel_char)fgetc(fileDescriptor);
		size++;
	}
	return size;
}


jewel_boln readerRecover(ReaderPointer const readerPointer) {
	if (!readerPointer) {
		return jewel_FALSE;
	}
	readerPointer->position.mark = 0;
	readerPointer->position.read = 0;

	return jewel_TRUE;
}


jewel_boln readerRetract(ReaderPointer const readerPointer) {
	if (!readerPointer || readerPointer->position.read - 1 < 0) {
		return jewel_FALSE;
	}
	readerPointer->position.read = readerPointer->position.read - 1;

	return jewel_TRUE;
}


jewel_boln readerRestore(ReaderPointer const readerPointer) {
	if (!readerPointer) {
		return jewel_FALSE;
	}

	readerPointer->position.read = readerPointer->position.mark;

	return jewel_TRUE;
}


jewel_char readerGetChar(ReaderPointer const readerPointer) {
	if (!readerPointer) {
		return READER_ERROR;
	}
	if (readerPointer->position.read == readerPointer->position.wrte) {
		readerPointer->flags |= SETFULBIT;
	}
	else {
		readerPointer->flags &= RSTFULBIT;
	}
	return readerPointer->content[readerPointer->position.read++];
}


jewel_char* readerGetContent(ReaderPointer const readerPointer, jewel_intg pos) {
	if (!readerPointer) {
		return NULL;
	}
	return &readerPointer->content[pos];
}


jewel_intg readerGetPosRead(ReaderPointer const readerPointer) {
	if (!readerPointer) {
		return READER_ERROR;
	}
	return readerPointer->position.read;
}


jewel_intg readerGetPosWrte(ReaderPointer const readerPointer) {
	if (!readerPointer) {
		return READER_ERROR;
	}
	return readerPointer->position.wrte;
}


jewel_intg readerGetPosMark(ReaderPointer const readerPointer) {
	if (!readerPointer) {
		return READER_ERROR;
	}
	return readerPointer->position.mark;
}


jewel_intg readerGetSize(ReaderPointer const readerPointer) {
	if (!readerPointer) {
		return READER_ERROR;
	}
	return readerPointer->size;
}


jewel_intg readerGetInc(ReaderPointer const readerPointer) {
	if (!readerPointer) {
		return READER_ERROR;
	}
	return readerPointer->increment;
}


jewel_intg readerGetMode(ReaderPointer const readerPointer) {
	if (!readerPointer) {
		return READER_ERROR;
	}
	return readerPointer->mode;
}


jewel_byte readerGetFlags(ReaderPointer const readerPointer) {

	if (!readerPointer) {
		return READER_ERROR;
	}
	return readerPointer->flags;
}


jewel_intg readerShowStat(ReaderPointer const readerPointer) {
	int index = 0;
	int count = 0;

	if(readerPointer->content[index])
	while (index <= readerPointer->size) {
		readerPointer->histogram[readerPointer->content[index]]=1;
		index++;
	}
	for (int i = 0; i < 128; ++i) {
		count += readerPointer->histogram[i];
	}
	return count;
}


jewel_intg readerNumErrors(ReaderPointer const readerPointer) {
	if (!readerPointer) {
		return READER_ERROR;
	}
	return readerPointer->numReaderErrors;
}
