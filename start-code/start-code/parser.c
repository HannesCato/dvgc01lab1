/**********************************************************************/
/* lab 1 DVG C01 - Parser OBJECT                                      */
/**********************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "keytoktab.h"
#include "lexer.h"
#include "symtab.h"
#include "optab.h"

#define DEBUG 0
#define NAMELEN 20
static int lookahead = 0;
static int is_parse_ok = 1;


// Function prototypes
static void prog();
static void program_header();
static void var_part();
static void var_dec_list();
static void var_dec();
static void id_list();
static void type();
static void stat_part();
static void stat_list();
static void stat();
static void assign_stat();
static toktyp expr();
static toktyp term();
static toktyp factor();
static toktyp operand();

/**********************************************************************/
/* RAPID PROTOTYPING – simulated token stream (removed in step 3)     */
/**********************************************************************/

/* Step 1 enum – not used anymore */
/*
enum tvalues { program = 257, id, input, output, var, integer, real, boolean, begin, end, number, assign };
*/

/* Temporary token stream – will be removed in step 3
static int tokens[] = {
    program, id, '(', input, ',', output, ')', ';',
    var, id, ',', id, ',', id, ':', integer, ';',
    begin, id, assign, id, '+', id, '*', number,
    end, '.', '$'
};

static int pget_token() {
    static int i = 0;
    if (tokens[i] != '$') return tokens[i++];
    else return '$';
} */

/**********************************************************************/
/* Helper functions                                                   */
/**********************************************************************/
static void in(char* s) {
    if (DEBUG) printf("\n *** In  %s", s);
}
static void out(char* s) {
    if (DEBUG) printf("\n *** Out %s", s);
}

static void match(int t) {
    if (DEBUG)
        printf("\n *** In  match        expected %s found %s", tok2lex(t), tok2lex(lookahead));

    if (lookahead == t)
        lookahead = get_token();
    else {
        is_parse_ok = 0;
        printf("\n *** Syntax Error: expected %s, found %s\n", tok2lex(t), tok2lex(lookahead));
    }
}



/**********************************************************************/
/* Grammar functions                                                  */
/**********************************************************************/
static void prog() {
    in("prog");
    program_header();
    var_part();
    stat_part();
    out("prog");
}

static void program_header() {
    in("program_header");
    match(program); 
    char program_name[NAMELEN];
    strcpy(program_name, get_lexeme());
    match(id);
    addp_name(program_name);
    
    match('('); match(input);
    match(','); match(output); match(')'); match(';');
    out("program_header");
}

static void var_part() {
    in("var_part");
    if (lookahead == var) {
        match(var);
        var_dec_list();
    }
    out("var_part");
}

static void var_dec_list() {
    in("var_dec_list");
    var_dec();
    if (lookahead == id) {
        var_dec_list();
    }
    out("var_dec_list");
}

static void var_dec() {
    in("var_dec");
    id_list();
    match(':');
    type();
    match(';');
    out("var_dec");
}

static void id_list() {
    in("id_list");
    char varname[NAMELEN];
    strcpy(varname, get_lexeme());
    match(id);
    addv_name(varname);
    if (lookahead == ',') {
        match(',');
        id_list();
    }
    out("id_list");
}

static void type() {
    in("type");
    toktyp typval = lookahead;
    if (lookahead == integer) {
        match(integer);
    } else if (lookahead == real) {
        match(real);
    } else if (lookahead == boolean) {
        match(boolean);
    }
    setv_type(typval);
    out("type");
}

static void stat_part() {
    in("stat_part");
    match(begin);
    stat_list();
    match(end);
    match('.');
    out("stat_part");
}

static void stat_list() {
    in("stat_list");
    stat();
    if (lookahead == ';') {
        match(';');
        stat_list();
    }
    out("stat_list");
}

static void stat() {
    in("stat");
    assign_stat();
    out("stat");
}

static void assign_stat() {
    in("assign_stat");
    char idname[NAMELEN];
    strcpy(idname, get_lexeme());
    match(id);
    toktyp t_left = get_ntype(idname);
    match(assign);
    toktyp t_right = expr();

    if(t_left != t_right){
        is_parse_ok = 0;
    }
    out("assign_stat");
}

static toktyp expr() {
    in("expr");
    toktyp t1 = term();
    if (lookahead == '+') {
        match('+');
        toktyp t2 = expr();
        t1 = get_otype('+', t1, t2);
    }
    out("expr");
    return t1;
}

static toktyp term() {
    in("term");
    toktyp t1 = factor();
    if (lookahead == '*') {
        match('*');
        toktyp t2 = term();
        t1 = get_otype('*', t1, t2);
    }
    out("term");
    return t1;
}

static toktyp factor() {
    in("factor");
    toktyp t;
    if (lookahead == '(') {
        match('(');
        t = expr();
        match(')');
    } else {
        t = operand();
    }
    out("factor");
    return t;
}

static toktyp operand() {
    in("operand");
    toktyp t = undef;
    if (lookahead == id) {
        char idname[NAMELEN];
        strcpy(idname, get_lexeme());
        match(id);
        t = get_ntype(idname);
    } else if (lookahead == number) {
        match(number);
        t = integer;
    }
    out("operand");
    return t;
}

/**********************************************************************/
/* PUBLIC FUNCTION                                                   **/
/**********************************************************************/
int parser() {
    in("parser");
    lookahead = get_token(); 
    prog();                   
    out("parser");
    p_symtab();
    return is_parse_ok;        
}
