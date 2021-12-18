/*
 * file1.c
 *
 *  Created on: 2020Äê12ÔÂ23ÈÕ
 *      Author: XIABIN
 */

#include "gpio.h"
#include "DataType.h"
#include "GlobalDefine.h"

const uint8 PRODUCTTYPE[4] __attribute__ ((section(".ecutype"))) = {PRODUCT_TYPE_TYPE,(uint8)(PRODUCT_TYPE_NUM/256), (uint8)(PRODUCT_TYPE_NUM&0xff), PRODUCT_TYPE_SUFFIX};
const uint8 SWVERDATA[4] __attribute__ ((section(".SWVER"))) = {SwV_,SwsV_,SVN_LOG/256,SVN_LOG&0xff};
const uint8 HWCSWVERDATA[4] __attribute__ ((section(".HWCSWVER"))) = {SwV_CLIENT, SwsV_CLIENT, HwV_, HwsV_};

void func() {

}
