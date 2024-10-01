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
 * @file      driver_ssd1309.c
 * @brief     driver ssd1309 source file
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

#include "driver_ssd1309.h"
#include "driver_ssd1309_font.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Solomon Systech SSD1309"        /**< chip name */
#define MANUFACTURER_NAME         "Solomon Systech"                /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        1.65f                            /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        3.3f                             /**< chip max supply voltage */
#define MAX_CURRENT               0.58f                            /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                           /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                            /**< chip max operating temperature */
#define DRIVER_VERSION            1000                             /**< driver version */

/**
 * @brief chip command data definition
 */
#define SSD1309_CMD          0        /**< command */
#define SSD1309_DATA         1        /**< data */

/**
 * @brief chip command definition
 */
#define SSD1309_CMD_LOWER_COLUMN_START_ADDRESS              0x00        /**< command lower column start address */
#define SSD1309_CMD_HIGHER_COLUMN_START_ADDRESS             0x10        /**< command higher column start address */
#define SSD1309_CMD_MEMORY_ADDRESSING_MODE                  0x20        /**< command memory addressing mode */
#define SSD1309_CMD_SET_COLUMN_ADDRESS                      0x21        /**< command set column address */
#define SSD1309_CMD_SET_PAGE_ADDRESS                        0x22        /**< command set page address */
#define SSD1309_CMD_RIGHT_HORIZONTAL_SCROLL                 0x26        /**< command right horizontal scroll */
#define SSD1309_CMD_LEFT_HORIZONTAL_SCROLL                  0x27        /**< command left horizontal scroll */
#define SSD1309_CMD_VERTICAL_RIGHT_HORIZONTAL_SCROLL        0x29        /**< command vertical right horizontal scroll */
#define SSD1309_CMD_VERTICAL_LEFT_HORIZONTAL_SCROLL         0x2A        /**< command vertical left horizontal scroll */
#define SSD1309_CMD_RIGHT_HORIZONTAL_SCROLL_ONE_COL         0x2C        /**< command right horizontal scroll by one column */
#define SSD1309_CMD_LEFT_HORIZONTAL_SCROLL_ONE_COL          0x2D        /**< command left horizontal scroll by one column */
#define SSD1309_CMD_DEACTIVATE_SCROLL                       0x2E        /**< command deactivate scroll */
#define SSD1309_CMD_ACTIVATE_SCROLL                         0x2F        /**< command activate scroll */
#define SSD1309_CMD_DISPLAY_START_LINE                      0x40        /**< command display start line */
#define SSD1309_CMD_CONTRAST_CONTROL                        0x81        /**< command contrast control */
#define SSD1309_CMD_COLUMN_0_MAPPED_TO_SEG0                 0xA0        /**< command column 0 mapped to seg 0 */
#define SSD1309_CMD_COLUMN_127_MAPPED_TO_SEG0               0xA1        /**< command column 127 mapped to seg 0 */
#define SSD1309_CMD_VERTICAL_SCROLL_AREA                    0xA3        /**< command vertical scroll area */
#define SSD1309_CMD_ENTIRE_DISPLAY_OFF                      0xA4        /**< command entire display off */ 
#define SSD1309_CMD_ENTIRE_DISPLAY_ON                       0xA5        /**< command entire display on */ 
#define SSD1309_CMD_NORMAL_DISPLAY                          0xA6        /**< command normal display */ 
#define SSD1309_CMD_INVERSE_DISPLAY                         0xA7        /**< command inverse display */ 
#define SSD1309_CMD_MULTIPLEX_RATIO                         0xA8        /**< command multiplex ratio */ 
#define SSD1309_CMD_DISPLAY_OFF                             0xAE        /**< command display off */ 
#define SSD1309_CMD_DISPLAY_ON                              0xAF        /**< command display on */ 
#define SSD1309_CMD_PAGE_ADDR                               0xB0        /**< command page address */ 
#define SSD1309_CMD_SCAN_DIRECTION_COM0_START               0xC0        /**< command scan direction com 0 start */ 
#define SSD1309_CMD_SCAN_DIRECTION_COMN_1_START             0xC8        /**< command scan direction com n-1 start */ 
#define SSD1309_CMD_DISPLAY_OFFSET                          0xD3        /**< command display offset */ 
#define SSD1309_CMD_DISPLAY_CLOCK_DIVIDE                    0xD5        /**< command display clock divide */ 
#define SSD1309_CMD_PRE_CHARGE_PERIOD                       0xD9        /**< command pre charge period */ 
#define SSD1309_CMD_COM_PINS_CONF                           0xDA        /**< command com pins conf */ 
#define SSD1309_CMD_COMH_DESLECT_LEVEL                      0xDB        /**< command comh deslect level */ 
#define SSD1309_CMD_GPIO                                    0xDC        /**< command set gpio */ 
#define SSD1309_CMD_LOCK                                    0xFD        /**< command set lock */

/**
 * @brief     write one byte
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] data is the write data
 * @param[in] cmd is the command or data type
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_ssd1309_write_byte(ssd1309_handle_t *handle, uint8_t data, uint8_t cmd)
{
    uint8_t res;
    
    if (handle->iic_spi == SSD1309_INTERFACE_IIC)                              /* if iic */
    {
        if (cmd != 0)                                                          /* if data */
        {
            if (handle->iic_write(handle->iic_addr, 0x40, &data, 1) != 0)      /* write data */
            {
                return 1;                                                      /* return error */
            }
            else
            {
                return 0;                                                      /* success return 0 */
            }
        }
        else
        {
            if (handle->iic_write(handle->iic_addr, 0x00, &data, 1) != 0)      /* write command */
            {
                return 1;                                                      /* return error */
            }
            else
            {
                return 0;                                                      /* success return 0 */
            }
        }
    }
    else if (handle->iic_spi == SSD1309_INTERFACE_SPI)                         /* if spi */
    {
        res = handle->spi_cmd_data_gpio_write(cmd);                            /* write data command */
        if (res != 0)                                                          /* check error */
        {
            return 1;                                                          /* return error */
        }
        
        if (handle->spi_write_cmd(&data, 1) != 0)                              /* write command */
        {
            return 1;                                                          /* return error */
        }
        else
        {
            return 0;                                                          /* success return 0 */
        }
    }
    else
    {
        return 1;                                                              /* return error */
    }
}

/**
 * @brief     write multiple bytes
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] *data points to a data buffer
 * @param[in] len is the data length
 * @param[in] cmd is the command or data type
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_ssd1309_multiple_write_byte(ssd1309_handle_t *handle, uint8_t *data, uint8_t len, uint8_t cmd)
{    
    uint8_t res;
    
    if (handle->iic_spi == SSD1309_INTERFACE_IIC)                               /* if iic */
    {
        if (cmd != 0)                                                           /* if data */
        {
            if (handle->iic_write(handle->iic_addr, 0x40, data, len) != 0)      /* write data */
            {
                return 1;                                                       /* return error */
            }
            else
            {
                return 0;                                                       /* success return 0 */
            }
        }
        else
        {
            if (handle->iic_write(handle->iic_addr, 0x00, data, len) != 0)      /* write command */
            {
                return 1;                                                       /* return error */
            }
            else
            {
                return 0;                                                       /* success return 0 */
            }
        }
    }
    else if (handle->iic_spi == SSD1309_INTERFACE_SPI)                          /* if spi */
    {
        res = handle->spi_cmd_data_gpio_write(cmd);                             /* write data command */
        if (res != 0)                                                           /* check error */
        {
            return 1;                                                           /* return error */
        }
        
        if (handle->spi_write_cmd(data, len) != 0)                              /* write command */
        {
            return 1;                                                           /* return error */
        }
        else
        {
            return 0;                                                           /* success return 0 */
        }
    }
    else
    {
        return 1;                                                               /* return error */
    }
}

/**
 * @brief     draw a point in gram
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] x is the coordinate x
 * @param[in] y is the coordinate y
 * @param[in] data is the write data
 * @return    status code
 *            - 0 success
 *            - 1 gram draw point failed
 * @note      none
 */
static uint8_t a_ssd1309_gram_draw_point(ssd1309_handle_t *handle, uint8_t x, uint8_t y, uint8_t data)
{
    uint8_t pos;
    uint8_t bx;
    uint8_t temp = 0;
    
    pos = y / 8;                              /* get y page */
    bx = y % 8;                               /* get y point */
    temp = 1 << bx;                           /* set data */
    if (data != 0)                            /* if 1  */
    {
        handle->gram[x][pos] |= temp;         /* set 1 */
    }
    else
    {
        handle->gram[x][pos] &= ~temp;        /* set 0 */
    }
  
    return 0;                                 /* success return 0 */
}

/**
 * @brief     draw a char in gram
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] x is the coordinate x
 * @param[in] y is the coordinate y
 * @param[in] chr is the write char
 * @param[in] size is the char size
 * @param[in] mode is the display mode
 * @return    status code
 *            - 0 success
 *            - 1 gram show char failed
 * @note      none
 */
static uint8_t a_ssd1309_gram_show_char(ssd1309_handle_t *handle, uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode)
{
    uint8_t temp, t, t1;
    uint8_t y0 = y;
    uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);                 /* get size */
    
    chr = chr - ' ';                                                                /* get index */
    for (t = 0; t < csize; t++)                                                     /* write size */
    {   
        if (size == 12)                                                             /* if size 12 */
        {
            temp = gsc_ssd1309_ascii_1206[chr][t];                                  /* get ascii 1206 */
        }
        else if (size == 16)                                                        /* if size 16 */
        {
            temp = gsc_ssd1309_ascii_1608[chr][t];                                  /* get ascii 1608 */
        }
        else if(size == 24)                                                         /* if size 24 */
        {
            temp = gsc_ssd1309_ascii_2412[chr][t];                                  /* get ascii 2412 */
        }
        else
        {
            return 1;                                                               /* return error */
        }
        for (t1 = 0; t1 < 8; t1++)                                                  /* write one line */
        {
            if ((temp & 0x80) != 0)                                                 /* if 1 */
            {
                if (a_ssd1309_gram_draw_point(handle, x, y, mode) != 0)             /* draw point */
                {
                    return 1;                                                       /* return error */
                }
            }
            else 
            {
                if (a_ssd1309_gram_draw_point(handle, x, y, !mode) != 0)            /* draw point */
                {
                    return 1;                                                       /* return error */
                }
            }
            temp <<= 1;                                                             /* left shift 1 */
            y++;
            if ((y - y0) == size)                                                   /* reset size */
            {
                y = y0;                                                             /* set y */
                x++;                                                                /* x++ */
                
                break;                                                              /* break */
            }
        }
    }
  
    return 0;                                                                       /* success return 0 */
}

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
uint8_t ssd1309_clear(ssd1309_handle_t *handle)
{
    uint8_t i;
    uint8_t n;
    
    if (handle == NULL)                                                                               /* check handle */
    {
        return 2;                                                                                     /* return error */
    }
    if (handle->inited != 1)                                                                          /* check handle initialization */
    {
        return 3;                                                                                     /* return error */
    }
    
    for (i = 0; i < 8; i++)                                                                           /* write 8 page */
    {  
        if (a_ssd1309_write_byte(handle, SSD1309_CMD_PAGE_ADDR + i, SSD1309_CMD) != 0)                /* set page */
        {
            handle->debug_print("ssd1309: write byte failed.\n");                                     /* write byte failed */
            
            return 1;                                                                                 /* return error */
        }
        if (a_ssd1309_write_byte(handle, SSD1309_CMD_LOWER_COLUMN_START_ADDRESS, SSD1309_CMD) != 0)   /* set lower column 0 */
        {
            handle->debug_print("ssd1309: write byte failed.\n");                                     /* write byte failed */
            
            return 1;                                                                                 /* return error */
        }
        if (a_ssd1309_write_byte(handle, SSD1309_CMD_HIGHER_COLUMN_START_ADDRESS, SSD1309_CMD) != 0)  /* set higher column 0 */
        {
            handle->debug_print("ssd1309: write byte failed.\n");                                     /* write byte failed */
            
            return 1;                                                                                 /* return error */
        }
        for (n = 0; n < 128; n++)                                                                     /* write 128 */
        {
            handle->gram[n][i] = 0x00;                                                                /* set black */
            if (a_ssd1309_write_byte(handle, handle->gram[n][i], SSD1309_DATA) != 0)                  /* write data */
            {
                handle->debug_print("ssd1309: write byte failed.\n");                                 /* write byte failed */
                
                return 1;                                                                             /* return error */
            }
        }
    }
    
    return 0;                                                                                         /* success return 0 */
}

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
uint8_t ssd1309_gram_update(ssd1309_handle_t *handle)
{
    uint8_t i;
    uint8_t n;
    
    if (handle == NULL)                                                                               /* check handle */
    {
        return 2;                                                                                     /* return error */
    }
    if (handle->inited != 1)                                                                          /* check handle initialization */
    {
        return 3;                                                                                     /* return error */
    }
    
    for (i = 0; i < 8; i++)                                                                           /* write 8 page */
    {  
        if (a_ssd1309_write_byte(handle, SSD1309_CMD_PAGE_ADDR + i, SSD1309_CMD) != 0)                /* set page */
        {
            handle->debug_print("ssd1309: write byte failed.\n");                                     /* write byte failed */
            
            return 1;                                                                                 /* return error */
        }
        if (a_ssd1309_write_byte(handle, SSD1309_CMD_LOWER_COLUMN_START_ADDRESS, SSD1309_CMD) != 0)   /* set lower column 0 */
        {
            handle->debug_print("ssd1309: write byte failed.\n");                                     /* write byte failed */
            
            return 1;                                                                                 /* return error */
        }
        if (a_ssd1309_write_byte(handle, SSD1309_CMD_HIGHER_COLUMN_START_ADDRESS, SSD1309_CMD) != 0)  /* set higher column 0 */
        {
            handle->debug_print("ssd1309: write byte failed.\n");                                     /* write byte failed */
            
            return 1;                                                                                 /* return error */
        }
        for (n = 0; n < 128; n++)                                                                     /* write 128 */
        {
            if (a_ssd1309_write_byte(handle, handle->gram[n][i], SSD1309_DATA) != 0)                  /* write data */
            {
                handle->debug_print("ssd1309: write byte failed.\n");                                 /* write byte failed */
                
                return 1;                                                                             /* return error */
            }
        }
    }
    
    return 0;                                                                                         /* success return 0 */
}

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
uint8_t ssd1309_write_point(ssd1309_handle_t *handle, uint8_t x, uint8_t y, uint8_t data)
{
    uint8_t pos;
    uint8_t bx;
    uint8_t temp = 0;
    
    if (handle == NULL)                                                                                        /* check handle */
    {
        return 2;                                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                                   /* check handle initialization */
    {
        return 3;                                                                                              /* return error */
    }
    if ((x > 127) || (y > 63))                                                                                 /* check x, y */
    {
        handle->debug_print("ssd1309: x or y is invalid.\n");                                                  /* x or y is invalid */
        
        return 4;                                                                                              /* return error */
    }
    
    pos = y / 8;                                                                                               /* get y page */
    bx = y % 8;                                                                                                /* get y point */
    temp = 1 << bx;                                                                                            /* set data */
    if (data != 0)                                                                                             /* check the data */
    {
        handle->gram[x][pos] |= temp;                                                                          /* set 1 */
    }
    else
    {
        handle->gram[x][pos] &= ~temp;                                                                         /* set 0 */
    }
    if (a_ssd1309_write_byte(handle, SSD1309_CMD_PAGE_ADDR + pos, SSD1309_CMD) != 0)                           /* write page addr */
    {
        handle->debug_print("ssd1309: write byte failed.\n");                                                  /* write byte failed */
        
        return 1;                                                                                              /* return error */
    }
    if (a_ssd1309_write_byte(handle, SSD1309_CMD_LOWER_COLUMN_START_ADDRESS|(x&0x0F), SSD1309_CMD) != 0)       /* write lower column */
    {
        handle->debug_print("ssd1309: write byte failed.\n");                                                  /* write byte failed */
        
        return 1;                                                                                              /* return error */
    }
    if (a_ssd1309_write_byte(handle, SSD1309_CMD_HIGHER_COLUMN_START_ADDRESS|((x>4)&0x0F), SSD1309_CMD) != 0)  /* write higher column */
    {
        handle->debug_print("ssd1309: write byte failed.\n");                                                  /* write byte failed */
        
        return 1;                                                                                              /* return error */
    }
    if (a_ssd1309_write_byte(handle, handle->gram[x][pos], SSD1309_DATA) != 0)                                 /* write data */
    {
        handle->debug_print("ssd1309: write byte failed.\n");                                                  /* write byte failed */
        
        return 1;                                                                                              /* return error */
    }
    else
    {
       return 0;                                                                                               /* success return 0 */
    }
}

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
uint8_t ssd1309_read_point(ssd1309_handle_t *handle, uint8_t x, uint8_t y, uint8_t *data)
{
    uint8_t pos;
    uint8_t bx;
    uint8_t temp = 0;
    
    if (handle == NULL)                                              /* check handle */
    {
        return 2;                                                    /* return error */
    }
    if (handle->inited != 1)                                         /* check handle initialization */
    {
        return 3;                                                    /* return error */
    }
    if ((x > 127) || (y > 63))                                       /* check x, y */
    {
        handle->debug_print("ssd1309: x or y is invalid.\n");        /* x or y is invalid */
        
        return 4;                                                    /* return error */
    }
    
    pos = y / 8;                                                     /* get y page */
    bx = y % 8;                                                      /* get y point */
    temp = 1 << bx;                                                  /* set data */
    if ((handle->gram[x][pos] & temp) != 0)                          /* get data */
    {
        *data = 1;                                                   /* set 1 */
    }
    else
    {
        *data = 0;                                                   /* set 0 */
    }
  
    return 0;                                                        /* success return 0 */
}

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
uint8_t ssd1309_gram_write_point(ssd1309_handle_t *handle, uint8_t x, uint8_t y, uint8_t data)
{
    uint8_t pos;
    uint8_t bx;
    uint8_t temp = 0;
    
    if (handle == NULL)                                              /* check handle */
    {
        return 2;                                                    /* return error */
    }
    if (handle->inited != 1)                                         /* check handle initialization */
    {
        return 3;                                                    /* return error */
    }
    if ((x > 127) || (y > 63))                                       /* check x, y */
    {
        handle->debug_print("ssd1309: x or y is invalid.\n");        /* x or y is invalid */
        
        return 4;                                                    /* return error */
    }
    
    pos = y / 8;                                                     /* get y page */
    bx = y % 8;                                                      /* get y point */
    temp = 1 << bx;                                                  /* set data */
    if (data != 0)                                                   /* if 1 */
    {
        handle->gram[x][pos] |= temp;                                /* set 1 */
    }
    else
    {
        handle->gram[x][pos] &= ~temp;                               /* set 0 */
    }
  
    return 0;                                                        /* success return 0 */
}

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
uint8_t ssd1309_gram_read_point(ssd1309_handle_t *handle, uint8_t x, uint8_t y, uint8_t *data)
{
    uint8_t pos;
    uint8_t bx;
    uint8_t temp = 0;
    
    if (handle == NULL)                                              /* check handle */
    {
        return 2;                                                    /* return error */
    }
    if (handle->inited != 1)                                         /* check handle initialization */
    {
        return 3;                                                    /* return error */
    }
    if ((x > 127) || (y > 63))                                       /* check x, y */
    {
        handle->debug_print("ssd1309: x or y is invalid.\n");        /* x or y is invalid */
        
        return 4;                                                    /* return error */
    }
    
    pos = y / 8;                                                     /* get y page */
    bx = y % 8;                                                      /* get y point */
    temp = 1 << bx;                                                  /* set data */
    if ((handle->gram[x][pos] & temp) != 0)                          /* get data */
    {
        *data = 1;                                                   /* set 1 */
    }
    else
    {
        *data = 0;                                                   /* set 0 */
    }
  
    return 0;                                                        /* success return 0 */
}

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
uint8_t ssd1309_gram_write_string(ssd1309_handle_t *handle, uint8_t x, uint8_t y, char *str, uint16_t len, uint8_t color, ssd1309_font_t font)
{    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
    if((x > 127) || (y > 63))                                                /* check x, y */
    {
        handle->debug_print("ssd1309: x or y is invalid.\n");                /* x or y is invalid */
        
        return 4;                                                            /* return error */
    }
    
    while ((len != 0) && (*str <= '~') && (*str >= ' '))                     /* write all string */
    {       
        if (x > (127 - (font / 2)))                                          /* check x point */
        {
            x = 0;                                                           /* set x */
            y += (uint8_t)font;                                              /* set next row */
        }
        if (y > (63 - font))                                                 /* check y pont */
        {
            y = x = 0;                                                       /* reset to 0,0 */
        }
        if (a_ssd1309_gram_show_char(handle, x, y, *str, font, color) != 0)  /* show a char */
        {
            return 1;                                                        /* return error */
        }
        x += (uint8_t)(font / 2);                                            /* x + font/2 */
        str++;                                                               /* str address++ */
        len--;                                                               /* str length-- */
    }
    
    return 0;                                                                /* success return 0 */
}

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
uint8_t ssd1309_gram_fill_rect(ssd1309_handle_t *handle, uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint8_t color)
{
    uint8_t x, y;  
    
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    if ((left > 127) || (top > 63))                                             /* check left top */
    {
        handle->debug_print("ssd1309: left or top is invalid.\n");              /* left or top is invalid */
        
        return 4;                                                               /* return error */
    }
    if ((right > 127) || (bottom > 63))                                         /* check right bottom */
    {
        handle->debug_print("ssd1309: right or bottom is invalid.\n");          /* right or bottom is invalid */
        
        return 5;                                                               /* return error */
    }
    if ((left > right) || (top > bottom))                                       /* check left right top bottom */
    {
        handle->debug_print("ssd1309: left > right or top > bottom.\n");        /* left > right or top > bottom */
        
        return 6;                                                               /* return error */
    }
    
    for (x = left; x <= right; x++)                                             /* write x */
    {
        for (y = top; y <= bottom; y++)                                         /* write y */
        {
            if (a_ssd1309_gram_draw_point(handle, x, y, color) != 0)            /* draw point */
            {
                return 1;                                                       /* return error */
            }
        }
    }
    
    return 0;                                                                   /* return error */
}

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
uint8_t ssd1309_gram_draw_picture(ssd1309_handle_t *handle, uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint8_t *img)
{    
    uint8_t x, y;  
    
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    if ((left > 127) || (top > 63))                                             /* check left top */
    {
        handle->debug_print("ssd1309: left or top is invalid.\n");              /* left or top is invalid */
        
        return 4;                                                               /* return error */
    }
    if ((right > 127) || (bottom > 63))                                         /* check right bottom */
    {
        handle->debug_print("ssd1309: right or bottom is invalid.\n");          /* right or bottom is invalid */
        
        return 5;                                                               /* return error */
    }
    if ((left > right) || (top > bottom))                                       /* check left right top bottom */
    {
        handle->debug_print("ssd1309: left > right or top > bottom.\n");        /* left > right or top > bottom */
        
        return 6;                                                               /* return error */
    }
    
    for (x = left; x <= right; x++)                                             /* write x */
    {
        for (y = top; y <= bottom; y++)                                         /* write y */
        {
            if (a_ssd1309_gram_draw_point(handle, x, y, *img) != 0)             /* draw point */
            {
                return 1;                                                       /* return error */
            }
            img++;                                                              /* img++ */
        }
    }
    
    return 0;                                                                   /* succeed return 0 */
}

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
uint8_t ssd1309_init(ssd1309_handle_t *handle)
{
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->debug_print == NULL)                                                /* check debug_print */
    {
        return 3;                                                                   /* return error */
    }
    if (handle->iic_init == NULL)                                                   /* check iic_init */
    {
        handle->debug_print("ssd1309: iic_init is null.\n");                        /* iic_init is null */
        
        return 3;                                                                   /* return error */
    }
    if (handle->iic_deinit == NULL)                                                 /* check iic_deinit */
    {
        handle->debug_print("ssd1309: iic_deinit is null.\n");                      /* iic_deinit is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->iic_write == NULL)                                                  /* check iic_write */
    {
        handle->debug_print("ssd1309: iic_write is null.\n");                       /* iic_write is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->spi_init == NULL)                                                   /* check spi_init */
    {
        handle->debug_print("ssd1309: spi_init is null.\n");                        /* spi_init is null */
        
        return 3;                                                                   /* return error */
    }
    if (handle->spi_deinit == NULL)                                                 /* check spi_deinit */
    {
        handle->debug_print("ssd1309: spi_deinit is null.\n");                      /* spi_deinit is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->spi_write_cmd == NULL)                                              /* check spi_write_cmd */
    {
        handle->debug_print("ssd1309: spi_write_cmd is null.\n");                   /* spi_write_cmd is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->delay_ms == NULL)                                                   /* check delay_ms */
    {
        handle->debug_print("ssd1309: delay_ms is null.\n");                        /* delay_ms is null */
        
        return 3;                                                                   /* return error */
    }
    if (handle->spi_cmd_data_gpio_init == NULL)                                     /* check spi_cmd_data_gpio_init */
    {
        handle->debug_print("ssd1309: spi_cmd_data_gpio_init is null.\n");          /* spi_cmd_data_gpio_init is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->spi_cmd_data_gpio_deinit == NULL)                                   /* check spi_cmd_data_gpio_deinit */
    {
        handle->debug_print("ssd1309: spi_cmd_data_gpio_deinit is null.\n");        /* spi_cmd_data_gpio_deinit is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->spi_cmd_data_gpio_write == NULL)                                    /* check spi_cmd_data_gpio_write */
    {
        handle->debug_print("ssd1309: spi_cmd_data_gpio_write is null.\n");         /* spi_cmd_data_gpio_write is null */
        
        return 3;                                                                   /* return error */
    }
    if (handle->reset_gpio_init == NULL)                                            /* check reset_gpio_init */
    {
        handle->debug_print("ssd1309: reset_gpio_init is null.\n");                 /* reset_gpio_init is null */
        
        return 3;                                                                   /* return error */
    }
    if (handle->reset_gpio_deinit == NULL)                                          /* check reset_gpio_deinit */
    {
        handle->debug_print("ssd1309: reset_gpio_deinit is null.\n");               /* reset_gpio_deinit is null */
        
        return 3;                                                                   /* return error */
    }
    if(handle->reset_gpio_write == NULL)                                            /* check reset_gpio_write */
    {
        handle->debug_print("ssd1309: reset_gpio_write is null.\n");                /* reset_gpio_write is null */ 
        
        return 3;                                                                   /* return error */
    }
    
    if (handle->spi_cmd_data_gpio_init() != 0)                                      /* check spi_cmd_data_gpio_init */
    {
        handle->debug_print("ssd1309: spi cmd data gpio init failed.\n");           /* spi cmd data gpio init failed */
        
        return 5;                                                                   /* return error */
    }
    if (handle->reset_gpio_init() != 0)                                             /* reset gpio init */
    {
        handle->debug_print("ssd1309: reset gpio init failed.\n");                  /* reset gpio init failed */
        (void)handle->spi_cmd_data_gpio_deinit();                                   /* spi_cmd_data_gpio_deinit */
        
        return 4;                                                                   /* return error */
    }
    if (handle->reset_gpio_write(0) != 0)                                           /* write 0 */
    {
        handle->debug_print("ssd1309: reset gpio write failed.\n");                 /* reset gpio write failed */
        (void)handle->spi_cmd_data_gpio_deinit();                                   /* spi_cmd_data_gpio_deinit */
        (void)handle->reset_gpio_deinit();                                          /* reset_gpio_deinit */
        
        return 4;                                                                   /* return error */
    }
    handle->delay_ms(100);                                                          /* delay 100 ms */
    if (handle->reset_gpio_write(1) != 0)                                           /* write 1 */
    {
        handle->debug_print("ssd1309: reset gpio write failed.\n");                 /* reset gpio write failed */
        (void)handle->spi_cmd_data_gpio_deinit();                                   /* spi_cmd_data_gpio_deinit */
        (void)handle->reset_gpio_deinit();                                          /* reset_gpio_deinit */
        
        return 4;                                                                   /* return error */
    }
    if (handle->iic_spi == SSD1309_INTERFACE_IIC)                                   /* if iic interface */
    {
        if (handle->iic_init() != 0)                                                /* iic init */
        {
            handle->debug_print("ssd1309: iic init failed.\n");                     /* iic init failed */
            (void)handle->spi_cmd_data_gpio_deinit();                               /* spi_cmd_data_gpio_deinit */
            (void)handle->reset_gpio_deinit();                                      /* reset_gpio_deinit */
            
            return 1;                                                               /* return error */
        }
    }
    else if (handle->iic_spi == SSD1309_INTERFACE_SPI)                              /* if spi interface */
    {
        if (handle->spi_init() != 0)                                                /* spi init */
        {
            handle->debug_print("ssd1309: spi init failed.\n");                     /* spi init failed */
            (void)handle->spi_cmd_data_gpio_deinit();                               /* spi_cmd_data_gpio_deinit */
            (void)handle->reset_gpio_deinit();                                      /* reset_gpio_deinit */
            
            return 1;                                                               /* return error */
        }
    }
    else
    {
        handle->debug_print("ssd1309: interface is invalid.\n");                    /* interface is invalid */
        (void)handle->spi_cmd_data_gpio_deinit();                                   /* spi_cmd_data_gpio_deinit */
        (void)handle->reset_gpio_deinit();                                          /* reset_gpio_deinit */
        
        return 6;                                                                   /* return error */
    }
    handle->inited = 1;                                                             /* flag inited */
    
    return 0;                                                                       /* success return 0 */
}

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
uint8_t ssd1309_deinit(ssd1309_handle_t *handle)
{
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    if (a_ssd1309_write_byte(handle, SSD1309_CMD_DISPLAY_OFF, SSD1309_CMD) != 0)     /* write display off */
    {
        handle->debug_print("ssd1309: write command failed.\n");                     /* write command failed */
            
        return 4;                                                                    /* return error */
    }
    if (handle->reset_gpio_deinit() != 0)                                            /* reset gpio deinit */
    {
        handle->debug_print("ssd1309: reset gpio deinit failed.\n");                 /* reset gpio deinit failed */
            
        return 5;                                                                    /* return error */
    }
    if (handle->spi_cmd_data_gpio_deinit() != 0)                                     /* spi cmd data gpio deinit */
    {
        handle->debug_print("ssd1309: spi cmd data gpio deinit failed.\n");          /* spi cmd data gpio deinit failed */
            
        return 6;                                                                    /* return error */
    }
    if (handle->iic_spi == SSD1309_INTERFACE_IIC)                                    /* if iic interface */
    {
        if (handle->iic_deinit() != 0)                                               /* iic deinit */
        {
            handle->debug_print("ssd1309: iic deinit failed.\n");                    /* iic deinit failed */
            
            return 1;                                                                /* return error */
        }
    }
    else if (handle->iic_spi == SSD1309_INTERFACE_SPI)                               /* if spi interface */
    {
        if (handle->spi_deinit() != 0)                                               /* spi deinit */
        {
            handle->debug_print("ssd1309: spi deinit failed.\n");                    /* spi deinit failed */
            
            return 1;                                                                /* return error */
        }
    }
    else
    {
        handle->debug_print("ssd1309: interface is invalid.\n");                     /* interface is invalid */
        
        return 7;                                                                    /* return error */
    }
    handle->inited = 0;                                                              /* flag close */
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief     set the chip interface
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t ssd1309_set_interface(ssd1309_handle_t *handle, ssd1309_interface_t interface)
{
    if (handle == NULL)                          /* check handle */
    {
        return 2;                                /* return error */
    }
    
    handle->iic_spi = (uint8_t)interface;        /* set interface */
    
    return 0;                                    /* success return 0 */
}

/**
 * @brief      get the chip interface
 * @param[in]  *handle points to an ssd1309 handle structure
 * @param[out] *interface points to a chip interface buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ssd1309_get_interface(ssd1309_handle_t *handle, ssd1309_interface_t *interface)
{
    if (handle == NULL)                                         /* check handle */
    {
        return 2;                                               /* return error */
    }
    
    *interface = (ssd1309_interface_t)(handle->iic_spi);        /* get interface */
    
    return 0;                                                   /* success return 0 */
}

/**
 * @brief     set the chip iic address
 * @param[in] *handle points to an ssd1309 handle structure
 * @param[in] addr_pin is the iic address
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t ssd1309_set_addr_pin(ssd1309_handle_t *handle, ssd1309_address_t addr_pin)
{
    if (handle == NULL)                          /* check handle */
    {
        return 2;                                /* return error */
    }
    
    handle->iic_addr = (uint8_t)addr_pin;        /* set addr pin */
    
    return 0;                                    /* success return 0 */
}

/**
 * @brief      get the chip iic address
 * @param[in]  *handle points to an ssd1309 handle structure
 * @param[out] *addr_pin points to an iic address buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ssd1309_get_addr_pin(ssd1309_handle_t *handle, ssd1309_address_t *addr_pin)
{
    if (handle == NULL)                                       /* check handle */
    {
        return 2;                                             /* return error */
    }
    
    *addr_pin = (ssd1309_address_t)(handle->iic_addr);        /* set address */
    
    return 0;                                                 /* success return 0 */
}

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
uint8_t ssd1309_set_low_column_start_address(ssd1309_handle_t *handle, uint8_t addr)
{
    if (handle == NULL)                                                                                         /* check handle */
    {
        return 2;                                                                                               /* return error */
    }
    if (handle->inited != 1)                                                                                    /* check handle initialization */
    {
        return 3;                                                                                               /* return error */
    }
    if (addr > 0x0F)                                                                                            /* check addr */
    {
        handle->debug_print("ssd1309: addr is invalid.\n");                                                     /* addr is invalid */
        
        return 4;                                                                                               /* return error */
    }
  
    return a_ssd1309_write_byte(handle, SSD1309_CMD_LOWER_COLUMN_START_ADDRESS | (addr & 0x0F), SSD1309_CMD);   /* write command */
}

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
uint8_t ssd1309_set_high_column_start_address(ssd1309_handle_t *handle, uint8_t addr)
{
    if (handle == NULL)                                                                                         /* check handle */
    {
        return 2;                                                                                               /* return error */
    }
    if (handle->inited != 1)                                                                                    /* check handle initialization */
    {
        return 3;                                                                                               /* return error */
    }
    if (addr > 0x0F)                                                                                            /* check addr */
    {
        handle->debug_print("ssd1309: addr is invalid.\n");                                                     /* addr is invalid */
        
        return 4;                                                                                               /* return error */
    }
  
    return a_ssd1309_write_byte(handle, SSD1309_CMD_HIGHER_COLUMN_START_ADDRESS | (addr & 0x0F), SSD1309_CMD);  /* write command */
}

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
uint8_t ssd1309_set_memory_addressing_mode(ssd1309_handle_t *handle, ssd1309_memory_addressing_mode_t mode)
{
    uint8_t buf[2];

    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
  
    buf[0] = SSD1309_CMD_MEMORY_ADDRESSING_MODE;                                        /* set command mode */
    buf[1] = mode;                                                                      /* set mode */
  
    return a_ssd1309_multiple_write_byte(handle, (uint8_t *)buf, 2, SSD1309_CMD);       /* write command */
}

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
uint8_t ssd1309_set_column_address_range(ssd1309_handle_t *handle, uint8_t start_addr, uint8_t end_addr)
{
    uint8_t buf[3];
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    if (start_addr > 0x7F)                                                              /* check start addr */
    {
        handle->debug_print("ssd1309: start addr is invalid.\n");                       /* start addr is invalid */
        
        return 4;                                                                       /* return error */
    }
    if (end_addr > 0x7F)                                                                /* check end addr */
    {
        handle->debug_print("ssd1309: end addr is invalid.\n");                         /* end addr is invalid */
        
        return 5;                                                                       /* return error */
    }
    
    buf[0] = SSD1309_CMD_SET_COLUMN_ADDRESS;                                            /* set command */
    buf[1] = start_addr & 0x7F;                                                         /* set start address */
    buf[2] = end_addr & 0x7F;                                                           /* set end address */
  
    return a_ssd1309_multiple_write_byte(handle, (uint8_t *)buf, 3, SSD1309_CMD);       /* write command */
}

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
uint8_t ssd1309_set_page_address_range(ssd1309_handle_t *handle, uint8_t start_addr, uint8_t end_addr)
{
    uint8_t buf[3];

    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    if (start_addr > 0x07)                                                              /* check start addr */
    {
        handle->debug_print("ssd1309: start addr is invalid.\n");                       /* start addr is invalid */
        
        return 4;                                                                       /* return error */
    }
    if (end_addr > 0x07)                                                                /* check end addr */
    {
        handle->debug_print("ssd1309: end addr is invalid.\n");                         /* end_addr is invalid */
        
        return 5;                                                                       /* return error */
    }
    
    buf[0] = SSD1309_CMD_SET_PAGE_ADDRESS;                                              /* set command */
    buf[1] = start_addr & 0x07;                                                         /* set start address */
    buf[2] = end_addr & 0x07;                                                           /* set end address */
  
    return a_ssd1309_multiple_write_byte(handle, (uint8_t *)buf, 3, SSD1309_CMD);       /* write command */
}

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
                                            uint8_t start_column_addr, uint8_t end_column_addr)
{
    uint8_t buf[8];
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    if (start_page_addr > 0x07)                                                         /* check start_page_addr */
    {
        handle->debug_print("ssd1309: start page addr is invalid.\n");                  /* start page addr is invalid */
        
        return 4;                                                                       /* return error */
    }
    if (end_page_addr > 0x07)                                                           /* check end_page_addr */
    {
        handle->debug_print("ssd1309: end page addr is invalid.\n");                    /* end page addr is invalid */
        
        return 5;                                                                       /* return error */
    }
    if (start_column_addr > 0x7F)                                                       /* check start column addr */
    {
        handle->debug_print("ssd1309: start column addr is invalid.\n");                /* start column addr is invalid */
        
        return 6;                                                                       /* return error */
    }
    if (end_column_addr > 0x7F)                                                         /* check end column addr */
    {
        handle->debug_print("ssd1309: end column addr is invalid.\n");                  /* end column addr is invalid */
        
        return 7;                                                                       /* return error */
    }
    
    buf[0] = SSD1309_CMD_RIGHT_HORIZONTAL_SCROLL;                                       /* set command */
    buf[1] = 0x00;                                                                      /* set null */
    buf[2] = start_page_addr & 0x07;                                                    /* set start page address */
    buf[3] = frames & 0x07;                                                             /* set frames */
    buf[4] = end_page_addr & 0x07;                                                      /* set end page address */
    buf[5] = 0x00;                                                                      /* set null */
    buf[6] = start_column_addr;                                                         /* set start column addr */
    buf[7] = end_column_addr;                                                           /* set end column addr */
  
    return a_ssd1309_multiple_write_byte(handle, (uint8_t *)buf, 8, SSD1309_CMD);       /* write command */
}

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
                                           uint8_t start_column_addr, uint8_t end_column_addr)
{
    uint8_t buf[8];
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    if (start_page_addr > 0x07)                                                         /* check start_page_addr */
    {
        handle->debug_print("ssd1309: start page addr is invalid.\n");                  /* start page addr is invalid */
        
        return 4;                                                                       /* return error */
    }
    if (end_page_addr > 0x07)                                                           /* check end_page_addr */
    {
        handle->debug_print("ssd1309: end page addr is invalid.\n");                    /* end page addr is invalid */
        
        return 5;                                                                       /* return error */
    }
    if (start_column_addr > 0x7F)                                                       /* check start column addr */
    {
        handle->debug_print("ssd1309: start column addr is invalid.\n");                /* start column addr is invalid */
        
        return 6;                                                                       /* return error */
    }
    if (end_column_addr > 0x7F)                                                         /* check end column addr */
    {
        handle->debug_print("ssd1309: end column addr is invalid.\n");                  /* end column addr is invalid */
        
        return 7;                                                                       /* return error */
    }
    
    buf[0] = SSD1309_CMD_LEFT_HORIZONTAL_SCROLL;                                        /* set command */
    buf[1] = 0x00;                                                                      /* set null */
    buf[2] = start_page_addr & 0x07;                                                    /* set start page address */
    buf[3] = frames & 0x07;                                                             /* set frames */
    buf[4] = end_page_addr & 0x07;                                                      /* set end page address */
    buf[5] = 0x00;                                                                      /* set null */
    buf[6] = start_column_addr;                                                         /* set start column addr */
    buf[7] = end_column_addr;                                                           /* set end column addr */
  
    return a_ssd1309_multiple_write_byte(handle, (uint8_t *)buf, 8, SSD1309_CMD);       /* write command */
}

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
                                                     uint8_t start_column_addr, uint8_t end_column_addr)
{
    uint8_t buf[8];

    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    if (start_page_addr > 0x07)                                                         /* check start_page_addr */
    {
        handle->debug_print("ssd1309: start_page_addr is invalid.\n");                  /* start_page_addr is invalid */
        
        return 4;                                                                       /* return error */
    }
    if (end_page_addr > 0x07)                                                           /* check end page addr */
    {
        handle->debug_print("ssd1309: end_page_addr is invalid.\n");                    /* end_page_addr is invalid */
        
        return 5;                                                                       /* return error */
    }
    if (rows > 0x3F)                                                                    /* check rows */
    {
        handle->debug_print("ssd1309: rows is invalid.\n");                             /* rows is invalid */
       
        return 6;                                                                       /* return error */
    }
    if (start_column_addr > 0x7F)                                                       /* check start column addr */
    {
        handle->debug_print("ssd1309: start column addr is invalid.\n");                /* start column addr is invalid */
        
        return 7;                                                                       /* return error */
    }
    if (end_column_addr > 0x7F)                                                         /* check end column addr */
    {
        handle->debug_print("ssd1309: end column addr is invalid.\n");                  /* end column addr is invalid */
        
        return 8;                                                                       /* return error */
    }
    
    buf[0] = SSD1309_CMD_VERTICAL_RIGHT_HORIZONTAL_SCROLL;                              /* set command */
    buf[1] = horizontal_scroll & 0x01;                                                  /* set horizontal scroll */
    buf[2] = start_page_addr & 0x07;                                                    /* set start page addr */
    buf[3] = frames & 0x07;                                                             /* set frames */
    buf[4] = end_page_addr & 0x07;                                                      /* set end page addr */
    buf[5] = rows & 0x3F;                                                               /* set rows */
    buf[6] = start_column_addr & 0x7F;                                                  /* set start column addr */
    buf[7] = end_column_addr & 0x7F;                                                    /* set end column addr */
  
    return a_ssd1309_multiple_write_byte(handle, (uint8_t *)buf, 8, SSD1309_CMD);       /* write command */
}

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
                                                    uint8_t start_column_addr, uint8_t end_column_addr)
{
    uint8_t buf[8];

    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    if (start_page_addr > 0x07)                                                         /* check start_page_addr */
    {
        handle->debug_print("ssd1309: start_page_addr is invalid.\n");                  /* start_page_addr is invalid */
        
        return 4;                                                                       /* return error */
    }
    if (end_page_addr > 0x07)                                                           /* check end page addr */
    {
        handle->debug_print("ssd1309: end_page_addr is invalid.\n");                    /* end_page_addr is invalid */
        
        return 5;                                                                       /* return error */
    }
    if (rows > 0x3F)                                                                    /* check rows */
    {
        handle->debug_print("ssd1309: rows is invalid.\n");                             /* rows is invalid */
       
        return 6;                                                                       /* return error */
    }
    if (start_column_addr > 0x7F)                                                       /* check start column addr */
    {
        handle->debug_print("ssd1309: start column addr is invalid.\n");                /* start column addr is invalid */
        
        return 7;                                                                       /* return error */
    }
    if (end_column_addr > 0x7F)                                                         /* check end column addr */
    {
        handle->debug_print("ssd1309: end column addr is invalid.\n");                  /* end column addr is invalid */
        
        return 8;                                                                       /* return error */
    }
    
    buf[0] = SSD1309_CMD_VERTICAL_LEFT_HORIZONTAL_SCROLL;                               /* set command */
    buf[1] = horizontal_scroll & 0x01;                                                  /* set horizontal scroll */
    buf[2] = start_page_addr & 0x07;                                                    /* set start page addr */
    buf[3] = frames & 0x07;                                                             /* set frames */
    buf[4] = end_page_addr & 0x07;                                                      /* set end page addr */
    buf[5] = rows & 0x3F;                                                               /* set rows */
    buf[6] = start_column_addr & 0x7F;                                                  /* set start column addr */
    buf[7] = end_column_addr & 0x7F;                                                    /* set end column addr */
  
    return a_ssd1309_multiple_write_byte(handle, (uint8_t *)buf, 8, SSD1309_CMD);       /* write command */
}

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
uint8_t ssd1309_deactivate_scroll(ssd1309_handle_t *handle)
{
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
  
    return a_ssd1309_write_byte(handle, SSD1309_CMD_DEACTIVATE_SCROLL, SSD1309_CMD);       /* write command */
}

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
uint8_t ssd1309_activate_scroll(ssd1309_handle_t *handle)
{
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    return a_ssd1309_write_byte(handle, SSD1309_CMD_ACTIVATE_SCROLL, SSD1309_CMD);       /* write command */
}

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
uint8_t ssd1309_set_display_start_line(ssd1309_handle_t *handle, uint8_t l)
{
    if (handle == NULL)                                                                                 /* check handle */
    {
        return 2;                                                                                       /* return error */
    }
    if (handle->inited != 1)                                                                            /* check handle initialization */
    {
        return 3;                                                                                       /* return error */
    }
    if (l > 0x3F)                                                                                       /* check line */
    {
        handle->debug_print("ssd1309: line is invalid.\n");                                             /* line is invalid */
        
        return 4;                                                                                       /* return error */
    }
  
    return a_ssd1309_write_byte(handle, SSD1309_CMD_DISPLAY_START_LINE | (l & 0x3F), SSD1309_CMD);      /* write command */
}

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
uint8_t ssd1309_set_contrast(ssd1309_handle_t *handle, uint8_t contrast)
{
    uint8_t buf[2];
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    buf[0] = SSD1309_CMD_CONTRAST_CONTROL;                                              /* set command */
    buf[1] = contrast;                                                                  /* set contrast */
  
    return a_ssd1309_multiple_write_byte(handle, (uint8_t *)buf, 2, SSD1309_CMD);       /* write command */
}

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
uint8_t ssd1309_set_segment_remap(ssd1309_handle_t *handle, ssd1309_segment_column_remap_t remap)
{
    if (handle == NULL)                                                                                /* check handle */
    {
        return 2;                                                                                      /* return error */
    }
    if (handle->inited != 1)                                                                           /* check handle initialization */
    {
        return 3;                                                                                      /* return error */
    }
    
    if (remap != 0)                                                                                    /* check remap */
    {
        return a_ssd1309_write_byte(handle, SSD1309_CMD_COLUMN_127_MAPPED_TO_SEG0, SSD1309_CMD);       /* write remap */
    }
    else
    {
        return a_ssd1309_write_byte(handle, SSD1309_CMD_COLUMN_0_MAPPED_TO_SEG0, SSD1309_CMD);         /* write remap */
    }
}

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
uint8_t ssd1309_set_vertical_scroll_area(ssd1309_handle_t *handle, uint8_t start_row, uint8_t end_row)
{
    uint8_t buf[3];
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    if (start_row > 0x3F)                                                               /* check start row */
    {
        handle->debug_print("ssd1309: start_row is invalid.\n");                        /* start_row is invalid */
       
        return 4;                                                                       /* return error */
    }
    if (end_row > 0x7F)                                                                 /* check end_row */
    {
        handle->debug_print("ssd1309: end_row is invalid.\n");                          /* end_row is invalid */
       
        return 5;                                                                       /* return error */
    }
    if (end_row > start_row)                                                            /* check start_row and end_row */
    {
        handle->debug_print("ssd1309: end_row > start_row.\n");                         /* end_row > start_row */
       
        return 6;                                                                       /* return error */
    }
    
    buf[0] = SSD1309_CMD_VERTICAL_SCROLL_AREA;                                          /* set command */
    buf[1] = start_row;                                                                 /* set start row */
    buf[2] = end_row;                                                                   /* set end row */
  
    return a_ssd1309_multiple_write_byte(handle, (uint8_t *)buf, 3, SSD1309_CMD);       /* write command */
}

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
uint8_t ssd1309_set_entire_display(ssd1309_handle_t *handle, ssd1309_entire_display_t enable)
{
    if (handle == NULL)                                                                         /* check handle */
    {
        return 2;                                                                               /* return error */
    }
    if (handle->inited != 1)                                                                    /* check handle initialization */
    {
        return 3;                                                                               /* return error */
    }
    
    if (enable != 0)                                                                            /* if enable */
    {
        return a_ssd1309_write_byte(handle, SSD1309_CMD_ENTIRE_DISPLAY_ON, SSD1309_CMD);        /* write command */
    }
    else
    {
        return a_ssd1309_write_byte(handle, SSD1309_CMD_ENTIRE_DISPLAY_OFF, SSD1309_CMD);       /* write command */
    }
}

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
uint8_t ssd1309_set_display_mode(ssd1309_handle_t *handle, ssd1309_display_mode_t mode)
{
    if (handle == NULL)                                                                         /* check handle */
    {
        return 2;                                                                               /* return error */
    }
    if (handle->inited != 1)                                                                    /* check handle initialization */
    {
        return 3;                                                                               /* return error */
    }
    
    if (mode != 0)                                                                              /* check mode */
    {
        return a_ssd1309_write_byte(handle, SSD1309_CMD_INVERSE_DISPLAY, SSD1309_CMD);          /* write command */
    }
    else
    {
        return a_ssd1309_write_byte(handle, SSD1309_CMD_NORMAL_DISPLAY, SSD1309_CMD);           /* write command */
    }
}

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
uint8_t ssd1309_set_multiplex_ratio(ssd1309_handle_t *handle, uint8_t multiplex)
{
    uint8_t buf[2];

    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    if (multiplex < 0x0F)                                                               /* check multiplex */
    {
        handle->debug_print("ssd1309: multiplex is too small.\n");                      /* multiplex is too small */
       
        return 4;                                                                       /* return error */
    }
    if (multiplex > 0x3F)                                                               /* check multiplex */
    {
        handle->debug_print("ssd1309: multiplex is too large.\n");                      /* multiplex is too large */
       
        return 5;                                                                       /* return error */
    }
    
    buf[0] = SSD1309_CMD_MULTIPLEX_RATIO;                                               /* set command */
    buf[1] = multiplex;                                                                 /* set multiplex */
  
    return a_ssd1309_multiple_write_byte(handle, (uint8_t *)buf, 2, SSD1309_CMD);       /* write command */
}

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
uint8_t ssd1309_set_display(ssd1309_handle_t *handle, ssd1309_display_t on_off)
{
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    if (on_off != 0)                                                                     /* check on off */
    {
        return a_ssd1309_write_byte(handle, SSD1309_CMD_DISPLAY_ON, SSD1309_CMD);        /* write command */
    }
    else
    {
        return a_ssd1309_write_byte(handle, SSD1309_CMD_DISPLAY_OFF, SSD1309_CMD);       /* write command */
    }
}

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
uint8_t ssd1309_set_page_address(ssd1309_handle_t *handle, uint8_t addr)
{
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }
    if (addr > 0x07)                                                                           /* check addr */
    {
        handle->debug_print("ssd1309: addr is invalid.\n");                                    /* addr is invalid */
        
        return 4;                                                                              /* return error */
    }
    
    return a_ssd1309_write_byte(handle, SSD1309_CMD_PAGE_ADDR | (addr & 0x07), SSD1309_CMD);   /* write command */
}

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
uint8_t ssd1309_set_scan_direction(ssd1309_handle_t *handle, ssd1309_scan_direction_t dir)
{
    if (handle == NULL)                                                                                  /* check handle */
    {
        return 2;                                                                                        /* return error */
    }
    if (handle->inited != 1)                                                                             /* check handle initialization */
    {
        return 3;                                                                                        /* return error */
    }
    
    if (dir != 0)                                                                                        /* choose dir */
    {
        return a_ssd1309_write_byte(handle, SSD1309_CMD_SCAN_DIRECTION_COMN_1_START, SSD1309_CMD);       /* write command */
    }
    else
    {
        return a_ssd1309_write_byte(handle, SSD1309_CMD_SCAN_DIRECTION_COM0_START, SSD1309_CMD);         /* write command */
    }
}

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
uint8_t ssd1309_set_display_offset(ssd1309_handle_t *handle, uint8_t offset)
{
    uint8_t buf[2];

    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    if (offset > 0x3F)                                                                  /* check offset */
    {
        handle->debug_print("ssd1309: offset is invalid.\n");                           /* offset is invalid */
       
        return 4;                                                                       /* return error */
    }
    
    buf[0] = SSD1309_CMD_DISPLAY_OFFSET ;                                               /* set command */
    buf[1] = offset;                                                                    /* set offset */
  
    return a_ssd1309_multiple_write_byte(handle, (uint8_t *)buf, 2, SSD1309_CMD);       /* write command */
}

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
uint8_t ssd1309_set_display_clock(ssd1309_handle_t *handle, uint8_t oscillator_frequency, uint8_t clock_divide)
{
    uint8_t buf[2];
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    if (oscillator_frequency> 0x0F)                                                     /* check oscillator_frequency */
    {
        handle->debug_print("ssd1309: oscillator frequency is invalid.\n");             /* oscillator frequency is invalid */
        
        return 4;                                                                       /* return error */
    }
    if (clock_divide> 0x0F)                                                             /* check clock_divide */
    {
        handle->debug_print("ssd1309: clock divide is invalid.\n");                     /* clock divide is invalid */
        
        return 5;                                                                       /* return error */
    }
    
    buf[0] = SSD1309_CMD_DISPLAY_CLOCK_DIVIDE;                                          /* set command */
    buf[1] = (oscillator_frequency << 4) | clock_divide;                                /* set oscillator frequency and clock divide */
  
    return a_ssd1309_multiple_write_byte(handle, (uint8_t *)buf, 2, SSD1309_CMD);       /* write command */
}

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
uint8_t ssd1309_set_precharge_period(ssd1309_handle_t *handle, uint8_t phase1_period, uint8_t phase2_period)
{
    uint8_t buf[2];

    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    if (phase1_period> 0x0F)                                                            /* check phase1 period */
    {
        handle->debug_print("ssd1309: phase1 period is invalid.\n");                    /* phase1 period is invalid */
        
        return 4;                                                                       /* return error */
    }
    if (phase2_period> 0x0F)                                                            /* check phase2 period */
    {
        handle->debug_print("ssd1309: phase2 period is invalid.\n");                    /* phase2 period is invalid */
       
        return 5;                                                                       /* return error */
    }
    
    buf[0] = SSD1309_CMD_PRE_CHARGE_PERIOD;                                             /* set command */
    buf[1] = (phase2_period << 4) | phase1_period;                                      /* set period */
  
    return a_ssd1309_multiple_write_byte(handle, (uint8_t *)buf, 2, SSD1309_CMD);       /* write command */
}

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
uint8_t ssd1309_set_com_pins_hardware_conf(ssd1309_handle_t *handle, ssd1309_pin_conf_t conf, ssd1309_left_right_remap_t remap)
{
    uint8_t buf[2];
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    buf[0] = SSD1309_CMD_COM_PINS_CONF;                                                 /* set command */
    buf[1] = (uint8_t)((conf << 4) | (remap << 5) | 0x02);                              /* set com pins */
  
    return a_ssd1309_multiple_write_byte(handle, (uint8_t *)buf, 2, SSD1309_CMD);       /* write command */
}

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
uint8_t ssd1309_set_deselect_level(ssd1309_handle_t *handle, ssd1309_deselect_level_t level)
{
    uint8_t buf[2];
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    buf[0] = SSD1309_CMD_COMH_DESLECT_LEVEL;                                            /* set command */
    buf[1] = (uint8_t)(level << 2);                                                     /* set level */
  
    return a_ssd1309_multiple_write_byte(handle, (uint8_t *)buf, 2, SSD1309_CMD);       /* write command */
}

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
                                                       uint8_t end_column_addr)
{
    uint8_t buf[8];
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    if (start_page_addr > 0x07)                                                         /* check start_page_addr */
    {
        handle->debug_print("ssd1309: start page addr is invalid.\n");                  /* start page addr is invalid */
        
        return 4;                                                                       /* return error */
    }
    if (end_page_addr > 0x07)                                                           /* check end_page_addr */
    {
        handle->debug_print("ssd1309: end page addr is invalid.\n");                    /* end page addr is invalid */
        
        return 5;                                                                       /* return error */
    }
    if (start_column_addr > 0x7F)                                                       /* check start column addr */
    {
        handle->debug_print("ssd1309: start column addr is invalid.\n");                /* start column addr is invalid */
        
        return 6;                                                                       /* return error */
    }
    if (end_column_addr > 0x7F)                                                         /* check end column addr */
    {
        handle->debug_print("ssd1309: end column addr is invalid.\n");                  /* end column addr is invalid */
        
        return 7;                                                                       /* return error */
    }
    
    buf[0] = SSD1309_CMD_RIGHT_HORIZONTAL_SCROLL_ONE_COL;                               /* set command */
    buf[1] = 0x00;                                                                      /* set null */
    buf[2] = start_page_addr & 0x07;                                                    /* set start page address */
    buf[3] = 0x00;                                                                      /* set 0x00 */
    buf[4] = end_page_addr & 0x07;                                                      /* set end page address */
    buf[5] = 0x00;                                                                      /* set null */
    buf[6] = start_column_addr;                                                         /* set start column addr */
    buf[7] = end_column_addr;                                                           /* set end column addr */
  
    return a_ssd1309_multiple_write_byte(handle, (uint8_t *)buf, 8, SSD1309_CMD);       /* write command */
}

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
                                                      uint8_t end_column_addr)
{
    uint8_t buf[8];
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    if (start_page_addr > 0x07)                                                         /* check start_page_addr */
    {
        handle->debug_print("ssd1309: start page addr is invalid.\n");                  /* start page addr is invalid */
        
        return 4;                                                                       /* return error */
    }
    if (end_page_addr > 0x07)                                                           /* check end_page_addr */
    {
        handle->debug_print("ssd1309: end page addr is invalid.\n");                    /* end page addr is invalid */
        
        return 5;                                                                       /* return error */
    }
    if (start_column_addr > 0x7F)                                                       /* check start column addr */
    {
        handle->debug_print("ssd1309: start column addr is invalid.\n");                /* start column addr is invalid */
        
        return 6;                                                                       /* return error */
    }
    if (end_column_addr > 0x7F)                                                         /* check end column addr */
    {
        handle->debug_print("ssd1309: end column addr is invalid.\n");                  /* end column addr is invalid */
        
        return 7;                                                                       /* return error */
    }
    
    buf[0] = SSD1309_CMD_LEFT_HORIZONTAL_SCROLL_ONE_COL;                                /* set command */
    buf[1] = 0x00;                                                                      /* set null */
    buf[2] = start_page_addr & 0x07;                                                    /* set start page address */
    buf[3] = 0x00;                                                                      /* set 0x00 */
    buf[4] = end_page_addr & 0x07;                                                      /* set end page address */
    buf[5] = 0x00;                                                                      /* set null */
    buf[6] = start_column_addr;                                                         /* set start column addr */
    buf[7] = end_column_addr;                                                           /* set end column addr */
  
    return a_ssd1309_multiple_write_byte(handle, (uint8_t *)buf, 8, SSD1309_CMD);       /* write command */
}

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
uint8_t ssd1309_set_gpio(ssd1309_handle_t *handle, ssd1309_gpio_t gpio)
{
    uint8_t buf[2];
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    buf[0] = SSD1309_CMD_GPIO;                                                          /* set command */
    buf[1] = gpio;                                                                      /* set gpio */
  
    return a_ssd1309_multiple_write_byte(handle, (uint8_t *)buf, 2, SSD1309_CMD);       /* write command */
}

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
uint8_t ssd1309_set_mcu_interface_lock(ssd1309_handle_t *handle, ssd1309_bool_t enable)
{
    uint8_t buf[2];
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    buf[0] = SSD1309_CMD_LOCK;                                                          /* set command */
    buf[1] = enable;                                                                    /* set bool */
  
    return a_ssd1309_multiple_write_byte(handle, (uint8_t *)buf, 2, SSD1309_CMD);       /* write command */
}

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
uint8_t ssd1309_write_cmd(ssd1309_handle_t *handle, uint8_t *buf, uint8_t len)
{
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    return a_ssd1309_multiple_write_byte(handle, (uint8_t *)buf, len, SSD1309_CMD);       /* write command */
}

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
uint8_t ssd1309_write_data(ssd1309_handle_t *handle, uint8_t *buf, uint8_t len)
{
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
  
    return a_ssd1309_multiple_write_byte(handle, (uint8_t *)buf, len, SSD1309_DATA);       /* write data */
}

/**
 * @brief      get chip's information
 * @param[out] *info points to an ssd1309 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ssd1309_info(ssd1309_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(ssd1309_info_t));                        /* initialize ssd1309 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC SPI", 8);                         /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
