#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "cppjson.h"

using namespace pson;

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format) \
    do {\
        test_count++;\
        if (equality)\
            test_pass++;\
        else {\
            fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n", __FILE__, __LINE__, expect, actual);\
            main_ret = 1;\
        }\
    } while(0)

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")

static void test_parse_null() {
    pson_value v;
    v.type = JSON_FALSE;
    EXPECT_EQ_INT(PARSE_OK, parse(&v, "null"));
    EXPECT_EQ_INT(JSON_NULL, getType(&v));
}

static void test_parse_true() {
    pson_value v;
    v.type = JSON_FALSE;
    EXPECT_EQ_INT(PARSE_OK, parse(&v, "true"));
    EXPECT_EQ_INT(JSON_NULL, getType(&v));
}

static void test_parse_false() {
    pson_value v;
    v.type = JSON_FALSE;
    EXPECT_EQ_INT(PARSE_OK, parse(&v, "false"));
    EXPECT_EQ_INT(JSON_NULL, getType(&v));
}

static void test_parse_expect_value() {
    pson_value v;

    v.type = JSON_FALSE;
    EXPECT_EQ_INT(PARSE_EXPECT_VALUE, parse(&v, ""));
    EXPECT_EQ_INT(JSON_NULL, getType(&v));

    v.type = JSON_FALSE;
    EXPECT_EQ_INT(PARSE_EXPECT_VALUE, parse(&v, " "));
    EXPECT_EQ_INT(JSON_NULL, getType(&v));
}

static void test_parse_invalid_value() {
    pson_value v;
    v.type = JSON_FALSE;
    EXPECT_EQ_INT(PARSE_INVALID_VALUE, parse(&v, "nul"));
    EXPECT_EQ_INT(JSON_NULL, getType(&v));

    v.type = JSON_FALSE;
    EXPECT_EQ_INT(PARSE_INVALID_VALUE, parse(&v, "?"));
    EXPECT_EQ_INT(JSON_NULL, getType(&v));
}

static void test_parse_root_not_singular() {
    pson_value v;
    v.type = JSON_FALSE;
    EXPECT_EQ_INT(PARSE_ROOT_NOT_SINGULAR, parse(&v, "null x"));
    EXPECT_EQ_INT(JSON_NULL, getType(&v));
}

static void test_parse() {
    test_parse_null();
    test_parse_true();
    test_parse_false();
    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
}

int main() {
    test_parse();
    std::cout<<test_pass<<"/"<<test_count<<" ("<<test_pass * 100.0 / test_count<<"%%) passed\n"<<std::endl;
    return main_ret;
}
