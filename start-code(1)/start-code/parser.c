/**********************************************************************/
/* lab 1 DVG C01 - Parser OBJECT                                      */
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
/* #include "keytoktab.h"   */       /* when the keytoktab is added   */
/* #include "lexer.h"       */       /* when the lexer     is added   */
/* #include "symtab.h"      */       /* when the symtab    is added   */
/* #include "optab.h"       */       /* when the optab     is added   */

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define DEBUG 1
static int  lookahead=0;
static int  is_parse_ok=1;

/**********************************************************************/
/* RAPID PROTOTYPING - simulate the token stream & lexer (get_token)  */
/**********************************************************************/
/* define tokens + keywords NB: remove this when keytoktab.h is added */
/**********************************************************************/
enum tvalues { program = 257, id, input, output, var, integer, real, boolean, begin_, end_, number, assign};
/**********************************************************************/
/* Simulate the token stream for a given program                      */
/**********************************************************************/
static int tokens[] = {program, id, '(', input, ',', output, ')', ';','$',
                        var, id, ',', id, ',', id, ':', integer, ';',
                        begin_, id, assign, id, '+', id, '*', number, end_, '.'};

/**********************************************************************/
/*  Simulate the lexer -- get the next token from the buffer          */
/**********************************************************************/
static int pget_token()
{
    static int i=0;
    if (tokens[i] != '$') return tokens[i++]; else return '$';
}

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
static void in(char* s)
{
    if(DEBUG) printf("\n *** In  %s", s);
}
static void out(char* s)
{
    if(DEBUG) printf("\n *** Out %s", s);
}
/**********************************************************************/
/* The Parser functions                                               */
/**********************************************************************/
static void match(int t)
{
    if(DEBUG) printf("\n --------In match expected: %4d, found: %4d",
                    t, lookahead);
    if (lookahead == t) lookahead = pget_token();
    else {
    is_parse_ok=0;
    printf("\n *** Unexpected Token: expected: %4d found: %4d (in match)",
              t, lookahead);
    }
}

/**********************************************************************/
/* The grammar functions                                              */
/**********************************************************************/
static void prog(){
in("prog");
program_header();
var_part();
stat_part();
out("prog");
}
static void prog_header(){

}
static void var_part(){
   in("var_part");
   if(lookahead == var){
    match(var);
    var_dec_list();
   }
   out("var_part");
}
static void var_dec_list(){
    in("var_dec_list");
    var_dec();
    if(lookahead == id){
        var_dec();
    }
    out("var_dec_list");
}
static void var_dec(){
    in("var_dec");
    id_list();
    match(':');
    if(lookahead == integer){
        match(integer);
    }else if(lookahead == real){
        match(real);
    }
    match(';');
    out("var_dec");    

}
static void id_list(){
    in("id_list");
    match(id);
    if(lookahead == ';'){
        match(',');
        match(id);
    }
    out("id_list");
}

static void type(){
    in("type");
    if(lookahead == integer){
        match(integer);
    }else if(lookahead == real){
        match(real);
    }else if(lookahead == boleean){
        match(boolean);
    }
    out("type");
}

static void stat_part(){
    in("stat_part");
    match(begin_);
    stat_list();
    match(end_); 
    match('.');
    out("stat_part");

}
static void stat_list(){
    in("stat_list");
        stat();
        if(lookahead == ';'){
            match(';');
            stat();
        }
    
    out("stat_list");
    }
static void stat(){
in("stat");
assign_stat();
}
static void assign_stat(){
    in("assign_stat");
    match(id);
    match(assign);
    expr();
    out("assign_stat");

}
static void expr(){
    in("expr");
    term();
    if(lookahead == '+'){
        match('+');
        term();
    }
    out("expr");
}
static void term(){
    in("term");
    factor();
    if(lookahead == '*'){
        match('*');
        factor();
    }
    out("term");
}
static void factor(){
    in("factor");
    if(lookahead == '('){
        match('(');
        expr();
        match(')');
    }else{
        operand();
    }
    out("factor");
}
static void operand(){
    in("operand");
    if(lookahead == id){
        match(id);
        
    }else if(lookahead == number){
        match(number);
    }
    out("operand");
}

static void program_header()
{
    in("program_header");
    match(program); match(id); match('('); match(input);
    match(','); match(output); match(')'); match(';');
    out("program_header");
}

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/

int parser()
{
    in("parser");
    lookahead = pget_token();       // get the first token
    program_header();               // call the first grammar rule
    out("parser");
    return is_parse_ok;             // status indicator
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
