/**
  * @File       scan.h
  * @brief      입력 파일을 lexical analysis, LALR1 parse 하기 위한 함수 선언
  * @details    선언한 함수는 flex를 사용하여 정의됨
  */
#ifndef _SCAN_H_
#define _SCAN_H_

#include "globals.h"

#define MAXTOKENLEN 40

extern char g_tokenString[MAXTOKENLEN + 1];

// defined at scan.l
TokenType getToken(void);

#endif
