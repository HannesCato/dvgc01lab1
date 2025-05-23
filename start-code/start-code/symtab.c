/**********************************************************************/
/* lab 1 DVG C01 - Symbol Table OBJECT                                */
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
#define TABSIZE 1024                   /* symbol table size           */
#define NAMELEN   20                   /* name length                 */
#define LINE 50

typedef char tname[NAMELEN];

static tname  name[TABSIZE];
static toktyp role[TABSIZE];
static toktyp type[TABSIZE];
static int    size[TABSIZE];
static int    addr[TABSIZE];

static int numrows=0;                  /* number of rows in the ST    */
static int startp =0;                  /* start position program in ST*/
static int next_addr =0;

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
/**********************************************************************/
/*  GET methods (one for each attribute)                              */
/**********************************************************************/
static char * get_name(int ftref)   { return name[ftref]; }
static toktyp get_role(int ftref)   { return role[ftref]; }
static toktyp get_type(int ftref)   { return type[ftref]; }
static int    get_size(int ftref)   { return size[ftref]; }
static int    get_addr(int ftref)   { return addr[ftref]; }

/**********************************************************************/
/*  SET methods (one for each attribute)                              */
/**********************************************************************/
static void set_name(int ftref, char * fpname) { strcpy(name[ftref],
                                                        fpname);}
static void set_role(int ftref, toktyp frole)  { role[ftref] = frole; }
static void set_type(int ftref, toktyp ftype)  { type[ftref] = ftype; }
static void set_size(int ftref, int    fsize)  { size[ftref] = fsize; }
static void set_addr(int ftref, int    faddr)  { addr[ftref] = faddr; }

/**********************************************************************/
/*  Add a row to the symbol table                                     */
/**********************************************************************/
static void addrow(char *fname, toktyp frole, toktyp ftype,
                   int fsize, int faddr)
{
    if(numrows < TABSIZE){
        set_name(numrows, fname);
        set_role(numrows, frole);
        set_type(numrows, ftype);
        set_size(numrows, fsize);
        set_addr(numrows, faddr);
    numrows++;
    } else {
        fprintf(stderr, "SYMTAB IS FULL");
    }
}
/**********************************************************************/
/*  Initialise the symbol table                                       */
/**********************************************************************/
static void initst()
{
    addrow(tok2lex(predef),  typ, predef, 0, 0);
    addrow(tok2lex(undef),   typ, predef, 0, 0);
    addrow(tok2lex(error),   typ, predef, 0, 0);
    addrow(tok2lex(integer), typ, predef, 4, 0);
    addrow(tok2lex(boolean), typ, predef, 4, 0);
    addrow(tok2lex(real),    typ, predef, 8, 0);
}
/**********************************************************************/
/*  return a reference to the ST (index) if name found else nfound    */
/**********************************************************************/
static int get_ref(char * fpname)
{
    for(int i = 0; i < numrows; i++){
        if(strcmp(name[i], fpname) == 0)
            return i;
        } 
    return nfound;
}

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/*  Display the symbol table                                          */
/**********************************************************************/
static void p_symrow(int ftref)
{
    printf(" %8s    %8s  %8s  %4d  %4d \n",
        get_name(ftref),
        tok2lex(get_role(ftref)),
        tok2lex(get_type(ftref)),
        get_size(ftref),
        get_addr(ftref));
}
void p_line(){
    
    for(int i = 0; i < LINE; i++){
        printf("-");
    }
    printf("\n");
}

void p_symtab()
{   
    int total = 0;
    for(int i = 0; i < numrows; i++){
        if(get_role(i) == var){
            total += get_size(i);
        }
    }
    set_size(startp, next_addr);

    printf("\n");
    p_line();
    printf(" THE SYMBOL TABLE\n");
    p_line();
    printf("   NAME         ROLE     TYPE     SIZE  ADDR \n");
    p_line();
    for (int i = 0; i < numrows; i++) {
        if (!(get_role(i) == typ && get_type(i) == predef)) {
            p_symrow(i);
        }
    }    
    p_line();
    printf(" STATIC STORAGE REQUIRED IS %d BYTES \n", total);
    p_line();
}

/**********************************************************************/
/*  Add a program name to the symbol table                            */
/**********************************************************************/
void addp_name(char * fpname)
{
    static int initialized = 0;
    if(!initialized){
        initst();
        initialized = 1;
    }


    set_name(numrows, fpname);
    set_role(numrows, program);
    set_type(numrows, program);
    set_size(numrows, 0);
    set_addr(numrows, 0);
    startp = numrows;
    numrows++;
}

/**********************************************************************/
/*  Add a variable name to the symbol table                           */
/**********************************************************************/
void addv_name(char * fpname)
{
    set_name(numrows, fpname);
    set_role(numrows, var);
    set_type(numrows, undef);
    set_size(numrows, 0);
    set_addr(numrows, 0);
    numrows++;
}

/**********************************************************************/
/*  Find a name in the the symbol table                               */
/*  return a Boolean (true, false) if the name is in the ST           */
/**********************************************************************/
int find_name(char * fpname)
{
    int ref = get_ref(fpname);
    return ref != nfound;
}

/**********************************************************************/
/*  Set the type of an id list in the symbol table                    */
/**********************************************************************/

void setv_type(toktyp ftype)
{
    int size = 0;
    const char *typename = tok2lex(ftype); 

    for (int i = 0; i < numrows; i++) {
        if (strcmp(get_name(i), typename) == 0 &&
            get_role(i) == typ &&
            get_type(i) == predef) {

            size = get_size(i);
            break;
        }
    }
    for (int i = 0; i < numrows; i++) {
        if (get_role(i) == var && get_type(i) == undef) {
            set_type(i, ftype);
            set_size(i, size);
            set_addr(i, next_addr);
            next_addr += size;
        } 
    }
}

/**********************************************************************/
/*  Get the type of a variable from the symbol table                  */
/**********************************************************************/
toktyp get_ntype(char * fpname)
{
    return get_type(get_ref(fpname));
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
