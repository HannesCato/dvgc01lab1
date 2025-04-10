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
    //fclose(stdin);
}

/**********************************************************************/
/* Display the buffer                                                 */
/**********************************************************************/

static void pbuffer()
{
    printf("BUFFER: %s\n", buffer);
}

/**********************************************************************/
/* Copy a character from the program buffer to the lexeme buffer      */
/**********************************************************************/

static void get_char()
{
    lexbuf[plex++] = buffer[pbuf++];
    //buffer[pbuf] = '\0';
    
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
   static int initialized = 0;
   if(!initialized){
    get_prog();
    pbuffer();
    initialized = 1;
   } 
  //get_prog();
  //pbuffer(); 

   if (buffer[pbuf] == '$') {
    strcpy(lexbuf, "$");
    return '$';
}

   while(isspace(buffer[pbuf]))pbuf++;
   plex = 0;
   
   char c = buffer[pbuf];
   if(isalpha(c)){
    //id or key
      while(isalnum(buffer[pbuf]))get_char();
    lexbuf[plex] = '\0';
    return lex2tok(lexbuf);
   } 
   else if(isdigit(c)) {
    //number
    while(isdigit(buffer[pbuf]))get_char();
    lexbuf[plex] = '\0';
    return number;
   }
   else if(c == ':' && buffer[pbuf + 1] == '='){
    //:= operator
    get_char();
    get_char();
    lexbuf[plex] = '\0';
    return assign;
   }
   else if(ispunct(c)){
    //symboler och operatorer
    get_char();
    lexbuf[plex] = '\0';
    return lex2tok(lexbuf);
   } 
   else {
      get_char();
      lexbuf[plex] = '\0';
      return error;       
   }
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
