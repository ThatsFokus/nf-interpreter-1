//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#ifndef SYS_DEV_I2C_NATIVE_TARGET_H
#define SYS_DEV_I2C_NATIVE_TARGET_H

#include <sys_dev_i2c_native.h>
#include <string.h>
#include "Board.h"
#include <ti/drivers/I2C.h>

// struct representing the I2C
typedef struct
{
    I2C_Handle i2c;
    I2C_Params i2cParams;
    I2C_Transaction i2cTransaction;
} NF_PAL_I2C;

///////////////////////////////////////////
// declaration of the the I2C PAL strucs //
///////////////////////////////////////////
extern NF_PAL_I2C I2C1_PAL;

#endif //SYS_DEV_I2C_NATIVE_TARGET_H