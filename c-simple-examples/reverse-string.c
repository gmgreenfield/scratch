#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char *s = "example";

    printf("%s\n", s);

    char *p = (s + (strlen(s) - 1));
    while (*s++) {
        printf("%c", *p--);
    }

    printf("\n");

    return 0;
}
