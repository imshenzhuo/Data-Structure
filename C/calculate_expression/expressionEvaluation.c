/**
 * expressionEvaluation.c
 * @Author   shenzhuo
 * @DateTime 2018-07-31T15:57:46+0800
 */

#include "expressionEvaluation.h"
#include <ctype.h>		//	isdigit() isspace()
#include <string.h>		//	strlen()

/**
 * check if ch is an operator
 * @param    ch
 * @return   1 => true 0 => false
 */
int isOperator(char ch);


/**
 * print the infix expression and the point the position where error occured 
 * @param    infixExpression          [description]
 * @param    t1                       point 1
 * @param    t2                       point 2
 * @param    count                    the number of points
 */
void printErrorIndex(char *infixExpression, int t1, int t2, int count);


/**
 * infixExpression to PostfixExpression
 * @param    infixExpression          [description]
 * @param    postfixExpression        [description]
 * @return                            State
 */
State infixToPostfix(char *infixExpression,char postfixExpression[]) {
    char *infixExpressionHead = infixExpression;                /*	save the point of infixExpression to print error message */
    SequentialStackChar *s = (SequentialStackChar *)malloc(sizeof(SequentialStackChar));
    if (s == NULL){
        printf("Memory allocation failed.\n");
        return FAILED;
    }
    InitStack(s);
    int infix_index = 0, post_index = 0, prev_index = -1, countBrace = 0;
    char ch;                                                    /* save current char in infix expression */
    char prev_ch = '\0';                                        /* save prev non space char in infix expression */
    char tmp;
    while ( (ch = *infixExpression) != '\0') { 
        switch (ch) {
            case ' ':
            case '\t':  break;                                 /*   skip the space */
            case '(':   countBrace++;
                        if (isdigit(prev_ch)) {             /*   the '(' immediately follows a number */
                            printf("Wrong expression!! No operator between a number and '('.\n");
                            printErrorIndex(infixExpressionHead, infix_index, prev_index, 2);
                            DestroyStack(s);
                            return FAILED;
                        } else if (')'== prev_ch) {            /*   the '(' immediately follows '(' */
                            printf("Wrong expression!! No operator between ')' and '('.\n");
                            printErrorIndex(infixExpressionHead, infix_index, prev_index, 2);
                            DestroyStack(s);
                            return FAILED;
                        }
                        /* No abnormalities found, push the char */
                        if (Push(s, ch) == FAILED)   return FAILED;
                        break;
            case ')':   countBrace--;
                        if (prev_ch == '\0') {
                            printf("Wrong expression. No matched '(' before ')'\n");
                            printErrorIndex(infixExpressionHead, infix_index, prev_index, 2);
                            DestroyStack(s);
                            return FAILED;
                        } else if (prev_ch == '(') {
                            /* the prev_ch of ')' is '('. no operand in braces */
                            printf("Wrong expression!! No operand in braces.\n");
                            printErrorIndex(infixExpressionHead, infix_index, prev_index, 2);
                            DestroyStack(s);
                            return FAILED;
                        } else if (isOperator(prev_ch)) {
                            /* the prev_ch of ')' is an operator. an operator immediately follows ')' */
                            printf("Wrong expression!! No operand between the '%c' and ')'.\n", prev_ch);
                            printErrorIndex(infixExpressionHead, prev_index, infix_index, 2);
                            DestroyStack(s);
                            return FAILED;
                        } else if (countBrace < 0){
                            printf("Wrong expression. No matched '(' before ')'\n");
                            DestroyStack(s);
                            return FAILED;
                        }
                        // retrieve the char until '(' 
                        while (StackEmpty(s) == FAILED) {
                            Pop(s, &tmp);
                            if (tmp == '(')   break;
                            postfixExpression[post_index++] = tmp;
                        }
                        break;
            case '+':
            case '-':   if (prev_ch == '(' || prev_ch == '\0') { 
                            //start of the expression, unary operator
                            if (isspace(*(infixExpression+1)) ) {
                                if ('-' == ch)  printf("Wrong expression!! A space follows a unary minus.\n");
                                else    printf("Wrong expression!! A space follows a unary plus.\n");
                                printErrorIndex(infixExpressionHead, infix_index, 0, 1);
                                return FAILED;
                            }
                            if (ch == '-' && FAILED == Push(s, '$'))    return FAILED;  /* '$' represents unary '-' */
                            else if(ch == '+' && FAILED == Push(s, '@') )          return FAILED;  /* '@' represents unary '+' */
                        } else if (isOperator(prev_ch)) {    /*  the '+, -, *, /, ' immediately follows '+/-' */
                            // binary operator
                            printf("Wrong expression!! Opreator '%c' immediately follows '%c' in the expression.\n", ch, prev_ch);
                            printErrorIndex(infixExpressionHead, infix_index, prev_index, 2);
                            DestroyStack(s);
                            return FAILED;
                        } else {
                            /* pop the operators with the same priority or high priority */
                            while(FAILED == StackEmpty(s)) {
                                GetTop(s, &tmp);
                                if ( isOperator(tmp)  || tmp == '@' || tmp == '$') {
                                    postfixExpression[post_index++] = tmp;
                                    Pop(s, &tmp);
                                } else {
                                    break;
                                }
                            }
                            if (Push(s, ch) == FAILED) { return FAILED;	}
                        }
                        break;
            case '*':
            case '/':   /* '*' or '/' can not appear in the first */
                        if (prev_ch == '\0' || '(' == prev_ch ) {
                            printf("Wrong expression!! No operand before the '%c'.\n", ch);
                            printErrorIndex(infixExpressionHead,infix_index,-1,1);
                            DestroyStack(s);
                            return FAILED;
                        }
                        if (isOperator(prev_ch)) {          /* two consecutive operators */
                            printf("Wrong expression!! Opreator '%c' immediately follows '%c' in the expression.\n", ch, prev_ch);
                            printErrorIndex(infixExpressionHead, infix_index, prev_index, 2);
                            DestroyStack(s);
                            return FAILED;
                        }
                        /* pop the operators with the same priority or high priority */
                        while(FAILED == StackEmpty(s)) {
                            GetTop(s, &tmp);
                            if (  tmp == '/' || tmp == '*'|| tmp == '@' || tmp == '$') {
                                postfixExpression[post_index++] = tmp;
                                Pop(s, &tmp);
                            } else {
                                break;
                            }
                        }
                        if (FAILED == Push(s, ch))    return FAILED;
                        break;
            
            default:    if (isdigit(ch)) {
                            postfixExpression[post_index++] = ch;
                            if (')' == prev_ch) {           /*  a number immediately follows ')' in the expression.*/
                                printf("Wrong expression!! No operator between a number and ')'\n");
                                printErrorIndex(infixExpressionHead, infix_index, prev_index, 2);
                                return FAILED;
                            }
                            else if (isdigit(prev_ch)) {    /*  two consecutive operands */
                                printf("Wrong expression!! No operator between two numbers.\n");
                                printErrorIndex(infixExpressionHead, infix_index, prev_index, 2);
                                return FAILED;
                            } else {                        /* get consecutive digits  */
                                while(isdigit(*(infixExpression+1))) {
                                    infix_index++;
                                    infixExpression++;
                                    ch = *infixExpression;
                                    postfixExpression[post_index++] = ch;
                                }
                                postfixExpression[post_index++] = '_';
                            }
                        } else {
                            printf("Wrong expression!! Illegal character '%c' in the expression.\n", ch); 
                            DestroyStack(s); 
                            printErrorIndex(infixExpressionHead, infix_index, 1, 1);
                            return FAILED;
                        }
        }
        infixExpression++;
        if (!isspace(ch)) {
            prev_ch = ch;
            prev_index = infix_index;
        }
        infix_index++;  
    }
    /* check the rest of char in the stack */
    if (isOperator(prev_ch)) {
        printErrorIndex(infixExpressionHead, prev_index, infix_index, 1);
        printf("Wrong expression!! No operand after the '%c' in the expression.\n", prev_ch);
        DestroyStack(s);
        return FAILED;
    }
    if (countBrace > 0) {
        printf("Wrong expression!!  No matched ')' after '('.\n");
        DestroyStack(s);
        return FAILED;
    }
    /* get the rest of char in the stack */
    while (StackEmpty(s) == FAILED) {
        char ch;
        Pop(s, &ch);
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
 * @param    postfixExpression        postfixExpression
 * @param    value                    point of the result value
 * @return                            State
 */
State computeValueFromPostfix(char *postfixExpression, double *value) {
    /* check whether the postfixExpresion is empty.*/
    if (strlen(postfixExpression) <= 0) {
        printf("Wrong expression!! No operands in the expression!\n");
        return FAILED;
    }
    *value = 0;
    double t1, t2;
    SequentialStackDouble* stack = (SequentialStackDouble *)malloc(sizeof(SequentialStackDouble));
    if (stack == NULL) {
        printf("Memory allocation failed.\n");
        return FAILED;
    }
    InitStackDouble(stack);
    while (*postfixExpression != '\0') { 
        char ch = *postfixExpression;
        if (isdigit(ch)) {                                   /* push digit to stack */
            double i = ch - '0';
            while(isdigit(*(postfixExpression+1))) {        /* get the complete number */
                postfixExpression++;
                i = i * 10 + (*postfixExpression) - '0';
            }
            if (FAILED == PushDouble(stack, i))           return FAILED;
        } else if(ch != '_' && ch != '@'){                  /* skip space and '@' */
            if (ch == '$'){                                 /* unary operator */
                if (FAILED == PopDouble(stack, &t1))      return FAILED;
                if (FAILED == PushDouble(stack, -1*t1))   return FAILED;
            }else {
                if (FAILED == PopDouble(stack, &t1))      return FAILED;
                if (FAILED == PopDouble(stack, &t2))      return FAILED;
                switch(ch) {
                    case '+': *value = t1 + t2; break;
                    case '-': *value = t2 - t1; break;
                    case '*': *value = t2 * t1; break;
                    case '/': if(t1 == 0)   {   printf("Wrong expression!! Zero divisor!\n");   return FAILED;}
                              *value = t2 / t1;	break;
                    default:  printf("error! \n"); return FAILED;
                }
                if (PushDouble(stack,*value) == FAILED)    return FAILED;
            }
        }
        postfixExpression++;
    }
    if (PopDouble(stack, value) == FAILED) {
        printf("Something wrong happened when we are poping value from stack.\n");
        DestroyStackDouble(stack);
        return FAILED;
    }
    DestroyStackDouble(stack);
    return SUCCEEDED;
}


int isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}


void printErrorIndex(char *infixExpression, int t1, int t2, int count) {
    printf("%s\n", infixExpression);
    int index = 0;
    while(*infixExpression != '\0'){
        if (count == 1){
            if (index == t1)                    printf("^");
            else if(*infixExpression == '\t')   printf("\t");
            else                                printf(" ");
        } else if(count == 2){
            if (index == t1 || index == t2)     printf("^");
            else if(*infixExpression == '\t')   printf("\t");
            else                                printf(" ");
        }
        infixExpression++;
        index++;
    }
    printf("\n");
}
