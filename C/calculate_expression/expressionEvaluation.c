/**
 * expressionEvaluation.c
 * @Author   shenzhuo
 * @DateTime 2018-07-31T15:57:46+0800
 */

#include "expressionEvaluation.h"
#include <ctype.h>		//	isdigit() isspace()
#include <string.h>		//	strlen()


/**
 * the status of the program
 * INIT 	: 	initialization
 * OPERATOR :	just getting an operator
 * OPERAND 	:	just getting an operand
 */
#define INIT 	 0
#define OPERATOR 1
#define OPERAND  2

/**
 * get a complete number from infixExpression and put it into postfixExpression
 * @Author   shenzhuo
 * @DateTime 2018-07-31T15:57:46+0800
 * @param    infixExpression          infixExpression
 * @param    postfixExpression        postfixExpression
 * @param    post_index               current index of postfixExpression
 * @param    infix_index              current index of infixExpression
 * @return                            the point of infixExpression after getting a number
 */
char *getNumber(char *infixExpression, char postfixExpression[], int *post_index, int *infix_index);


/**
 * find the left brace which is matched with current right brace and pop the value
 * @Author   shenzhuo
 * @DateTime 2018-07-31T16:03:15+0800
 * @param    s                        the stack of operators
 * @param    infixExpression          infixExpression
 * @param    postfixExpression        postfixExpression
 * @param    post_index               postfixExpression
 * @return                            the point of infixExpression after getting a number
 */
char *getLeftBrace(SequentialStackChar *s, char *infixExpression, char postfixExpression[], int *post_index);


/**
 * pop the prioritized operators
 * @Author   shenzhuo
 * @DateTime 2018-07-31T16:08:28+0800
 * @param    postfixExpression        [description]
 * @param    post_index               [description]
 * @param    s                        [description]
 * @param    c                        current operator
 */
void PopPriority(char postfixExpression[], int *post_index, SequentialStackChar *s, char c);


/**
 * check if ch is an operator
 * @Author   shenzhuo
 * @DateTime 2018-07-31T16:10:21+0800
 * @param    ch                       [description]
 * @return                            [description]
 */
int isOperator(char ch);


/**
 * get the next char that is not a space character
 * @Author   shenzhuo
 * @DateTime 2018-07-31T16:11:16+0800
 * @param    infixExpression          [description]
 * @param    infixExpressionHead      [description]
 * @return                            [description]
 */
char getPrevChar(char *infixExpression, char *infixExpressionHead);


/**
 * get the next char that is not a space character
 * @Author   shenzhuo
 * @DateTime 2018-07-31T16:11:16+0800
 * @param    infixExpression          [description]
 * @param    infixExpressionHead      [description]
 * @return                            [description]
 */
char getNextChar(char *infixExpression, int *next_index);


/**
 * print the position in the expression
 * @Author   shenzhuo
 * @DateTime 2018-08-10T15:25:31+0800
 * @param    infixExpression          [description]
 * @param    t1                       [description]
 * @param    t2                       [description]
 * @param    count                    [description]
 */
void printErrorIndex(char *infixExpression, int t1, int t2, int count);


/**
 * get the index of next char
 * @Author   shenzhuo
 * @DateTime 2018-08-10T15:26:03+0800
 * @param    infixExpressionHead      [description]
 * @param    infixExpression          [description]
 * @param    curr_index               [description]
 * @return                            [description]
 */
int getPrevCharIndex(char *infixExpressionHead, char *infixExpression, int curr_index);


/**
 * try to find matched right brace
 * @Author   shenzhuo
 * @DateTime 2018-08-10T15:26:08+0800
 * @param    infixExpression          [description]
 * @return                            [description]
 */
int findRightBrace(char *infixExpression);


/**
 * get the last char in the infix expression
 * @Author   shenzhuo
 * @DateTime 2018-08-10T15:03:33+0800
 * @param    infixExpression          [description]
 * @param    curr_index               [description]
 * @return                            [description]
 */
int getLastCharIndex(char *infixExpression, int curr_index);


/**
 * infixExpression to PostfixExpression
 * @Author   shenzhuo
 * @DateTime 2018-07-31T16:12:57+0800
 * @param    infixExpression          [description]
 * @param    postfixExpression        [description]
 * @return                            [description]
 */
State infixToPostfix(char *infixExpression,char postfixExpression[]) {
	char *infixExpressionHead = infixExpression;				/*	save the point of infixExpression to infixExpressionHead	*/
	int status = INIT;											/*	set the status to INIT 	*/
	int infix_index = 0;
	int post_index = 0;
	SequentialStackChar *s = (SequentialStackChar *)malloc(sizeof(SequentialStackChar));			/*	initialization stack 	*/
	if (s == NULL){
		printf("Memory allocation failed.\n");
		return FAILED;
	}
	InitStack(s);
	while (*infixExpression != '\0') {							/*	for every char in infixExpression */
		char ch = *infixExpression;
		if (isdigit(ch)) {
			if (status == OPERAND)	{							/*	the status should be OPERATOR, it means just getting a number 	*/
				int prev_index = getPrevCharIndex(infixExpressionHead, infixExpression-1, infix_index-1);
				char prev_ch = getPrevChar(infixExpression, infixExpressionHead);
				if (')' == prev_ch)								/*	a number immediately follows ')' in the expression.*/
					printf("Wrong expression!! No operator between a number and ')'\n");
				else											/*	two consecutive operands */
					printf("Wrong expression!! No operator between two numbers.\n");
				if (prev_index != -1)							/*	show the position of error */
					printErrorIndex(infixExpressionHead, infix_index, prev_index, 2);
				return FAILED;
			}
			status = OPERAND;									/*	update the status and get the complete number */
			infixExpression = getNumber(infixExpression, postfixExpression, &post_index, &infix_index);
		} else {												/*	get the indexes and values of the prev_char and the next_char of ch */
			int prev_index = getPrevCharIndex(infixExpressionHead, infixExpression-1, infix_index-1);
			char prev_ch = getPrevChar(infixExpression, infixExpressionHead);
			int next_index = infix_index;
			char next_ch = getNextChar(infixExpression, &next_index);
			switch (ch) {
				case ' ':
				case '\t':	break;								/*	skip the space */
				case '(':	if ( (isdigit(prev_ch)) ) {			/* 	the '(' immediately follows a number */
								printf("Wrong expression!! No operator between a number and '('.\n");
								printErrorIndex(infixExpressionHead, infix_index, prev_index, 2);
								DestroyStack(s);
								return FAILED;
							} else if (')'== prev_ch) {			/* 	the '(' immediately follows '(' */
								printf("Wrong expression!! No operator between ')' and '('.\n");
								printErrorIndex(infixExpressionHead, infix_index, prev_index, 2);
								DestroyStack(s);
								return FAILED;
							}
							if (!isdigit(next_ch)) {			/*	check the next char of '(' */	
								if ('*' == next_ch || '/' == next_ch) {
									/* 	the '*' or the '/' immediately follows '(' */
									printf("Wrong expression!! No operand between '(' and '%c'.\n", next_ch);
									printErrorIndex(infixExpressionHead, infix_index, next_index, 2);
									DestroyStack(s);
									return FAILED;
								} else if (next_ch == '-' || next_ch == '+') {		
									/* may be the unary operator or the lack of operand here. */
									char tmp = *(infixExpressionHead+next_index+1);
									if (!isdigit(tmp)) {
										if (isspace(tmp)){
											printf("Wrong expression!! A space follows a unary '%c'.\n", next_ch);
											printErrorIndex(infixExpressionHead, next_index, next_index, 1);
											DestroyStack(s);
											return FAILED;
										}
										else if (tmp != '('){
											printf("Wrong expression!! No operand between '(' and '%c'.\n", next_ch);
											printErrorIndex(infixExpressionHead, infix_index, next_index, 2);
											DestroyStack(s);
											return FAILED;
										}
									}
								}
							}
							/* check the matched ')' after current '(' */
							if (!findRightBrace(infixExpression)) {	
								printf("Wrong expression!! No matched ')' after '('.\n");
								printErrorIndex(infixExpressionHead, infix_index, 1, 1);
								DestroyStack(s);
								return FAILED;
							}
							/* No abnormalities found, push the char */
							if(!Push(s, ch)){	
								return FAILED;
							}
							break;
				case '+':
				case '-':	/* check the prev_ch of ch */
							if (status == OPERATOR && isOperator(prev_ch)) {
								/* two consecutive operators */
								printf("Wrong expression!! Opreator '%c' immediately follows '%c' in the expression.\n", ch, prev_ch);
								printErrorIndex(infixExpressionHead, infix_index, prev_index, 2);
								return FAILED;
							}
							if (prev_ch == '(' || prev_ch == '!'|| prev_ch == '\0'){
								/**
								 * '+/-' may be a unary operator if it is the first not space character 
								 * in the expression or the first non space character after '('. 
								 */
								next_ch = *(infixExpression+1);
								if (isdigit(next_ch) || '(' == next_ch) {
									/* a digit immediately follows '+/-', which shows '+/-' is a unary operator */
									if (ch == '-')	{
										/* '$' represents unary '-' */
										if(!Push(s, '$'))	{
											return FAILED;
										}		
									}
									else {
										/* '@' represents unary '+' */
										if(!Push(s, '@'))	{
											return FAILED;
										}
									}
									break;
								}else if(isOperator(next_ch)){
									/* two consecutive operators */
									printf("Wrong expression!! Operator '%c' immediately follows '%c' in the expression.\n", next_ch, ch);
									printErrorIndex(infixExpressionHead, next_index, infix_index, 2);
									return FAILED;
								}else if( (next_ch == ' ' || next_ch == '\0') && ch == '+'){
									/* No space is available after the unit operator is specified */
									printf("Wrong expression!! A space follows a unary plus.\n");
									printErrorIndex(infixExpressionHead, infix_index, 0, 1);
									return FAILED;
								}else if((next_ch == ' ' || next_ch == '\0') && (ch == '-')) {
									printf("Wrong expression!! A space follows a unary minus.\n");
									printErrorIndex(infixExpressionHead, infix_index, 0, 1);
									return FAILED;
								}
							}else {
								/* pop the operators with the same priority or high priority */
								PopPriority(postfixExpression, &post_index, s, ch);
								if (!Push(s, ch)) {
									return FAILED;
								}
								status = OPERATOR;
								break;
							}
				case '*' :
				case '/' :	/* '*' or '/' can not appear in the first */
							if (status == INIT || '!' == prev_ch) {
								printf("Wrong expression!! No operand before the '%c'.\n", ch);
								printErrorIndex(infixExpressionHead,infix_index,-1,1);
								DestroyStack(s);
								return FAILED;
							}
							/* two consecutive operators */
							if (status == OPERATOR){
								printf("Wrong expression!! Opreator '%c' immediately follows '%c' in the expression.\n", ch, prev_ch);
								printErrorIndex(infixExpressionHead, infix_index, prev_index, 2);
								DestroyStack(s);
								return FAILED;
							}
							/* pop the operators with the same priority or high priority */
							PopPriority(postfixExpression, &post_index, s, ch);
							if(!Push(s, ch)) {
								return FAILED;
							}	
							status = OPERATOR;
							break;
				case ')' :	if ('(' == prev_ch) {
								/* the prev_ch of ')' is '('. no operand in braces */
								printf("Wrong expression!! No operand in braces.\n");
								printErrorIndex(infixExpressionHead, infix_index, prev_index, 2);
								DestroyStack(s);
								return FAILED;
							}
							if (isOperator(prev_ch)) {
								/* the prev_ch of ')' is an operator. an operator immediately follows ')' */
								printf("Wrong expression!! No operand between the '%c' and ')'.\n", prev_ch);
								printErrorIndex(infixExpressionHead, prev_index, infix_index, 2);
								DestroyStack(s);
								return FAILED;
							}
							/* find the matched '(' before ch */
							infixExpression = getLeftBrace(s, infixExpression, postfixExpression, &post_index);
							if (infixExpression == NULL)	{
								printf("Wrong expression. No matched '(' before ')'\n");
								printErrorIndex(infixExpressionHead, infix_index, 1, 1);
								DestroyStack(s);
								return FAILED;
							}
							break;
				default: 	if (ch == '\b')
								printf("Wrong expression!! Illegal character '\\b' in the expression.\n"); 
							else if (ch == '\n')
								printf("Wrong expression!! Illegal character '\\n' in the expression.\n"); 
							else 
								printf("Wrong expression!! Illegal character '%c' in the expression.\n", ch); 
							DestroyStack(s); 
							printErrorIndex(infixExpressionHead, infix_index, 1, 1);
							return FAILED;
			}
			infixExpression++;
		}
		infix_index++;
	}
	
	/* check the rest of char in the stack */
	if (status == OPERATOR) {
		/* status should be OPERAND, something wrong happened */
		printf("Wrong expression!! No operand after the '%c' in the expression.\n", s->data[s->top]);
		int err_index = getLastCharIndex(infixExpressionHead, infix_index);
		printErrorIndex(infixExpressionHead, err_index, -1, 1);
		DestroyStack(s);
		return FAILED;
	}

	/* get the rest of char in the stack */
	while (StackEmpty(s)!=1) {
		char ch;
		Pop(s, &ch);
		/* ch is not supposed to '(' */
		if(ch == '('){
			printf("Wrong expression. No matched ')' after '('\n");
			DestroyStack(s);
			return FAILED;
		}
		postfixExpression[post_index++] = ch;
	}
	postfixExpression[post_index++] = '\0';
	DestroyStack(s);
	return SUCCEEDED;
}


/**
 * [computeValueFromPostfix description]
 * @Author   shenzhuo
 * @DateTime 2018-07-31T16:31:30+0800
 * @param    postfixExpression        [description]
 * @param    value                    [description]
 * @return                            [description]
 */
State computeValueFromPostfix(char *postfixExpression, double *value) {
	/* check whether the postfixExpresion is empty.*/
	if(strlen(postfixExpression)<=0)	{
		printf("Wrong expression!! No operands in the expression!\n");
		return FAILED;
	}
	*value = 0;
	double t1, t2;
    SequentialStackDouble* stack = (SequentialStackDouble *)malloc(sizeof(SequentialStackDouble));
    if (stack == NULL)	{
    	printf("Memory allocation failed.\n");
    	return FAILED;
    }
    InitStackDouble(stack);

    while(*postfixExpression != '\0') { 	
        char ch = *postfixExpression;
        if(isdigit(ch)) {
            double i = ch - '0';
            /* get the complete number */
            while(isdigit(*(postfixExpression+1))){
                postfixExpression++;
                i = i * 10 + (*postfixExpression) - '0';
            }
            /* push every number */
            if (!PushDouble(stack, i)) {
				return FAILED;
            }
        } else if(ch != '_' && ch != '@'){
        	/* skip space and '@' */
        	if (ch == '$'){
        		if (!PopDouble(stack, &t1)) {
        			return FAILED;
        		}
        		if (!PushDouble(stack, -1*t1)) {
        			return FAILED;
        		}
        	}else {
        		if (!PopDouble(stack, &t1)) {
        			return FAILED;
        		}
	            if (!PopDouble(stack, &t2)) {
	            	return FAILED;
	            }
	            switch(ch) {
	            	case '+': *value = t1 + t2;	break;
	            	case '-': *value = t2 - t1; break;
	            	case '*': *value = t2 * t1; break;
	            	case '/': if(t1 == 0)	{	printf("Wrong expression!! Zero divisor!\n");	return FAILED;}
	            			 *value = t2 / t1;	break;
	            	default: printf("error! \n"); return FAILED;
	            }
	            if (!PushDouble(stack,*value)) {
	            	return FAILED;
	            }
        	}
        }
        postfixExpression++;
    }
    if(stack->top == 0){
        if (!PopDouble(stack, value)) {
        	return FAILED;
        }
    } else {
    	printf("Something wrong happened when we are poping value from stack.\n");
		DestroyStackDouble(stack);
    	return FAILED;
    }

    DestroyStackDouble(stack);
    return SUCCEEDED;
}


int getLastCharIndex(char *infixExpression, int curr_index) {
	char ch;
	while(curr_index >= 0) {
		ch = *(infixExpression + curr_index);
		if (ch != '\0' && !isspace(ch))	
			return curr_index;
		curr_index--;
	}
	return -1;
}


int findRightBrace(char *infixExpression) {
	infixExpression++;
	int count = 1;
	char ch;
	while('\0' != (ch = *infixExpression)) {
		if (ch == '(')	count++;
		else if (ch == ')')	count--;
		if (count == 0)	return 1;
		infixExpression++;
	}
	return 0;
}



char *getNumber(char *infixExpression, char postfixExpression[], int *post_index, int *infix_index) {
	char ch;
	while(isdigit(ch = *infixExpression)) {
		postfixExpression[*post_index] = ch;
		++(*post_index);
		++infixExpression;
		++(*infix_index);
	}
	--(*infix_index);
	postfixExpression[*post_index] = '_';
	++(*post_index);
	return infixExpression;
}


char *getLeftBrace(SequentialStackChar *s, char *infixExpression, char postfixExpression[], int *post_index) {
	char ch;
	while (!StackEmpty(s) && Pop(s, &ch)){
		if (ch != '(') {
			postfixExpression[*post_index] = ch;
			(*post_index)++;
		} else {
			return infixExpression;;
		}
	}
	return NULL;
}


void PopPriority(char postfixExpression[], int *post_index, SequentialStackChar *s, char c) {
	char ch;
	if (c == '+' || c == '-') {
		while(!StackEmpty(s)) {
			GetTop(s, &ch);
			if (ch == '*' || ch == '/' || ch == '+' || ch == '-' || ch == '@' || ch == '$') {
				postfixExpression[*post_index] = ch;
				Pop(s, &ch);
				(*post_index)++;
			} else {
				return;
			}
		}
	}else if (c == '*' || c == '/'){
		while(!StackEmpty(s)) {
			GetTop(s, &ch);
			if (ch == '*' || ch == '/' || ch == '@' || ch == '$') {
				postfixExpression[*post_index] = ch;
				Pop(s, &ch);
				(*post_index)++;
			}else {
				return;
			}
		}
	}
}


int isOperator(char ch) {
	return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}


int getPrevCharIndex(char *infixExpressionHead, char *infixExpression, int curr_index) {
	while(infixExpression!=infixExpressionHead && curr_index > -1){
		if(!isspace(*infixExpression)) 
			return curr_index;
		curr_index--;
		infixExpression--;
	}
	if(!isspace(*infixExpression)) return 0;
	return FAILED;
}


char getPrevChar(char *infixExpression, char *infixExpressionHead) {
	char ch;
	if (infixExpressionHead == infixExpression){
		return '!';
	}
	infixExpression--;
	
	do {
		if (!isspace(ch = *infixExpression))	return ch;
		infixExpression--;
	}while(infixExpression != infixExpressionHead);
	if (!isspace(ch = *infixExpression))	return ch;
	return '!';
}


char getNextChar(char *infixExpression, int *next_index) {
	char ch;
	if ('\0' == infixExpression){
		return '!';
	}
	infixExpression++;
	(*next_index)++;
	do {
		if (!isspace(ch = *infixExpression))	
			return ch;
		infixExpression++;
		(*next_index)++;
	}while(*infixExpression != '\0');

	if (!isspace(ch = *infixExpression))	
		return ch;
	return '!';
}


void printErrorIndex(char *infixExpression, int t1, int t2, int count) {
	printf("%s\n", infixExpression);
	int index = 0;
	while(*infixExpression != '\0'){
		if (count == 1){
			if (index == t1)	
				printf("^");
			else if(*infixExpression == '\t')	
				printf("\t");
			else if(*infixExpression == '\b')	
				printf("^b");	
			else printf(" ");
		} else if(count == 2){
			if (index == t1 || index == t2)	
				printf("^");
			else if(*infixExpression == '\t')	
				printf("\t");
			else if(*infixExpression == '\b')	
				printf("^b");	
			else	
				printf(" ");
		}
		
		infixExpression++;
		index++;
	}
	printf("\n");
}
