#ifndef SL_COMPONENT_CATALOG_H
#define SL_COMPONENT_CATALOG_H

#include <target_platform.h>

// APIs present in project
#define SL_CATALOG_DEVICE_INIT_NVIC_PRESENT
#define SL_CATALOG_EMLIB_CORE_DEBUG_CONFIG_PRESENT
#define SL_CATALOG_LED0_PRESENT
#define SL_CATALOG_LED1_PRESENT
#define SL_CATALOG_SIMPLE_LED_PRESENT
#define SL_CATALOG_SIMPLE_LED_LED0_PRESENT
#define SL_CATALOG_SLEEPTIMER_PRESENT

#if HAL_WP_USE_SERIAL == TRUE

#define SL_CATALOG_IOSTREAM_UART_COMMON_PRESENT
#define SL_CATALOG_IOSTREAM_USART_PRESENT

#endif

// TODO
// #define SL_CATALOG_KERNEL_PRESENT

#if defined(I_AM_NANOCLR) && GECKO_FEATURE_USBD_HID == TRUE

#define SL_CATALOG_USB_DEVICE_PRESENT
#define SL_CATALOG_USB_DEVICE_HID_PRESENT
#define SL_CATALOG_USB_DEVICE_CONFIGURATION_PRESENT
#define SL_CATALOG_USB_DEVICE_CORE_PRESENT
#define SL_CATALOG_USB_DEVICE_DRIVER_PRESENT

#endif

#if defined(I_AM_NANOCLR) && GECKO_FEATURE_USBD_WINUSB == TRUE

#ifndef SL_CATALOG_USB_DEVICE_PRESENT
#define SL_CATALOG_USB_DEVICE_PRESENT
#endif

#ifndef SL_CATALOG_USB_DEVICE_CONFIGURATION_PRESENT
#define SL_CATALOG_USB_DEVICE_CONFIGURATION_PRESENT
#endif

#ifndef SL_CATALOG_USB_DEVICE_CORE_PRESENT
#define SL_CATALOG_USB_DEVICE_CORE_PRESENT
#endif

#ifndef SL_CATALOG_USB_DEVICE_DRIVER_PRESENT
#define SL_CATALOG_USB_DEVICE_DRIVER_PRESENT
#endif

#endif

#if HAL_WP_USE_USB_CDC == TRUE

#ifndef SL_CATALOG_USB_DEVICE_PRESENT
#define SL_CATALOG_USB_DEVICE_PRESENT
#endif

#define SL_CATALOG_USB_DEVICE_CDC_PRESENT
#define SL_CATALOG_USB_DEVICE_CDC_ACM_PRESENT

#ifndef SL_CATALOG_USB_DEVICE_CONFIGURATION_PRESENT
#define SL_CATALOG_USB_DEVICE_CONFIGURATION_PRESENT
#endif

#ifndef SL_CATALOG_USB_DEVICE_CORE_PRESENT
#define SL_CATALOG_USB_DEVICE_CORE_PRESENT
#endif

#ifndef SL_CATALOG_USB_DEVICE_DRIVER_PRESENT
#define SL_CATALOG_USB_DEVICE_DRIVER_PRESENT
#endif

#endif

#endif // SL_COMPONENT_CATALOG_H
