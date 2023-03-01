#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MaxStr 100

typedef struct Stack
{
    int top;
    char postfix[MaxStr];
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
    int i, j;
    char c;

    Stack s;
    s.top = -1;

    for (i = 0, j = 0; infix[i] != '\0'; i++) //
    {
        c = infix[i];

        // 정수인지 확인
        if (c >= '0' && c <= '9')
        {
            postfix[j++] = c;
        }
        // 연산자인지 확인
        else if (checkOperator(c))
        {
            // 연산자도 다 똑같은 연산자가 아님, 사칙연산에도 우선순위가 있음
            // 연산자 우선순위에 따른 순서를 변경하는 조건문
            // 현재 infix의 i번째 부분이 연산자인데 스택에 가장 최근에
            // 추가된 연산자가 현재 연산자보다 작거나 같은지 확인
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
            pop(&s); // discard the '('
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

int main()
{
    // infix는 중위 표기법을 입력
    // postfix는 중위 표기법을 후위 표기법으로 바꾼 문장을 저장할 변수
    char infix[MaxStr], postfix[MaxStr];

    scanf("%s", infix);

    infixToPostfix(infix, postfix);

    printf("%s\n", postfix);

    return 0;
}
