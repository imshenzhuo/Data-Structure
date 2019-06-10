#include "sequentialStack.h"


/* Interfaces for the 1st stack for char elements*/

/*  Initialize the sequential stack
 */
State InitStack(SequentialStackChar *s){
	s->top = -1;
	return SUCCEEDED;
}

/*  Release the memory of the sequential stack
 */
void DestroyStack(SequentialStackChar *s){
	free(s);
	s = NULL;
}

/*  Get the size of the sequential stack
 */
int getStackSize(SequentialStackChar *s) {
	return s->top + 1;
}

/*  Check if the stack is empty.
    Return SUCCEEDED to indicate the stack is empty.
    Return FAILED to indicate the stack is NOT empty.
 */
State StackEmpty(SequentialStackChar *s){
	if (getStackSize(s) <= 0)	return SUCCEEDED;
	else 	return FAILED;
}

/*  If the stack is NOT full, push e into it and return SUCCEEDED to indicate a successful push.
    If the stack is full, merely return FAILED to indicate an unsuccessful push.
 */
State Push(SequentialStackChar *s, ElemType e){
	if(getStackSize(s) >= MaxSize) {
		printf("Something wrong happened when we are pushing '%c' into the stack.\n", e);
		printf("the stack is full! Please resize the stack.\n");
		return FAILED;
	}
	s->data[++s->top] = e;
	return SUCCEEDED;
}

/*  If the stack is NOT empty, pop the top of the stack into e and return SUCCEEDED to indicate a successful pop.
    If the stack is empty, merely return FAILED to indicate an unsuccessful pop.
 */
State Pop(SequentialStackChar *s, ElemType *e) {
	if (getStackSize(s) <= 0){
		printf("Something wrong happened when we are popping a char from stack.\n");
		printf("the stack is empty! Please check the stack.\n");
		return FAILED;
	}
	*e = s->data[s->top--];
	return SUCCEEDED;
}

/*  If the stack is NOT empty, assign the top of the stack to e and return SUCCEEDED to indicate a successful gettop.
    If the stack is empty, merely return FAILED to indicate an unsuccessful gettop.
 */
State GetTop(SequentialStackChar *s, ElemType *e) {
	if (getStackSize(s) <= 0){
		printf("Something wrong happened when we are get a char from stack.\n");
		printf("the stack is empty! Please check the stack.\n");
		return FAILED;
	}
	*e = s->data[s->top];
	return SUCCEEDED;
}



/* Interfaces for the 2nd stack for double elements*/

/*  Initialize the sequential stack
 */
State InitStackDouble(SequentialStackDouble *s) {
	s->top = -1;
	return SUCCEEDED;
}

/*  Release the memory of the sequential stack
 */
void DestroyStackDouble(SequentialStackDouble *s) {
	free(s);
	s = NULL;
}

/*  Get the size of stack 
 */
int getStackSizeDouble(SequentialStackDouble *s) {
	return s->top + 1;
}

/*  Check if the stack is empty.
 Return SUCCEEDED to indicate the stack is empty.
 Return FAILED to indicate the stack is NOT empty.
 */
State StackEmptyDouble(SequentialStackDouble *s) {
	if (getStackSizeDouble(s) <= 0)	return SUCCEEDED;
	else 	return FAILED;
}

/*  If the stack is NOT full, push e into it and return SUCCEEDED to indicate a successful push.
    If the stack is full, merely return FAILED to indicate an unsuccessful push.
 */
State PushDouble(SequentialStackDouble *s, ElemTypeDouble e) {
	if (getStackSizeDouble(s) >= MaxSize) {
		printf("Something wrong happened when we are pushing %lf into the stack.\n", e);
		printf("the stack is full! Please resize the stack.\n");
		return FAILED;
	}
	s->data[++s->top] = e;
	return SUCCEEDED;
}

/*  If the stack is NOT empty, pop the top of the stack into e and return SUCCEEDED to indicate a successful pop.
    If the stack is empty, merely return FAILED to indicate an unsuccessful pop.
 */
State PopDouble(SequentialStackDouble *s,ElemTypeDouble *e) {
	if (SUCCEEDED ==  StackEmptyDouble(s)) {
		printf("Something wrong happened when we are popping a double from stack.\n");
		printf("the stack is empty! Please check the stack.\n");
		return FAILED;
	}
	*e = s->data[s->top--];
	return SUCCEEDED;
}

/*  If the stack is NOT empty, assign the top of the stack to e and return SUCCEEDED to indicate a successful gettop.
    If the stack is empty, merely return FAILED to indicate an unsuccessful gettop.
 */
State GetTopDouble(SequentialStackDouble *s,ElemTypeDouble *e) {
	if (SUCCEEDED == StackEmptyDouble(s)) {
		printf("Something wrong happened when we are get a char from stack.\n");
		printf("the stack is empty! Please check the stack.\n");
		return FAILED;
	}
	*e = s->data[s->top];
	return SUCCEEDED;
}
