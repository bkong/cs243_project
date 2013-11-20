#include <stdlib.h>
#include <stdio.h>

int zero() {
    return 0;
}

int main(int argc, char**argv) {
    int val = 0;
    for(int i = 0; i<1000000; i++)
        val += zero();
    printf("", val);
}
