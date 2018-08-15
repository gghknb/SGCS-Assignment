#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Stack identifier. */
typedef struct{
	int count;
	struct node* top;
} stack;

/* Premise. */
/* 1. 'top' element points a first element. */
/* 2. push function inserts a node at front. */
/* 3. pop function deletes a node at front. */

typedef struct node{
	char data;
	struct node* link;
} stack_node;

/* Stack Operation. */
void push (stack *pStack, char data);		/* Same as InsertAtFront. */
char pop (stack *pStack);					/* Same as DeleteAtFront. */

/* Parenthesis Mathcing. */
int paren_matching (stack *pStack);	/* Retrun value is 1 if Mathed, otherwise, return 0. */

int main(void)
{
	char input[100];
	int length, i;
	int result;
	stack *pStack;

	/* Memory Allocation. */
	pStack = (stack*)malloc (sizeof (stack));

	/* Initialize. */
	pStack->count = 0;
	pStack->top = NULL;

	/* Input. */	
	fgets (input, 100, stdin);
	length = strlen (input);

	for (i=0; i<length; i++)
		push (pStack, input[i]);

	/* Process. */
    result = paren_matching (pStack);

	if (result == 1)
		printf("Matched Parentheses!\n");
	else if (result == 0)
		printf("UnMatched Parentheses!\n");

	return 0;
}


/* Before push operation.
 * -------------------------------
 * top -> a -> b -> c -> d -> NULL|
 * -------------------------------
 * After push operation.
 * -----------------------------------
 * top -> x -> a-> b -> c -> d -> NULL|
 * -----------------------------------
 */

void push (stack *pStack, char data)
{
	stack_node *new;

	/* Memory allocation. */
	new = (stack_node*)malloc (sizeof (stack_node));
	
	/* Initialize. */
	new->data = data;
	new->link = NULL;
	
	if (pStack->top == NULL) /* There is no element in stack. */
	{
		pStack->top = new;
	}
	else					 /* There are some elements in stack. */
	{
		new->link = pStack->top;
		pStack->top = new;
	}
}

/* Before pop operation.
 * -------------------------------
 * top -> a -> b -> c -> d -> NULL|
 * -------------------------------
 * After pop operation.
 * -----------------------------------
 * top -> b -> c -> d -> NULL		  |
 * -----------------------------------
 */
char pop (stack *pStack)
{
	char result;
	stack_node *temp;

	if (pStack->top)	/* There are some elements in stack. */
	{
		temp = pStack->top;
		result = pStack->top->data;
		pStack->top = pStack->top->link;
		free (temp);

		return result;
	}
	else
		return -1;
}

/* Initially count is 0.
 * If right parenthesis, count increase 1.
 * If left parenthesis, count decrese 1.
 * In checking, if count is smaller than 0, return false.
 *				otherwise, continue the checking.
 * After checking, if count is zero, return true.
 *				   otherwise, return false.
 */
int paren_matching (stack *pStack)
{
	int temp;
	
	/* In Checking. */
	while ((temp = pop(pStack)) != -1)
	{
		if (temp == '(' || temp == '{' || temp == '[')
			pStack->count--;
		else if (temp == ')' || temp == '}' || temp == ']')
			pStack->count++;

		if (pStack->count < 0)
			return 0;
	}

	/* After Checking. */
	if (!pStack->count)
		return 1;
	else
		return 0;
}
