/*******************************************************************************
*                 
*                 		       ���пƼ�
--------------------------------------------------------------------------------
* ʵ �� ��		 : CANͨ��ʵ��
* ʵ��˵��       : ���������ؽ�ȥ������λ��LCD����ʾCAN�Ĺ���ģʽ�����ͺͽ��յ�
					ID�����з��ͼ����յ�ÿλ���ݣ�Ĭ�Ͻ��յ���ID0��������ѡ�����450KHz
					LEDָʾ����˸Ч����	LCD������ʾ����Ĳ���ָ����
					���¸�λ������������,�����Ӻ���ע��
					���Բο�֮ǰ�ĺ���ע��
* ���ӷ�ʽ       : 	
* ע    ��		 : 	���ú�����ͷ�ļ�.c�ļ���
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
uint8_t CAN_SendData[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}; //�����λ������ŷ���ID�ͽ���ID
uint8_t *Mode;
const uint8_t CAN_ModeNormal[9] = {"����ģʽ"};
const uint8_t CAN_ModeLoopBack[9] = {"����ģʽ"};

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

    /* ��ʼ�� */
    TFT_Init();
    FLASH_Init(); 
    SYSTICK_Config();
    USART1_Config(9600);
    LED_Config();
    KEY_Config();

    /* ������ʾ��ʼ�� */
    GUI_DisplayInit();
    m = 10;
    canMode = 1; //Ҫ����canMode������CAN_SendData[10],�Ա����ѭ��һ��ʼ�ͳ�ʼ��
	while(1)
	{   
        /* �޸�ģʽ */
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
        /* �������ID�ı䣬���������ý���ID */
        if(receiveId != CAN_SendData[9])
        {
            receiveId = CAN_SendData[9];
            if(receiveId != 0x00)
            {
                CAN1_Config16BitFilter(CAN_SendData[9], 0x00);
            }                
        }

        /* ��ʾ���� */
        GUI_DisplayData(m);
        /* LED����˸ */
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

         /* ���ݼ��̣�������Ӧ�Ĳ��� */
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
                GUI_Show12Char(175, 20, "�ȴ�����", RED, BLACK);
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
                GUI_Show12Char(175, 20, "�ȴ�����", RED, BLACK);
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
                CAN1_SendMesg(CAN_SendData[8], 8, CAN_SendData);//��������
                GUI_Show12Char(175, 20, "���ͳɹ�", BLUE, BLACK);
                break;
            default:
                break;     
         }
	}
}

/****************************************************************************
* Function Name  : GUI_DisplayInit
* Description    : ��ʼ��������ʾ
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void GUI_DisplayInit(void)
{
    TFT_ClearScreen(BLACK);
//    GUI_Show16Chinese(80, 0, "���пƼ�", RED, BLACK);
//    GUI_Show12Char(90, 21, "PRECHIN", RED, BLACK);
//    GUI_Show12Char(60, 42, "www.prechin.com", RED, BLACK);
    GUI_Show12Char(76, 0, "CANͨ��ʵ��", RED, BLACK);
    /* ��ʾ����ģʽ */
    GUI_Show12Char(0, 20, "CAN����ģʽ��", RED, BLACK);

    /* ��ʾ���ͽ���ID */
    GUI_Show12Char(0, 40, "����ID:", RED, BLACK);
    GUI_Show12Char(120, 40, "����ID:", RED, BLACK);
    
    /* ��ʾ���ͺͽ������ݵ�8λ�� */    
    GUI_Show12Char(0, 60, "���͵�һλ��", RED, BLACK);
    GUI_Show12Char(120, 60, "���յ�һλ��", RED, BLACK);
    GUI_Show12Char(0, 80, "���͵ڶ�λ��", RED, BLACK);
    GUI_Show12Char(120, 80, "���յڶ�λ��", RED, BLACK);
    GUI_Show12Char(0, 100, "���͵���λ��", RED, BLACK);
    GUI_Show12Char(120, 100, "���յ���λ��", RED, BLACK);
    GUI_Show12Char(0, 120, "���͵���λ��", RED, BLACK);
    GUI_Show12Char(120, 120, "���յ���λ��", RED, BLACK);
    GUI_Show12Char(0, 140, "���͵���λ��", RED, BLACK);
    GUI_Show12Char(120, 140, "���յ���λ��", RED, BLACK);
    GUI_Show12Char(0, 160, "���͵���λ��", RED, BLACK);
    GUI_Show12Char(120, 160, "���յ���λ��", RED, BLACK);
    GUI_Show12Char(0, 180, "���͵���λ��", RED, BLACK);
    GUI_Show12Char(120, 180, "���յ���λ��", RED, BLACK);
    GUI_Show12Char(0, 200, "���͵ڰ�λ��", RED, BLACK);
    GUI_Show12Char(120, 200, "���յڰ�λ��", RED, BLACK);
    GUI_Show12Char(0, 220, "�ϼ����������ּ�һ", BLUE, BLACK);
    GUI_Show12Char(0, 240, "�¼����������ּ�һ", BLUE, BLACK);
    GUI_Show12Char(0, 260, "������������������ƶ�һλ", BLUE, BLACK);
    GUI_Show12Char(0, 280, "�Ҽ�����������", BLUE, BLACK);
    GUI_Show12Char(0, 300, "ע��Ĭ�Ͻ���ID0,������Ϊ450K", BLUE, BLACK);
}

/****************************************************************************
* Function Name  : GUI_DisplayData
* Description    : ��ʾ���Է��ͺͽ��յ����ݣ����������ò���
* Input          : num��������λ��
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
            /* ������յ����ݣ���ʾ���� */
            /* ����ȡ����16λ����ת���ɿ�����ʾ��ASCII�� */
            ShowData[0] = CAN_RX_BUFF[addr] >> 4;
            if(ShowData[0] > 9)
            {
                ShowData[0] += '7';  //������9ʱ����ABCDEF����ʾ
            }
            else
            {
                ShowData[0] += '0';    
            }
            ShowData[1] = CAN_RX_BUFF[addr] & 0x0F;
            if(ShowData[1] > 9)
            {
                ShowData[1] += '7';  //������9ʱ����ABCDEF����ʾ
            }
            else
            {
                ShowData[1] += '0';    
            }
            /* ��ʾ���յ������� */
            GUI_Show12Char(208, (60 + addr * 20), ShowData, RED, BLACK);
        }
        
        /* ��Ҫ���͵�16λ����ת���ɿ�����ʾ��ASCII�� */
        ShowData[0] = CAN_SendData[addr] >> 4;
        if(ShowData[0] > 9)
        {
            ShowData[0] += '7';    //������9ʱ����ABCDEF����ʾ
        }
        else
        {
            ShowData[0] += '0';    
        }
        ShowData[1] = CAN_SendData[addr] & 0x0F;
        if(ShowData[1] > 9)
        {
            ShowData[1] += '7';    //������9ʱ����ABCDEF����ʾ
        }
        else
        {
            ShowData[1] += '0';    
        }

        if(addr < 8)
        {
            /* ��ʾ�������ݣ�ѡ�����λ�� */
            if(num == addr)
            {
                GUI_Show12Char(88, (60 + addr * 20), ShowData, GREEN, BLACK);
            }
            else
            {
                GUI_Show12Char(88, (60 + addr * 20), ShowData, RED, BLACK);
            }
        }
        else //��ʾID����
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
