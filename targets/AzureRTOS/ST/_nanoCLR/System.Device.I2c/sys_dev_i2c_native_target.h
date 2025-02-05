//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#ifndef SYS_DEV_I2C_NATIVE_TARGET_H
#define SYS_DEV_I2C_NATIVE_TARGET_H

#include <hal.h>
#include <targetPAL.h>
#include <nanoHAL.h>
#include <sys_dev_i2c_native.h>

// receiver thread
#define I2C_THREAD_STACK_SIZE 256
#define I2C_THREAD_PRIORITY   5

// struct representing the I2C
typedef struct NF_PAL_I2C_
{
    I2CDriver *Driver;
    I2CConfig Configuration;
    TX_THREAD *WorkingThread;
    uint32_t *WorkingThreadStack;
    msg_t TransactionResult;
    i2caddr_t Address;
    float ByteTime;

    uint8_t *WriteBuffer;
    uint8_t WriteSize;

    uint8_t *ReadBuffer;
    uint8_t ReadSize;
} NF_PAL_I2C;

////////////////////////////////////////////
// declaration of the the I2C PAL structs //
////////////////////////////////////////////
#if (STM32_I2C_USE_I2C1 == TRUE)
extern NF_PAL_I2C I2C1_PAL;
#endif
#if defined(STM32_I2C_USE_I2C2) && (STM32_I2C_USE_I2C2 == TRUE)
extern NF_PAL_I2C I2C2_PAL;
#endif
#if defined(STM32_I2C_USE_I2C3) && (STM32_I2C_USE_I2C3 == TRUE)
extern NF_PAL_I2C I2C3_PAL;
#endif
#if defined(STM32_I2C_USE_I2C4) && (STM32_I2C_USE_I2C4 == TRUE)
extern NF_PAL_I2C I2C4_PAL;
#endif

// the following macro defines a function that configures the GPIO pins for a STM32 I2C peripheral
// it gets called in the Windows_Devices_I2c_I2cDevice::NativeInit function
// this is required because the I2C peripherals can use multiple GPIO configuration combinations
#if defined(STM32F7XX)

#define I2C_CONFIG_PINS(num, gpio_port_scl, gpio_port_sda, scl_pin, sda_pin, alternate_function)                       \
    void ConfigPins_I2C##num()                                                                                         \
    {                                                                                                                  \
        palSetPadMode(                                                                                                 \
            gpio_port_scl,                                                                                             \
            scl_pin,                                                                                                   \
            (PAL_MODE_ALTERNATE(alternate_function) | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_OTYPE_OPENDRAIN));          \
        palSetPadMode(                                                                                                 \
            gpio_port_sda,                                                                                             \
            sda_pin,                                                                                                   \
            (PAL_MODE_ALTERNATE(alternate_function) | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_OTYPE_OPENDRAIN));          \
    }

#elif defined(STM32L4XX)

#define I2C_CONFIG_PINS(num, gpio_port_scl, gpio_port_sda, scl_pin, sda_pin, alternate_function)                       \
    void ConfigPins_I2C##num()                                                                                         \
    {                                                                                                                  \
        palSetPadMode(                                                                                                 \
            gpio_port_scl,                                                                                             \
            scl_pin,                                                                                                   \
            (PAL_MODE_ALTERNATE(alternate_function) | PAL_STM32_OSPEED_HIGH | PAL_STM32_OTYPE_OPENDRAIN));             \
        palSetPadMode(                                                                                                 \
            gpio_port_sda,                                                                                             \
            sda_pin,                                                                                                   \
            (PAL_MODE_ALTERNATE(alternate_function) | PAL_STM32_OSPEED_HIGH | PAL_STM32_OTYPE_OPENDRAIN));             \
    }

#else
#error "Series missing in macro definition"
#endif

//////////////////////////////////////////////////////////////////////////////////////////////
// when an I2C is defined the declarations below will have the real function/configuration //
// in the target folder @ target_windows_devices_i2c_config.cpp                             //
//////////////////////////////////////////////////////////////////////////////////////////////
void ConfigPins_I2C1();
void ConfigPins_I2C2();
void ConfigPins_I2C3();
void ConfigPins_I2C4();

#endif // SYS_DEV_I2C_NATIVE_TARGET_H