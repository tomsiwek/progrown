#include <stdio.h>

int main(int argc, char* argv[]) {
    int i;
    for(i = 0; i < argc; i = i + 1)
    {
        printf("Argument no %d is %s.\n", i + 1, argv[i]);
    }
    return 0;
}
