#include "decorator.c"
#include <stdio.h>
#include <assert.h>

static void crf_TestDecoratorSize() {
    crf_context ctx = crf_context_create();

    crf_decorator_create_info createInfo;
    createInfo.pczMemberLayout = "cdfi";
    createInfo.ppMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    assert(decorator->szMemberCount == 4);
    assert(decorator->szStructTotal == (1+8+4+4));
    assert(decorator->pMemberTypes != NULL);
    assert(decorator->pMemberTypes[0] == CRF_MEMBER_CHAR &&
        decorator->pMemberTypes[1] == CRF_MEMBER_DOUBLE &&
        decorator->pMemberTypes[2] == CRF_MEMBER_FLOAT &&
        decorator->pMemberTypes[3] == CRF_MEMBER_INTEGER);

    crf_free_decorator(ctx, decorator);
    crf_context_free(ctx);

    printf("Passed %s!\n", __FUNCTION__);
}
