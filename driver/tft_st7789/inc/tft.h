/**
 *******************************************************************************
 * @file      : tft.h
 * @brief     :
 * @history   :
 *  Version     Date               Author             Note
 *  V0.9.0      2024/2/5         Huang Huichuan     1. <note>
 *******************************************************************************
 * @attention :
 *******************************************************************************
 *  Copyright (c) 2024 Reborn Team, University of Science and Technology Beijing.
 *  All Rights Reserved.
 *******************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __tft_H_
#define __tft_H_

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE         	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色

#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


#ifdef __cplusplus

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmath"
#include "font.h"
#include "cmsis_os.h"
/* Exported macro ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct{
  uint16_t height;
  uint16_t width_;
}ScreenSize;

class TFT{
 public:
  void Init(SPI_HandleTypeDef *_spi, uint16_t _width, uint16_t _height);
  void Clear(uint16_t Color);
  void Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);
  void DrawPoint(uint16_t x,uint16_t y,uint16_t color);
  void DrawLargePoint(uint16_t x,uint16_t y,uint16_t color);
  void DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);
  void DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);
  void DrawCircle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t color);
  void ShowChar(uint16_t x,uint16_t y,uint8_t num,uint16_t color);
  uint32_t pow(uint16_t m,uint16_t n);
  void ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint16_t color);
  void ShowString(uint16_t x,uint16_t y,const uint8_t *str,uint16_t color);
  void ShowImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *p);
 private:
  SPI_HandleTypeDef *spi_;

  ScreenSize size_;

  void WriteOneByte(uint8_t byte_);
  void WriteCmd(uint8_t cmd_);
  void WriteData8(uint8_t data_);
  void WriteData(uint16_t data_);
  void AddressSet(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

  void DC_Set(void){
    HAL_GPIO_WritePin(DC_GPIO_Port,DC_Pin,GPIO_PIN_SET);
  }
  void DC_Clr(void){
    HAL_GPIO_WritePin(DC_GPIO_Port,DC_Pin,GPIO_PIN_RESET);
  }
  void RES_Set(void){
    HAL_GPIO_WritePin(RES_GPIO_Port,RES_Pin,GPIO_PIN_SET);
  }
  void RES_Clr(void){
    HAL_GPIO_WritePin(RES_GPIO_Port,RES_Pin,GPIO_PIN_RESET);
  }
};
/* Exported variables --------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/

#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/

#ifdef  __cplusplus
}
#endif

#endif /* __tft_H_ */