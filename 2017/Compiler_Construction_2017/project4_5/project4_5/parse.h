/**
  * @File       parse.h
  * @brief      입력 파일을 lexical analysis, LALR1 parse 하기 위한 함수 선언
  * @details    선언한 함수는 flex, yacc을 사용하여 정의됨
  */
#ifndef _PARSER_H_
#define _PARSER_H_

#include "globals.h"

// defined at parser.y
AST_node* parse(void);

#endif
