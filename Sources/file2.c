/*
 * file2.c
 *
 *  Created on: 2020��12��23��
 *      Author: XIABIN
 */


#include "file.h"

struct myStruct {
  int a;
};

void useObj(ptrMyStruct ptr) {
    (ptr->a)++;
}
