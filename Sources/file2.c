/*
 * file2.c
 *
 *  Created on: 2020Äê12ÔÂ23ÈÕ
 *      Author: XIABIN
 */


#include "file.h"

struct myStruct {
  int a;
};

void useObj(ptrMyStruct ptr) {
    (ptr->a)++;
}
