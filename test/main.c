#include "test_layout.h"
#include "test_allocations.h"

#include <stdio.h>

int main() {
    crf_TestDecoratorSize();
    crf_TestCreateDecoratorNullMalloc();
    crf_TestCreateDecoratorOddMalloc();

    return EXIT_SUCCESS;
}