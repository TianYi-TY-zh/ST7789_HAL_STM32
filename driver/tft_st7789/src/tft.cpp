/**
 *******************************************************************************
 * @file      : tft.cpp
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
/* Includes ------------------------------------------------------------------*/
#include "tft.h"
/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void TFT::Init(SPI_HandleTypeDef *_spi, uint16_t _width, uint16_t _height)
{
  spi_ = _spi;

  size_.width_ = _width;
  size_.height = _height;

  RES_Clr();
  osDelay(50);
  RES_Set();
  osDelay(50);
  //OLED_BLK_Set();

  //************* Start Initial Sequence **********//
  WriteCmd(0x36);
  WriteData8(0x00);
  //WriteData8(0x60);//横屏

  WriteCmd(0x3A);
  WriteData8(0x05);

  WriteCmd(0xB2);
  WriteData8(0x0C);
  WriteData8(0x0C);
  WriteData8(0x00);
  WriteData8(0x33);
  WriteData8(0x33);

  WriteCmd(0xB7);
  WriteData8(0x35);

  WriteCmd(0xBB);
  WriteData8(0x19);

  WriteCmd(0xC0);
  WriteData8(0x2C);

  WriteCmd(0xC2);
  WriteData8(0x01);

  WriteCmd(0xC3);
  WriteData8(0x12);

  WriteCmd(0xC4);
  WriteData8(0x20);

  WriteCmd(0xC6);
  WriteData8(0x0F);

  WriteCmd(0xD0);
  WriteData8(0xA4);
  WriteData8(0xA1);

  WriteCmd(0xE0);
  WriteData8(0xD0);
  WriteData8(0x04);
  WriteData8(0x0D);
  WriteData8(0x11);
  WriteData8(0x13);
  WriteData8(0x2B);
  WriteData8(0x3F);
  WriteData8(0x54);
  WriteData8(0x4C);
  WriteData8(0x18);
  WriteData8(0x0D);
  WriteData8(0x0B);
  WriteData8(0x1F);
  WriteData8(0x23);

  WriteCmd(0xE1);
  WriteData8(0xD0);
  WriteData8(0x04);
  WriteData8(0x0C);
  WriteData8(0x11);
  WriteData8(0x13);
  WriteData8(0x2C);
  WriteData8(0x3F);
  WriteData8(0x44);
  WriteData8(0x51);
  WriteData8(0x2F);
  WriteData8(0x1F);
  WriteData8(0x1F);
  WriteData8(0x20);
  WriteData8(0x23);

  WriteCmd(0x21);

  WriteCmd(0x11);
  //osDelay(120);

  WriteCmd(0x29);

}

void TFT::WriteOneByte(const uint8_t byte_)
{
  HAL_SPI_Transmit(spi_, &byte_, 1,100);
}

void TFT::WriteCmd(uint8_t cmd_)
{
  DC_Clr();
  WriteOneByte(cmd_);
}

void TFT::WriteData8(uint8_t data_)
{
  DC_Set();
  WriteOneByte(data_);
}

void TFT::WriteData(uint16_t data_)
{
  DC_Set();
  WriteOneByte(data_ >> 8);
  WriteOneByte(data_);
}

void TFT::AddressSet(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  WriteCmd(0x2a);
  //WriteData8(x1 >> 8);
  //WriteData8(x1);
  //WriteData8(x2 >> 8);
  //WriteData8(x2);
  WriteData(x1);
  WriteData(x2);

  WriteCmd(0x2b);
  //WriteData8(y1 >> 8);
  //WriteData8(y1);
  //WriteData8(y2 >> 8);
  //WriteData8(y2);
  WriteData(y1);
  WriteData(y2);

  WriteCmd(0x2C);
}

/**
 * @brief 清屏函数
 * @param Color 要清屏的颜色
 */
void TFT::Clear(uint16_t Color)
{
  AddressSet(0, 0, size_.width_ - 1, size_.height - 1);
  for(auto i=0; i<size_.width_; i++)
  {
    for (auto j=0;j<size_.height;j++)
    {
      WriteData(Color);
    }
  }
}

void TFT::Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
  AddressSet(xsta,ysta,xend,yend);      //设置光标位置
  for(auto i=ysta;i<=yend;i++)
  {
    for(auto j=xsta;j<=xend;j++)WriteData(color);//设置光标位置
  }
}

void TFT::DrawPoint(uint16_t x, uint16_t y,uint16_t color)
{
  AddressSet(x,y,x,y);//设置光标位置
  WriteData(color);
}

void TFT::DrawLargePoint(uint16_t x, uint16_t y, uint16_t color)
{
  Fill(x,y,x+1,y+1,color);
}

void TFT::DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
  int xerr=0,yerr=0,delta_x,delta_y,distance;
  int incx,incy,uRow,uCol;

  delta_x=x2-x1; //计算坐标增量
  delta_y=y2-y1;
  uRow=x1;
  uCol=y1;
  if(delta_x>0)incx=1; //设置单步方向
  else if(delta_x==0)incx=0;//垂直线
  else {incx=-1;delta_x=-delta_x;}
  if(delta_y>0)incy=1;
  else if(delta_y==0)incy=0;//水平线
  else{incy=-1;delta_y=-delta_y;}
  if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴
  else distance=delta_y;
  for(auto t=0;t<=distance+1;t++ )//画线输出
  {
    DrawPoint(uRow,uCol,color);//画点
    xerr+=delta_x ;
    yerr+=delta_y ;
    if(xerr>distance)
    {
      xerr-=distance;
      uRow+=incx;
    }
    if(yerr>distance)
    {
      yerr-=distance;
      uCol+=incy;
    }
  }
}

/**
 * @brief 画矩形
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param color
 */
void TFT::DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
  DrawLine(x1,y1,x2,y1,color);
  DrawLine(x1,y1,x1,y2,color);
  DrawLine(x1,y2,x2,y2,color);
  DrawLine(x2,y1,x2,y2,color);
}

/**
 * @brief 画圆
 * @param x0 中心点
 * @param y0 中心点
 * @param r 半径
 * @param color
 */
void TFT::DrawCircle(uint16_t x, uint16_t y, uint8_t r, uint16_t color)
{
  uint16_t xm = 0,ym = r;
  uint16_t i = 0;
  //先画出圆在x、y轴上的四个点
  DrawPoint(x,y+r,color);
  DrawPoint(x+r,y,color);
  DrawPoint(x,y-r,color);
  DrawPoint(x-r,y,color);

  while(xm<=ym)//判断是否画完
  {
    if((xm*xm+ym*ym)< (r*r))//在圆内
      xm++;
    else   //在圆外
      ym--;
    DrawPoint(x+xm,y+ym,color);
    DrawPoint(x-xm,y+ym,color);
    DrawPoint(x-ym,y+xm,color);
    DrawPoint(x-ym,y-xm,color);
    DrawPoint(x+ym,y+xm,color);
    DrawPoint(x+ym,y-xm,color);
    DrawPoint(x+xm,y-ym,color);
    DrawPoint(x-xm,y-ym,color);
  }
}

/**
 * @brief 指定位置显示一个字符
 * @param x
 * @param y
 * @param num
 * @param color
 */
void TFT::ShowChar(uint16_t x, uint16_t y, uint8_t num,uint16_t color)
{
  uint8_t temp;
  uint8_t pos,t;
  uint16_t x0=x;
  if(x>size_.width_-16||y>size_.height-16)return;
  //设置窗口
  num=num-' ';//得到偏移后的值
  AddressSet(x,y,x+8-1,y+16-1);      //设置光标位置

    for(pos=0;pos<16;pos++)
    {
      temp=asc2_1608[(uint16_t )num*16+pos];		 //调用1608字体
      for(t=0;t<8;t++)
      {
        if(temp&0x01)DrawPoint(x+t,y+pos,color);//画一个点
        temp>>=1;
      }
    }
}

uint32_t TFT::pow(uint16_t m, uint16_t n)
{
  uint32_t result=1;
  while(n--)result*=m;
  return result;
}


/**
 * @brief 显示数字
 * @param x
 * @param y
 * @param num 数值(0~4294967295)
 * @param len 数字位数
 */
void TFT::ShowNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len,uint16_t color)
{
  uint8_t t,temp;
  uint8_t enshow=0;
  num=(uint16_t )num;
  for(t=0;t<len;t++)
  {
    temp=(num/pow(10,len-t-1))%10;
    if(enshow==0&&t<(len-1))
    {
      if(temp==0)
      {
        ShowChar(x+8*t,y,' ',color);
        continue;
      }else enshow=1;

    }
    ShowChar(x+8*t,y,temp+48,color);
  }
}

void TFT::ShowString(uint16_t x, uint16_t y, const uint8_t *str,uint16_t color)
{
  while(*str!='\0')
  {
    if(x>size_.width_-16){ x=0;y+=16;}
    if(y>size_.height-16){y=x=0;Clear(RED);}
    ShowChar(x,y,*str,color);
    x+=8;
    str++;
  }
}

void TFT::ShowImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *p)
{
  uint32_t picL = 0;
  uint32_t picH = 0;

  AddressSet(x, y, x+width-1, y+height-1);
  for(auto i=0; i<width*height; i++)
  {
    picL=*(p+i*2);	//数据低位在前
    picH=*(p+i*2+1);
    WriteData(picH<<8|picL);
  }
}
