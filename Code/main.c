#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

typedef float TYPE;

typedef struct node
{
    TYPE data;
    struct node *next;
    struct node *prev;
} node;

typedef struct
{
    node *head;
    node *tail;
} deque;

typedef struct
{
    deque *deque;
} stack;

typedef struct
{
    deque *deque;
} queue;

// Primary functions for deque
node *new_node(TYPE data);
deque *new_deque();
void distNode(node *n);
void insHead(deque *d, TYPE data);
void remHead(deque *d);
void insTail(deque *d, TYPE data);
void remTail(deque *d);
void printDeque(deque *d);

// Stack functions
stack *new_stack();
void push_stack(stack *s, TYPE data);
TYPE pop_stack(stack *s);
TYPE peek_stack(stack *s);
int isEmptyStack(stack *s);
void displayStack(stack *s);

node *new_node(TYPE data)
{
    node *n = malloc(sizeof(node));
    if (n == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    n->data = data;
    n->next = n->prev = NULL;
    return n;
}

deque *new_deque()
{
    deque *d = malloc(sizeof(deque));
    if (d == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    d->head = d->tail = NULL;
    return d;
}

void distNode(node *n)
{
    free(n);
}

void insHead(deque *d, TYPE data)
{
    node *n = new_node(data);
    if (!d->head)
        d->head = d->tail = n;
    else
    {
        n->next = d->head;
        d->head->prev = n;
        d->head = n;
    }
}

void remHead(deque *d)
{
    if (d->head)
    {
        if (d->head == d->tail)
        {
            d->head = d->tail = NULL;
        }
        else
        {
            d->head = d->head->next;
            distNode(d->head->prev);
            d->head->prev = NULL;
        }
    }
}

void insTail(deque *d, TYPE data)
{
    node *n = new_node(data);
    if (!d->head)
        d->head = d->tail = n;
    else
    {
        d->tail->next = n;
        n->prev = d->tail;
        d->tail = n;
    }
}

void remTail(deque *d)
{
    if (d->tail)
    {
        if (d->head == d->tail)
        {
            d->head = d->tail = NULL;
        }
        else
        {
            d->tail = d->tail->prev;
            distNode(d->tail->next);
            d->tail->next = NULL;
        }
    }
}

void printDeque(deque *d)
{
    node *current = d->head;
    while (current != NULL)
    {
        printf("%d\n", current->data);
        current = current->next;
    }
}

/************** Stack Functions **************/
stack *new_stack()
{
    stack *s = malloc(sizeof(stack));
    if (s == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    s->deque = new_deque();
    return s;
}

void push_stack(stack *s, TYPE data)
{
    insHead(s->deque, data);
}

TYPE pop_stack(stack *s)
{
    if (!isEmptyStack(s))
    {
        TYPE data = s->deque->head->data;
        remHead(s->deque);
        return data;
    }
    else
        return -1;
}

TYPE peek_stack(stack *s)
{
    return !isEmptyStack(s) ? s->deque->head->data : -1;
}

int isEmptyStack(stack *s)
{
    return s->deque->head == NULL ? 1 : 0;
}

void displayStack(stack *s)
{
    node *temp = s->deque->head;
    while (temp->next != NULL)
    {
        printf("%d\n", temp->data);
        temp = temp->next;
    }
    printf("%d\n", temp->data);
}
/*********************************************/

int preced(char c)
{
    switch (c)
    {
    case '^':
        return 4;
    case '*':
    case '/':
    case '%':
        return 3;
    case '+':
    case '-':
        return 2;
    case '(':
        return 1;
    default:
        return 0;
    }
}

char associativity(char c){
    if(c == '^')
        return 'R';
    return 'L';
}

int isvalid_input(char *infix)
{
    int n = strlen(infix);
    if(infix[n-1] == '\n')
        infix[n-1] = '\0';
    stack *s = new_stack();
    for(int i = 0;i < strlen(infix); i++){
        if(infix[i] != '+' && infix[i] != '-' 
            && infix[i] != '*' && infix[i] != '/' 
            && infix[i] != '^' && infix[i] != '%' 
            && infix[i] != '(' && infix[i] != ')'
            && infix[i] != ' ' && infix[i] != '.' 
            && !isdigit(infix[i])){
                printf("\nError: Invalid Operator \"%c\"\n", infix[i]);
                return 0;
            }
        if(infix[i] == '(')
            push_stack(s, infix[i]);
        else if(infix[i] == ')'){
            if(isEmptyStack(s))
                return 0;
            else 
                pop_stack(s);
        }
    }
    if(isEmptyStack(s))
        return 1;
    else
        return 0;
}

char *conv_post_to_infix(char *infix)
{
    int n = strlen(infix), j = 0;
    char *exp = malloc(n);
    strcpy(exp, infix);
    stack *s = new_stack();
    char *postExp = malloc(2 * n);
    char last_char = ' ';
    for (int i = 0; i < n; i++)
    {
        
        if (infix[i] == ' ')
            continue;
        else if (isdigit(infix[i]))
        {
            char *token = strtok(exp + i, " +-*/%^()");
            int n_tok = strlen(token);
            i += n_tok - 1;
            for (int k = 0; k < n_tok; k++)
            {
                postExp[j++] = token[k];
            }
            postExp[j++] = ' ';
        }
        else if (infix[i] == '(')
            push_stack(s, infix[i]);
        else if (infix[i] == ')')
        {
            while (!isEmptyStack(s) && peek_stack(s) != '(')
            {
                postExp[j++] = pop_stack(s);
                postExp[j++] = ' ';
            }
            pop_stack(s);
        }
        else
        {
            if(infix[i] == '-' && last_char != ')' && !isdigit(last_char)){
                postExp[j++] = '-';
                continue;
            }
            while (!isEmptyStack(s) && preced(infix[i]) <= preced(peek_stack(s)) && associativity(infix[i]) == 'L')
            {
                postExp[j++] = pop_stack(s);
                postExp[j++] = ' ';
            }
            push_stack(s, infix[i]);
        }
        last_char = infix[i];
    }
    while (!isEmptyStack(s))
    {
        postExp[j++] = pop_stack(s);
        postExp[j++] = ' ';
    }
    postExp[j] = '\0';
    return postExp;
}

float evalPostfix(char *exp)
{
    int n = strlen(exp);
    stack *s = new_stack();
    char *token;
    token = strtok(exp, " ");
    while (token)
    {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1])))
        {
            push_stack(s, atof(token));
        }
        else
        {
            float num2 = pop_stack(s);
            float num1 = pop_stack(s);
            switch (token[0])
            {
            case '+':
                push_stack(s, (num1 + num2));
                break;
            case '-':
                push_stack(s, (num1 - num2));
                break;
            case '*':
                push_stack(s, (num1 * num2));
                break;
            case '/':
                if(num2 == 0){
                    printf("\nError: Division by Zero\n");
                    exit(1);
                }
                push_stack(s, (num1 / num2));
                break;
            case '^':
                if (num1 < 0 && num2 != floor(num2)) {
                    printf("\nError: Negative base with non-integer exponent\n");
                    exit(3);
                }
                push_stack(s, pow(num1, num2));
                break;
            case '%':
                if(num2 == 0){
                    printf("\nError: Modulo by Zero\n");
                    exit(1);
                }
                push_stack(s, fmod(num1, num2));
                break;
            default:
                printf("\nError: Invalid Operator \"%c\"\n", token[0]);
                exit(2);
            }
        }
        token = strtok(NULL, " ");
    }
    return pop_stack(s);
}

void main()
{
    // 1+2*4+3 // (1+2)*4+3 // 10+3*5/(16-4) // 2+3*4 // 2+(-2.5+3.14)*(-5.4+8.1)^(-0.5)
    char *infExp = malloc(80);
    printf("Enter an infix expression: ");
    fgets(infExp, 80, stdin);
    while(!isvalid_input(infExp)){
        printf("Enter a valid infix expression: ");
        fgets(infExp, 80, stdin);
    }
    char *postExp = conv_post_to_infix(infExp);
    printf("Infix expression:\t%s\n", infExp);
    printf("Postfix expression:\t%s\n", postExp);
    printf("Evaluated postfix expression: \t%.3f\n", evalPostfix(postExp));
}
