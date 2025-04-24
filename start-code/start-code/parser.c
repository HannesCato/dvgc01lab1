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

    if (lookahead == t){
        lookahead = get_token();
    } else {
        printf("\nSYNTAX:   Symbol expected %s found %s", tok2lex(t), get_lexeme());
        is_parse_ok = 0;
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
    if(lookahead == id){
        char pname[NAMELEN];
        strcpy(pname, get_lexeme());
        addp_name(pname);
        match(id);
    } else {
        printf("\nSYNTAX: ID expected found %s", get_lexeme());
        addp_name("???");
    }
    match('('); match(input);
    match(','); match(output); 
    match(')'); match(';');
    out("program_header");
}

static void var_part() {
    in("var_part");
    if (lookahead == var) {
        match(var);
        var_dec_list();
    } else {
        printf("\nSYNTAX: SYMBOL expected var found %s", get_lexeme());
        is_parse_ok = 0;
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
    if(lookahead == id){
        char name[NAMELEN];
        strcpy(name, get_lexeme());
        match(id);
        if(find_name(name)){
            printf("\nSEMANTIC: ID already declared: %s", name);
            is_parse_ok = 0;
        }
        addv_name(name);
        if (lookahead == ',') {
            match(',');
            id_list();
        }
    } else {
        printf("\nSYNTAX:   ID expected found %s", get_lexeme());
        is_parse_ok = 0;
        lookahead = get_token();
        if (lookahead == id) {
            id_list();
        }
    }
    out("id_list");
}

static void type() {
    in("type");
    toktyp typval = lookahead;
    
    if (lookahead == integer || lookahead == real || lookahead == boolean) {
        match(lookahead);
        setv_type(typval);
    } else {
        printf("\nSYNTAX:   Type name expected found %s", get_lexeme());
        is_parse_ok = 0;
        setv_type(error);  
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
    char idname[NAMELEN];
    if(lookahead == id){
        strcpy(idname, get_lexeme());
        match(id);
        if(!find_name(idname)){
            printf("\nSEMANTIC: ID NOT declared: %s", idname);
            is_parse_ok = 0;
        }
    } else {
        printf("\nSYNTAX:   ID expected found %s", get_lexeme());
        is_parse_ok = 0;
    }
    toktyp t_left = get_ntype(idname);
        match(assign);
        toktyp t_right = expr();
        if(t_left != t_right){
            printf("\nSEMANTIC: Assign types: %s := %s", tok2lex(t_left), tok2lex(t_right));
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
    toktyp t = error;
    if (lookahead == id) {
        char idname[NAMELEN];
        strcpy(idname, get_lexeme());
        match(id);
        if (!find_name(idname)) {
            printf("\nSEMANTIC: ID NOT declared: %s", idname);
            is_parse_ok = 0;
        }
        t = get_ntype(idname);
    } else if (lookahead == number) {
        match(number);
        t = integer;
    } else if (lookahead == '('){
        match('(');
        t = expr();
        match(')');
    } else {
        printf("\nSYNTAX:   Operand Expected");
        is_parse_ok = 0;
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
    if (lookahead == '$') {
        printf("SYNTAX: Input file is empty\n");
        p_symtab();
        return 0;
    }
    prog();   
    if (lookahead != '$'){
        printf("\nSYNTAX:   Extra symbols after end of parse!");
        printf("\n          ");
        is_parse_ok = 0;
        while(lookahead != '$'){
            printf("%s ", get_lexeme());
            lookahead = get_token();
        }
    }                
    out("parser");
    p_symtab();
    return is_parse_ok;        
}
