/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_ssd1309.h
 * @brief     driver ssd1309 header file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2024-10-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2024/10/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_SSD1309_H
#define DRIVER_SSD1309_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup ssd1309_driver ssd1309 driver function
 * @brief    ssd1309 driver modules
 * @{
 */

/**
 * @addtogroup ssd1309_base_driver
 * @{
 */

/**
 * @brief ssd1309 interface enumeration definition
 */
typedef enum
{
    SSD1309_INTERFACE_IIC = 0x00,        /**< interface iic */
    SSD1309_INTERFACE_SPI = 0x01,        /**< interface spi */
} ssd1309_interface_t;

/**
 * @brief ssd1309 address pin enumeration definition
 */
typedef enum  
{
    SSD1309_ADDR_SA0_0 = 0x78,        /**< address pin GND */
    SSD1309_ADDR_SA0_1 = 0x7A,        /**< address pin VCC */
} ssd1309_address_t;

/**
 * @brief ssd1309 bool enumeration definition
 */
typedef enum
{
    SSD1309_BOOL_FALSE = 0x00,        /**< false */
    SSD1309_BOOL_TRUE  = 0x01,        /**< true */
} ssd1309_bool_t;

/**
 * @brief ssd1309 memory addressing mode enumeration definition
 */
typedef enum  
{
    SSD1309_MEMORY_ADDRESSING_MODE_HORIZONTAL = 0x00,        /**< horizontal addressing mode */
    SSD1309_MEMORY_ADDRESSING_MODE_VERTICAL   = 0x01,        /**< vertical addressing mode */
    SSD1309_MEMORY_ADDRESSING_MODE_PAGE       = 0x02,        /**< page addressing mode */
} ssd1309_memory_addressing_mode_t;

/**
 * @brief ssd1309 scroll frame enumeration definition
 */
typedef enum  
{
    SSD1309_SCROLL_FRAME_1    = 0x07,        /**< scroll frame 1 */
    SSD1309_SCROLL_FRAME_2    = 0x04,        /**< scroll frame 2 */
    SSD1309_SCROLL_FRAME_3    = 0x05,        /**< scroll frame 3 */
    SSD1309_SCROLL_FRAME_4    = 0x06,        /**< scroll frame 4 */
    SSD1309_SCROLL_FRAME_5    = 0x00,        /**< scroll frame 5 */
    SSD1309_SCROLL_FRAME_64   = 0x01,        /**< scroll frame 64 */
    SSD1309_SCROLL_FRAME_128  = 0x02,        /**< scroll frame 128 */
    SSD1309_SCROLL_FRAME_256  = 0x03,        /**< scroll frame 256 */
} ssd1309_scroll_frame_t;

/**
 * @brief ssd1309 segment column remap enumeration definition
 */
typedef enum  
{
    SSD1309_SEGMENT_COLUMN_ADDRESS_0   = 0x00,        /**< segment column remap address 0 */
    SSD1309_SEGMENT_COLUMN_ADDRESS_127 = 0x01,        /**< segment column remap address 127 */
} ssd1309_segment_column_remap_t;

/**
 * @brief ssd1309 entire display enumeration definition
 */
typedef enum  
{
    SSD1309_ENTIRE_DISPLAY_OFF = 0x00,        /**< entire display off */
    SSD1309_ENTIRE_DISPLAY_ON  = 0x01,        /**< entire display on */
} ssd1309_entire_display_t;

/**
 * @brief ssd1309 display mode enumeration definition
 */
typedef enum  
{
    SSD1309_DISPLAY_MODE_NORMAL  = 0x00,        /**< display mode normal */
    SSD1309_DISPLAY_MODE_INVERSE = 0x01,        /**< display mode inverse */
} ssd1309_display_mode_t;

/**
 * @brief ssd1309 display enumeration definition
 */
typedef enum  
{
    SSD1309_DISPLAY_OFF = 0x00,        /**< close display */
    SSD1309_DISPLAY_ON  = 0x01,        /**< open display */
} ssd1309_display_t;

/**
 * @brief ssd1309 scan direction enumeration definition
 */
typedef enum  
{
    SSD1309_SCAN_DIRECTION_COM0_START   = 0x00,        /**< scan direction com 0 start */
    SSD1309_SCAN_DIRECTION_COMN_1_START = 0x01,        /**< scan direction com N-1 start */
} ssd1309_scan_direction_t;

/**
 * @brief ssd1309 pin conf enumeration definition
 */
typedef enum  
{
    SSD1309_PIN_CONF_SEQUENTIAL  = 0x00,        /**< pin conf sequential */
    SSD1309_PIN_CONF_ALTERNATIVE = 0x01,        /**< pin conf alternative */
} ssd1309_pin_conf_t;

/**
 * @brief ssd1309 left right remap enumeration definition
 */
typedef enum  
{
    SSD1309_LEFT_RIGHT_REMAP_DISABLE = 0x00,        /**< disable left right remap */
    SSD1309_LEFT_RIGHT_REMAP_ENABLE  = 0x01,        /**< enable left right remap */
} ssd1309_left_right_remap_t;

/**
 * @brief ssd1309 deselect level enumeration definition
 */
typedef enum  
{
    SSD1309_DESELECT_LEVEL_0P64 = 0x00,        /**< deselect level 0.64 */
    SSD1309_DESELECT_LEVEL_0P78 = 0x0D,        /**< deselect level 0.78 */
    SSD1309_DESELECT_LEVEL_0P84 = 0x0F,        /**< deselect level 0.84 */
} ssd1309_deselect_level_t;

/**
 * @brief ssd1309 gpio enumeration definition
 */
typedef enum  
{
    SSD1309_GPIO_HIZ_INPUT_DISABLE = 0x00,        /**< input disabled */
    SSD1309_GPIO_HIZ_INPUT_ENABLE  = 0x01,        /**< input enabled */
    SSD1309_GPIO_OUTPUT_LOW        = 0x02,        /**< output low */
    SSD1309_GPIO_OUTPUT_HIGH       = 0x03,        /**< output high */
} ssd1309_gpio_t;

/**
 * @brief ssd1309 font enumeration definition
 */
typedef enum  
{
    SSD1309_FONT_12 = 0x0C,        /**< font 12 */
    SSD1309_FONT_16 = 0x10,        /**< font 16 */
    SSD1309_FONT_24 = 0x18,        /**< font 24 */
} ssd1309_font_t;

/**
 * @brief ssd1309 handle structure definition
 */
typedef struct ssd1309_handle_s
{
    uint8_t (*iic_init)(void);                                                          /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                                        /**< point to an iic_deinit function address */
    uint8_t (*iic_write)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);        /**< point to an iic_write function address */
    uint8_t (*spi_init)(void);                                                          /**< point to a spi_init function address */
    uint8_t (*spi_deinit)(void);                                                        /**< point to a spi_deinit function address */
    uint8_t (*spi_write_cmd)(uint8_t *buf, uint16_t len);                               /**< point to a spi_write_cmd function address */
    uint8_t (*spi_cmd_data_gpio_init)(void);                                            /**< point to a spi_cmd_data_gpio_init function address */
    uint8_t (*spi_cmd_data_gpio_deinit)(void);                                          /**< point to a spi_cmd_data_gpio_deinit function address */
    uint8_t (*spi_cmd_data_gpio_write)(uint8_t value);                                  /**< point to a spi_cmd_data_gpio_write function address */
    uint8_t (*reset_gpio_init)(void);                                                   /**< point to a reset_gpio_init function address */
    uint8_t (*reset_gpio_deinit)(void);                                                 /**< point to a reset_gpio_deinit function address */
    uint8_t (*reset_gpio_write)(uint8_t value);                                         /**< point to a reset_gpio_write function address */
    void (*debug_print)(const char *const fmt, ...);                                    /**< point to a debug_print function address */
    void (*delay_ms)(uint32_t ms);                                                      /**< point to a delay_ms function address */
    uint8_t inited;                                                                     /**< inited flag */
    uint8_t iic_addr;                                                                   /**< iic address */
    uint8_t iic_spi;                                                                    /**< iic spi type */
    uint8_t gram[128][8];                                                               /**< gram buffer */
} ssd1309_handle_t;

/**
 * @brief ssd1309 information structure definition
 */
typedef struct ssd1309_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} ssd1309_info_t;

/**
 * @}
 */

/**
 * @defgroup ssd1309_link_driver ssd1309 link driver function
 * @brief    ssd1309 link driver modules
 * @ingroup  ssd1309_driver
 * @{
 */

/**
 * @brief     initialize ssd1309_handle_t structure
 * @param[in] HANDLE points to an ssd1309 handle structure
 * @param[in] STRUCTURE is ssd1309_handle_t
 * @note      none
 */
#define DRIVER_SSD1309_LINK_INIT(HANDLE, STRUCTURE)                     memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE points to an ssd1309 handle structure
 * @param[in] FUC points to an iic_init function address
 * @note      none
 */
#define DRIVER_SSD1309_LINK_IIC_INIT(HANDLE, FUC)                      (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE points to an ssd1309 handle structure
 * @param[in] FUC points to an iic_deinit function address
 * @note      none
 */
#define DRIVER_SSD1309_LINK_IIC_DEINIT(HANDLE, FUC)                    (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_write function
 * @param[in] HANDLE points to an ssd1309 handle structure
 * @param[in] FUC points to an iic_write function address
 * @note      none
 */
#define DRIVER_SSD1309_LINK_IIC_WRITE(HANDLE, FUC)                     (HANDLE)->iic_write = FUC

/**
 * @brief     link spi_init function
 * @param[in] HANDLE points to an ssd1309 handle structure
 * @param[in] FUC points to a spi_init function address
 * @note      none
 */
#define DRIVER_SSD1309_LINK_SPI_INIT(HANDLE, FUC)                      (HANDLE)->spi_init = FUC

/**
 * @brief     link spi_deinit function
 * @param[in] HANDLE points to an ssd1309 handle structure
 * @param[in] FUC points to a spi_deinit function address
 * @note      none
 */
#define DRIVER_SSD1309_LINK_SPI_DEINIT(HANDLE, FUC)                    (HANDLE)->spi_deinit = FUC

/**
 * @brief     link spi_write_cmd function
 * @param[in] HANDLE points to an ssd1309 handle structure
 * @param[in] FUC points to a spi_write_cmd function address
 * @note      none
 */
#define DRIVER_SSD1309_LINK_SPI_WRITE_COMMAND(HANDLE, FUC)             (HANDLE)->spi_write_cmd = FUC

/**
 * @brief     link spi_cmd_data_gpio_init function
 * @param[in] HANDLE points to an ssd1309 handle structure
 * @param[in] FUC points to a spi_cmd_data_gpio_init function address
 * @note      none
 */
#define DRIVER_SSD1309_LINK_SPI_COMMAND_DATA_GPIO_INIT(HANDLE, FUC)    (HANDLE)->spi_cmd_data_gpio_init = FUC

/**
 * @brief     link spi_cmd_data_gpio_deinit function
 * @param[in] HANDLE points to an ssd1309 handle structure
 * @param[in] FUC points to a spi_cmd_data_gpio_deinit function address
 * @note      none
 */
#define DRIVER_SSD1309_LINK_SPI_COMMAND_DATA_GPIO_DEINIT(HANDLE, FUC)  (HANDLE)->spi_cmd_data_gpio_deinit = FUC

/**
 * @brief     link spi_cmd_data_gpio_write function
 * @param[in] HANDLE points to an ssd1309 handle structure
 * @param[in] FUC points to a spi_cmd_data_gpio_write function address
 * @note      none
 */
#define DRIVER_SSD1309_LINK_SPI_COMMAND_DATA_GPIO_WRITE(HANDLE, FUC)   (HANDLE)->spi_cmd_data_gpio_write = FUC

/**
 * @brief     link reset_gpio_init function
 * @param[in] HANDLE points to an ssd1309 handle structure
 * @param[in] FUC points to a reset_gpio_init function address
 * @note      none
 */
#define DRIVER_SSD1309_LINK_RESET_GPIO_INIT(HANDLE, FUC)               (HANDLE)->reset_gpio_init = FUC

/**
 * @brief     link reset_gpio_deinit function
 * @param[in] HANDLE points to an ssd1309 handle structure
 * @param[in] FUC points to a reset_gpio_deinit function address
 * @note      none
 */
#define DRIVER_SSD1309_LINK_RESET_GPIO_DEINIT(HANDLE, FUC)             (HANDLE)->reset_gpio_deinit = FUC

/**
 * @brief     link reset_gpio_write function
 * @param[in] HANDLE points to an ssd1309 handle structure
 * @param[in] FUC points to a reset_gpio_write function address
 * @note      none
 */
#define DRIVER_SSD1309_LINK_RESET_GPIO_WRITE(HANDLE, FUC)              (HANDLE)->reset_gpio_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to an ssd1309 handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_SSD1309_LINK_DELAY_MS(HANDLE, FUC)                      (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to an ssd1309 handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_SSD1309_LINK_DEBUG_PRINT(HANDLE, FUC)                   (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup ssd1309_base_driver ssd1309 base driver function
 * @brief    ssd1309 base driver modules
 * @ingroup  ssd1309_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info points to an ssd1309 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ssd1309_info(ssd1309_info_t *info);

/**
 * @brief     set the chip interface
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t ssd1309_set_interface(ssd1309_handle_t *handle, ssd1309_interface_t interface);

/**
 * @brief      get the chip interface
 * @param[in]  *handle points to an ssd1309 handle structure
 * @param[out] *interface points to a chip interface buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ssd1309_get_interface(ssd1309_handle_t *handle, ssd1309_interface_t *interface);

/**
 * @brief     set the chip iic address
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] addr_pin is the iic address
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t ssd1309_set_addr_pin(ssd1309_handle_t *handle, ssd1309_address_t addr_pin);

/**
 * @brief      get the chip iic address
 * @param[in]  *handle points to an ssd1309 handle structure
 * @param[out] *addr_pin points to an iic address buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ssd1309_get_addr_pin(ssd1309_handle_t *handle, ssd1309_address_t *addr_pin);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to an ssd1309 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic or spi initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 reset failed
 *            - 5 command && data init failed
 *            - 6 interface param is invalid
 * @note      none
 */
uint8_t ssd1309_init(ssd1309_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to an ssd1309 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic or spi deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 power down failed
 *            - 5 reset gpio deinit failed
 *            - 6 command && data deinit failed
 *            - 7 interface param is invalid
 * @note      none
 */
uint8_t ssd1309_deinit(ssd1309_handle_t *handle);

/**
 * @brief     clear the screen
 * @param[in] *handle points to an ssd1309 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 clear failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1309_clear(ssd1309_handle_t *handle);

/**
 * @brief     write a point
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] x is the coordinate x
 * @param[in] y is the coordinate y
 * @param[in] data is the write data
 * @return    status code
 *            - 0 success
 *            - 1 write point failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 x or y is invalid
 * @note      none
 */
uint8_t ssd1309_write_point(ssd1309_handle_t *handle, uint8_t x, uint8_t y, uint8_t data);

/**
 * @brief      read a point
 * @param[in]  *handle points to an ssd1309 handle structure
 * @param[in]  x is the coordinate x
 * @param[in]  y is the coordinate y
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read point failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 x or y is invalid
 * @note       none
 */
uint8_t ssd1309_read_point(ssd1309_handle_t *handle, uint8_t x, uint8_t y, uint8_t *data);

/**
 * @brief     write a point in the gram
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] x is the coordinate x
 * @param[in] y is the coordinate y
 * @param[in] data is the write data
 * @return    status code
 *            - 0 success
 *            - 1 gram write point failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 x or y is invalid
 * @note      none
 */
uint8_t ssd1309_gram_write_point(ssd1309_handle_t *handle, uint8_t x, uint8_t y, uint8_t data);

/**
 * @brief      read a point from the gram
 * @param[in]  *handle points to an ssd1309 handle structure
 * @param[in]  x is the coordinate x
 * @param[in]  y is the coordinate y
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 gram read point failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 x or y is invalid
 * @note       none
 */
uint8_t ssd1309_gram_read_point(ssd1309_handle_t *handle, uint8_t x, uint8_t y, uint8_t *data);

/**
 * @brief     draw a string in the gram
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] x is the coordinate x
 * @param[in] y is the coordinate y
 * @param[in] *str points to a write string address
 * @param[in] len is the length of the string
 * @param[in] color is the display color
 * @param[in] font is the display font size
 * @return    status code
 *            - 0 success
 *            - 1 gram write string failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 x or y is invalid
 * @note      none
 */
uint8_t ssd1309_gram_write_string(ssd1309_handle_t *handle, uint8_t x, uint8_t y, char *str, uint16_t len, uint8_t color, ssd1309_font_t font);

/**
 * @brief     fill a rectangle in the gram
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] left is the left coordinate x
 * @param[in] top is the top coordinate y
 * @param[in] right is the right coordinate x
 * @param[in] bottom is the bottom coordinate y
 * @param[in] color is the display color
 * @return    status code
 *            - 0 success
 *            - 1 gram fill rect failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 left or top is invalid
 *            - 5 right or bottom is invalid
 *            - 6 left > right or top > bottom
 * @note      none
 */
uint8_t ssd1309_gram_fill_rect(ssd1309_handle_t *handle, uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint8_t color);

/**
 * @brief     draw a picture in the gram
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] left is the left coordinate x
 * @param[in] top is the top coordinate y
 * @param[in] right is the right coordinate x
 * @param[in] bottom is the bottom coordinate y
 * @param[in] *img points to an image buffer
 * @return    status code
 *            - 0 success
 *            - 1 gram draw picture failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 left or top is invalid
 *            - 5 right or bottom is invalid
 *            - 6 left > right or top > bottom
 * @note      none
 */
uint8_t ssd1309_gram_draw_picture(ssd1309_handle_t *handle, uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint8_t *img);

/**
 * @brief     update the gram data
 * @param[in] *handle points to an ssd1309 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 gram update failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1309_gram_update(ssd1309_handle_t *handle);

/**
 * @brief     set the low column start address
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] addr is the low column start address
 * @return    status code
 *            - 0 success
 *            - 1 set low column start address failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 addr is invalid
 * @note      addr <= 0xF
 */
uint8_t ssd1309_set_low_column_start_address(ssd1309_handle_t *handle, uint8_t addr);

/**
 * @brief     set the high column start address
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] addr is the high column start address
 * @return    status code
 *            - 0 success
 *            - 1 set high column start address failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 addr is invalid
 * @note      addr <= 0xF
 */
uint8_t ssd1309_set_high_column_start_address(ssd1309_handle_t *handle, uint8_t addr);

/**
 * @brief     set the memory addressing mode
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] mode is the memory addressing mode
 * @return    status code
 *            - 0 success
 *            - 1 set memory addressing mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1309_set_memory_addressing_mode(ssd1309_handle_t *handle, ssd1309_memory_addressing_mode_t mode);

/**
 * @brief     set the column address range
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] start_addr is the column start address
 * @param[in] end_addr is the column end address
 * @return    status code
 *            - 0 success
 *            - 1 set column address range failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 start addr is invalid
 *            - 5 end addr is invalid
 * @note      start addr and end addr can't be over 0x7F
 */
uint8_t ssd1309_set_column_address_range(ssd1309_handle_t *handle, uint8_t start_addr, uint8_t end_addr);

/**
 * @brief     set the page address range
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] start_addr is the page start address
 * @param[in] end_addr is the page end address
 * @return    status code
 *            - 0 success
 *            - 1 set page address range failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 start addr is invalid
 *            - 5 end addr is invalid
 * @note      start addr and end addr can't be over 0x07
 */
uint8_t ssd1309_set_page_address_range(ssd1309_handle_t *handle, uint8_t start_addr, uint8_t end_addr);

/**
 * @brief     set the right horizontal scroll
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] start_page_addr is the start page address
 * @param[in] end_page_addr is the end page address
 * @param[in] frames is the scroll frames
 * @param[in] start_column_addr is the start column address
 * @param[in] end_column_addr is the end column address
 * @return    status code
 *            - 0 success
 *            - 1 set right horizontal scroll failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 start page addr is invalid
 *            - 5 end page addr is invalid
 *            - 6 start column addr is invalid
 *            - 7 end column addr is invalid
 * @note      start_page_addr <= 0x07
 *            end_page_addr <= 0x07
 *            start_column_addr <= 0x7F
 *            end_column_addr <= 0x7F
 */
uint8_t ssd1309_set_right_horizontal_scroll(ssd1309_handle_t *handle, uint8_t start_page_addr, 
                                            uint8_t end_page_addr, ssd1309_scroll_frame_t frames, 
                                            uint8_t start_column_addr, uint8_t end_column_addr);

/**
 * @brief     set the left horizontal scroll
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] start_page_addr is the start page address
 * @param[in] end_page_addr is the end page address
 * @param[in] frames is the scroll frames
 * @param[in] start_column_addr is the start column address
 * @param[in] end_column_addr is the end column address
 * @return    status code
 *            - 0 success
 *            - 1 set left horizontal scroll failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 start page addr is invalid
 *            - 5 end page addr is invalid
 *            - 6 start column addr is invalid
 *            - 7 end column addr is invalid
 * @note      start_page_addr <= 0x07
 *            end_page_addr <= 0x07
 *            start_column_addr <= 0x7F
 *            end_column_addr <= 0x7F
 */
uint8_t ssd1309_set_left_horizontal_scroll(ssd1309_handle_t *handle, uint8_t start_page_addr,
                                           uint8_t end_page_addr, ssd1309_scroll_frame_t frames,
                                           uint8_t start_column_addr, uint8_t end_column_addr);

/**
 * @brief     set the vertical right horizontal scroll
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] horizontal_scroll is a bool value
 * @param[in] start_page_addr is the start page address
 * @param[in] end_page_addr is the end page address
 * @param[in] rows is the row address
 * @param[in] frames is the scroll frames
 * @param[in] start_column_addr is the start column address
 * @param[in] end_column_addr is the end column address
 * @return    status code
 *            - 0 success
 *            - 1 set vertical right horizontal scroll failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 start_page_addr is invalid
 *            - 5 end_page_addr is invalid
 *            - 6 rows is invalid
 *            - 7 start column addr is invalid
 *            - 8 end column addr is invalid
 * @note      start_page_addr <= 0x07
              end_page_addr <= 0x07
              rows <= 0x3F
 *            start_column_addr <= 0x7F
 *            end_column_addr <= 0x7F
 */
uint8_t ssd1309_set_vertical_right_horizontal_scroll(ssd1309_handle_t *handle, ssd1309_bool_t horizontal_scroll, 
                                                     uint8_t start_page_addr, uint8_t end_page_addr, 
                                                     uint8_t rows, ssd1309_scroll_frame_t frames,
                                                     uint8_t start_column_addr, uint8_t end_column_addr);

/**
 * @brief     set the vertical left horizontal scroll
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] horizontal_scroll is a bool value
 * @param[in] start_page_addr is the start page address
 * @param[in] end_page_addr is the end page address
 * @param[in] rows is the row address
 * @param[in] frames is the scroll frames
 * @param[in] start_column_addr is the start column address
 * @param[in] end_column_addr is the end column address
 * @return    status code
 *            - 0 success
 *            - 1 set vertical left horizontal scroll failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 start_page_addr is invalid
 *            - 5 end_page_addr is invalid
 *            - 6 rows is invalid
 *            - 7 start column addr is invalid
 *            - 8 end column addr is invalid
 * @note      start_page_addr <= 0x07
              end_page_addr <= 0x07
              rows <= 0x3F
 *            start_column_addr <= 0x7F
 *            end_column_addr <= 0x7F
 */
uint8_t ssd1309_set_vertical_left_horizontal_scroll(ssd1309_handle_t *handle, ssd1309_bool_t horizontal_scroll, 
                                                    uint8_t start_page_addr, uint8_t end_page_addr, 
                                                    uint8_t rows, ssd1309_scroll_frame_t frames,
                                                    uint8_t start_column_addr, uint8_t end_column_addr);

/**
 * @brief     deactivate the scroll
 * @param[in] *handle points to an ssd1309 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 deactivate scroll failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1309_deactivate_scroll(ssd1309_handle_t *handle);

/**
 * @brief     activate the scroll
 * @param[in] *handle points to an ssd1309 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 activate scroll failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1309_activate_scroll(ssd1309_handle_t *handle);

/**
 * @brief     set the display start line
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] l is the start line
 * @return    status code
 *            - 0 success
 *            - 1 set display start line failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 line is invalid
 * @note      line <= 0x3F
 */
uint8_t ssd1309_set_display_start_line(ssd1309_handle_t *handle, uint8_t l);

/**
 * @brief     set the display contrast
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] contrast is the display contrast
 * @return    status code
 *            - 0 success
 *            - 1 set contrast failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1309_set_contrast(ssd1309_handle_t *handle, uint8_t contrast);

/**
 * @brief     set the segment remap
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] remap is the segment remap param
 * @return    status code
 *            - 0 success
 *            - 1 set segment remap failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1309_set_segment_remap(ssd1309_handle_t *handle, ssd1309_segment_column_remap_t remap);

/**
 * @brief     set the vertical scroll area
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] start_row is the start row
 * @param[in] end_row is the end row
 * @return    status code
 *            - 0 success
 *            - 1 set vertical scroll area failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 start_row is invalid
 *            - 5 end_row is invalid
 *            - 6 end_row > start_row
 * @note      start_row <= 0x3F
 *            end_row <= 0x7F
 *            start_row >= end_row
 */
uint8_t ssd1309_set_vertical_scroll_area(ssd1309_handle_t *handle, uint8_t start_row, uint8_t end_row);

/**
 * @brief     enable or disable the entire display
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set entire display failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1309_set_entire_display(ssd1309_handle_t *handle, ssd1309_entire_display_t enable);

/**
 * @brief     set the display mode
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] mode is the display mode
 * @return    status code
 *            - 0 success
 *            - 1 set display mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1309_set_display_mode(ssd1309_handle_t *handle, ssd1309_display_mode_t mode);

/**
 * @brief     set the multiplex ratio
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] multiplex is the multiplex ratio
 * @return    status code
 *            - 0 success
 *            - 1 set multiplex ratio failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 multiplex is too small
 *            - 5 multiplex is too large
 * @note      multiplex must be over 0x0E and less than 0x40
 */
uint8_t ssd1309_set_multiplex_ratio(ssd1309_handle_t *handle, uint8_t multiplex);

/**
 * @brief     enable or disable the display
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] on_off is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set display failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1309_set_display(ssd1309_handle_t *handle, ssd1309_display_t on_off);

/**
 * @brief     set the page address
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] addr is the page address
 * @return    status code
 *            - 0 success
 *            - 1 set page address failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 addr is invalid
 * @note      addr <= 0x07
 */
uint8_t ssd1309_set_page_address(ssd1309_handle_t *handle, uint8_t addr);

/**
 * @brief     set the scan direction
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] dir is the scan direction
 * @return    status code
 *            - 0 success
 *            - 1 set scan direction failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1309_set_scan_direction(ssd1309_handle_t *handle, ssd1309_scan_direction_t dir);

/**
 * @brief     set the display offset
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] offset is the display offset
 * @return    status code
 *            - 0 success
 *            - 1 set display offset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 offset is invalid
 * @note      offset <= 0x3F
 */
uint8_t ssd1309_set_display_offset(ssd1309_handle_t *handle, uint8_t offset);

/**
 * @brief     set the display clock
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] oscillator_frequency is the oscillator frequency
 * @param[in] clock_divide is the clock divide
 * @return    status code
 *            - 0 success
 *            - 1 set display clock failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 oscillator frequency is invalid
 *            - 5 clock divide is invalid
 * @note      oscillator_frequency <= 0x0F
 *            clock_divide <= 0x0F
 */
uint8_t ssd1309_set_display_clock(ssd1309_handle_t *handle, uint8_t oscillator_frequency, uint8_t clock_divide);

/**
 * @brief     set the pre charge period
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] phase1_period is the phase1 period
 * @param[in] phase2_period is the phase2 period
 * @return    status code
 *            - 0 success
 *            - 1 set pre charge period failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 phase1 period is invalid
 *            - 5 phase2 period is invalid
 * @note      phase1_period <= 0x0F
 *            phase2_period <= 0x0F
 */
uint8_t ssd1309_set_precharge_period(ssd1309_handle_t *handle, uint8_t phase1_period, uint8_t phase2_period);

/**
 * @brief     set the hardware com pins
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] conf is the pin conf
 * @param[in] remap is the left right remap
 * @return    status code
 *            - 0 success
 *            - 1 set com pins hardware conf failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1309_set_com_pins_hardware_conf(ssd1309_handle_t *handle, ssd1309_pin_conf_t conf, ssd1309_left_right_remap_t remap);

/**
 * @brief     set the deselect level
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] level is the deselect level
 * @return    status code
 *            - 0 success
 *            - 1 set deselect level failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1309_set_deselect_level(ssd1309_handle_t *handle, ssd1309_deselect_level_t level);

/**
 * @brief     set right horizontal scroll one column
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] start_page_addr is the start page address
 * @param[in] end_page_addr is the end page address
 * @param[in] start_column_addr is the start column address
 * @param[in] end_column_addr is the end column address
 * @return    status code
 *            - 0 success
 *            - 1 set right horizontal scroll one column failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 start page addr is invalid
 *            - 5 end page addr is invalid
 *            - 6 start column addr is invalid
 *            - 7 end column addr is invalid
 * @note      start_page_addr <= 0x07
 *            end_page_addr <= 0x07
 *            start_column_addr <= 0x7F
 *            end_column_addr <= 0x7F
 */
uint8_t ssd1309_set_right_horizontal_scroll_one_column(ssd1309_handle_t *handle, uint8_t start_page_addr, 
                                                       uint8_t end_page_addr, uint8_t start_column_addr,
                                                       uint8_t end_column_addr);

/**
 * @brief     set left horizontal scroll one column
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] start_page_addr is the start page address
 * @param[in] end_page_addr is the end page address
 * @param[in] start_column_addr is the start column address
 * @param[in] end_column_addr is the end column address
 * @return    status code
 *            - 0 success
 *            - 1 set left horizontal scroll one column failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 start page addr is invalid
 *            - 5 end page addr is invalid
 *            - 6 start column addr is invalid
 *            - 7 end column addr is invalid
 * @note      start_page_addr <= 0x07
 *            end_page_addr <= 0x07
 *            start_column_addr <= 0x7F
 *            end_column_addr <= 0x7F
 */
uint8_t ssd1309_set_left_horizontal_scroll_one_column(ssd1309_handle_t *handle, uint8_t start_page_addr, 
                                                      uint8_t end_page_addr, uint8_t start_column_addr,
                                                      uint8_t end_column_addr);

/**
 * @brief     set gpio
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] gpio is the gpio config
 * @return    status code
 *            - 0 success
 *            - 1 set gpio failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1309_set_gpio(ssd1309_handle_t *handle, ssd1309_gpio_t gpio);

/**
 * @brief     enable or disable mcu interface lock
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set mcu interface lock failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1309_set_mcu_interface_lock(ssd1309_handle_t *handle, ssd1309_bool_t enable);

/**
 * @}
 */

/**
 * @defgroup ssd1309_extend_driver ssd1309 extend driver function
 * @brief    ssd1309 extend driver modules
 * @ingroup  ssd1309_driver
 * @{
 */

/**
 * @brief     write the register command
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1309_write_cmd(ssd1309_handle_t *handle, uint8_t *buf, uint8_t len);

/**
 * @brief     write the register data
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1309_write_data(ssd1309_handle_t *handle, uint8_t *buf, uint8_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
