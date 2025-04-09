/**********************************************************************/
/* lab 1 DVG C01 - Driver OBJECT                                      */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */    
/**********************************************************************/
#include "keytoktab.h"

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
/**********************************************************************/
/* type definitions                                                   */
/**********************************************************************/
typedef struct tab {
	char 	* text;
	int 	token;
}tab;

/**********************************************************************/
/* data objects (tables)                                              */
/**********************************************************************/
static tab tokentab[ ] = {
    {"id", 	            id},
    {"number",      number},
    {":=", 	        assign},
    {"undef", 	     undef},
    {"predef",      predef},
    {"tempty",      tempty},
    {"error",        error},
    {"type",           typ},
    {"$",              '$'},
    {"(",              '('},
    {")",              ')'},
    {"*",              '*'},
    {"+",              '+'},
    {",",              ','},
    {"-",              '-'},
    {".",              '.'},
    {"/",              '/'},
    {":",              ':'},
    {";",              ';'},
    {"=",              '='},
    {"TERROR", 	    nfound}
};


static tab keywordtab[ ] = {
	{"program", 	program},
	{"input", 	      input},
	{"output", 	     output},
	{"var", 	        var},
	{"begin", 	      begin},
	{"end", 	        end},
	{"boolean", 	boolean},
	{"integer", 	integer},
	{"real", 	       real},
	{"KERROR", 	     nfound}
};

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* Display the tables                                                 */
/**********************************************************************/
void p_toktab()
{
    printf("Token table\n");
    for (int i = 0; tokentab[i].token != nfound; i++) {
        printf("  %-10s -> %d\n", tokentab[i].text, tokentab[i].token);
    }

}

/**********************************************************************/
/* lex2tok - convert a lexeme to a token                              */
/**********************************************************************/
toktyp lex2tok(char *fplex)
{
    
    for (int i = 0; keywordtab[i].token != nfound; i++) {
        if (strcmp(keywordtab[i].text, fplex) == 0)
            return keywordtab[i].token;
    }


    for (int i = 0; tokentab[i].token != nfound; i++) {
        if (strcmp(tokentab[i].text, fplex) == 0)
            return tokentab[i].token;
    }

    return error;
}


/**********************************************************************/
/* key2tok - convert a keyword to a token                             */
/**********************************************************************/
toktyp key2tok(char * fplex)
{
    for (int i = 0; keywordtab[i].token != nfound; i++) {
        if (strcmp(keywordtab[i].text, fplex) == 0)
            return keywordtab[i].token;
    }
    return nfound;
}


/**********************************************************************/
/* tok2lex - convert a token to a lexeme                              */
/**********************************************************************/
char * tok2lex(toktyp ftok)
{
    for (int i = 0; keywordtab[i].token != nfound; i++) {
        if (keywordtab[i].token == ftok)
            return keywordtab[i].text;
    }
    for (int i = 0; tokentab[i].token != nfound; i++) {
        if (tokentab[i].token == ftok)
            return tokentab[i].text;
    }
    return "UNKNOWN";
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
