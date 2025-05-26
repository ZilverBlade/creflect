#include "test_layout.h"
#include "test_allocations.h"

void main() {
    crf_TestDecoratorSize();
    crf_TestCreateDecoratorNullMalloc();
    crf_TestCreateDecoratorOddMalloc();
}