#include "cppjson.h"
#include <assert.h>
#include <iostream>

// 宏里有多过一个语句（statement），就需要用 do { /*...*/ } while(0) 包裹成单个语句
#define Expect(c, ch) do { assert(*c->json == (ch)); c->json++; } while(0)

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

  static int parseNull(pson_context *c, pson_value *v) {
    Expect(c, 'n');
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l') {
      return PARSE_INVALID_VALUE;
    }
    c->json += 3;
    parseWhitespace(c);
    // C语言标准字符串以'\0'结束，空字符串首字符为'\0'
    if (c->json[0] != '\0') {
      return PARSE_ROOT_NOT_SINGULAR;
    }
    v->type = JSON_NULL;
    return PARSE_OK;
  }

  static int parseTrue(pson_context *c, pson_value *v) {
    Expect(c, 't');
    if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e') {
      return PARSE_INVALID_VALUE;
    }
    c->json += 3;
    parseWhitespace(c);
    if (c->json[0] != '\0') {
      return PARSE_ROOT_NOT_SINGULAR;
    }
    v->type = JSON_TRUE;
    return PARSE_OK;
  }

  static int parseFalse(pson_context *c, pson_value *v) {
    Expect(c, 'f');
    if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e') {
      return PARSE_INVALID_VALUE;
    }
    c->json += 4;
    parseWhitespace(c);
    if (c->json[0] != '\0') {
      return PARSE_ROOT_NOT_SINGULAR;
    }
    v->type = JSON_FALSE;
    return PARSE_OK;
  }

  static int parseValue(pson_context *c, pson_value *v) {
    // switch (c->json) { // ????
    switch (*c->json) {
      case 'n': return parseNull(c, v);
      case 't': return parseTrue(c, v);
      case 'f': return parseFalse(c, v);
      case '\0': return PARSE_EXPECT_VALUE;
      default: return PARSE_INVALID_VALUE;
    }
  }

  int parse(pson_value *v, const char* json) {
    pson_context c;
    assert(v != NULL);
    c.json = json;
    v->type = JSON_NULL;
    parseWhitespace(&c);
    return parseValue(&c, v);
  }

  pson_type getType(const pson_value *v) {
    assert(v != NULL);
    return v->type;
  }
}
