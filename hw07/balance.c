#include <stdio.h>
#include <stdlib.h>
#include "istack.h"

int main(void)
{
    istack_t *stack = istack_create();

    if (stack == NULL) {
        return 1;
    }

    int ch;
    int line = 1;
    int column = 0;
    int pairs = 0;
    int deepest = 0;

    while ((ch = getchar()) != EOF) {
        column++;

        if (ch == '(' || ch == '[' || ch == '{') {
            if (istack_push(stack, ch) == -1) {
                istack_destroy(stack);
                return 1;
            }

            if ((int)istack_count(stack) > deepest) {
                deepest = (int)istack_count(stack);
            }
        }
        else if (ch == ')' || ch == ']' || ch == '}') {
            int opening;

            if (istack_peek(stack, &opening) == -1) {
                printf("line %d, column %d: found '%c', nothing open\n",
                       line, column, ch);
                istack_destroy(stack);
                return 1;
            }

            int matches =
                (opening == '(' && ch == ')') ||
                (opening == '[' && ch == ']') ||
                (opening == '{' && ch == '}');

            if (!matches) {
                printf("line %d, column %d: found '%c', expected '%c'\n",
                       line, column, ch,
                       opening == '(' ? ')' :
                       opening == '[' ? ']' : '}');

                istack_destroy(stack);
                return 1;
            }

            istack_pop(stack, NULL);
            pairs++;
        }

        if (ch == '\n') {
            line++;
            column = 0;
        }
    }

    if (!istack_is_empty(stack)) {
        int opening;
        istack_peek(stack, &opening);

        printf("end of input: %zu still open, innermost '%c'\n",
               istack_count(stack), opening);

        istack_destroy(stack);
        return 1;
    }

    printf("balanced: %d pairs, deepest %d\n", pairs, deepest);

    istack_destroy(stack);
    return 0;
}
