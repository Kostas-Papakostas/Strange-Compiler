// Rantoglou-Melas Spyridon 	AM=2342
//Papakostas Kostantinos        AM=2399

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Symbols.h"


int lineNum = 1;
int state = state0;
int id,i,counter=0;
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
	if (strlen(input) == 1 && !isdigit(input) && !isalpha(input)){
		if (input == "="){
			return EQUALS;
		}
		else if (input == "+"){
			return PLUS;
		}
		else if (input == "*"){
			return MULTIPLY;
		}
		else if (input == "/"){
			return DIVIDE;
		}
		else if (input == "-"){
			return MINUS;
		}
		else if (input == "<"){
			return LESS_THAN;
		}
		else if (input==">"){
			return GREATER_THAN;
		}
		else if (input == ";"){
			return SEMI_COL;
		}
		else if (input == ","){
			return COMMA;
		}
		else if (input == "("){
			return OPEN_PAR;
		}
		else if (input == ")"){
			return CLOSE_PAR;
		}
		else if (input == "{"){
			return OPEN_BRAC;
		}
		else if (input == "}"){
			return CLOSE_BRAC;
		}
		else if (input == "["){
			return OSQUARE_BRAC;
		}
		else if (input == "]"){
			return CSQUARE_BRAC;
		}
	}
	else if (isdigit(input[0])){
		return NUMERIC;
	}
	else{

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
    strcpy(lexis,"");

	state = state0;
	i=0;
	fseek(fp,counter,SEEK_SET);
	c = fgetc(fp);
	counter++;
	while (state != OK && state != ERROR){
		if (c == '\n') {
			lineNum++;
		}
		else if (state == state0 && c == EOF){
			state = OK;
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
			counter--;
		}
		else if (state == state0 && isdigit(c)){
			state = state2;
			lexis[i++] = c;
		}
		else if (state == state2 && isdigit(c)){
			state = state2;
			lexis[i++] = c;
		}
		else if (state == state2 && isdigit(c)){
			state = OK;
			ungetc(c, fp);
			counter--;
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
		else if (state == state4 && c == '='){
			state = OK;
			lexis[i++] = c;
		}
		else if (state == state3 && c != '>'&& c != '='){
			state = OK;
			ungetc(c, fp);
			counter--;
		}
		else if (state == state4 &&  c != '='){
			state = OK;
			ungetc(c, fp);
			counter--;
			break;
		}
		else if (state == state0 && c == ':'){
			state = state5;
		}
		else if (state == state5 && c == '='){
			state = OK;
		}
		else if (state == state5 && c != '='){
			printf("Error in %d line, expected \"=\" after \":\"  ", lineNum);
			state = ERROR;
		}
		else if (state == state0 && c == '*'){
			c=fgetc(fp);
			counter++;
			if(c=='/'){
				printf("Error in %d line, excpected \"/*\" before \"*/\" ", lineNum);
	
				state = ERROR;
			}
			else{
				ungetc(c,fp);
				counter--;
				state = OK;
			}
		}
		else if (state == state0 && c=='/'){
			c=fgetc(fp);
			if(c=='*'){
				state = state6;
			}
			else{
				ungetc(c,fp);
				counter--;
				state=OK;
			}
		}
		else if (state == state6 && c != EOF && c!='*'){
			state = state6;
		}
		else if (state == state6 && c=='*'){
			c=fgetc(fp);
			counter++;
			if(c=='/'){
				state = state0;
			}
			else{
				ungetc(c,fp);
				counter--;
			}
		}
		else if (isAcceptable(c)){
			state = OK;
		}
		else {
			state = ERROR;
		}
		c = fgetc(fp);
		counter++;	
		
	}

	if (state == ERROR){
		return -999;
	}
	id = IDget(lexis);
	if (id == NUMERIC){

		if (atoi(lexis) < (-32768) && (atoi(lexis) > 32768)){

			printf("Error in line %d ,Strange supports number range between -32768 and 32768 ",lineNum);
			return -998;
		}
	}
	if(c==EOF){
		return -1;
	}
	return id;

}

void programtk(){
	if (id == PROGRAM){
		id = lex(fp);
		if (id == ID){
			id = lex(fp);
			block();
		}
		else{
			perror("Syntax error in line %d:Program name expected" ,lineNum);
		}
	}
	else{
		perror("Syntax error in line %d: word \" program \" expected", lineNum);
	}
}

void block(){
	if (id == OPEN_BRAC){
		id = lex(fp);
		declarations();
		subprograms();
		sequence();
		if (id == CLOSE_BRAC){
			id = lex(fp);
		}
		else{
			perror("Syntax error in line %d: \"}\" expected after \"{\"",lineNum);
		}
	}
	else{
		perror("Syntax error in line %d: \"{\" expected",lineNum);
	}
}

void declarations(){
	if (id == DECLARE){
		id = lex(fp);
		varlist();
		if (id == ENDDECLARE){
			id = lex(fp);
		}
		else{
			perror("Syntax error in line %d: \"enddeclare\" expected",lineNum);
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
				perror("Syntax error in line %d: ID after \",\"",lineNum);
			}
		}
	}
}

void subprogram(){
	while (id == FUNCTION){
		id = lex(fp);
		function();
	}
}

void function(){
	if (id == PROCEDURE){
		id = lex(fp);
		if (id == ID){
			id = lex(fp);
			funcbody();
		}
		else{
			perror("Syntax error in line %d: expected ID after procedure", lineNum);
		}
	}
	else if (id == FUNCTION){
		id = lex(fp);
		if (id == ID){
			id = lex(fp);
			funcbody();
		}
		else{
			perror("Syntax error in line %d: expected ID after function", lineNum);
		}
	}
	else{
		perror("Syntax error in line %d: expected \"procedure\" or \"function\" keywords",lineNum);
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
			perror("Syntax error in line %d: expected closed parenthesis\n", lineNum);
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
			perror("Syntax error in line %d: expected ID after \"in\"\n", lineNum);
		}
	}
	else if (id == INOUT){
		id = lex(fp);
		if (id == ID){
			id = lex(fp);
		}
		else{
			perror("Syntax error in line %d: expected ID after \"inout\"\n", lineNum);
		}
	}
	else if (id == COPY){
		id = lex(fp);
		if (id == ID){
			id = lex(fp);
		}
		else{
			perror("Syntax error in line %d: expected ID after \"copy\"\n", lineNum);
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
			perror("Syntax error in line %d: expected closed brackets\n", lineNum);
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
	if (id == ID){
		id = lex(fp);
		if (id == DECL_EQUALS){
			expression();
		}
		else{
			perror("Syntax error in line %d: expected \":=\" after ID\n", lineNum);
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
			perror("Syntax error in line %d: no open parenthesis before condition\n", lineNum);
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
					perror("Syntax error in line %d: no close parenthesis after condition\n", lineNum);
				}
			}
			else{
				perror("Syntax error in line %d: no open parenthesis before condition\n", lineNum);
			}
		}
		else{
			perror("Syntax error in line %d: expected keyword \"while\"\n", lineNum);
		}
	}
}

void exit_stat(){
	if (id == EXIT){
		id = lex(fp);
	}
	else{
		perror("Syntax error in line %d: expected keyword \"exit\"\n", lineNum);
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
				perror("Syntax error in line %d: closed parenthesis expected after expression\n", lineNum);
			}
		}
		else{
			perror("Syntax error in line %d: opened parenthesis expected after \"return\"\n", lineNum);
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
				perror("Syntax error in line %d: closed parenthesis expected after expression\n", lineNum);
			}
		}
		else{
			perror("Syntax error in line %d: opened parenthesis expected after \"print\"\n", lineNum);
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
				perror("Syntax error in line %d: no closed parenthesis after condition\n", lineNum);
			}
		}
		else{
			perror("Syntax error in line %d: no opened parenthesis before condition\n", lineNum);
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
						perror("Syntax error in line %d: closed parenthesis expected\n", lineNum);
					}
				}
				else{
					perror("Syntax error in line %d: opened parenthesis expected\n", lineNum);
				}
			}
			if (id == CLOSE_BRAC){
				id = lex(fp);
			}
			else{
				perror("Syntax error in line %d: closed bracket expected\n", lineNum);
			}
		}
		else{
			perror("Syntax error in line %d: opened bracket expected\n", lineNum);
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
						perror("Syntax error in line %d: closed parenthesis expected\n", lineNum);
					}
				}
				else{
					perror("Syntax error in line %d: opened parenthesis expected\n", lineNum);
				}
			}
			if (id == CLOSE_BRAC){
				id = lex(fp);
			}
			else{
				perror("Syntax error in line %d: closed bracket expected\n", lineNum);
			}
		}
		else{
			perror("Syntax error in line %d: opened bracket expected\n", lineNum);
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
					perror("Syntax error in line %d: closed parenthesis expected\n", lineNum);
				}
			}
			else{
				perror("Syntax error in line %d: opened parenthesis expected\n", lineNum);
			}
		}
		else{
			perror("Syntax error in line %d: ID expected\n", lineNum);
		}
	}
	else{
		perror("Syntax error in line %d: \"call\" keyword expected\n", lineNum);
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
			perror("Syntax error in line %d: expected closed parenthesis\n", lineNum);
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
			perror("Syntax error in line %d: expected ID after \"inout\"\n", lineNum);
		}
	}
	else if (id == COPY){
		id = lex(fp);
		if (id == ID){
			id = lex(fp);
		}
		else{
			perror("Syntax error in line %d: expected ID after \"copy\"\n", lineNum);
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
			conditon();
			if (id == CSQUARE_BRAC){
				id = lex(fp);
			}
			else{
				perror("Syntax error in line %d: expected ]\n", lineNum);
			}
		}
		else{
			perror("Syntax error in line %d: expected [\n", lineNum);
		}
	}
	else if (id == OSQUARE_BRAC){
		conditon();
		if (id == CSQUARE_BRAC){
			id = lex(fp);
		}
		else{
			perror("Syntax error in line %d: expected ]\n", lineNum);
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
		addoper();
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
	if (id == CONSTANTID){
		id = lex(fp);
	}
	else if (id == OPEN_PAR){
		expression();
		if (id == CLOSE_PAR){
			id = lex(fp);
		}
		else{
			perror("Syntax error in line %d: expected )\n", lineNum);
		}
	}
	else if (id == ID){
		idtail();
	}
	else{
		perror("Syntax error in line %d: contstant | () | ID\n", lineNum);
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







/*int main(int argc, char *argv[]){

	FILE *fp;

	int token;
	if (argc < 2){
		exit(1);
	}
	printf("%s",argv[1]);

	fp = fopen(argv[1], "r");
	token = lex(fp);
	printf("%d",token);
	printf("	/t ");
	
	while (token >= 0){
		token = lex(fp);		
		printf("  %d",token);
		printf("\t ");

	}
	fclose(fp);

}*/
