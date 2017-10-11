typedef struct PID
{
    int SetPoint; //�趨Ŀ��Desired Value
    double Proportion; //��������Proportional Const
    double Integral; //���ֳ���Integral Const
    double Derivative; //΢�ֳ���Derivative Const
    int LastError; //Error[-1]
    int PrevError; //Error[-2]
} PID;
//*****************************************************
//������غ�
//*****************************************************
#define P_DATA 100
#define I_DATA 0.6
#define D_DATA 1
#define HAVE_NEW_VELOCITY 0X01
//*****************************************************
//����PIDʵ��
//*****************************************************
static PID sPID;
static PID *sptr = &sPID;
//*****************************************************
//PID������ʼ��
//*****************************************************
void IncPIDInit(void)
{
sptr->LastError = 0; //Error[-1]
sptr->PrevError = 0; //Error[-2]
sptr->Proportion =P_DATA; //��������Proportional Const
sptr->Integral =I_DATA; //���ֳ���Integral Const
sptr->Derivative =D_DATA; //΢�ֳ���Derivative Const
sptr->SetPoint =100;  Ŀ����100
}
//*****************************************************
//����ʽPID�������
//*****************************************************
int IncPIDCalc(int NextPoint)
{
   int iError, iIncpid; //��ǰ���
   iError = sptr->SetPoint - NextPoint; //��������
   iIncpid = sptr->Proportion * iError //E[k]��
             - sptr->Integral * sptr->LastError //E[k��1]��
             + sptr->Derivative * sptr->PrevError; //E[k��2]��
    sptr->PrevError = sptr->LastError;   //�洢�������´μ���
    sptr->LastError = iError;
    return(iIncpid);                         //��������ֵ
}
Int g_CurrentVelocity;
Int g_Flag;

void main(void)
{
   DisableInterrupt
InitMCu();
   IncPIDInit();
g_CurrentVelocity=0;  //ȫ�ֱ���Ҳ��ʼ��
g_Flag=0;               //ȫ�ֱ���Ҳ��ʼ��
EnableInterrupt;
    While(1)
{
  if (g_Flag& HAVE_NEW_VELOCITY)
     {
         PWMOUT+= IncPIDCalc(CurrentVelocity);
           g_Flag&=~ HAVE_NEW_VELOCITY;
}
}
}
//****************************************
//��������T
//****************************************
Interrrupt TIME void
{
  CurrentVelocity =GetCurrentVelocity;
  g_Flag|= HAVE_NEW_VELOCITY;
}