#include "cppjson.h"
#include <assert.h>
#include <errno.h>   /* errno, ERANGE */
#include <math.h>    /* HUGE_VAL */
#include <iostream>
#include <stdlib.h>

// 宏里有多过一个语句（statement），就需要用 do { /*...*/ } while(0) 包裹成单个语句
#define Expect(c, ch) do { assert(*c->json == (ch)); c->json++; } while(0)
#define IsDigit1To9(ch) ((ch) >= '1' && (ch) <= '9')
// #define IsDigit1To9(ch) do {\
//   switch (ch) {\
//     case '1':\
//     case '2':\
//     case '3':\
//     case '4':\
//     case '5':\
//     case '6':\
//     case '7':\
//     case '8':\
//     case '9':\
//       return true;\
//     default:\
//       return false;\
//   }\
// } while(0)
#define IsDigit(ch) ((ch) >= '0' && (ch) <= '9')

namespace pson {
  typedef struct {
    const char* json;
  } pson_context;

  static void parseWhitespace(pson_context *c) {
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') {
      p++;
    }
    c->json = p;
  }

  /**
   * @deprecated parseLiteral 代替
   */
  static int parseNull(pson_context *c, pson_value *v) {
    Expect(c, 'n');
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l') {
      return PARSE_INVALID_VALUE;
    }
    c->json += 3;
    v->type = JSON_NULL;
    return PARSE_OK;
  }

  /**
   * @deprecated parseLiteral 代替
   */
  static int parseTrue(pson_context *c, pson_value *v) {
    Expect(c, 't');
    if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e') {
      return PARSE_INVALID_VALUE;
    }
    c->json += 3;
    v->type = JSON_TRUE;
    return PARSE_OK;
  }

  /**
   * @deprecated parseLiteral 代替
   */
  static int parseFalse(pson_context *c, pson_value *v) {
    Expect(c, 'f');
    if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e') {
      return PARSE_INVALID_VALUE;
    }
    c->json += 4;
    v->type = JSON_FALSE;
    return PARSE_OK;
  }
  
  static int parseLiteral(pson_context *c, pson_value *v, const char *expect, pson_type t) {
    size_t i = 0;
    while (expect[i] != '\0') {
      if (c->json[i] != expect[i]) {
        return PARSE_INVALID_VALUE;
      }
      i++;
    }
    c->json += i;
    v->type = t;
    return PARSE_OK;
  }

  static int parseNumber(pson_context *c, pson_value *v) {
    const char *p = c->json;
    if (*p == '-') p++; // 负号 - 直接跳过
    if (*p == '0') { // 0 - 直接跳过
      p++;
    } else {
      if (!IsDigit1To9(*p)) return PARSE_INVALID_VALUE;
      for (p++; IsDigit(*p); p++);
    }
    if (*p == '.') {
      p++;
      if (!IsDigit(*p)) return PARSE_INVALID_VALUE;
      for (p++; IsDigit(*p); p++);
    }
    if (*p == 'e' || *p == 'E') {
      p++;
      if (*p == '+' || *p == '-') p++;
      if (!IsDigit(*p)) return PARSE_INVALID_VALUE;
      for (p++; IsDigit(*p); p++);
    }
    errno = 0; // ???
    char *end;
    v->n = strtod(c->json, &end);
    // if (c->json == end) {
    //   return PARSE_INVALID_VALUE;
    // }
    // c->json = end;
    if (errno == ERANGE && (v->n == HUGE_VAL || v->n == -HUGE_VAL))
      return PARSE_NUMBER_TOO_BIG;
    v->type = JSON_NUMBER;
    c->json = p;
    return PARSE_OK;
  }

  static int parseValue(pson_context *c, pson_value *v) {
    // switch (c->json) { // ????
    switch (*c->json) {
      case 'n': return parseLiteral(c, v, "null", JSON_NULL);
      case 't': return parseLiteral(c, v, "true", JSON_TRUE);
      case 'f': return parseLiteral(c, v, "false", JSON_FALSE);
      case '\0': return PARSE_EXPECT_VALUE;
      default: return parseNumber(c, v);
    }
  }

  int parse(pson_value *v, const char* json) {
    pson_context c;
    int res;
    assert(v != NULL);
    c.json = json;
    v->type = JSON_NULL;
    parseWhitespace(&c);
    res = parseValue(&c, v);
    if (PARSE_OK == res) {
      parseWhitespace(&c);
      // C语言标准字符串以'\0'结束，空字符串首字符为'\0'
      if (*c.json != '\0') {
        return PARSE_ROOT_NOT_SINGULAR;
      }
    }
    return res;
  }

  pson_type getType(const pson_value *v) {
    assert(v != NULL);
    return v->type;
  }

  double getNumber(const pson_value *v) {
    assert(v != NULL && v->type == JSON_NUMBER);
    return v->n;
  }
}
