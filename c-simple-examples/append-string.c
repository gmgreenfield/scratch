#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char *s = "example";

    printf("%s\n", s);

    char test[sizeof(s)];

    char *p = (s + (strlen(s) - 1));
    while (*s++) {
        printf("%c", *p--);
        strcat(test, *p);
    }

    printf("\n");

    printf("%s\n", p);

    return 0;
}
