<<<<<<< HEAD
// Rantoglou-Melas Spyridon 	AM=2342
//Papakostas Kostantinos        AM=2399

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Symbols.h"

void condition(struct list *Etrue, struct list *Efalse);
void boolterm(struct list *Btrue, struct list *Bfalse);
void boolfactor(struct list *Qtrue, struct list *Qfalse);


int lineNum = 0, itemp = 0, jtemp = 0,nesting=0;
int state = state0;
int quadnum = 100;
int id, i, token;
char lexis[500];
char progname[500];
char c, b;
FILE *f;
char t[50];

////////////////////////////////////////Symbols Table///////////////////////////////////////////////////
struct Entity{
	char name[500];
	int type;
	int offset;
	int startquad;
	int framelenght;
	char value[500];
	int parMode;
	struct Argument *arg;
	struct entity *nextEntity;
};

struct scope{
	struct Entity *en;
	struct scope *NextScope;
	int nestinglvl;
};

struct Argument{
	int parMode;
	int type;
	struct Argument *nextarg;
};
struct scope *scopehead=NULL;
struct Argument *globArg=NULL;
struct Entity *entitylist;

void createScope(){
	struct scope *scop;
	scop = (struct scope*)malloc(sizeof(struct scope));
	if (scop == NULL){
		printf("cannot allocate memory\n");
		exit(5);
	}
	scop->en = NULL;
	if (scopehead == NULL){
		scop->nestinglvl = 0;
		scop->NextScope = NULL;
		scopehead = scop;
	}
	else{
		nesting++;
		scop->nestinglvl = nesting;
		scop->NextScope = scopehead;
		scopehead = scop;
	}
}

void deleteScope(){

	if (scopehead == NULL){
		printf("can't delete from list");
		exit(6);
	}
	scopehead = scopehead->NextScope;
}

void insertEntity(char n[],int ty,int start){
	struct Entity *enlist;
	struct Entity *previous, *current;

	int off=0;
	enlist = (struct Entity*)malloc(sizeof(struct Entity));
	if (enlist == NULL){
		printf("couldn't allocate memory\n");
		exit(7);
	}
	strcpy(enlist->name, n);
	enlist->type = ty;
	enlist->startquad = start;
	enlist->nextEntity = NULL;

	previous = NULL;
	current = scopehead->en;
	while (current != NULL){
		previous = current;
		if (ty != FUNCTION && ty!=PROCEDURE){
			off = previous->offset;
		}
		current = current->nextEntity;

	}
	if (previous == NULL){
		scopehead->en = enlist;
		enlist->offset = 12;
	}
	else{
		previous->nextEntity = enlist;
		enlist->offset = off + 4;
	}
}

struct Entity *searchEntity(char name[])
{
	struct Entity *enlist;
	struct scope *scop; 

	scop = scopehead;
	while (scop != NULL){
		enlist = scop->en;
		while (enlist != NULL){
			if (strcmp(enlist->name, name) == 0){
				printf("enlist name %s\n",enlist->name);
				return enlist;
			}
			enlist = enlist->nextEntity;
		}
		scop = scop->NextScope;
	}
	return NULL;

}

void print_table()
{
	struct Entity *list;
	struct scope *scop;
	struct Argument *Arg;

	int i=0;

	scop = scopehead;
	while (scop != NULL){
		list = scop->en;
		
		printf("scope: ");
		printf("       ");
		while (list != NULL){
			if (list->type == ID){
				printf("%s (%d) --- ", list->name, list->offset);
			}
			else{
				printf("%s --- ", list->name);
			}
			list = list->nextEntity;
		}
		printf("\n");
		scop = scop->NextScope;
	}
}

void addArguments(char name[],int ty,int ParM){
	struct Argument *current,*previous,*head;
	struct Entity *e;
	int i = 0;
	current = (struct Argument*)malloc(sizeof(struct Argument));
	e = searchEntity(name);
	current->type=ty;
	current->parMode=ParM;
	
	head = e->arg;

	if (head == NULL){
		head->parMode = ParM;
		head->type = ty;
		current->nextarg = NULL;
	}
	while (head!=NULL){
		head = head->nextarg;
	}
	head->parMode = ParM;
	head->type = ty;
	
	head->nextarg =head;
	head = NULL;
	

}
////////////////////////////////////////Symbols Table///////////////////////////////////////////////////

struct quad {
	char *op;
	char *x;
	char *y;
	char *z;

};

struct list
{

	struct quad *q;
	struct list *next;

};

struct list *head = NULL;
struct list *curr = NULL;


// ############ lexical analysis ##############
FILE *fp;
int isAcceptable(char c)
{

	if (c == ',' || c == ';' || c == '(' || c == ')' || c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '[' || c == ']' || c == '{' || c == '}'){
		return 1;
	}
	return 0;
}

int IDget(char* input){


	if (strcmp(input, "=") == 0){
		return EQUALS;
	}
	else if (strcmp(input, "+") == 0){
		return PLUS;
	}
	else if (strcmp(input, "*") == 0){
		return MULTIPLY;
	}
	else if (strcmp(input, "/") == 0){
		return DIVIDE;
	}
	else if (strcmp(input, "-") == 0){
		return MINUS;
	}
	else if (strcmp(input, "<") == 0){
		return LESS_THAN;
	}
	else if (strcmp(input, ">") == 0){
		return GREATER_THAN;
	}
	else if (strcmp(input, ";") == 0){
		return SEMI_COL;
	}
	else if (strcmp(input, ",") == 0){
		return COMMA;
	}
	else if (strcmp(input, "(") == 0){
		return OPEN_PAR;
	}
	else if (strcmp(input, ")") == 0){
		return CLOSE_PAR;
	}
	else if (strcmp(input, "{") == 0){
		return OPEN_BRAC;
	}
	else if (strcmp(input, "}") == 0){
		return CLOSE_BRAC;
	}
	else if (strcmp(input, "[") == 0){
		return OSQUARE_BRAC;
	}
	else if (strcmp(input, "]") == 0){
		return CSQUARE_BRAC;
	}

	else if (isdigit(input[0])){
		return NUMERIC;
	}
	else if (strlen(input) >= 1){

		if (strcmp(input, "if") == 0){
			return IF;
		}
		else if (strcmp(input, "else") == 0){
			return ELSE;
		}
		else if (strcmp(input, "do") == 0){
			return DO;
		}
		else if (strcmp(input, "while") == 0){
			return WHILE;
		}
		else if (strcmp(input, "forcase") == 0){
			return FORCASE;
		}
		else if (strcmp(input, "incase") == 0){
			return INCASE;
		}
		else if (strcmp(input, "when") == 0){
			return WHEN;
		}
		else if (strcmp(input, "EOF") == 0){
			return EOF;
		}
		else if (strcmp(input, "print") == 0){
			return PRINT;
		}
		else if (strcmp(input, "return") == 0){
			return RETURN;
		}
		else if (strcmp(input, "call") == 0){
			return CALL;
		}
		else if (strcmp(input, "function") == 0){
			return FUNCTION;
		}
		else if (strcmp(input, "and") == 0){
			return AND;
		}
		else if (strcmp(input, "or") == 0){
			return OR;
		}
		else if (strcmp(input, "not") == 0){
			return NOT;
		}
		else if (strcmp(input, "declare") == 0){
			return DECLARE;
		}
		else if (strcmp(input, "enddeclare") == 0){
			return ENDDECLARE;
		}
		else if (strcmp(input, "exit") == 0){
			return EXIT;
		}
		else if (strcmp(input, "procedure") == 0){
			return PROCEDURE;
		}
		else if (strcmp(input, "inout") == 0){
			return INOUT;
		}
		else if (strcmp(input, "in") == 0){
			return IN;
		}
		else if (strcmp(input, "program") == 0){
			return PROGRAM;
		}
		else if (strcmp(input, "copy") == 0){
			return COPY;
		}
		else if (strcmp(input, "declare") == 0){
			return DECLARE;
		}
		else if (strcmp(input, "enddeclare") == 0){
			return ENDDECLARE;
		}
		else if (strcmp(input, "CONSTANT") == 0){
			return CONSTANTID;
		}
		else if (strcmp(input, "<=") == 0){
			return LESS_EQ;
		}
		else if (strcmp(input, ">=") == 0){
			return GREATER_EQ;
		}
		else if (strcmp(input, "<>") == 0){
			return DIFFERENT;
		}
		else if (strcmp(input, ":=") == 0){
			return DECL_EQUALS;
		}
		else{
			return ID;
		}
	}
	return 400;
}

int lex(FILE *fp){

	memset(lexis, 0, sizeof(lexis));
	state = state0;
	i = 0;


	while (state != OK && state != ERROR){
		c = fgetc(fp);
		if (c == '\n') {
			lineNum++;
		}
		else if (state == state0 && isspace(c)){
			state = state0;
		}
		else if (state == state0 && isalpha(c)){
			state = state1;
			lexis[i++] = c;
		}
		else if (state == state1 && (isalpha(c) || isdigit(c))){
			state = state1;
			lexis[i++] = c;

		}
		else if (state == state1 && !(isalpha(c) || isdigit(c))){
			state = OK;
			ungetc(c, fp);
		}
		else if (state == state0 && isdigit(c)){
			state = state2;
			lexis[i++] = c;
		}
		else if (state == state2 && isdigit(c)){
			state = state2;
			lexis[i++] = c;
		}
		else if (state == state2 && !isdigit(c)){
			state = OK;
			ungetc(c, fp);
		}
		else if (state == state0 && c == '<'){
			state = state3;
			lexis[i++] = c;

		}
		else if (state == state3 && c == '>'){
			state = OK;
			lexis[i++] = c;
		}
		else if (state == state3 && c == '='){
			state = OK;
			lexis[i++] = c;
		}
		else if (state == state0 && c == '>'){
			state = state4;
			lexis[i++] = c;
		}
		else if (state == state0 && c == '='){
			state = OK;
			lexis[i++] = c;
		}
		else if (state == state4 && c == '='){
			state = OK;
			lexis[i++] = c;
		}
		else if (state == state3 && c != '>'&& c != '='){
			state = OK;
			lexis[i++] = c;
			ungetc(c, fp);
		}
		else if (state == state4 &&  c != '='){
			state = OK;
			lexis[i++] = c;
			ungetc(c, fp);
		}
		else if (state == state0 && c == ':'){
			lexis[i++] = c;
			state = state5;
		}
		else if (state == state5 && c == '='){
			lexis[i++] = c;
			state = OK;
		}
		else if (state == state5 && c != '='){
			printf("Error in %d line, expected \"=\" after \":\"  ", lineNum);
			state = ERROR;
		}
		else if (state == state0 && c == '*'){
			lexis[i++] = c;
			c = fgetc(fp);
			if (c == '\\'){
				printf("Error in %d line, excpected \"\\*\" before \"*\\\" ", lineNum);
				state = ERROR;
			}
			else{
				ungetc(c, fp);
				state = OK;
			}
		}
		else if (state == state0 && c == '\\'){
			c = fgetc(fp);
			if (c == '*'){
				state = state6;
			}
			else{
				ungetc(c, fp);
				state = OK;
			}
		}
		else if (state == state6 && c != EOF && c != '*'){
			state = state6;
		}
		else if (state == state6 && c == '*'){
			c = fgetc(fp);
			if (c == '\\'){
				state = state0;
			}
			else{
				ungetc(c, fp);
			}
		}
		else if (isAcceptable(c) == 1){
			lexis[i++] = c;
			state = OK;

		}
		else {
			state = ERROR;
		}
		if (c == EOF){
			return -1;
		}
	}
	printf("lexis %s\n", lexis);

	if (state == ERROR){
		memset(lexis, 0, sizeof(lexis));
		return -999;
	}
	id = IDget(lexis);
	if (id == NUMERIC){
		if (atoi(lexis) < (-32768) && (atoi(lexis) > 32768)){
			printf("Error in line %d ,Strange supports number range between -32768 and 32768 ", lineNum);
			memset(lexis, 0, sizeof(lexis));
			return -998;
		}
	}

	return id;

}



// ############################   syntax analysis ###############################
void programtk(){
	id = lex(fp);
	if (id == PROGRAM){
		id = lex(fp);
		if (id == ID){

			strcpy(progname, lexis);


			id = lex(fp);

			createScope();

			block(&progname);

			

			genquad("halt", "_", "_", "_");

			//print_table();
			deleteScope();
		}
		else{
			printf("Syntax error in line %d:Program name expected", lineNum);
		}
	}
	else{
		printf("Syntax error in line %d: word \" program \" expected", lineNum);
	}
}

void block(char *progname){


	if (id == OPEN_BRAC){
		id = lex(fp);
		declarations();
		subprogram();


		genquad("begin_block", progname, "_", "_");
		sequence();
		genquad("end_block", progname, "_", "_");
		if (id == CLOSE_BRAC){
			id = lex(fp);
		}
		else{
			printf("Syntax error in line %d: \"}\" expected after \"{\"\n", lineNum);
		}
	}
	else{
		printf("Syntax error in line %d: \"{\" expected\n", lineNum);
	}
}

void declarations(){


	if (id == DECLARE){
		id = lex(fp);
=======
// Rantoglou-Melas Spyridon 	AM=2342
//Papakostas Kostantinos        AM=2399

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Symbols.h"


int lineNum = 1;
int state = state0;
int id,i,token;
char lexis[500];
char c,b;

FILE *fp;
int isAcceptable(char c)
{

	if (c==',' || c==';' || c=='(' || c==')' || c=='+' || c=='-' || c=='*' || c=='/' || c=='=' || c=='['||c==']' || c=='{'|| c=='}'){
		return 1;
	}
	return 0;
}

int IDget(char* input){
    /*if (strcmp(input,"+")==0){
        return PLUS;
    }*/

		if (strcmp(input, "=")==0){
			return EQUALS;
		}
		else if (strcmp(input,"+")==0){
			return PLUS;
		}
		else if (strcmp(input,"*")==0){
			return MULTIPLY;
		}
		else if (strcmp(input,"/")==0){
			return DIVIDE;
		}
		else if (strcmp(input,"-")==0){
			return MINUS;
		}
		else if (strcmp(input,"<")==0){
			return LESS_THAN;
		}
		else if (strcmp(input,">")==0){
			return GREATER_THAN;
		}
		else if (strcmp(input,";")==0){
			return SEMI_COL;
		}
		else if (strcmp(input,",")==0){
			return COMMA;
		}
		else if (strcmp(input,"(")==0){
			return OPEN_PAR;
		}
		else if (strcmp(input,")")==0){
			return CLOSE_PAR;
		}
		else if (strcmp(input,"{")==0){
			return OPEN_BRAC;
		}
		else if (strcmp(input,"}")==0){
			return CLOSE_BRAC;
		}
		else if (strcmp(input,"[")==0){
			return OSQUARE_BRAC;
		}
		else if (strcmp(input,"]")==0){
			return CSQUARE_BRAC;
		}

	else if (isdigit(input[0])){
		return NUMERIC;
	}
	else if(strlen(input)>=1){

		if (strcmp(input, "if") == 0){
			return IF;
		}
		else if (strcmp(input, "else") == 0){
			return ELSE;
		}
		else if (strcmp(input, "do") == 0){
			return DO;
		}
		else if (strcmp(input, "while") == 0){
			return WHILE;
		}
		else if (strcmp(input, "forcase") == 0){
			return FORCASE;
		}
		else if (strcmp(input, "incase") == 0){
			return INCASE;
		}
		else if (strcmp(input, "when") == 0){
			return WHEN;
		}
		else if (strcmp(input, "EOF") == 0){
			return EOF;
		}
		else if (strcmp(input, "print") == 0){
			return PRINT;
		}
		else if (strcmp(input, "return") == 0){
			return RETURN;
		}
		else if (strcmp(input, "call") == 0){
			return CALL;
		}
		else if (strcmp(input, "function") == 0){
			return FUNCTION;
		}
		else if (strcmp(input, "and") == 0){
			return AND;
		}
		else if (strcmp(input, "or") == 0){
			return OR;
		}
		else if (strcmp(input, "not") == 0){
			return NOT;
		}
		else if (strcmp(input, "declare") == 0){
			return DECLARE;
		}
		else if (strcmp(input, "enddeclare") == 0){
			return ENDDECLARE;
		}
		else if (strcmp(input, "exit") == 0){
			return EXIT;
		}
		else if (strcmp(input, "procedure") == 0){
			return PROCEDURE;
		}
		else if (strcmp(input, "inout") == 0){
			return INOUT;
		}
		else if (strcmp(input, "program") == 0){
			return PROGRAM;
		}
		else if (strcmp(input, "copy") == 0){
			return COPY;
		}
		else if (strcmp(input, "declare") == 0){
			return DECLARE;
		}
		else if (strcmp(input, "enddeclare") == 0){
			return ENDDECLARE;
		}
		else if (strcmp(input, "CONSTANT") == 0){
			return CONSTANTID;
		}
		else if (strcmp(input, "<=")==0){
			return LESS_EQ;
		}
		else if (strcmp(input, ">=")==0){
			return GREATER_EQ;
		}
		else if(strcmp(input,"<>")==0){
			return DIFFERENT;
		}
		else if(strcmp(input,":=")==0){
			return DECL_EQUALS;
		}
		else{
			return ID;
		}
	}
	return 400;
}

int lex(FILE *fp){


	state = state0;
	i=0;


	while (state != OK && state != ERROR){
        c = fgetc(fp);
		if (c == '\n') {
			lineNum++;
		}
		else if (state == state0 && isspace(c)){
			state = state0;
		}
		else if (state == state0 && isalpha(c)){
			state = state1;
			lexis[i++] = c;
		}
		else if (state == state1 && (isalpha(c) || isdigit(c))){
			state = state1;
			lexis[i++] = c;

		}
		else if (state == state1 && !(isalpha(c) || isdigit(c))){
			state = OK;
			ungetc(c, fp);
		}
		else if (state == state0 && isdigit(c)){
			state = state2;
			lexis[i++] = c;
		}
		else if (state == state2 && isdigit(c)){
			state = state2;
			lexis[i++] = c;
		}
		else if (state == state2 && !isdigit(c)){
			state = OK;
			ungetc(c, fp);
		}
		else if (state == state0 && c == '<'){
			state = state3;
			lexis[i++] = c;

		}
		else if (state == state3 && c == '>'){
			state = OK;
			lexis[i++] = c;
		}
		else if (state == state3 && c == '='){
			state = OK;
			lexis[i++] = c;
		}
		else if (state == state0 && c == '>'){
			state = state4;
			lexis[i++] = c;
		}
		else if (state == state0 && c == '='){
			state = OK;
			lexis[i++] = c;
		}
		else if (state == state4 && c == '='){
			state = OK;
			lexis[i++] = c;
		}
		else if (state == state3 && c != '>'&& c != '='){
			state = OK;
			lexis[i++]=c;
			ungetc(c, fp);
        }
		else if (state == state4 &&  c != '='){
			state = OK;
			lexis[i++]=c;
			ungetc(c, fp);
		}
		else if (state == state0 && c == ':'){
            lexis[i++]=c;
			state = state5;
		}
		else if (state == state5 && c == '='){
            lexis[i++]=c;
			state = OK;
		}
		else if (state == state5 && c != '='){
			printf("Error in %d line, expected \"=\" after \":\"  ", lineNum);
			state = ERROR;
		}
        else if (state == state0 && c == '*'){
            lexis[i++] = c;
			c=fgetc(fp);
			if(c=='\\'){
				printf("Error in %d line, excpected \"\\*\" before \"*\\\" ", lineNum);
				state = ERROR;
			}
			else{
				ungetc(c,fp);
				state = OK;
			}
		}
		else if (state == state0 && c=='\\'){
			c=fgetc(fp);
			if(c=='*'){
				state = state6;
			}
			else{
				ungetc(c,fp);
				state=OK;
			}
		}
		else if (state == state6 && c != EOF && c!='*'){
			state = state6;
		}
		else if (state == state6 && c=='*'){
			c=fgetc(fp);
			if(c=='\\'){
				state = state0;
			}
			else{
				ungetc(c,fp);
			}
		}
		else if (isAcceptable(c)==1){
            lexis[i++]=c;
			state = OK;

		}
		else {
			state = ERROR;
		}
		if(c==EOF){
            return -1;
        }
	}
    printf("lexis %s\n",lexis);

	if (state == ERROR){
        memset( lexis, 0, sizeof(lexis) );
		return -999;
	}
	id = IDget(lexis);
	if (id == NUMERIC){
		if (atoi(lexis) < (-32768) && (atoi(lexis) > 32768)){
			printf("Error in line %d ,Strange supports number range between -32768 and 32768 ",lineNum);
			memset( lexis, 0, sizeof(lexis) );
			return -998;
		}
	}
	memset( lexis, 0, sizeof(lexis) );
	return id;

}

void programtk(){
    id = lex(fp);
	if (id == PROGRAM){
		id = lex(fp);
		if (id == ID){
			id = lex(fp);
			block();
		}
		else{
			printf("Syntax error in line %d:Program name expected" ,lineNum);
		}
	}
	else{
		printf("Syntax error in line %d: word \" program \" expected", lineNum);
	}
}

void block(){
	if (id == OPEN_BRAC){
        declarations();
		subprogram();
		sequence();
		printf("my id %d\n",id);
		if (id == CLOSE_BRAC){
			id = lex(fp);
		}
		else{
			printf("Syntax error in line %d: \"}\" expected after \"{\"\n",lineNum);
		}
	}
	else{
		printf("Syntax error in line %d: \"{\" expected\n",lineNum);
	}
}

void declarations(){
    id = lex(fp);
	if (id == DECLARE){
		id = lex(fp);
>>>>>>> 1f22a8414e0323862c3502661296bc94d40364ee
		varlist();
		//id = lex(fp);
		if (id == ENDDECLARE){
<<<<<<< HEAD
			id = lex(fp);
		}
		else{
			printf("Syntax error in line %d: \"enddeclare\" expected\n", lineNum);
		}
	}
}

void varlist(){
	if (id == ID){

		insertEntity(lexis, NUMERIC, nextquad());

		id = lex(fp);
		while (id == COMMA){
			id = lex(fp);
			if (id == ID){
				id = lex(fp);

				insertEntity(lexis, NUMERIC, nextquad());

			}
			else{
				printf("Syntax error in line %d: ID after \",\"\n", lineNum);
			}
		}
	}
}

void subprogram(){
	while (id == FUNCTION || id == PROCEDURE){

		createScope();

		func();

		print_table();
		deleteScope();
	}
}

void func(){
	if (id == PROCEDURE){
		id = lex(fp);
		if (id == ID){

			insertEntity(lexis, PROCEDURE, nextquad());

			id = lex(fp);
			funcbody();
		}
		else{
			printf("Syntax error in line %d: expected ID after procedure", lineNum);
		}
	}
	else if (id == FUNCTION){
		id = lex(fp);
		if (id == ID){

			insertEntity(lexis, FUNCTION, nextquad());

			id = lex(fp);
			funcbody();
		}
		else{
			printf("Syntax error in line %d: expected ID after function", lineNum);
		}
	}
	else{
		printf("Syntax error in line %d: expected \"procedure\" or \"function\" keywords", lineNum);
	}
}

void funcbody(){
	formalpars();
	
	createScope();
	block(&progname);
	print_table();
	deleteScope();
}

void formalpars(){
	if (id == OPEN_PAR){
		id = lex(fp);
		formalparlist();

		if (id == CLOSE_PAR){
			id = lex(fp);
		}
		else{
			printf("Syntax error in line %d: expected closed parenthesis\n", lineNum);
		}
	}

}

void formalparlist(){
	formalparitem();
	while (id == COMMA){
		id = lex(fp);
		formalparitem();

	}
}

void formalparitem(){
	if (id == IN){
		id = lex(fp);
		if (id == ID){
			insertEntity(lexis, IN, nextquad());
			id = lex(fp);
		}
		else{
			printf("Syntax error in line %d: expected ID after \"in\"\n", lineNum);
		}
	}
	else if (id == INOUT){
		id = lex(fp);
		if (id == ID){
			insertEntity(lexis, INOUT, nextquad());
			id = lex(fp);

		}
		else{
			printf("Syntax error in line %d: expected ID after \"inout\"\n", lineNum);
		}
	}
	else if (id == COPY){
		id = lex(fp);
		if (id == ID){
			insertEntity(lexis, COPY, nextquad());
			id = lex(fp);

		}
		else{
			printf("Syntax error in line %d: expected ID after \"copy\"\n", lineNum);
		}
	}
}

void sequence(){
	statement();

	if (id == SEMI_COL){
		statement();
		id = lex(fp);
		while (id == SEMI_COL)
		{
			statement();
			id = lex(fp);
		}
	}
}

void brackets_seq(){
	if (id == OPEN_BRAC){
		sequence();
		if (id == CLOSE_BRAC){
			id = lex(fp);
		}
		else{
			printf("Syntax error in line %d: expected closed brackets\n", lineNum);
		}
	}
}

void brack_or_stat(){
	if (id == OPEN_BRAC){
		id = lex(fp);
		brackets_seq();
	}
	else{
		sequence();

	}
}

void statement(){
	if (id == ID){
		assignment_stat();
	}
	else if (id == IF){
		if_stat();
	}
	else if (id == WHILE){
		while_stat();
	}
	else if (id == DO){
		do_while_stat();
	}
	else if (id == EXIT){
		exit_stat();
	}
	else if (id == INCASE){
		incase_stat();
	}
	else if (id == FORCASE){
		forcase_stat();
	}
	else if (id == CALL){
		call_stat();
	}
	else if (id == RETURN){
		return_stat();
	}
	else if (id == PRINT){
		print_stat();
	}
}

void assignment_stat(){

	char e[500];
	char name[100];
	if (id == ID){
		strcpy(name, lexis);
		id = lex(fp);
		if (id == DECL_EQUALS){
			expression(&e);
			genquad(":=", e, "_", name);
		}
		else{
			printf("Syntax error in line %d: expected \":=\" after ID\n", lineNum);
		}
	}
}

void if_stat(){
	struct list *Btrue, *Bfalse, *iflist;

	Btrue = emptylist();
	Bfalse = emptylist();
	iflist = emptylist();

	struct quad q;
	if (id == IF){
		id = lex(fp);
		if (id == OPEN_PAR){
			condition(Btrue, Bfalse);
			if (id == CLOSE_PAR){
				backpatch(Btrue, nextquad());

				brack_or_stat();

				q = genquad("jump", "_", "_", "_");
				iflist = makelist(&q);
				backpatch(Bfalse, nextquad());

				elsepart();
				backpatch(iflist, nextquad());
			}
		}
		else{
			printf("Syntax error in line %d: no open parenthesis before condition\n", lineNum);
		}
	}
}

void elsepart(){
	if (id == ELSE){
		id = lex(fp);
		brack_or_stat();
	}
}

void do_while_stat(){

	struct list *Condtrue, *Condfalse;
	Condtrue = emptylist();
	Condfalse = emptylist();
	int squad;


	if (id == DO){
		squad = nextquad();
		brack_or_stat();
		if (id == WHILE){
			id = lex(fp);
			if (id == OPEN_PAR){
				condition(Condtrue, Condfalse);
				id = lex(fp);
				if (id == CLOSE_PAR){
					backpatch(Condfalse, squad);
					backpatch(Condtrue, nextquad());
					id = lex(fp);
				}
				else{
					printf("Syntax error in line %d: no close parenthesis after condition\n", lineNum);
				}
			}
			else{
				printf("Syntax error in line %d: no open parenthesis before condition\n", lineNum);
			}
		}
		else{
			printf("Syntax error in line %d: expected keyword \"while\"\n", lineNum);
		}
	}
}

void exit_stat(){
	if (id == EXIT){
		genquad("exit", "_", "_", "_");
		id = lex(fp);
	}
	else{
		printf("Syntax error in line %d: expected keyword \"exit\"\n", lineNum);
	}
}

void return_stat(){
	char t[500];
	if (id == RETURN){
		id = lex(fp);

		if (id == OPEN_PAR){
			expression(&t);

			genquad("retv", t, "_", "_");

			if (id == CLOSE_PAR){
				id = lex(fp);
			}
			else{
				printf("Syntax error in line %d: closed parenthesis expected after expression\n", lineNum);
			}
		}
		else{
			printf("Syntax error in line %d: opened parenthesis expected after \"return\"\n", lineNum);
		}
	}
}
void print_stat(){
	char t[500];
	if (id == PRINT){
		id = lex(fp);
		if (id == OPEN_PAR){

			expression(&t);

			genquad("out", term, "_", "_");

			if (id == CLOSE_PAR){
				id = lex(fp);
			}
			else{
				printf("Syntax error in line %d: closed parenthesis expected after expression\n", lineNum);
			}
		}
		else{
			printf("Syntax error in line %d: opened parenthesis expected after \"print\"\n", lineNum);
		}
	}
}

void while_stat(){

	struct list *Btrue, *Bfalse;
	int Bquad = nextquad();

	Btrue = emptylist();
	Bfalse = emptylist();

	if (id == WHILE){
		id = lex(fp);
		if (id == OPEN_PAR){
			condition(Btrue, Bfalse);
			id = lex(fp);
			if (id == CLOSE_PAR){
				backpatch(Btrue, nextquad());

				brack_or_stat();

				genquad("jump", "_", "_", Bquad);
				backpatch(Bfalse, nextquad());

			}
			else{
				printf("Syntax error in line %d: no closed parenthesis after condition\n", lineNum);
			}
		}
		else{
			printf("Syntax error in line %d: no opened parenthesis before condition\n", lineNum);
		}

	}
}

void incase_stat(){
	struct list *condtrue, *condfalse;

	condtrue = emptylist();
	condfalse = emptylist();
	int pquad, flag_ = 1;
	if (id == INCASE){
		id = lex(fp);
		if (id == OPEN_BRAC){
			id = lex(fp);

			pquad = nextquad();
			genquad(":=", "1", "_", flag_);
			while (id == WHEN){
				id = lex(fp);
				if (id == OPEN_PAR){
					condition(condtrue, condfalse);
					id = lex(fp);
					if (id == CLOSE_PAR){

						backpatch(condtrue, nextquad());
						genquad(":=", "0", "_", "flag_");
						brack_or_stat();
						backpatch(condfalse, nextquad());
					}
					else{
						printf("Syntax error in line %d: closed parenthesis expected\n", lineNum);
					}
				}
				else{
					printf("Syntax error in line %d: opened parenthesis expected\n", lineNum);
				}
			}
			if (id == CLOSE_BRAC){
				id = lex(fp);

				genquad("=", "0", "flag_", pquad);

			}
			else{
				printf("Syntax error in line %d: closed bracket expected\n", lineNum);
			}
		}
		else{
			printf("Syntax error in line %d: opened bracket expected\n", lineNum);
		}
	}
}

void forcase_stat(){
	int start;
	struct list *condtrue, *condfalse;
	condtrue = emptylist();
	condfalse = emptylist();
	if (id == FORCASE){

		start = nextquad();
		id = lex(fp);
		if (id == OPEN_BRAC){
			id = lex(fp);
			while (id == WHEN){
				id = lex(fp);
				if (id == OPEN_PAR){
					condition(condtrue, condfalse);

					id = lex(fp);
					if (id == CLOSE_PAR){
						backpatch(condtrue, nextquad());
						brack_or_stat();
						genquad("jump", "_", "_", start);
						backpatch(condfalse, nextquad());
					}
					else{
						printf("Syntax error in line %d: closed parenthesis expected\n", lineNum);
					}
				}
				else{
					printf("Syntax error in line %d: opened parenthesis expected\n", lineNum);
				}
			}
			if (id == CLOSE_BRAC){
				id = lex(fp);
			}
			else{
				printf("Syntax error in line %d: closed bracket expected\n", lineNum);
			}
		}
		else{
			printf("Syntax error in line %d: opened bracket expected\n", lineNum);
		}
	}
}

void call_stat(){
	if (id == CALL){
		id = lex(fp);
		if (id == ID){

			progname[100] = lexis;
			genquad("call", progname, "_", "_");

			id = lex(fp);
			if (id == OPEN_PAR){
				actualpars();
				id = lex(fp);
				if (id == CLOSE_PAR){
					id = lex(fp);
				}
				else{
					printf("Syntax error in line %d: closed parenthesis expected\n", lineNum);
				}
			}
			else{
				printf("Syntax error in line %d: opened parenthesis expected\n", lineNum);
			}
		}
		else{
			printf("Syntax error in line %d: ID expected\n", lineNum);
		}
	}
	else{
		printf("Syntax error in line %d: \"call\" keyword expected\n", lineNum);
	}
}

void actualpars(){
	if (id == OPEN_PAR){
		id = lex(fp);
		actualparlist();
		if (id == CLOSE_PAR){
			id = lex(fp);
		}
		else{
			printf("Syntax error in line %d: expected closed parenthesis\n", lineNum);
		}
	}
}

void actualparlist(){
	actualparitem();
	id = lex(fp);
	while (id == COMMA){
		actualparitem();
		id = lex(fp);
	}
}

void actualparitem(){
	char t[500];

	if (id == IN){
		expression(&t);
		genquad("par", t, "ref", "_");

		insertEntity(t, IN, nextquad());

	}
	else if (id == INOUT){
		id = lex(fp);

		genquad("par", t, "cv", "_");

		insertEntity(t, INOUT, nextquad());

		if (id == ID){
			id = lex(fp);
		}
		else{
			printf("Syntax error in line %d: expected ID after \"inout\"\n", lineNum);
		}
	}
	else if (id == COPY){
		id = lex(fp);
		if (id == ID){
			id = lex(fp);
			genquad("par", t, "cp", "_");
		
			insertEntity(t, COPY, nextquad());


		}
		else{
			printf("Syntax error in line %d: expected ID after \"copy\"\n", lineNum);
		}
	}
}

void condition(struct list *Etrue, struct list *Efalse){
	struct list *Btrue, *Bfalse;

	Btrue = emptylist();
	Bfalse = emptylist();

	boolterm(Btrue, Bfalse);
	id = lex(fp);
	Btrue = Etrue;
	Bfalse = Efalse;
	if (id == OR){
		backpatch(Bfalse, nextquad());
		boolterm(Btrue, Bfalse);
		id = lex(fp);
		while (id == OR)
		{
			boolterm(Btrue, Bfalse);
			id = lex(fp);
			Btrue = mergelist(Btrue, Etrue);
			Bfalse = Efalse;
		}
	}
}

void boolterm(struct list *Btrue, struct list *Bfalse){

	struct list *Qtrue, *Qfalse;
	Qtrue = emptylist();
	Qfalse = emptylist();

	boolfactor(Qtrue, Qfalse);


	id = lex(fp);
	if (id == AND){

		backpatch(Qtrue, nextquad());
		boolfactor(Qtrue, Qfalse);
		id = lex(fp);
		while (id == AND)
		{
			boolfactor(Qtrue, Qfalse);
			id = lex(fp);
		}
	}
}

void boolfactor(struct list *Qtrue, struct list *Qfalse){
	struct list *Rtrue, *Rfalse;
	Rtrue = emptylist();
	Rfalse = emptylist();

	char t[100];
	char t2[100];
	char relop[3];

	if (id == NOT){
		id = lex(fp);
		if (id == OSQUARE_BRAC){
			condition(Qtrue, Qfalse);
			if (id == CSQUARE_BRAC){
				id = lex(fp);
			}
			else{
				printf("Syntax error in line %d: expected ]\n", lineNum);
			}
		}
		else{
			printf("Syntax error in line %d: expected [\n", lineNum);
		}
	}
	else if (id == OSQUARE_BRAC){
		condition(Qtrue, Qfalse);
		if (id == CSQUARE_BRAC){
			id = lex(fp);
		}
		else{
			printf("Syntax error in line %d: expected ]\n", lineNum);
		}
	}
	else{
		expression(&t);
		relational_oper(&relop);
		expression(&t2);
	}
}

void expression(char *e){
	char term1[500], term2[500];
	char *w;
	optional_sign();

	strcpy(term1, lexis);

	term(&term1);
	while (id == PLUS || id == MINUS){
		add_oper();
		strcpy(term2, lexis);
		term(&term2);
		w = newTemp();

		insertEntity(w, 0, nextquad());

		genquad("+", &term1, &term2, w);
		strcpy(term2, w);
	}
	strcpy(e, &term1);
}

void term(char *t){

	factor(&t);

	while (id == MULTIPLY || id == DIVIDE){
		mul_oper();
		factor(&t);
		//id = lex(fp);
	}
}

void factor(char *t){
	id = lex(fp);
	if (id == OPEN_PAR){
		expression(&t);
		if (id == CLOSE_PAR){
			id = lex(fp);
		}
		else{
			printf("Syntax error in line %d: expected )\n", lineNum);
		}
	}
	else if (id == ID){
		id = lex(fp);
		idtail();
	}
	else if (id == NUMERIC){
		id = lex(fp);
	}
	else{
		printf("Syntax error in line %d: constant | () | ID\n", lineNum);
	}
}

void idtail(){
	actualpars();
}

void relational_oper(char *t){
	if (id == EQUALS){
		id = lex(fp);
	}
	else if (id == GREATER_THAN){
		id = lex(fp);
	}
	else if (id == LESS_THAN){
		id = lex(fp);
	}
	else if (id == GREATER_EQ){
		id = lex(fp);
	}
	else if (id == LESS_EQ){
		id = lex(fp);
	}
	else if (id == DIFFERENT){
		id = lex(fp);
	}

	t = lexis[500];
}

void add_oper(){
	if (id == PLUS){
		id = lex(fp);
	}
	if (id == MINUS){
		id = lex(fp);
	}
}

void mul_oper(){
	if (id == MULTIPLY){
		id = lex(fp);
	}
	else if (id == DIVIDE){
		id = lex(fp);
	}
}

void optional_sign(){
	add_oper();
}


struct list * emptylist(){
	struct list *new;
	new = malloc(sizeof(struct list));

	return new;

}

struct list* makelist(struct quad* x)
{

	struct list *ptr = (struct list*)malloc(sizeof(struct list));
	if (NULL == ptr)
	{
		printf("\n List creation failed \n");
		return NULL;
	}
	ptr->q = x;
	ptr->next = NULL;

	return ptr;
}

struct list* mergelist(struct list * list1, struct list *list2){

	list1->next = list2;


	return list1;

}

void printlist(struct list* current)
{

	if (current == NULL)
		return;

	printlist(current->next);

	strcpy(t, current->q->op);
	fprintf(f, " %s ", t);
	strcpy(t, current->q->x);
	fprintf(f, " %s ", t);
	strcpy(t, current->q->y);
	fprintf(f, " %s ", t);
	strcpy(t, current->q->z);
	fprintf(f, " %s ", t);


	fprintf(f, "\n");
}


void backpatch(struct list *list1, int x){
	struct list *current = list1;
	char t[50];
	char s[50];

	while (current != NULL){


		strcpy(t, current->q->z);
		if (strcmp(t, "_") == 0){


			current->q->z = (char *)malloc(strlen(t) + 1);

			if (current->q->z == NULL) exit(1);



			sprintf(s, 50, "%d", x);

			strcpy(current->q->z, s);


		}
		current = current->next;
	}
	//free(list1);
}
int nextquad(){

	return quadnum;
}

struct quad genquad(char *a, char  *x, char *y, char * z){
	struct list *mylist;

	struct quad *myq = malloc(sizeof(struct quad) + 1);

	myq->op = (char *)malloc(strlen(a) + 1);
	if (myq->op == NULL) exit(1);
	strcpy(myq->op, a);


	myq->x = (char *)malloc(strlen(x) + 1);
	if (myq->op == NULL) exit(1);
	strcpy(myq->x, x);

	myq->y = (char *)malloc(strlen(y) + 1);
	if (myq->y == NULL) exit(1);
	strcpy(myq->y, y);

	myq->z = (char *)malloc(strlen(z) + 1);
	if (myq->z == NULL) exit(1);
	strcpy(myq->z, z);



	if (quadnum == 100){

		head = (struct list*)malloc(sizeof(struct list));
		if (head == NULL)
		{
			printf("\n List creation failed \n");

		}
		head->q = myq;
		head->next = NULL;

	}
	else{

		struct list *mylist = (struct list*)malloc(sizeof(struct list));

		if (mylist == NULL)
		{
			printf("\n List creation failed \n");

		}

		mylist->q = myq;
		mylist->next = NULL;

		mylist->next = head;
		head = mylist;


	}

	quadnum += 10;

	return *myq;
}
const char * newTemp()
{
	int k;
	char str[20] = "T_";
	char str1[50];
	sprintf(str1, "%d", jtemp);
	for (k = 0; k < strlen(str1); k++){
		sprintf(str, "%s%c", str, str1[k]);

	}
	jtemp++;
	printf("%s\n", str);

	return str[20];
}

int main(int argc, char *argv[]){

	struct list *list1, *list2;
	fp = fopen("test.txt", "r");

	programtk();


	f = fopen("2342_2399_test1.txt", "w");


	if (f == NULL){
		printf("error opening file");
		exit(1);
	}


	printlist(head);
	fclose(f);
	fclose(fp);

	system("PAUSE");
}

=======
			id = lex(fp);
		}
		else{
			printf("Syntax error in line %d: \"enddeclare\" expected\n",lineNum);
		}
	}
}

void varlist(){
	if (id == ID){
		id = lex(fp);
		while (id == COMMA){
			id = lex(fp);
			if (id == ID){
				id = lex(fp);
			}
			else{
				printf("Syntax error in line %d: ID after \",\"\n",lineNum);
			}
		}
	}
}

void subprogram(){
	while (id == FUNCTION || id == PROCEDURE){
		function();
	}
}

void function(){
    printf("my id is %d",id);
	if (id == PROCEDURE){
		id = lex(fp);
		if (id == ID){
			id = lex(fp);
			funcbody();
		}
		else{
			printf("Syntax error in line %d: expected ID after procedure", lineNum);
		}
	}
	else if (id == FUNCTION){
		id = lex(fp);
		if (id == ID){
			id = lex(fp);
			funcbody();
		}
		else{
			printf("Syntax error in line %d: expected ID after function", lineNum);
		}
	}
	else{
		printf("Syntax error in line %d: expected \"procedure\" or \"function\" keywords",lineNum);
	}
}

void funcbody(){
	formalpars();
	block();
}

void formalpars(){
	if (id == OPEN_PAR){
		id = lex(fp);
		formalparlist();
		if (id == CLOSE_PAR){
			id = lex(fp);
		}
		else{
			printf("Syntax error in line %d: expected closed parenthesis\n", lineNum);
		}
	}
}

void formalparlist(){
	formalparitem();
	id = lex(fp);
	while (id == COMMA){
		formalparitem();
		id = lex(fp);
	}
}

void formalparitem(){
	if (id == IN){
		id = lex(fp);
		if (id == ID){
			id = lex(fp);
		}
		else{
			printf("Syntax error in line %d: expected ID after \"in\"\n", lineNum);
		}
	}
	else if (id == INOUT){
		id = lex(fp);
		if (id == ID){
			id = lex(fp);
		}
		else{
			printf("Syntax error in line %d: expected ID after \"inout\"\n", lineNum);
		}
	}
	else if (id == COPY){
		id = lex(fp);
		if (id == ID){
			id = lex(fp);
		}
		else{
			printf("Syntax error in line %d: expected ID after \"copy\"\n", lineNum);
		}
	}
}

void sequence(){
	statement();
	id = lex(fp);
	if (id == SEMI_COL){
		statement();
		id = lex(fp);
		while (id == SEMI_COL)
		{
			statement();
			id = lex(fp);
		}
	}
}

void brackets_seq(){
	if (id == OPEN_BRAC){
		sequence();
		if (id == CLOSE_BRAC){
			id = lex(fp);
		}
		else{
			printf("Syntax error in line %d: expected closed brackets\n", lineNum);
		}
	}
}

void brack_or_stat(){
	if (id == OPEN_BRAC){
		brackets_seq();
	}
	else{
		sequence();
		//id = lex(fp) NOT SURE IF NEEDED!!!!!!!!!!!!!!!
	}
}

void statement(){
	if (id == ID){
		assignment_stat();
	}
	else if (id == IF){
		if_stat();
	}
	else if (id == WHILE){
		while_stat();
	}
	else if (id == DO){
		do_while_stat();
	}
	else if (id == EXIT){
		exit_stat();
	}
	else if (id == INCASE){
		incase_stat();
	}
	else if (id == FORCASE){
		forcase_stat();
	}
	else if (id == CALL){
		call_stat();
	}
	else if (id == RETURN){
		return_stat();
	}
	else if (id == PRINT){
		print_stat();
	}
}

void assignment_stat(){
    //id=lex(fp);
	if (id == ID){
		id = lex(fp);
		if (id == DECL_EQUALS){
			expression();
		}
		else{
			printf("Syntax error in line %d: expected \":=\" after ID\n", lineNum);
		}
	}
}

void if_stat(){
	if (id == IF){
		id = lex(fp);
		if (id == OPEN_PAR){
			condition();
			id = lex(fp);
			if (id == CLOSE_PAR){
				id = lex(fp);
				brack_or_stat();
				elsepart();
			}
		}
		else{
			printf("Syntax error in line %d: no open parenthesis before condition\n", lineNum);
		}
	}
}

void elsepart(){
	if (id == ELSE){
		id = lex(fp);
		brack_or_stat();
	}
}

void do_while_stat(){

	if (id == DO){
		brack_or_stat();
		if (id == WHILE){
			id = lex(fp);
			if (id == OPEN_PAR){
				condition();
				id = lex(fp);
				if (id == CLOSE_PAR){
					id = lex(fp);
				}
				else{
					printf("Syntax error in line %d: no close parenthesis after condition\n", lineNum);
				}
			}
			else{
				printf("Syntax error in line %d: no open parenthesis before condition\n", lineNum);
			}
		}
		else{
			printf("Syntax error in line %d: expected keyword \"while\"\n", lineNum);
		}
	}
}

void exit_stat(){
	if (id == EXIT){
		id = lex(fp);
	}
	else{
		printf("Syntax error in line %d: expected keyword \"exit\"\n", lineNum);
	}
}

void return_stat(){
	if (id == RETURN){
		id = lex(fp);
		if (id == OPEN_PAR){
			expression();
			if (id == CLOSE_PAR){
				id = lex(fp);
			}
			else{
				printf("Syntax error in line %d: closed parenthesis expected after expression\n", lineNum);
			}
		}
		else{
			printf("Syntax error in line %d: opened parenthesis expected after \"return\"\n", lineNum);
		}
	}
}
void print_stat(){
	if (id == PRINT){
		id = lex(fp);
		if (id == OPEN_PAR){
			expression();
			if (id == CLOSE_PAR){
				id = lex(fp);
			}
			else{
				printf("Syntax error in line %d: closed parenthesis expected after expression\n", lineNum);
			}
		}
		else{
			printf("Syntax error in line %d: opened parenthesis expected after \"print\"\n", lineNum);
		}
	}
}

void while_stat(){


	if (id == WHILE){
		id = lex(fp);
		if (id == OPEN_PAR){
			condition();
			id = lex(fp);
			if (id == CLOSE_PAR){
				brack_or_stat();
			}
			else{
				printf("Syntax error in line %d: no closed parenthesis after condition\n", lineNum);
			}
		}
		else{
			printf("Syntax error in line %d: no opened parenthesis before condition\n", lineNum);
		}

	}
}

void incase_stat(){
	if (id == INCASE){
		id = lex(fp);
		if (id == OPEN_BRAC){
			id = lex(fp);
			while (id == WHEN){
				id = lex(fp);
				if (id == OPEN_PAR){
					condition();
					id = lex(fp);
					if (id == CLOSE_PAR){
						brack_or_stat();
					}
					else{
						printf("Syntax error in line %d: closed parenthesis expected\n", lineNum);
					}
				}
				else{
					printf("Syntax error in line %d: opened parenthesis expected\n", lineNum);
				}
			}
			if (id == CLOSE_BRAC){
				id = lex(fp);
			}
			else{
				printf("Syntax error in line %d: closed bracket expected\n", lineNum);
			}
		}
		else{
			printf("Syntax error in line %d: opened bracket expected\n", lineNum);
		}
	}
}

void forcase_stat(){
	if (id == FORCASE){
		id = lex(fp);
		if (id == OPEN_BRAC){
			id = lex(fp);
			while (id == WHEN){
				id = lex(fp);
				if (id == OPEN_PAR){
					condition();
					id = lex(fp);
					if (id == CLOSE_PAR){
						brack_or_stat();
					}
					else{
						printf("Syntax error in line %d: closed parenthesis expected\n", lineNum);
					}
				}
				else{
					printf("Syntax error in line %d: opened parenthesis expected\n", lineNum);
				}
			}
			if (id == CLOSE_BRAC){
				id = lex(fp);
			}
			else{
				printf("Syntax error in line %d: closed bracket expected\n", lineNum);
			}
		}
		else{
			printf("Syntax error in line %d: opened bracket expected\n", lineNum);
		}
	}
}

void call_stat(){
	if (id == CALL){
		id = lex(fp);
		if (id == ID){
			id = lex(fp);
			if (id == OPEN_PAR){
				actualpars();
				id = lex(fp);
				if (id == CLOSE_PAR){
					id = lex(fp);
				}
				else{
					printf("Syntax error in line %d: closed parenthesis expected\n", lineNum);
				}
			}
			else{
				printf("Syntax error in line %d: opened parenthesis expected\n", lineNum);
			}
		}
		else{
			printf("Syntax error in line %d: ID expected\n", lineNum);
		}
	}
	else{
		printf("Syntax error in line %d: \"call\" keyword expected\n", lineNum);
	}
}

void actualpars(){
	if (id == OPEN_PAR){
		id = lex(fp);
		actualparlist();
		if (id == CLOSE_PAR){
			id = lex(fp);
		}
		else{
			printf("Syntax error in line %d: expected closed parenthesis\n", lineNum);
		}
	}
}

void actualparlist(){
	actualparitem();
	id = lex(fp);
	while (id == COMMA){
		actualparitem();
		id = lex(fp);
	}
}

void actualparitem(){
	if (id == IN){
		expression();
	}
	else if (id == INOUT){
		id = lex(fp);
		if (id == ID){
			id = lex(fp);
		}
		else{
			printf("Syntax error in line %d: expected ID after \"inout\"\n", lineNum);
		}
	}
	else if (id == COPY){
		id = lex(fp);
		if (id == ID){
			id = lex(fp);
		}
		else{
			printf("Syntax error in line %d: expected ID after \"copy\"\n", lineNum);
		}
	}
}

void condition(){
	boolterm();
	id = lex(fp);
	if (id == OR){
		boolterm();
		id = lex(fp);
		while (id == OR)
		{
			boolterm();
			id = lex(fp);
		}
	}
}

void boolterm(){
	boolfactor();
	id = lex(fp);
	if (id == AND){
		boolfactor();
		id = lex(fp);
		while (id == AND)
		{
			boolfactor();
			id = lex(fp);
		}
	}
}

void boolfactor(){
	if (id == NOT){
		id = lex(fp);
		if (id == OSQUARE_BRAC){
			condition();
			if (id == CSQUARE_BRAC){
				id = lex(fp);
			}
			else{
				printf("Syntax error in line %d: expected ]\n", lineNum);
			}
		}
		else{
			printf("Syntax error in line %d: expected [\n", lineNum);
		}
	}
	else if (id == OSQUARE_BRAC){
		condition();
		if (id == CSQUARE_BRAC){
			id = lex(fp);
		}
		else{
			printf("Syntax error in line %d: expected ]\n", lineNum);
		}
	}
	else{
		expression();
		relational_oper();
		expression();
	}
}

void expression(){
	optional_sign();
	term();
	while (id == PLUS || id == MINUS){
		add_oper();
		term();
	}
}

void term(){
	factor();
	//id = lex(fp);
	while (id == MULTIPLY|| id == DIVIDE){
		mul_oper();
		factor();
		//id = lex(fp);
	}
}

void factor(){
    //id=lex(fp);
	if (id == CONSTANTID){
		id = lex(fp);
	}
	else if (id == OPEN_PAR){
		expression();
		if (id == CLOSE_PAR){
			id = lex(fp);
		}
		else{
			printf("Syntax error in line %d: expected )\n", lineNum);
		}
	}
	else if (id == ID){
		idtail();
	}
	else{
		printf("Syntax error in line %d: contstant | () | ID\n", lineNum);
	}
}

void idtail(){
	actualpars();
}

void relational_oper(){
	if (id == EQUALS){
		id = lex(fp);
	}
	else if (id == GREATER_THAN){
		id = lex(fp);
	}
	else if (id == LESS_THAN){
		id = lex(fp);
	}
	else if (id == GREATER_EQ){
		id = lex(fp);
	}
	else if (id == LESS_EQ){
		id = lex(fp);
	}
	else if (id == DIFFERENT){
		id = lex(fp);
	}
}

void add_oper(){
	if (id == PLUS){
		id = lex(fp);
	}
	else if (id == MINUS){
		id = lex(fp);
	}
}

void mul_oper(){
	if (id == MULTIPLY){
		id = lex(fp);
	}
	else if (id == DIVIDE){
		id = lex(fp);
	}
}

void optional_sign(){
	add_oper();
}



int main(int argc, char *argv[]){

	if (argc < 2){
		exit(1);
	}
	printf("%s",argv[1]);

	fp = fopen(argv[1], "r");

	 /*while (token >= 0){
		token = lex(fp);
		printf("  %d",token);
		printf("\t ");
	} */

	programtk();
	fclose(fp);
}
>>>>>>> 1f22a8414e0323862c3502661296bc94d40364ee
