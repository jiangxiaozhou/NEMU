#include "common.h"
#include "cpu/reg.h"
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256,LOG_OR=0,LOG_AND,OR,XOR,AND,EQ,NOT_EQ,LESS,MORE,NOT_LESS,
	NOT_MORE,LEFT_SHIFT,RIGHT_SHIFT,ADD,SUB,MUL,DIVI,MOD,DEREF,NOT,NEG,LOG_NOT,
	LEFT_PAR,RIGHT_PAR,NUM,HEX_NUM,REG,VAR


	/* TODO: Add more token types */

};
int isp[]={1,2,3,4,5,6,6,7,7,7,7,8,8,9,9,10,10,10,11,11,11,11,12,12,13,13,13,13};
int32_t char2x(char*);
uint32_t swaddr_read(swaddr_t,size_t);
CPU_state cpu;
swaddr_t find_name(char*);

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +", NOTYPE},				// white space
	{"\\+", ADD},					// plus
	{"\\*", MUL},                   // mul
	{"\\/", DIVI},                    // divi
	{"-",SUB},                       // sub
	{"\\(",LEFT_PAR},                     // left_parentheses
	{"\\)",RIGHT_PAR},                     // right_parentheses
	{"\\$ *e(ax|cx|dx|bx|sp|bp|si|di|ip)",REG},   //reg
	{"<{2}",LEFT_SHIFT},             //left shift
	{"<=",NOT_MORE},                    //not_more
	{"<{1}",LESS},                        //less
	{">{2}",RIGHT_SHIFT},           //right shift
	{">=",NOT_LESS},                   //not_less
	{">{1}",MORE},                       // more
	{"%",MOD},                       //mod
	{"\\^",XOR},                     //xor
	{"~",NOT},                      //not
	{"\\|{2}",LOG_OR},               //log_or
	{"\\|{1}",OR},                  //or
	{"&{2}",LOG_AND},               //log_and
	{"&{1}",AND},                    //and
	{"!=",NOT_EQ},                   //not_eq
	{"!",LOG_NOT},                   //log_not
	{"==", EQ},						// equal
	{"0x[a-fA-F0-9]{1,}", HEX_NUM},          // hex_num_
	{"[0-9]{1,}", NUM},               // number
    {"[a-zA-Z0-9_]+", VAR}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			test(0, "regex compilation failed: %s\n%s\n", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;
				//Log("match regex[%d] at position %d with len %d: %.*s", i, position, substr_len, substr_len, substr_start);
				//fflush(stdout);


				position += substr_len;
                
                int j=0; 

				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */
				if(rules[i].token_type == NOTYPE)
					break;
			    else if(rules[i].token_type == NUM || rules[i].token_type == VAR)
				{
					if(substr_len > 32) assert(0);
                    for(;j<=31;j++)
						tokens[nr_token].str[j] = '\0';
					strncpy(tokens[nr_token].str, substr_start, substr_len);
					//Log("%s",tokens[nr_token].str);
					tokens[nr_token].type=rules[i].token_type;
					nr_token++;
					break;
				}
			    else if(rules[i].token_type == HEX_NUM)
				{
					if(substr_len > 34) assert(0);
					for(;j<=31;j++)
						tokens[nr_token].str[j] = '\0';
                    strncpy(tokens[nr_token].str, substr_start+2, substr_len-2);
					tokens[nr_token].type=rules[i].token_type;
					nr_token++;
					break;
				}
				else if(rules[i].token_type >= LOG_OR && rules[i].token_type <= RIGHT_PAR)
				{
					tokens[nr_token].type=rules[i].token_type;
					nr_token++;
					break;
				}
			    else if(rules[i].token_type == REG)
				{
					tokens[nr_token].type=rules[i].token_type;
                 	for(;j<=31;j++)
						tokens[nr_token].str[j] = '\0';
					strncpy(tokens[nr_token].str, substr_start, substr_len);
                    nr_token++;
					break;
				}
				else assert(0);
				
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

int32_t char2num(char* p){
	int32_t sum = 0;
	while(*p != '\0')
	{
		int32_t n;
		n = *p - '0';
		sum = sum *10 + n;
		p++;
	}
	return sum;
}

int find_op(int p,int q){
	int result = p;
	int temp = p;
	while(temp<=q){
		if(tokens[temp].type < DEREF || tokens[temp].type> LOG_NOT)
			break;
		temp++;
	}
	if (temp == q) return p;
	//Log("HEHE");
	while(p<=q)
	{
		if(tokens[p].type >= RIGHT_PAR)   p++;
		else if(tokens[p].type == LEFT_PAR)
			while(tokens[p].type != RIGHT_PAR){
				p++;
			}
		else if(isp[tokens[p].type] <= isp[tokens[result].type]) {
			result = p;
		    p++;
     	}
		else p++;
	}
	//Log("op lacate:%d\n",result);
	return result;
}

bool check_parentheses(int p,int q)
{
	int j=0;
	if(tokens[p].type != LEFT_PAR || tokens[q].type != RIGHT_PAR) 
	{
		//Log("par_false\n");
		return false;
	}
	for(;p<=q;p++){
		if(tokens[p].type == LEFT_PAR) j++;
		if(tokens[p].type == RIGHT_PAR) j--;
		if(j<=0 && p != q) return false;
	}
	if(j == 0){
	    //Log("par_true\n");
		return true;
	}
	else {
		//Log("par_false\n");
		return false;
	}
}

int32_t eval(int p,int q){
	//Log("%d %d\n",p,q);
	if(p > q){
		printf("Bad expression.");
		assert(0);
	}
	else if(p == q){
		if(tokens[p].type == NUM)
		    return char2num(tokens[p].str);
		else if(tokens[p].type == HEX_NUM)
			return char2x(tokens[p].str);
		else if(tokens[p].type == VAR)
		{
			//Log("lala");
			return find_name(tokens[p].str);
		}
		else if(tokens[p].type == REG){ 
			if(strcmp(tokens[p].str, "$eax") == 0)
				return cpu.eax;
			else if(strcmp(tokens[p].str, "$ecx") == 0)
		     	return cpu.ecx;
			else if(strcmp(tokens[p].str, "$edx") == 0)
				return cpu.edx;
			else if(strcmp(tokens[p].str, "$ebx") == 0)
				return cpu.ebx;
			else if(strcmp(tokens[p].str, "$esp") == 0)
				return cpu.esp;
			else if(strcmp(tokens[p].str, "$ebp") == 0)
				return cpu.ebp;
			else if(strcmp(tokens[p].str, "$esi") == 0)
				return cpu.esi;
			else if(strcmp(tokens[p].str, "$edi") == 0)
				return cpu.edi;
			else if(strcmp(tokens[p].str, "$eip") == 0)
				return cpu.eip;
			else assert(0);
		}
		else assert(0);
	}
	else if(check_parentheses(p,q) == true){
		return eval(p+1,q-1);
	}
	else {
		int op;
		int32_t val1=0,val2=0;
		op = find_op(p,q);
		if(tokens[op].type == NOT || tokens[op].type == LOG_NOT || tokens[op].type == DEREF || tokens[op].type == NEG)
			val2 = eval(op+1,q);
		else
		{
		    val1 = eval(p, op-1);
            val2 = eval(op+1, q);
		}
		switch(tokens[op].type) {
			case ADD: return val1 + val2;break;
			case SUB: return val1 - val2;break;
			case MUL: return val1 * val2;break;
			case LOG_OR: return val1 || val2;break;
			case LOG_AND:return val1 && val2;break;
			case XOR: return val1 ^ val2;break;
			case OR:return val1 | val2;break;
			case NEG:return -eval(op+1,q);break;
			case AND:return val1 & val2;break;
			case NOT:return ~val2;break;
			case LOG_NOT:return !val2;break;
			case LESS:return val1 < val2 ;break;
			case MORE:return val1 > val2;break;
			case NOT_MORE:return val1 <= val2 ; break;
			case NOT_LESS:return val1 >= val2;break;
			case LEFT_SHIFT:return val1 << val2;break;
			case RIGHT_SHIFT:return val1 >> val2;break;
			case EQ:return val1 == val2;break;
			case NOT_EQ:return val1 != val2;break;
			case DIVI:
					  if(val2 == 0) {
						  printf("Divided by 0!");
						  assert(0);
					  } else return val1 / val2;
					  break;
			case MOD:if(val2 == 0) {
						 printf("Divided by 0!");
						 assert(0);
					 } else return val1 % val2;
					 break;
			case DEREF:return swaddr_read(val2,4);break;	
			default:assert(0);
		}
	}
	return 0;
}



int32_t expr(char *e, bool *success) {
	int i;
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	for(i = 0;i < nr_token;i++){
		if(tokens[i].type == MUL && (i == 0 || (tokens[i-1].type >= 0 && tokens[i-1].type <= 22) || tokens[i-1].type == LEFT_PAR))
			tokens[i].type=DEREF;
		  
		if(tokens[i].type == SUB && (i == 0 || (tokens[i-1].type >= 0 && tokens[i-1].type <= 22) || tokens[i-1].type == LEFT_PAR))
			tokens[i].type=NEG;
	}

	/* TODO: Implement code to evaluate the expression. */
	return eval(0, nr_token-1);
}

