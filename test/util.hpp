#pragma once
#include <creflect/context.h>
#include <gtest/gtest.h>

#define CRF_EXP_CTX_SUCCESS(ctx) EXPECT_EQ(crf_context_get_last_error(ctx), CRF_EC_SUCCESS)
#define EXPECT_NULL(ptr) EXPECT_EQ((ptr), NULL)
#define EXPECT_NOT_NULL(ptr) EXPECT_NE((ptr), NULL)