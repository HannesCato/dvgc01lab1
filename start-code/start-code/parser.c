/**********************************************************************/
/* lab 1 DVG C01 - Parser OBJECT                                      */
/**********************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "keytoktab.h"
#include "lexer.h"

#define DEBUG 1
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
static void expr();
static void term();
static void factor();
static void operand();

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
    match(program); match(id); match('('); match(input);
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
    match(id);
    if (lookahead == ',') {
        match(',');
        id_list();
    }
    out("id_list");
}

static void type() {
    in("type");
    if (lookahead == integer) {
        match(integer);
    } else if (lookahead == real) {
        match(real);
    } else if (lookahead == boolean) {
        match(boolean);
    }
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
    match(id);
    match(assign);
    expr();
    out("assign_stat");
}

static void expr() {
    in("expr");
    term();
    if (lookahead == '+') {
        match('+');
        expr();
    }
    out("expr");
}

static void term() {
    in("term");
    factor();
    if (lookahead == '*') {
        match('*');
        term();
    }
    out("term");
}

static void factor() {
    in("factor");
    if (lookahead == '(') {
        match('(');
        expr();
        match(')');
    } else {
        operand();
    }
    out("factor");
}

static void operand() {
    in("operand");
    if (lookahead == id) {
        match(id);
    } else if (lookahead == number) {
        match(number);
    }
    out("operand");
}

/**********************************************************************/
/* PUBLIC FUNCTION                                                   **/
/**********************************************************************/
int parser() {
    in("parser");
    lookahead = get_token(); 
    prog();                   
    out("parser");
    return is_parse_ok;        
}
