/*******************************************************************************
*                 
*                 		       普中科技
--------------------------------------------------------------------------------
* 实 验 名		 : CAN通信实验
* 实验说明       : 将程序下载进去后点击复位，LCD上显示CAN的工作模式及发送和接收的
					ID，还有发送及接收的每位数据，默认接收的是ID0，波特率选择的是450KHz
					LED指示灯闪烁效果，	LCD上面显示具体的操作指导。
					按下复位开关重新运行,其他子函数注释
					可以参考之前的函数注释
* 连接方式       : 	
* 注    意		 : 	所用函数在头文件.c文件内
*******************************************************************************/


#include "stm32f10x.h"
#include "key.h"
#include "led.h"
#include "system.h"
#include "usart.h"
#include "gui.h"
#include "flash.h"
#include "can.h"

uint8_t ShowData[5] = { '0', '0', 0};
uint8_t CAN_SendData[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}; //最后两位用来存放发送ID和接收ID
uint8_t *Mode;
const uint8_t CAN_ModeNormal[9] = {"正常模式"};
const uint8_t CAN_ModeLoopBack[9] = {"环回模式"};

void GUI_DisplayInit(void);
void GUI_DisplayData(uint8_t num);

/****************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

int main(void)
{
    uint8_t ledState, m, receiveId, i, canMode;

    /* 初始化 */
    TFT_Init();
    FLASH_Init(); 
    SYSTICK_Config();
    USART1_Config(9600);
    LED_Config();
    KEY_Config();

    /* 彩屏显示初始化 */
    GUI_DisplayInit();
    m = 10;
    canMode = 1; //要设置canMode不等于CAN_SendData[10],以便进入循环一开始就初始化
	while(1)
	{   
        /* 修改模式 */
        if(canMode != CAN_SendData[10] )
        {
            canMode = CAN_SendData[10];
            if(CAN_SendData[10])
            {
               CAN1_Config(CAN_Mode_Normal);
               Mode = (uint8_t *)CAN_ModeNormal;    
            }
            else
            {
                CAN1_Config(CAN_Mode_LoopBack);
                Mode = (uint8_t *)CAN_ModeLoopBack;
            }
            CAN1_Config16BitFilter(CAN_SendData[9], 0x00);    
        }
        /* 如果接收ID改变，就重新设置接收ID */
        if(receiveId != CAN_SendData[9])
        {
            receiveId = CAN_SendData[9];
            if(receiveId != 0x00)
            {
                CAN1_Config16BitFilter(CAN_SendData[9], 0x00);
            }                
        }

        /* 显示数据 */
        GUI_DisplayData(m);
        /* LED灯闪烁 */
        i++;       
        if(i > 0x5F)
        {   
            i = 0; 
            if(ledState == 0xFE)
            {
                ledState = 0xFF;
            }
            else
            {
                ledState = 0xFE;
            }
            LED_SetState(ledState);
         }

         /* 根据键盘，做出相应的操作 */
         switch(KEY_Scan())
         {
            case(KEY_UP):
                
                if(m == 10)
                {
                    CAN_SendData[10] = ~CAN_SendData[10];
                }
                else
                {
                    CAN_SendData[m]++;   
                }
                GUI_Show12Char(175, 20, "等待发送", RED, BLACK);
                break;
            case(KEY_DOWN):
                
                if(m == 10)
                {
                    CAN_SendData[10] = ~CAN_SendData[10];
                }
                else
                {
                    CAN_SendData[m]--;    
                }
                GUI_Show12Char(175, 20, "等待发送", RED, BLACK);
                break;
            case(KEY_LEFT):
                if(m == 10)
                {
                    m = 0;
                }
                else
                {
                    m++;
                }
                break;
            case(KEY_RIGHT):
                CAN1_SendMesg(CAN_SendData[8], 8, CAN_SendData);//发送数据
                GUI_Show12Char(175, 20, "发送成功", BLUE, BLACK);
                break;
            default:
                break;     
         }
	}
}

/****************************************************************************
* Function Name  : GUI_DisplayInit
* Description    : 初始化彩屏显示
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void GUI_DisplayInit(void)
{
    TFT_ClearScreen(BLACK);
//    GUI_Show16Chinese(80, 0, "普中科技", RED, BLACK);
//    GUI_Show12Char(90, 21, "PRECHIN", RED, BLACK);
//    GUI_Show12Char(60, 42, "www.prechin.com", RED, BLACK);
    GUI_Show12Char(76, 0, "CAN通信实验", RED, BLACK);
    /* 显示工作模式 */
    GUI_Show12Char(0, 20, "CAN工作模式：", RED, BLACK);

    /* 显示发送接收ID */
    GUI_Show12Char(0, 40, "发送ID:", RED, BLACK);
    GUI_Show12Char(120, 40, "接收ID:", RED, BLACK);
    
    /* 显示发送和接收数据的8位数 */    
    GUI_Show12Char(0, 60, "发送第一位：", RED, BLACK);
    GUI_Show12Char(120, 60, "接收第一位：", RED, BLACK);
    GUI_Show12Char(0, 80, "发送第二位：", RED, BLACK);
    GUI_Show12Char(120, 80, "接收第二位：", RED, BLACK);
    GUI_Show12Char(0, 100, "发送第三位：", RED, BLACK);
    GUI_Show12Char(120, 100, "接收第三位：", RED, BLACK);
    GUI_Show12Char(0, 120, "发送第四位：", RED, BLACK);
    GUI_Show12Char(120, 120, "接收第四位：", RED, BLACK);
    GUI_Show12Char(0, 140, "发送第五位：", RED, BLACK);
    GUI_Show12Char(120, 140, "接收第五位：", RED, BLACK);
    GUI_Show12Char(0, 160, "发送第六位：", RED, BLACK);
    GUI_Show12Char(120, 160, "接收第六位：", RED, BLACK);
    GUI_Show12Char(0, 180, "发送第七位：", RED, BLACK);
    GUI_Show12Char(120, 180, "接收第七位：", RED, BLACK);
    GUI_Show12Char(0, 200, "发送第八位：", RED, BLACK);
    GUI_Show12Char(120, 200, "接收第八位：", RED, BLACK);
    GUI_Show12Char(0, 220, "上键：高亮部分加一", BLUE, BLACK);
    GUI_Show12Char(0, 240, "下键：高亮部分减一", BLUE, BLACK);
    GUI_Show12Char(0, 260, "左键：高亮部分向下移动一位", BLUE, BLACK);
    GUI_Show12Char(0, 280, "右键：发送数据", BLUE, BLACK);
    GUI_Show12Char(0, 300, "注：默认接收ID0,波特率为450K", BLUE, BLACK);
}

/****************************************************************************
* Function Name  : GUI_DisplayData
* Description    : 显示可以发送和接收的数据，并高亮设置部分
* Input          : num：高亮的位置
* Output         : None
* Return         : None
****************************************************************************/

void GUI_DisplayData(uint8_t num)
{
    uint8_t addr;
    
    for(addr=0; addr<10; addr++)
    {   
        if(addr < 8)
        {
            /* 如果接收到数据，显示数据 */
            /* 将读取到的16位数据转换成可以显示的ASCII码 */
            ShowData[0] = CAN_RX_BUFF[addr] >> 4;
            if(ShowData[0] > 9)
            {
                ShowData[0] += '7';  //当大于9时，用ABCDEF来表示
            }
            else
            {
                ShowData[0] += '0';    
            }
            ShowData[1] = CAN_RX_BUFF[addr] & 0x0F;
            if(ShowData[1] > 9)
            {
                ShowData[1] += '7';  //当大于9时，用ABCDEF来表示
            }
            else
            {
                ShowData[1] += '0';    
            }
            /* 显示接收到的数据 */
            GUI_Show12Char(208, (60 + addr * 20), ShowData, RED, BLACK);
        }
        
        /* 将要发送的16位数据转换成可以显示的ASCII码 */
        ShowData[0] = CAN_SendData[addr] >> 4;
        if(ShowData[0] > 9)
        {
            ShowData[0] += '7';    //当大于9时，用ABCDEF来表示
        }
        else
        {
            ShowData[0] += '0';    
        }
        ShowData[1] = CAN_SendData[addr] & 0x0F;
        if(ShowData[1] > 9)
        {
            ShowData[1] += '7';    //当大于9时，用ABCDEF来表示
        }
        else
        {
            ShowData[1] += '0';    
        }

        if(addr < 8)
        {
            /* 显示发送数据，选择高亮位置 */
            if(num == addr)
            {
                GUI_Show12Char(88, (60 + addr * 20), ShowData, GREEN, BLACK);
            }
            else
            {
                GUI_Show12Char(88, (60 + addr * 20), ShowData, RED, BLACK);
            }
        }
        else //显示ID部分
        {
            if(addr == 8)
            {
                if(num == 8)
                {
                    GUI_Show12Char(52, 40, ShowData, GREEN, BLACK); 
                }
                else
                {
                    GUI_Show12Char(52, 40, ShowData, RED, BLACK);
                }
            }
            if(addr == 9)
            {
                if(num == 9)
                {
                    GUI_Show12Char(176, 40, ShowData, GREEN, BLACK); 
                }
                else
                {
                    GUI_Show12Char(176, 40, ShowData, RED, BLACK);
                }
            }
        }        
    }
    if(num > 9)
    {
        GUI_Show12Char(104, 20, Mode, GREEN, BLACK);
    }
    else
    {
        GUI_Show12Char(104, 20, Mode, RED, BLACK);        
    }
}
