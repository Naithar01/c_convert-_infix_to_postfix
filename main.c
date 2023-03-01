#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MaxStr 100

typedef struct Stack
{
    int top;
    char postfix[MaxStr];
    int calculator_top;
    int calculator_postfix[MaxStr];
} Stack;

int isEmpty(Stack *s)
{
    if (s->top == -1)
    {
        return 1;
    }
    return 0;
}

void push(Stack *s, char c)
{
    if (s->top == MaxStr - 1)
    {
        exit(1);
    }

    s->postfix[++(s->top)] = c;
}

char pop(Stack *s)
{
    if (isEmpty(s))
    {
        exit(1);
    }

    return s->postfix[(s->top)--];
}

int isEmptyCalculatorPostfix(Stack *s)
{
    if (s->calculator_top == -1)
    {
        return 1;
    }
    return 0;
}

void pushCalculatorPostfix(Stack *s, int d)
{
    if (s->calculator_top == MaxStr - 1)
    {
        exit(1);
    }

    s->calculator_postfix[++(s->calculator_top)] = d;
}

int popCalculatorPostfix(Stack *s)
{
    if (isEmptyCalculatorPostfix(s))
    {
        exit(1);
    }

    return s->calculator_postfix[(s->calculator_top)--];
}

// 매개변수로 받은 c가 연산자인지 아닌지 확인해주는 함수
int checkOperator(char c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/')
    {
        return 1;
    }
    return 0;
}

// 매개변수로 받은 c가 연산자 순위에 따른 정수 값을 반환하는 함수
int topOperator(char c)
{
    if (c == '*' || c == '/')
    {
        return 2;
    }
    else if (c == '+' || c == '-')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// 중위 표기법을 후위 표기법으로 바꿔줄 함수
void infixToPostfix(char infix[], char postfix[])
{
    int j = 0;
    Stack s;
    s.top = -1;

    for (int i = 0; infix[i] != '\0'; i++) //
    {
        char c = infix[i];

        // 정수인지 확인
        if (c >= '0' && c <= '9')
        {
            postfix[j++] = c;
        }
        // 연산자인지 확인
        else if (checkOperator(c))
        {
            // 연산자도 다 똑같은 연산자가 아님, 사칙연산에도 우선순위가 있음
            // 현재 '*' 혹은 '/' 가 들어가있고 '+' 혹은 '-' 가 들어왔으면 '*, /' 를 먼저 연산을 해줘야하기에
            // postfix에 연산자를 바로 삽입(대입)
            while (!isEmpty(&s) && topOperator(c) <= topOperator(s.postfix[s.top]))
            {
                postfix[j++] = pop(&s);
            }
            push(&s, c);
        }
        // (1 + 2) * 3 와같은 중위 표기법의 경우
        // 1 과 2를 더하고 3을 곱해야하기 때문에 괄호를 처리해줘야함
        // 여는 괄호가 나왔을 때 스택에 여는 괄호를 넣음
        else if (c == '(')
        {
            push(&s, c);
        }
        // 여는 괄호가 나오고 정수는 postfix 배열에 계속 저장,
        // 닫는 괄호가 나왔을 때 스택에서 여는 괄호가 나올 때까지
        // (위 조건문에서 스택에 넣어줬던 여는 괄호가 나올 때 까지)
        // 후위 표기법 문자열을 저장하는 문자열 배열 변수에 스택에 넣어줬던
        // 연산자를 넣어줌
        // ex) (1 + 2) * 3 =
        // 스택) (
        // postfix) 1
        // 스택) (+
        // postfix) 12
        // pop -> + && append
        // postfix) 12+

        // 여는 괄호는 제거하고 닫는 괄호는 처음부터 추가한 적 없음
        else if (c == ')')
        {
            while (!isEmpty(&s) && s.postfix[s.top] != '(')
            {
                postfix[j++] = pop(&s);
            }
            pop(&s); // 여는 괄호 제거 '('
        }
    }

    // 위에서 postfix에는 정수만 담겨있음,
    // (괄호가 있을 땐 예외사항)
    // 스택에서 저장해 두었던 연산자들을 합쳐줌
    // ex) 1 + 2 * 3 -> 123 ( 위의 for loop )
    // ex) 123 -> 123*+ (아래의 while loop )
    // 이렇게 바뀜
    while (!isEmpty(&s))
    {
        postfix[j++] = pop(&s);
    }

    // 후위 표기법으로 바꾸고 마지막 문자열 부분을
    // null 처리
    postfix[j] = '\0';
}

// 후위 표기법 수식을 계산
int calculatorOperator(char O, int data1, int data2)
{
    switch (O)
    {
    case '+':
        return data1 + data2;
        break;

    case '-':
        return data1 - data2;
        break;

    case '*':
        return data1 * data2;
        break;

    case '/':
        return data1 / data2;
        break;

    default:
        break;
    }
}

// 후위 표기법으로 바뀐 문자열을 계산해줄 함수
int calculatorPostfix(char postfix[])
{
    Stack s;
    s.calculator_top = -1;

    for (int i = 0; postfix[i] != '\0'; i++)
    {
        char c = postfix[i];

        // 정수인지 확인
        if (c >= '0' && c <= '9')
        {
            pushCalculatorPostfix(&s, c - '0');
        }
        else if (checkOperator(c))
        {
            pushCalculatorPostfix(&s, calculatorOperator(c, popCalculatorPostfix(&s), popCalculatorPostfix(&s)));
        }
    }

    printf("%d", popCalculatorPostfix(&s));
    return 0;
}

int main()
{
    // infix는 중위 표기법을 입력
    // postfix는 중위 표기법을 후위 표기법으로 바꾼 문장을 저장할 변수
    char infix[MaxStr], postfix[MaxStr];

    scanf("%s", infix);

    infixToPostfix(infix, postfix);

    printf("...: %s\n", postfix);

    calculatorPostfix(postfix);

    return 0;
}
