#ifndef CPPJSON_H__
#define CPPJSON_H__

namespace cppjson {
  typedef enum {
    JSON_NULL,
    JSON_FALSE,
    JSON_TRUE,
    JSON_NUMBER,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT
  } json_type;

  typedef struct {
    json_type type;
  } json_value;

  enum {
    PARSE_OK = 0,
    PARSE_EXPECT_VALUE,
    PARSE_INVALID_VALUE,
    PARSE_ROOT_NOT_SINGULAR
  };

  int parse(json_value *v, const char* json);

  json_type getType(const json_value * v);
}
#endif
