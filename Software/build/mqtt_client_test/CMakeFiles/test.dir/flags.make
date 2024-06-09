# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.25

# compile ASM with M:/Apps/Raspberry Pi/Pico SDK v1.5.1/gcc-arm-none-eabi/bin/arm-none-eabi-gcc.exe
# compile C with M:/Apps/Raspberry Pi/Pico SDK v1.5.1/gcc-arm-none-eabi/bin/arm-none-eabi-gcc.exe
# compile CXX with M:/Apps/Raspberry Pi/Pico SDK v1.5.1/gcc-arm-none-eabi/bin/arm-none-eabi-g++.exe
ASM_DEFINES = -DCFG_TUSB_MCU=OPT_MCU_RP2040 -DCFG_TUSB_OS=OPT_OS_PICO -DCYW43_LWIP=0 -DCYW43_LWIP=1 -DLIB_PICO_ASYNC_CONTEXT_THREADSAFE_BACKGROUND=1 -DLIB_PICO_BIT_OPS=1 -DLIB_PICO_BIT_OPS_PICO=1 -DLIB_PICO_CYW43_ARCH=1 -DLIB_PICO_DIVIDER=1 -DLIB_PICO_DIVIDER_HARDWARE=1 -DLIB_PICO_DOUBLE=1 -DLIB_PICO_DOUBLE_PICO=1 -DLIB_PICO_FIX_RP2040_USB_DEVICE_ENUMERATION=1 -DLIB_PICO_FLOAT=1 -DLIB_PICO_FLOAT_PICO=1 -DLIB_PICO_INT64_OPS=1 -DLIB_PICO_INT64_OPS_PICO=1 -DLIB_PICO_MALLOC=1 -DLIB_PICO_MEM_OPS=1 -DLIB_PICO_MEM_OPS_PICO=1 -DLIB_PICO_PLATFORM=1 -DLIB_PICO_PRINTF=1 -DLIB_PICO_PRINTF_PICO=1 -DLIB_PICO_RAND=1 -DLIB_PICO_RUNTIME=1 -DLIB_PICO_STANDARD_LINK=1 -DLIB_PICO_STDIO=1 -DLIB_PICO_STDIO_USB=1 -DLIB_PICO_STDLIB=1 -DLIB_PICO_SYNC=1 -DLIB_PICO_SYNC_CRITICAL_SECTION=1 -DLIB_PICO_SYNC_MUTEX=1 -DLIB_PICO_SYNC_SEM=1 -DLIB_PICO_TIME=1 -DLIB_PICO_UNIQUE_ID=1 -DLIB_PICO_UTIL=1 -DPICO_BOARD=\"pico_w\" -DPICO_BUILD=1 -DPICO_CMAKE_BUILD_TYPE=\"Debug\" -DPICO_COPY_TO_RAM=0 -DPICO_CXX_ENABLE_EXCEPTIONS=0 -DPICO_CYW43_ARCH_THREADSAFE_BACKGROUND=1 -DPICO_NO_FLASH=0 -DPICO_NO_HARDWARE=0 -DPICO_ON_DEVICE=1 -DPICO_RP2040_USB_DEVICE_UFRAME_FIX=1 -DPICO_TARGET_NAME=\"test\" -DPICO_USE_BLOCKED_RAM=0

ASM_INCLUDES = -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_stdlib\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_gpio\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_base\include" -IC:\Users\johan\ONEDRI~1\SEMEST~3\ELEC~1.DIG\PROYEC~1\PicoHome\Software\build\GENERA~1\PICO_B~1 -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\boards\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_platform\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2040\hardware_regs\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_base\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2040\hardware_structs\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_claim\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_sync\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_irq\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_sync\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_time\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_timer\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_util\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_uart\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_resets\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_clocks\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_pll\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_vreg\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_watchdog\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_xosc\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_divider\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_runtime\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_printf\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_bit_ops\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_divider\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_double\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_float\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_malloc\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_bootrom\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_binary_info\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_stdio\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_stdio_usb\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_unique_id\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_flash\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_usb_reset_interface\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_int64_ops\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_mem_ops\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\boot_stage2\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\lib\tinyusb\src" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\lib\tinyusb\src\common" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\lib\tinyusb\hw" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_fix\rp2040_usb_device_enumeration\include" -IC:\Users\johan\ONEDRI~1\SEMEST~3\ELEC~1.DIG\PROYEC~1\PicoHome\Software\MQTT_C~2\..\MQTT_C~1\include -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_async_context\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_lwip\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_rand\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\lib\lwip\src\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_cyw43_arch\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\lib\cyw43-driver\src" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\lib\cyw43-driver\firmware" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_cyw43_driver\cybt_shared_bus" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_pio\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_dma\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_exception\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_cyw43_driver\include" -IC:\Users\johan\ONEDRI~1\SEMEST~3\ELEC~1.DIG\PROYEC~1\PicoHome\Software\build\pico-sdk\src\RP2_CO~1\PICO_C~1

ASM_FLAGS = -mcpu=cortex-m0plus -mthumb -Og -g -Wall -Wno-format -Wno-unused-function -Wno-maybe-uninitialized -ffunction-sections -fdata-sections

C_DEFINES = -DCFG_TUSB_MCU=OPT_MCU_RP2040 -DCFG_TUSB_OS=OPT_OS_PICO -DCYW43_LWIP=0 -DCYW43_LWIP=1 -DLIB_PICO_ASYNC_CONTEXT_THREADSAFE_BACKGROUND=1 -DLIB_PICO_BIT_OPS=1 -DLIB_PICO_BIT_OPS_PICO=1 -DLIB_PICO_CYW43_ARCH=1 -DLIB_PICO_DIVIDER=1 -DLIB_PICO_DIVIDER_HARDWARE=1 -DLIB_PICO_DOUBLE=1 -DLIB_PICO_DOUBLE_PICO=1 -DLIB_PICO_FIX_RP2040_USB_DEVICE_ENUMERATION=1 -DLIB_PICO_FLOAT=1 -DLIB_PICO_FLOAT_PICO=1 -DLIB_PICO_INT64_OPS=1 -DLIB_PICO_INT64_OPS_PICO=1 -DLIB_PICO_MALLOC=1 -DLIB_PICO_MEM_OPS=1 -DLIB_PICO_MEM_OPS_PICO=1 -DLIB_PICO_PLATFORM=1 -DLIB_PICO_PRINTF=1 -DLIB_PICO_PRINTF_PICO=1 -DLIB_PICO_RAND=1 -DLIB_PICO_RUNTIME=1 -DLIB_PICO_STANDARD_LINK=1 -DLIB_PICO_STDIO=1 -DLIB_PICO_STDIO_USB=1 -DLIB_PICO_STDLIB=1 -DLIB_PICO_SYNC=1 -DLIB_PICO_SYNC_CRITICAL_SECTION=1 -DLIB_PICO_SYNC_MUTEX=1 -DLIB_PICO_SYNC_SEM=1 -DLIB_PICO_TIME=1 -DLIB_PICO_UNIQUE_ID=1 -DLIB_PICO_UTIL=1 -DPICO_BOARD=\"pico_w\" -DPICO_BUILD=1 -DPICO_CMAKE_BUILD_TYPE=\"Debug\" -DPICO_COPY_TO_RAM=0 -DPICO_CXX_ENABLE_EXCEPTIONS=0 -DPICO_CYW43_ARCH_THREADSAFE_BACKGROUND=1 -DPICO_NO_FLASH=0 -DPICO_NO_HARDWARE=0 -DPICO_ON_DEVICE=1 -DPICO_RP2040_USB_DEVICE_UFRAME_FIX=1 -DPICO_TARGET_NAME=\"test\" -DPICO_USE_BLOCKED_RAM=0

C_INCLUDES = -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_stdlib\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_gpio\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_base\include" -IC:\Users\johan\ONEDRI~1\SEMEST~3\ELEC~1.DIG\PROYEC~1\PicoHome\Software\build\GENERA~1\PICO_B~1 -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\boards\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_platform\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2040\hardware_regs\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_base\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2040\hardware_structs\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_claim\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_sync\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_irq\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_sync\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_time\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_timer\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_util\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_uart\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_resets\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_clocks\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_pll\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_vreg\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_watchdog\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_xosc\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_divider\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_runtime\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_printf\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_bit_ops\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_divider\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_double\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_float\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_malloc\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_bootrom\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_binary_info\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_stdio\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_stdio_usb\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_unique_id\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_flash\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_usb_reset_interface\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_int64_ops\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_mem_ops\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\boot_stage2\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\lib\tinyusb\src" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\lib\tinyusb\src\common" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\lib\tinyusb\hw" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_fix\rp2040_usb_device_enumeration\include" -IC:\Users\johan\ONEDRI~1\SEMEST~3\ELEC~1.DIG\PROYEC~1\PicoHome\Software\MQTT_C~2\..\MQTT_C~1\include -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_async_context\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_lwip\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_rand\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\lib\lwip\src\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_cyw43_arch\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\lib\cyw43-driver\src" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\lib\cyw43-driver\firmware" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_cyw43_driver\cybt_shared_bus" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_pio\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_dma\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_exception\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_cyw43_driver\include" -IC:\Users\johan\ONEDRI~1\SEMEST~3\ELEC~1.DIG\PROYEC~1\PicoHome\Software\build\pico-sdk\src\RP2_CO~1\PICO_C~1

C_FLAGS = -mcpu=cortex-m0plus -mthumb -Og -g -Wall -Wno-format -Wno-unused-function -Wno-maybe-uninitialized -ffunction-sections -fdata-sections -std=gnu11

CXX_DEFINES = -DCFG_TUSB_MCU=OPT_MCU_RP2040 -DCFG_TUSB_OS=OPT_OS_PICO -DCYW43_LWIP=0 -DCYW43_LWIP=1 -DLIB_PICO_ASYNC_CONTEXT_THREADSAFE_BACKGROUND=1 -DLIB_PICO_BIT_OPS=1 -DLIB_PICO_BIT_OPS_PICO=1 -DLIB_PICO_CYW43_ARCH=1 -DLIB_PICO_DIVIDER=1 -DLIB_PICO_DIVIDER_HARDWARE=1 -DLIB_PICO_DOUBLE=1 -DLIB_PICO_DOUBLE_PICO=1 -DLIB_PICO_FIX_RP2040_USB_DEVICE_ENUMERATION=1 -DLIB_PICO_FLOAT=1 -DLIB_PICO_FLOAT_PICO=1 -DLIB_PICO_INT64_OPS=1 -DLIB_PICO_INT64_OPS_PICO=1 -DLIB_PICO_MALLOC=1 -DLIB_PICO_MEM_OPS=1 -DLIB_PICO_MEM_OPS_PICO=1 -DLIB_PICO_PLATFORM=1 -DLIB_PICO_PRINTF=1 -DLIB_PICO_PRINTF_PICO=1 -DLIB_PICO_RAND=1 -DLIB_PICO_RUNTIME=1 -DLIB_PICO_STANDARD_LINK=1 -DLIB_PICO_STDIO=1 -DLIB_PICO_STDIO_USB=1 -DLIB_PICO_STDLIB=1 -DLIB_PICO_SYNC=1 -DLIB_PICO_SYNC_CRITICAL_SECTION=1 -DLIB_PICO_SYNC_MUTEX=1 -DLIB_PICO_SYNC_SEM=1 -DLIB_PICO_TIME=1 -DLIB_PICO_UNIQUE_ID=1 -DLIB_PICO_UTIL=1 -DPICO_BOARD=\"pico_w\" -DPICO_BUILD=1 -DPICO_CMAKE_BUILD_TYPE=\"Debug\" -DPICO_COPY_TO_RAM=0 -DPICO_CXX_ENABLE_EXCEPTIONS=0 -DPICO_CYW43_ARCH_THREADSAFE_BACKGROUND=1 -DPICO_NO_FLASH=0 -DPICO_NO_HARDWARE=0 -DPICO_ON_DEVICE=1 -DPICO_RP2040_USB_DEVICE_UFRAME_FIX=1 -DPICO_TARGET_NAME=\"test\" -DPICO_USE_BLOCKED_RAM=0

CXX_INCLUDES = -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_stdlib\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_gpio\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_base\include" -IC:\Users\johan\ONEDRI~1\SEMEST~3\ELEC~1.DIG\PROYEC~1\PicoHome\Software\build\GENERA~1\PICO_B~1 -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\boards\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_platform\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2040\hardware_regs\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_base\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2040\hardware_structs\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_claim\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_sync\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_irq\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_sync\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_time\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_timer\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_util\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_uart\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_resets\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_clocks\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_pll\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_vreg\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_watchdog\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_xosc\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_divider\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_runtime\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_printf\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_bit_ops\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_divider\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_double\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_float\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_malloc\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_bootrom\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_binary_info\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_stdio\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_stdio_usb\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_unique_id\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_flash\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\common\pico_usb_reset_interface\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_int64_ops\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_mem_ops\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\boot_stage2\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\lib\tinyusb\src" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\lib\tinyusb\src\common" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\lib\tinyusb\hw" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_fix\rp2040_usb_device_enumeration\include" -IC:\Users\johan\ONEDRI~1\SEMEST~3\ELEC~1.DIG\PROYEC~1\PicoHome\Software\MQTT_C~2\..\MQTT_C~1\include -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_async_context\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_lwip\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_rand\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\lib\lwip\src\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_cyw43_arch\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\lib\cyw43-driver\src" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\lib\cyw43-driver\firmware" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_cyw43_driver\cybt_shared_bus" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_pio\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_dma\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\hardware_exception\include" -I"M:\Apps\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src\rp2_common\pico_cyw43_driver\include" -IC:\Users\johan\ONEDRI~1\SEMEST~3\ELEC~1.DIG\PROYEC~1\PicoHome\Software\build\pico-sdk\src\RP2_CO~1\PICO_C~1

CXX_FLAGS = -mcpu=cortex-m0plus -mthumb -Og -g -Wall -Wno-format -Wno-unused-function -Wno-maybe-uninitialized -ffunction-sections -fdata-sections -fno-exceptions -fno-unwind-tables -fno-rtti -fno-use-cxa-atexit -std=gnu++17

