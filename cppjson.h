// 利用宏加入 include 防范（include guard）
#ifndef PSON_H__
#define PSON_H__

namespace pson {
  typedef enum {
    JSON_NULL,
    JSON_FALSE,
    JSON_TRUE,
    JSON_NUMBER,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT
  } pson_type;

  typedef struct {
    double n;
    pson_type type;
  } pson_value;

  enum {
    PARSE_OK = 0,
    PARSE_EXPECT_VALUE,
    PARSE_INVALID_VALUE,
    PARSE_ROOT_NOT_SINGULAR,
    PARSE_NUMBER_TOO_BIG
  };

  int parse(pson_value *v, const char* json);

  pson_type getType(const pson_value * v);

  double getNumber(const pson_value *v);
}
#endif
