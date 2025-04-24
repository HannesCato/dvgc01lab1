/**********************************************************************/
/* lab 1 DVG C01 - Lexer OBJECT                                       */
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
#define BUFSIZE 1024
#define LEXSIZE   30
#define LINE 50
static char buffer[BUFSIZE];
static char lexbuf[LEXSIZE];
static int  pbuf  = 0;               /* current index program buffer  */
static int  plex  = 0;               /* current index lexeme  buffer  */

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
/**********************************************************************/
/* buffer functions                                                   */
/**********************************************************************/
/**********************************************************************/
/* Read the input file into the buffer                                */
/**********************************************************************/

static void get_prog()
{   
    char c;
    int i = 0;
    while((c = getc(stdin)) != EOF && i < BUFSIZE - 2){
    buffer[i++] = c;
    }

    buffer[i] = '$';
    buffer[i + 1] = '\0';
    
    pbuf = 0;
}

/**********************************************************************/
/* Display the buffer                                                 */
/**********************************************************************/

void p_line(){
    for(int i = 0; i < LINE; i++){
        printf("-");
    }
    printf("\n")
}

static void pbuffer()
{
    p_line();
    printf(" THE PROGRAM TEXT\n");
    p_line();
    printf("%s\n", buffer);
}

/**********************************************************************/
/* Copy a character from the program buffer to the lexeme buffer      */
/**********************************************************************/

static void get_char()
{
    lexbuf[plex++] = buffer[pbuf++];
}

/**********************************************************************/
/* End of buffer handling functions                                   */
/**********************************************************************/

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* Return a token                                                     */
/**********************************************************************/
int get_token()
{
   static int initialized = 1;
   plex = 0;
   memset(lexbuf, 0, LEXSIZE);

   
   if(initialized == 1){
    get_prog();
    pbuffer();
    initialized = 0;
    pbuf = 0;
   } 

   if (buffer[pbuf] == '$') {
    strcpy(lexbuf, "$");
    return '$';
}

while(isspace(buffer[pbuf]))pbuf++;


if(isdigit(buffer[pbuf])) {
    //number
    while(isdigit(buffer[pbuf])){
        get_char();
    }
    return number;
} else if(isalpha(buffer[pbuf])){
    //id or key
      while(isalnum(buffer[pbuf])){
        get_char();
    }
    return key2tok(lexbuf);
} else if(ispunct(buffer[pbuf])){
    if(buffer[pbuf] == ':' && buffer[pbuf + 1] == '='){
    //symboler och operatorer
    get_char();
    get_char();
    return assign;
    } else {
      get_char();     
    }
    return lex2tok(lexbuf);
    }   
    return error;
}

/**********************************************************************/
/* Return a lexeme                                                    */
/**********************************************************************/
char * get_lexeme()
{
    return lexbuf;
}

/**********************************************************************/
/* End of code                                                       **/
/**********************************************************************/
