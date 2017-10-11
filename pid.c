typedef struct PID
{
    int SetPoint; //设定目标Desired Value
    double Proportion; //比例常数Proportional Const
    double Integral; //积分常数Integral Const
    double Derivative; //微分常数Derivative Const
    int LastError; //Error[-1]
    int PrevError; //Error[-2]
} PID;
//*****************************************************
//定义相关宏
//*****************************************************
#define P_DATA 100
#define I_DATA 0.6
#define D_DATA 1
#define HAVE_NEW_VELOCITY 0X01
//*****************************************************
//声明PID实体
//*****************************************************
static PID sPID;
static PID *sptr = &sPID;
//*****************************************************
//PID参数初始化
//*****************************************************
void IncPIDInit(void)
{
sptr->LastError = 0; //Error[-1]
sptr->PrevError = 0; //Error[-2]
sptr->Proportion =P_DATA; //比例常数Proportional Const
sptr->Integral =I_DATA; //积分常数Integral Const
sptr->Derivative =D_DATA; //微分常数Derivative Const
sptr->SetPoint =100;  目标是100
}
//*****************************************************
//增量式PID控制设计
//*****************************************************
int IncPIDCalc(int NextPoint)
{
   int iError, iIncpid; //当前误差
   iError = sptr->SetPoint - NextPoint; //增量计算
   iIncpid = sptr->Proportion * iError //E[k]项
             - sptr->Integral * sptr->LastError //E[k－1]项
             + sptr->Derivative * sptr->PrevError; //E[k－2]项
    sptr->PrevError = sptr->LastError;   //存储误差，用于下次计算
    sptr->LastError = iError;
    return(iIncpid);                         //返回增量值
}
Int g_CurrentVelocity;
Int g_Flag;

void main(void)
{
   DisableInterrupt
InitMCu();
   IncPIDInit();
g_CurrentVelocity=0;  //全局变量也初始化
g_Flag=0;               //全局变量也初始化
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
//采样周期T
//****************************************
Interrrupt TIME void
{
  CurrentVelocity =GetCurrentVelocity;
  g_Flag|= HAVE_NEW_VELOCITY;
}