/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2023
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@                                                               @@@@
@@          /       ==============================        /          @@
@         ////           @@       @@ #@                  ////         @
@         ////*          @@ @%  @ @@    @@  @ #@#       /////         @
@     ((( ////))))   @@  @% @%  @ @@ #@ @@  @ #@@#   ///////( ///     @
@     ((((,/ )))))    @@@@   @@@  @@ ##  @@@   #@#   ///// ,/////     @
@     (((((/)))((    ------------------------------    ((((./////*    @
@    //*./ /  .///   ---  PROGRAMMING LANGUAGE  ---  ////   / ( (//   @
@    ///// / /////   ==============================  * ////* / ////   @
@     ///// ///// ((                               (( ///// /////     @
@    ((((   / , (((((                             (((((  //   /(((    @
@    (((((((/ . (((((                          (((((* / (((((((       @
@      (((((( //((((/((((                    *((((/((((/( (((((/      @
@       .//,   * /   (((((                   (((((  ///    .//.       @
@     ,////////./(  (((((* ////         (///(((((( ,/(,////////       @
@         //////// ,// ((( /////,     ////// ((( //  /////// *        @
@            (((((((((,// * /////     /////   (//(((((((((            @
@            ((((((((((((/////         //.///  (((((((((((.           @
@                   (///////// //(   (//.//////////                   @
@                  (/////////             //////////                  @
@                                                                     @
@@          A L G O N Q U I N   C O L L E G E  -  2 0 2 3 W          @@
@@@@                                                               @@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

*/

/*
************************************************************
* File name: compilers.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12, A22, A32.
* Date: Jan 01 2023
* Professor: Paulo Sousa
* Purpose: This file is the main program of Compilers Project
* Function list: main().
************************************************************
*/

#include <stdio.h>
#include <stdlib.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif


jewel_intg main(int argc, char** argv) {
	jewel_intg i;
	jewel_char option;
	printLogo();
	if (DEBUG) {
		for (i = 0; i < argc; ++i)
			printf("argv[%d] = %s\n", i, argv[i]);
	}
	if (argc < 2) {
		printf("%s%c%s%c%s%c%s", "OPTIONS:\n* [",
			PGM_READER, "] - Reader\n* [",
			PGM_SCANNER, "] - Scanner\n* [",
			PGM_PARSER, "] - Parser\n");
		return EXIT_FAILURE;
	}
	option = argv[1][0];
	switch (option) {
	case PGM_READER:
		printf("%s%c%s", "\n[Option '", PGM_READER, "': Starting READER .....]\n\n");
		mainReader(argc, argv);
		break;
		
		case PGM_SCANNER:
			printf("%s%c%s", "\n[Option '", PGM_SCANNER, "': Starting SCANNER ....]\n\n");
			mainScanner(argc, argv);
			break;
		
		case PGM_PARSER:
			printf("%s%c%s", "\n[Option '", PGM_PARSER, "': Starting PARSER .....]\n\n");
			mainParser(argc, argv);
			break;
		
	default:
		printf("%s%c%s%c%s%c%s", "* OPTIONS:\n- [",
			PGM_READER, "] - Reader\n- [",
			PGM_SCANNER, "] - Scanner\n- [",
			PGM_PARSER, "] - Parser\n");
		break;
	}
	return EXIT_SUCCESS;
}

jewel_void printLogo() {
	printf("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"                    , gaaaaaaaagaaaaaaaaaaaaagaaaaaaaag,            \n",
		"                  , aP8b    _, dYba,    , adPb, _    d8Ya,          \n",
		"                , aP   Yb_,dP     Yba, ,adP     Yb,_dP   Ya,        \n",
		"              , aP     _88         )888(          88_      Ya,      \n",
		"           , aP    _,dP Yb,     adP  8  Yba,     dP Yb,_     Ya,    \n",
		"         , aPYb _, dP8    Yb, adP    8    Yba, dP    8Yb, _ dPYa,   \n",
		"       , aP   YdP  dP     YbdP       8       YbdP     Yb  YbP   Ya, \n",
		"       I8aaaaaa8aaa8baaaaaa88aaaaaaaa8aaaaaaaa88aaaaaad8aaa8aaaaaa8I\n",
		"       `Yb,   d8a, Ya      d8b,      8, d8b    aP,    a8b,      dP' \n",
		"          Yb,dP  Ya  8,   dI  Yb,    8    ,dP  Ib   ,8  aP  Yb,dP   \n",
		"            Y8,    YaI8, ,8'    Yb,  8  ,dP    `8, ,8IaP    ,8P     \n",
		"              Yb,   ` Y8ad'       Yb,8,dP       `ba8P '   ,dP       \n",
		"                Yb,    ` 8,         Y8P         ,8 '    ,dP         \n",
		"                  Yb,    `8,         8         ,8'    ,dP           \n",
		"                    Yb,   `Ya     -JEWEL-     aP'   ,dP             \n",
		"                      Yb,    8,      8      ,8    ,dP               \n",
		"                        Yb,  `8,     8     ,8'  ,dP                 \n",
		"                          Yb, `Ya    8    aP' ,dP                   \n",
		"                            Yb,  8,  8  ,8 , dP                     \n",
		"                              Yb,`8, 8 ,8',dP                       \n",
		"                                Yb,Ya8aP,dP                         \n",
		"                                  Y88888P                           \n",
		"                                    Y8P                             \n",
		"                             ALGONQUIN COLLEGE                      \n"
	);


}
