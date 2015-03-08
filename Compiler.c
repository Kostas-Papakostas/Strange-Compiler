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
		id = IDget(lexis);
		if (id == ID){
			id IDget(lexis);
			programBlock();
		}
		else{
			error("Syntax error:Program name expected");
		}
	}
	else{
		error("Syntax error: word \" program \" expected");
	}
}


int main(int argc, char *argv[]){

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

}
