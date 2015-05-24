// Rantoglou-Melas Spyridon AM=2342
//Papakostas Kostantinos AM=2399
enum tokens{
	EQUALS, PLUS, MULTIPLY, DIVIDE, MINUS, LESS_THAN, GREATER_THAN
	, SEMI_COL, COMMA, OPEN_PAR, CLOSE_PAR, OPEN_BRAC, CLOSE_BRAC, AND, OR, NOT, IN
	, IF, ELSE, DO, WHILE, FORCASE, INCASE, WHEN, PRINT, RETURN, CALL, FUNCTION, LESS_EQ, GREATER_EQ, ID, NUMERIC, EXIT,
	PROCEDURE, INOUT, PROGRAM, COPY, OSQUARE_BRAC, CSQUARE_BRAC, DECL_EQUALS, DIFFERENT, DECLARE, ENDDECLARE, CONSTANTID,REF
};
enum states{
	state0, state1, state2, state3
	, state4, state5, state6, OK, ERROR
};

void programtk();
void block();
void declarations();
void varlist();
void subprogram();
void func();
void funcbody();
void formalpars();
void formalparlist();
void formalparitem();
void sequence();
void brackets_seq();
void brack_or_stat();
void statement();
void assignment_stat();
void if_stat();
void elsepart();
void do_while_stat();
void exit_stat();
void return_stat();
void print_stat();
void while_stat();
void incase_stat();
void forcase_stat();
void call_stat();
void actualpars();
void actualparlist();
void actualparitem();
void condition(struct list, struct list);
void boolterm(struct list, struct list);
void boolfactor(struct list, struct list);
void expression(char);
void term(char);
void factor(char);
void idtail();
void relational_oper();
void add_oper();
void mul_oper();
void optional_sign();
int nextquad();
struct quad genquad(char, char, char, int);
struct list* makelist(int);
struct list* mergelist(struct list, struct list);
void backpatch(struct list, int);
void printlist(struct list);
struct list * emptylist();
const char * newTemp();
