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

#define TEST_ERROR(error, json)\
    do {\
        pson_value v;\
        v.type = JSON_FALSE;\
        EXPECT_EQ_INT(error, parse(&v, json));\
        EXPECT_EQ_INT(JSON_NULL, getType(&v));\
    } while(0)

#define EXPECT_EQ_DOUBLE(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%lf")

#define TEST_NUMBER(expect, json)\
    do {\
        pson_value v;\
        v.type = JSON_FALSE;\
        EXPECT_EQ_INT(PARSE_OK, parse(&v, json));\
        EXPECT_EQ_INT(JSON_NUMBER, getType(&v));\
        EXPECT_EQ_DOUBLE(expect, getNumber(&v));\
    } while(0)

static void test_parse_null() {
    // pson_value v;
    // v.type = JSON_FALSE;
    // EXPECT_EQ_INT(PARSE_OK, parse(&v, "null"));
    // EXPECT_EQ_INT(JSON_NULL, getType(&v));
    TEST_ERROR(PARSE_OK, "null");
}

static void test_parse_true() {
    pson_value v;
    v.type = JSON_FALSE;
    EXPECT_EQ_INT(PARSE_OK, parse(&v, "true"));
    EXPECT_EQ_INT(JSON_TRUE, getType(&v));
}

static void test_parse_false() {
    pson_value v;
    v.type = JSON_FALSE;
    EXPECT_EQ_INT(PARSE_OK, parse(&v, "false"));
    EXPECT_EQ_INT(JSON_FALSE, getType(&v));
}

static void test_parse_number() {
    TEST_NUMBER(0.0, "0");
    TEST_NUMBER(0.0, "-0");
    TEST_NUMBER(0.0, "-0.0");
    TEST_NUMBER(1.0, "1");
    TEST_NUMBER(-1.0, "-1");
    TEST_NUMBER(1.5, "1.5");
    TEST_NUMBER(-1.5, "-1.5");
    TEST_NUMBER(3.1416, "3.1416");
    TEST_NUMBER(1E10, "1E10");
    TEST_NUMBER(1e10, "1e10");
    TEST_NUMBER(1E+10, "1E+10");
    TEST_NUMBER(1E-10, "1E-10");
    TEST_NUMBER(-1E10, "-1E10");
    TEST_NUMBER(-1e10, "-1e10");
    TEST_NUMBER(-1E+10, "-1E+10");
    TEST_NUMBER(-1E-10, "-1E-10");
    TEST_NUMBER(1.234E+10, "1.234E+10");
    TEST_NUMBER(1.234E-10, "1.234E-10");
    TEST_NUMBER(0.0, "1e-10000"); /* must underflow */

    TEST_NUMBER(1.0000000000000002, "1.0000000000000002"); /* the smallest number > 1 */
    TEST_NUMBER( 4.9406564584124654e-324, "4.9406564584124654e-324"); /* minimum denormal */
    TEST_NUMBER(-4.9406564584124654e-324, "-4.9406564584124654e-324");
    TEST_NUMBER( 2.2250738585072009e-308, "2.2250738585072009e-308");  /* Max subnormal double */
    TEST_NUMBER(-2.2250738585072009e-308, "-2.2250738585072009e-308");
    TEST_NUMBER( 2.2250738585072014e-308, "2.2250738585072014e-308");  /* Min normal positive double */
    TEST_NUMBER(-2.2250738585072014e-308, "-2.2250738585072014e-308");
    TEST_NUMBER( 1.7976931348623157e+308, "1.7976931348623157e+308");  /* Max double */
    TEST_NUMBER(-1.7976931348623157e+308, "-1.7976931348623157e+308");
}

static void test_parse_expect_value() {
    // pson_value v;

    // v.type = JSON_FALSE;
    // EXPECT_EQ_INT(PARSE_EXPECT_VALUE, parse(&v, ""));
    // EXPECT_EQ_INT(JSON_NULL, getType(&v));

    // v.type = JSON_FALSE;
    // EXPECT_EQ_INT(PARSE_EXPECT_VALUE, parse(&v, " "));
    // EXPECT_EQ_INT(JSON_NULL, getType(&v));

    TEST_ERROR(PARSE_EXPECT_VALUE, "");
    TEST_ERROR(PARSE_EXPECT_VALUE, " ");
}

static void test_parse_invalid_value() {
    // pson_value v;
    // v.type = JSON_FALSE;
    // EXPECT_EQ_INT(PARSE_INVALID_VALUE, parse(&v, "nul"));
    // EXPECT_EQ_INT(JSON_NULL, getType(&v));

    // v.type = JSON_FALSE;
    // EXPECT_EQ_INT(PARSE_INVALID_VALUE, parse(&v, "?"));
    // EXPECT_EQ_INT(JSON_NULL, getType(&v));

    TEST_ERROR(PARSE_INVALID_VALUE, "nul");
    TEST_ERROR(PARSE_INVALID_VALUE, "?");

    /* invalid number */
    TEST_ERROR(PARSE_INVALID_VALUE, "+0");
    TEST_ERROR(PARSE_INVALID_VALUE, "+1");
    TEST_ERROR(PARSE_INVALID_VALUE, ".123"); /* at least one digit before '.' */
    TEST_ERROR(PARSE_INVALID_VALUE, "1.");   /* at least one digit after '.' */
    TEST_ERROR(PARSE_INVALID_VALUE, "INF");
    TEST_ERROR(PARSE_INVALID_VALUE, "inf");
    TEST_ERROR(PARSE_INVALID_VALUE, "NAN");
    TEST_ERROR(PARSE_INVALID_VALUE, "nan");
}

static void test_parse_root_not_singular() {
    // pson_value v;
    // v.type = JSON_FALSE;
    // EXPECT_EQ_INT(PARSE_ROOT_NOT_SINGULAR, parse(&v, "null x"));
    // EXPECT_EQ_INT(JSON_NULL, getType(&v));

    TEST_ERROR(PARSE_ROOT_NOT_SINGULAR, "null x");

    /* invalid number */
    TEST_ERROR(PARSE_ROOT_NOT_SINGULAR, "0123"); /* after zero should be '.' , 'E' , 'e' or nothing */
    TEST_ERROR(PARSE_ROOT_NOT_SINGULAR, "0x0");
    TEST_ERROR(PARSE_ROOT_NOT_SINGULAR, "0x123");
}

static void test_parse_number_too_big() {
// #if 0
    TEST_ERROR(PARSE_NUMBER_TOO_BIG, "1e309");
    TEST_ERROR(PARSE_NUMBER_TOO_BIG, "-1e309");
// #endif
}

static void test_parse() {
    test_parse_null();
    test_parse_true();
    test_parse_false();
    test_parse_number();
    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
    test_parse_number_too_big();
}

int main() {
    test_parse();
    std::cout<<test_pass<<"/"<<test_count<<" ("<<test_pass * 100.0 / test_count<<"%%) passed\n"<<std::endl;
    return main_ret;
}
