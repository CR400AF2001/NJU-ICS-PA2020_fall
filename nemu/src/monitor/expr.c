#include "nemu.h"
#include "cpu/reg.h"
#include "memory/memory.h"

#include <stdlib.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

extern uint32_t look_up_symtab(char *sym, bool *success);

enum
{
	NOTYPE = 256,
	EQ,
	NUM,
	HEX,
	REG,
	SYMB,
	ADD,
	SUB,
	MUL,
	DIV,
	LEFT,
	RIGHT,
	MINUS,
	POINT,
	EQUAL,
	NOTEQUAL,
	AND,
	OR,
	NOT

	/* TODO: Add more token types */

};

static struct rule
{
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{"0[xX][0-9a-fA-F]{8}", HEX},
	{"[0-9]+", NUM},
	{"\\$[a-z]+", REG},
	{"[a-zA-Z0-9_]+", SYMB},
	{" +", NOTYPE}, // white space
	{"\\+", ADD},
	{"-", SUB},
	{"\\*", MUL},
	{"/", DIV},
	{"\\(", LEFT},
	{"\\)", RIGHT},
	{"==", EQUAL},
	{"!=", NOTEQUAL},
	{"&&", AND},
	{"[\\|]{2}", OR},
	{"!", NOT},
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]))

static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
	int i;
	char error_msg[128];
	int ret;

	for (i = 0; i < NR_REGEX; i++)
	{
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if (ret != 0)
		{
			regerror(ret, &re[i], error_msg, 128);
			assert(ret != 0);
		}
	}
}

typedef struct token
{
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e)
{
	int position = 0;
	int i;
	regmatch_t pmatch;

	nr_token = 0;

	while (e[position] != '\0')
	{
		/* Try all rules one by one. */
		for (i = 0; i < NR_REGEX; i++)
		{
			if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0)
			{
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				printf("match regex[%d] at position %d with len %d: %.*s\n", i, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */

				switch (rules[i].token_type)
				{
				    case NOTYPE:
				        break;
				    case NUM:
				        tokens[nr_token].type = rules[i].token_type;
				        int num = 0;
				        for(int j = 0; j < substr_len; ++j){
				            num = num * 10 + (int) (e[position - substr_len + j] - '0');
				        }
				        uint32_t *puint = (uint32_t*) tokens[nr_token].str;
				        *puint = (uint32_t) num;
					    nr_token++;
					    break;
					case HEX:
				        tokens[nr_token].type = rules[i].token_type;
				        for(int j = 0; j < substr_len; ++j){
				            tokens[nr_token].str[j] = e[position - substr_len + j];
				        }
				        for(int j = substr_len; j < 32; ++j){
				            tokens[nr_token].str[j] = '\0';
				        }
					    nr_token++;
					    break;
					case REG:
				        tokens[nr_token].type = rules[i].token_type;
				        for(int j = 0; j < substr_len; ++j){
				            tokens[nr_token].str[j] = e[position - substr_len + j];
				        }
				        for(int j = substr_len; j < 32; ++j){
				            tokens[nr_token].str[j] = '\0';
				        }
					    nr_token++;
					    break;
					case SYMB:
				        tokens[nr_token].type = rules[i].token_type;
				        for(int j = 0; j < substr_len; ++j){
				            tokens[nr_token].str[j] = e[position - substr_len + j];
				        }
				        for(int j = substr_len; j < 32; ++j){
				            tokens[nr_token].str[j] = '\0';
				        }
					    nr_token++;
					    break;
				    default:
					    tokens[nr_token].type = rules[i].token_type;
					    nr_token++;
				}

				break;
			}
		}

		if (i == NR_REGEX)
		{
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true;
}

bool check_parentheses(int s, int e, bool *success){
    int left = 0;
    int right = 0;
    if(tokens[s].type != LEFT || tokens[e].type != RIGHT){
        return false;
    }
    for(int i = s + 1; i <= e - 1; ++i){
	    if(tokens[i].type == LEFT){
	        left++;
	    }
	    else if(tokens[i].type == RIGHT){
	        right++;
	    }
	    if(left < right){
	        return false;
	    }
	}
	return true;
}

uint32_t eval(int s, int e, bool *success){
    if(s > e || *success == false) {
        *success = false;
        return 0;
    }
    else if(s == e) {
        if(tokens[s].type == NUM){
            uint32_t *num = (uint32_t*) tokens[s].str;
            uint32_t result = *num;
            return result;
        }
        else if(tokens[s].type == HEX){
            unsigned int result = 0;
            for(int i = 2; i < 10; ++i){
                int temp = 0;
                if(tokens[s].str[i] >= '0' && tokens[s].str[i] <= '9'){
                    temp = (int) (tokens[s].str[i] - '0');
                }
                else if(tokens[s].str[i] >= 'a' && tokens[s].str[i] <= 'f'){
                    temp = (int) (tokens[s].str[i] - 'a') + 10;
                }
                else if(tokens[s].str[i] >= 'A' && tokens[s].str[i] <= 'F'){
                    temp = (int) (tokens[s].str[i] - 'A') + 10;
                }
                result = 16 * result + temp;
            }
            return result;
        }
        else if(tokens[s].type == REG){
            return get_reg_val(tokens[s].str + 1, success);
        }
        else if(tokens[s].type == SYMB){
            return look_up_symtab(tokens[s].str, success);
        }
    }
    else if(s == e - 1) { 
        if(tokens[s].type == NOT){
            return !eval(e, e, success);
        }
        else if(tokens[s].type == MINUS){
            return -1 * eval(s + 1, e, success); 
        }
        else if(tokens[s].type == POINT){
            return vaddr_read(eval(e, e, success), 0x0, 4);
        }
    }
    else if(check_parentheses(s, e, success) == true) {
        return eval(s + 1, e - 1, success); 
    }
    else {
        int op = 0;
        int minPriority = 5;
        int curPriority = 0;
        int brackets = 0;
        for(int i = s; i <= e; ++i){
            if(tokens[i].type == MUL || tokens[i].type == DIV){
	            if(brackets == 0){
	                curPriority = 4;
	            }
	            else{
	                curPriority = 5;
	            }
	        }
	        else if(tokens[i].type == ADD || tokens[i].type == SUB){
	            if(brackets == 0){
	                curPriority = 3;
	            }
	            else{
	                curPriority = 5;
	            }
	        }
	        else if(tokens[i].type == EQUAL || tokens[i].type == NOTEQUAL){
	            if(brackets == 0){
	                curPriority = 2;
	            }
	            else{
	                curPriority = 5;
	            }
	        }
	        else if(tokens[i].type == AND || tokens[i].type == OR){
	            if(brackets == 0){
	                curPriority = 1;
	            }
	            else{
	                curPriority = 5;
	            }
	        }
	        else if(tokens[i].type == LEFT){
	            brackets++;
	            continue;
	        }
	        else if(tokens[i].type == RIGHT){
	            brackets--;
	            continue;
	        }
	        else{
	            continue;
	        }
	        if(curPriority <= minPriority){
	            minPriority = curPriority;
	            op = i;
	        }
	    }
        uint32_t val1 = eval(s, op - 1, success);
        uint32_t val2 = eval(op + 1, e, success);
        switch(tokens[op].type) {
            case ADD: return val1 + val2;
            case SUB: return val1 - val2;
            case MUL: return val1 * val2;
            case DIV: return val1 / val2;
            case EQUAL: return val1 == val2;
            case NOTEQUAL: return val1 != val2;
            case AND: return val1 && val2;
            case OR: return val1 || val2;
            default: 
                assert(0);
        }
    }
    return 0;
}

uint32_t expr(char *e, bool *success)
{
	if (!make_token(e))
	{
		*success = false;
		return 0;
	}
	for(int i = 0; i < nr_token; ++i){
	    if(tokens[i].type == SUB){
	        if(i == 0){
	            tokens[i].type = MINUS;
	        }
	        else if(tokens[i - 1].type == ADD || tokens[i - 1].type == SUB || tokens[i - 1].type == MUL || tokens[i - 1].type == DIV || tokens[i - 1].type == LEFT || tokens[i - 1].type == MINUS || tokens[i - 1].type == POINT){
	            tokens[i].type = MINUS;
	        }
	    }
	    else if(tokens[i].type == MUL){
	        if(i == 0){
	            tokens[i].type = POINT;
	        }
	        else if(i == nr_token - 1){
	            *success = false;
		        return 0;
	        }
	        else if(tokens[i - 1].type == ADD || tokens[i - 1].type == SUB || tokens[i - 1].type == MUL || tokens[i - 1].type == DIV || tokens[i - 1].type == LEFT || tokens[i - 1].type == MINUS || tokens[i - 1].type == POINT){
	            tokens[i].type = POINT;
	        }
	    }
	}
	*success = true;
	return eval(0, nr_token - 1, success);
}
