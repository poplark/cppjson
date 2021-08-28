#include "cppjson.h"
#include <assert.h>
#include <iostream>

#define Expect(c, ch) do { assert(*c->json == (ch)); c->json++; } while(0)

namespace cppjson {
  typedef struct {
    const char* json;
  } json_context;

  static void parseWhitespace(json_context *c) {
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') {
      p++;
    }
    c->json = p;
  }

  static int parseNull(json_context *c, json_value *v) {
    Expect(c, 'n');
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l') {
      return PARSE_INVALID_VALUE;
    }
    c->json += 3;
    v->type = JSON_NULL;
    return PARSE_OK;
  }

  static int parseValue(json_context *c, json_value *v) {
    // switch (c->json) { // ????
    switch (*c->json) {
      case 'n': return parseNull(c, v);
      case '\0': return PARSE_EXPECT_VALUE;
      default: return PARSE_INVALID_VALUE;
    }
  }

  int parse(json_value *v, const char* json) {
    json_context c;
    assert(v != NULL);
    c.json = json;
    v->type = JSON_NULL;
    parseWhitespace(&c);
    return parseValue(&c, v);
  }

  json_type getType(const json_value *v) {
    assert(v != NULL);
    return v->type;
  }
}
