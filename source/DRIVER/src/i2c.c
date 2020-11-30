#include "i2c.h"

static I2C_MessageTypeDef I2C1_MessageStruct;//此变量只允许在本文件内使用
// static I2C_MessageTypeDef I2C2_MessageStruct;//此变量只允许在本文件内使用
// static I2C_MessageTypeDef I2C3_MessageStruct;//此变量只允许在本文件内使用

/* I2C public functions ------------------------------------------------------*/ 

/**
 * @brief  硬件I2C挂死解锁函数
 * @param  I2Cx: 指定I2Cx外设，x=1,2,3
 * @param  指定GPIO引脚
 * 希望可以通过GPIOx和I2Cx判断出引脚位置
 **/
void I2C_HangSlove(GPIO_TypeDef* GPIOx,I2C_TypeDef* I2Cx)
{
    //关闭I2C使能
    I2C_Cmd(I2Cx,DISABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
	//SCL切回GPIO通用输出，因为复用输出的时候输出模式已经是开漏，所以刚好可以用
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
    GPIO_Init(GPIOx,&GPIO_InitStruct);
	//SDA切回GPIO通用输入，用于检查SDA是否释放，这时候输出模式寄存器的开漏不用管，不会影响输入检测
	//检查SDA是否变回高了，不行就再来一次
	//SDA变通用输出
    //SCL切回复用功能，也就是回到SCL模式
    //SDA切回复用功能，也就是回到SDA模式
}

/* I2C1 private functions ----------------------------------------------------*/ 

/**
 * 下面的解释全错！重要！看根目录的modify.c
 * 修改记录：
 * 位置：source\FWLIB\src\stm32f4xx_i2c.c 204行
 * 原内容：freqrange = (uint16_t)(pclk1 / 1000000);
 * 新内容：freqrange = (uint16_t)(pclk1 / 4200000);
 * 原因：快速模式需要修改I2CX_CR2的FREQ，这里我相当于设置成为10MHz。
 * 
 * 位置：source\FWLIB\src\stm32f4xx_i2c.c 245行
 * 原内容：result = (uint16_t)(pclk1 / (I2C_InitStruct->I2C_ClockSpeed * 25));
 * 新内容：result = (uint16_t)(freqrange / (I2C_InitStruct->I2C_ClockSpeed * 25));
 * 原因：我觉得是硬件IIC少人用，参考手册误导了，这里程序也误导了，
 * 正确计算方法应该是用I2C_CR2的FREQ计算，就是说FREQ=10的话，计算
 * 频率就应该是10MHz而不是APB1的PCLK1=42MHz，因为这个是我之前验证
 * 过的。之前打寄存器的时候我IIC的时钟频率设为10MHZ，快速模式400KHz
 * 通信，计算应该是x=10M/（25*400K），详细推导请看参考手册RM0090
 * 的I2C_CCR计算部分，注意把TPCLK1换成FREQ的周期，I2C_TRISE的计算
 * 同理。
 * 
 * 吐槽：然后source\FWLIB\src\stm32f4xx_i2c.c 259行的最大上升时间
 * 设置又正确了，莫名奇妙。
 **/

void I2C1_Init(void)
{
    //开GPIOB时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
    //GPIO初始化结构体定义
    GPIO_InitTypeDef GPIO_InitStruct;
    //I2C1_SCL
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_I2C1);
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_OType=GPIO_OType_OD;
    GPIO_Init(GPIOB,&GPIO_InitStruct);
    //I2C1_SDA
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_I2C1);
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_OType=GPIO_OType_OD;
    GPIO_Init(GPIOB,&GPIO_InitStruct);

    //开I2C1时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
    //I2C初始化结构体定义
    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_ClockSpeed=400000;//400000=400KHz
    I2C_InitStructure.I2C_Mode=I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle=I2C_DutyCycle_2;
    // I2C_InitStructure.I2C_OwnAddress1=;//主模式用不上
    // I2C_InitStructure.I2C_Ack=I2C_Ack_Enable;//本外设的响应位由发送/接收程序决定
    // I2C_InitStructure.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;//主模式用不上
    I2C_Init(I2C1,&I2C_InitStructure);

    //定义NVIC初始化结构体
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel=I2C1_EV_IRQn; //I2C1事件中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel=I2C1_ER_IRQn; //I2C1错误处理中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3; //子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

    // 开I2C1中断
    I2C_ITConfig(I2C1,I2C_IT_BUF,ENABLE);
    I2C_ITConfig(I2C1,I2C_IT_EVT,ENABLE);
    I2C_ITConfig(I2C1,I2C_IT_ERR,ENABLE);
    I2C_Cmd(I2C1,ENABLE);
}

/**
 * @brief   I2C主模式，只写连续存储数据发送和接收，链表之类的暂时没必要（太慢了）
 * @param   I2Cx: where x can be 1, 2 or 3 to select the I2C peripheral.
 * @param   BufAddr: 需要发送的数据缓存区首地址
 * @param   BufLen: 需要发送的数据缓存区长度
 * @param   SlaAddr: 从设备地址
 * @param   I2C_AcknowledgedAddress: 从设备地址模式，借用库函数参数，下同
 *           This parameter can be one of the following values:
 *             @arg I2C_AcknowledgedAddress_7bit: 七位寻址模式，不包含读写位，支持0x08（包含）到0x77（包含）
 *             @arg I2C_AcknowledgedAddress_10bit: 十位寻址模式
 * @param   I2C_Direction: specifies whether the I2C device will be a Transmitter
 *          or a Receiver. 
 *           This parameter can be one of the following values
 *             @arg I2C_Direction_Transmitter: Transmitter mode
 *             @arg I2C_Direction_Receiver: Receiver mode
 **/
void I2C_Master(I2C_TypeDef* I2Cx,uint8_t* BufAddr,uint8_t BufLen,uint16_t SlaAddr,
                uint16_t I2C_AcknowledgedAddress,uint8_t I2C_Direction)
{
    //值初始化到本文件的全局变量，由I2Cx决定信息写入到哪个I2C的静态全局变量
    if(I2Cx==I2C1)
    {   
        if((I2C1_MessageStruct.I2CStat_Enum==I2C_Stat_Finish)&&
           (I2C_GetFlagStatus(I2Cx,I2C_FLAG_BUSY)==RESET))//需要在外设上一个任务已经完成，且检测busy位也为free才能初始化这些值
        {
            I2C1_MessageStruct.I2C_BufAddr=BufAddr;
            I2C1_MessageStruct.I2C_BufLen=BufLen;
            I2C1_MessageStruct.I2C_DataNum=0;
            I2C1_MessageStruct.I2C_DevAddr=SlaAddr;
            I2C1_MessageStruct.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress;
            I2C1_MessageStruct.I2C_Direction=I2C_Direction;
            I2C1_MessageStruct.I2CStat_Enum=I2C_Stat_Start;
            if(I2C1_MessageStruct.I2C_Direction==I2C_Direction_Receiver)
            {
                if(I2C1_MessageStruct.I2C_BufLen==1)
                {
                    I2C_AcknowledgeConfig(I2C1,DISABLE);
                    I2C_NACKPositionConfig(I2C1,DISABLE);
                }
                else if(I2C1_MessageStruct.I2C_BufLen==2)
                {
                    I2C_AcknowledgeConfig(I2C1,DISABLE);
                    I2C_NACKPositionConfig(I2C1,ENABLE);
                }
                else if(I2C1_MessageStruct.I2C_BufLen>2)
                {
                    I2C_AcknowledgeConfig(I2C1,ENABLE);
                    I2C_NACKPositionConfig(I2C1,DISABLE);
                }
            }
        }
        // else
    }
    // else if(I2Cx==I2C2)
    // {
    //     I2C2_MessageStruct.I2C_BufAddr=BufAddr;
    //     I2C2_MessageStruct.I2C_BufLen=BufLen;
    //     I2C2_MessageStruct.I2C_DevAddr=SlaAddr;
    //     I2C2_MessageStruct.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress;
    // }
    // else if(I2Cx==I2C3)
    // {
    //     I2C3_MessageStruct.I2C_BufAddr=BufAddr;
    //     I2C3_MessageStruct.I2C_BufLen=BufLen;
    //     I2C3_MessageStruct.I2C_DevAddr=SlaAddr;
    //     I2C3_MessageStruct.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress;
    // }
    else
        return;

    //等待总线空闲
    while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_BUSY));
    //给一个超时，超时之后进入挂死解锁函数
    //发送起始位
    I2C_GenerateSTART(I2Cx,ENABLE);
    //等待CR1_START清零
    while(I2C_ReadRegister(I2Cx,I2C_Register_CR1)==I2C_CR1_START);
    //进入EV5中断

}

void I2C1_EV_IRQHandler(void)
{
    switch (I2C_GetLastEvent(I2C1))//能进入中断，说明有对应的事件产生，这里已经读取了SR1和SR2
    {
    case I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED://EV1，从发送模式，第一个地址
        //基本不用
        //如果是10位地址的iic，转跳 I2C_EVENT_SLAVE_TRANSMITTER_SECONDADDRESS_MATCHED
        break;
    case I2C_EVENT_SLAVE_TRANSMITTER_SECONDADDRESS_MATCHED://EV1，从发送模式，第二个地址
        //基本不用
        break;
    case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED://EV1，从接收模式，第一个地址
        //基本不用
        //如果是10位地址的iic，转跳 I2C_EVENT_SLAVE_RECEIVER_SECONDADDRESS_MATCHED
        break;
    case I2C_EVENT_SLAVE_RECEIVER_SECONDADDRESS_MATCHED://EV1，从接收模式，第二个地址
        //基本不用
        break;
    case I2C_EVENT_SLAVE_GENERALCALLADDRESS_MATCHED://EV1，从模式广播呼叫，对地址0x00应答
        //基本不用
        break;
    
    case I2C_EVENT_MASTER_MODE_SELECT://EV5，起始位发送成功，在此之前CR1的START自动清零
        //读取SR1，在上面switch已经做了
        //写入DR
        if(I2C1_MessageStruct.I2C_AcknowledgedAddress==I2C_AcknowledgedAddress_7bit)//七位模式
        {
            if(I2C1_MessageStruct.I2CStat_Enum==I2C_Stat_Start)
            {
                I2C_Send7bitAddress(I2C1,(uint8_t)I2C1_MessageStruct.I2C_DevAddr,
                                    I2C1_MessageStruct.I2C_Direction);
            }
        }
        else if(I2C1_MessageStruct.I2C_AcknowledgedAddress==I2C_AcknowledgedAddress_10bit)//十位模式
        {
            //用I2C_Send7bitAddress发头序列，I2C_SendData发从地址
            //无论主发送还是主接受，十位I2C都要先发11110xx0
            if(I2C1_MessageStruct.I2CStat_Enum==I2C_Stat_Start)
            {
                uint8_t headAddr;//头地址序列
                headAddr=(uint8_t)(((I2C1_MessageStruct.I2C_DevAddr>>8)<<1)|0xF0);
                I2C_SendData(I2C1,headAddr);
            }
            else if(I2C1_MessageStruct.I2CStat_Enum==I2C_Stat_RptStart)
            {
                uint8_t headAddr;//头地址序列
                headAddr=(uint8_t)(((I2C1_MessageStruct.I2C_DevAddr>>8)<<1)|0xF1);
                I2C_SendData(I2C1,headAddr);
            }
        }
        else//瞎几把写了个值，直接返回
            return;
        break;
    case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED://EV6，主模式发送器
        /**
         * 理论上主发送的七位模式和十位模式下，I2C的EV6不需要
         * 任何操作，因为进来的时候读了SR1和SR2。
         **/
        /**
         * 对于十位I2C的主接收，EV6需要发送一个重复起始位。之
         * 所以重复起始位要放在这里，是因为十位模式下发送地址头序列
         * 时，地址时11110xx0，0结尾是发送器，所以进入这个
         * EV6发重复起始位。
         **/
        if((I2C1_MessageStruct.I2C_AcknowledgedAddress==I2C_AcknowledgedAddress_10bit)&&
           (I2C1_MessageStruct.I2C_Direction==I2C_Direction_Receiver)&&
           (I2C1_MessageStruct.I2CStat_Enum==I2C_Stat_Start))
        {
            I2C_GenerateSTART(I2C1,ENABLE);
            I2C1_MessageStruct.I2CStat_Enum=I2C_Stat_RptStart;
        }
        break;
    case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED://EV6，主模式接收器
        /**
         * 七位主接收的EV6，十位主接收的第二个EV6是这个case
         **/
        /**
         * 在主接收模式下，如果只接收单个字节，要在EV6期间禁止应
         * 答。EV6期间是指ADDR清零之前，因为进入到这个case
         * 的时候我们已经读了SR1和SR2了。
         **/
        /**
         * 所以应在七位模式下发送从设备地址之后立刻设置禁止响应位，
         * 也就是七位主接收的EV5。
         **/
        /**
         * 十位模式下发送地址头序列11110xx1之后立刻设置禁止
         * 响应位，也就是十位主接收重复起始位之后的EV5。
         **/
        /**
         * 但是，理论上其实可以在函数使用的时候不使能应答。
         **/
        /**
         * 进到EV6的时候，位移寄存器已经在接收数据了，这时候设置
         * 一个停止位，停止位会在数据接受完之后生成。
         * 也有可能需要放在EV7
         **/
        if((I2C1_MessageStruct.I2C_Direction==I2C_Direction_Receiver)&&
           (I2C1_MessageStruct.I2C_BufLen==1))
        {
            I2C_GenerateSTOP(I2C1,ENABLE);
            I2C1_MessageStruct.I2CStat_Enum=I2C_Stat_Working;
        }
        else
            I2C1_MessageStruct.I2CStat_Enum=I2C_Stat_Working;
        break;
    case I2C_EVENT_MASTER_BYTE_RECEIVED://EV7，主模式接受完第一字节数据了
        if(I2C1_MessageStruct.I2C_BufLen==1)
        {
            I2C1_MessageStruct.I2C_BufAddr[0]=I2C_ReceiveData(I2C1);
            I2C1_MessageStruct.I2CStat_Enum=I2C_Stat_Finish;
        }
        else if(I2C1_MessageStruct.I2C_BufLen==2)
        {
            if(I2C1_MessageStruct.I2C_DataNum==0)
            {
                while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BTF)!=SET);
                I2C_GenerateSTOP(I2C1,ENABLE);
                I2C1_MessageStruct.I2C_BufAddr[0]=I2C_ReceiveData(I2C1);
                ++I2C1_MessageStruct.I2C_DataNum;
                I2C1_MessageStruct.I2CStat_Enum=I2C_Stat_Working;
            }
            else if(I2C1_MessageStruct.I2C_DataNum==1)
            {
                I2C1_MessageStruct.I2C_BufAddr[1]=I2C_ReceiveData(I2C1);
                I2C1_MessageStruct.I2CStat_Enum=I2C_Stat_Finish;
            }
        }
        else if(I2C1_MessageStruct.I2C_BufLen>2)
        {
            if(I2C1_MessageStruct.I2C_DataNum<I2C1_MessageStruct.I2C_BufLen-3)//还没到N-2
            {
                I2C1_MessageStruct.I2C_BufAddr[I2C1_MessageStruct.I2C_DataNum]=I2C_ReceiveData(I2C1);
                ++I2C1_MessageStruct.I2C_DataNum;
                I2C1_MessageStruct.I2CStat_Enum=I2C_Stat_Working;
            }
            else if(I2C1_MessageStruct.I2C_DataNum==I2C1_MessageStruct.I2C_BufLen-3)
            {
                while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BTF)!=SET);
                I2C_AcknowledgeConfig(I2C1,DISABLE);
                I2C1_MessageStruct.I2C_BufAddr[I2C1_MessageStruct.I2C_DataNum]=I2C_ReceiveData(I2C1);
                ++I2C1_MessageStruct.I2C_DataNum;
            }
            else if (I2C1_MessageStruct.I2C_DataNum==I2C1_MessageStruct.I2C_BufLen-2)
            {
                while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BTF)!=SET);
                I2C_GenerateSTOP(I2C1,ENABLE);
                I2C1_MessageStruct.I2C_BufAddr[I2C1_MessageStruct.I2C_DataNum]=I2C_ReceiveData(I2C1);
                ++I2C1_MessageStruct.I2C_DataNum;
            }
            else if (I2C1_MessageStruct.I2C_DataNum==I2C1_MessageStruct.I2C_BufLen-1)
            {
                I2C1_MessageStruct.I2C_BufAddr[I2C1_MessageStruct.I2C_DataNum]=I2C_ReceiveData(I2C1);
                I2C1_MessageStruct.I2CStat_Enum=I2C_Stat_Finish;
            }
        }
        break;
    case I2C_EVENT_MASTER_BYTE_TRANSMITTING://EV8&EV8_1，主模式正在发送数据
        /**
         * 手册有个EV8_1，当第一个数据都没法送的时候，数据寄存
         * 器为空，位移寄存器也为空，但是因为是第一个数据都没有发送，
         * 所以此时BTF不会置位，EV8_1只会出现一次。
         **/
        /**
         * 在正常发送的情况下，也就是虽然I2C的TxE有可能被别的
         * 东西打断，但是打断期间位移寄存器还在发数据，等回到这个中
         * 断的时候位移寄存器还没发完，就会进入这个EV8。
         **/
        //所以说进EV8的话，发就完事了
        if(I2C1_MessageStruct.I2C_DataNum<I2C1_MessageStruct.I2C_BufLen)
        {
            I2C_SendData(I2C1,I2C1_MessageStruct.I2C_BufAddr[I2C1_MessageStruct.I2C_DataNum]);
            ++I2C1_MessageStruct.I2C_DataNum;
            I2C1_MessageStruct.I2CStat_Enum=I2C_Stat_Working;
        }
        else//雨我无瓜
            return;
        break;
    case I2C_EVENT_MASTER_BYTE_TRANSMITTED://EV8_2，主模式发送完数据了
        /**
         * EV8_2有两种情况，第一个情况是数据确实发送完了，位移
         * 寄存器和数据寄存器都没有值，这时候就要发送停止位。
         **/
        /**
         * 另一种情况是I2C的TxE被别的东西打断了，而且打断时间
         * 还挺长，导致位移寄存器和数据寄存器都没有数据，此时BTF
         * 置位，告诉你所有东西都发完了，而且会延长SCL的低电平等
         * 你发后续的数据。
         **/
        if(I2C1_MessageStruct.I2C_DataNum==I2C1_MessageStruct.I2C_BufLen)
        {
            //发送结束，来个停止位
            I2C_GenerateSTOP(I2C1,ENABLE);
            I2C1_MessageStruct.I2CStat_Enum=I2C_Stat_Finish;
        }
        else if(I2C1_MessageStruct.I2C_DataNum<I2C1_MessageStruct.I2C_BufLen)
        {
            //还没发完
            I2C_SendData(I2C1,I2C1_MessageStruct.I2C_BufAddr[I2C1_MessageStruct.I2C_DataNum]);
            ++I2C1_MessageStruct.I2C_DataNum;
            I2C1_MessageStruct.I2CStat_Enum=I2C_Stat_Working;
        }
        else//传输数据位数>数据长度？？？
            return;
        break;
    case I2C_EVENT_MASTER_MODE_ADDRESS10://EV9，十位地址的头序列11110xx0发送完毕
        I2C_SendData(I2C1,(uint8_t)I2C1_MessageStruct.I2C_DevAddr);//发送从地址
        break;

    default:
        break;
    }
}

void I2C1_ER_IRQHandler(void)
{
    //SMBus报警
    if(I2C_GetITStatus(I2C1,I2C_IT_SMBALERT)!=RESET)
    {
        //仅SMBus模式有用
        I2C_ClearITPendingBit(I2C1,I2C_IT_SMBALERT);
    }
    //超时/Tlow错误
    if(I2C_GetITStatus(I2C1,I2C_IT_TIMEOUT)!=RESET)
    {
        I2C_ClearITPendingBit(I2C1,I2C_IT_TIMEOUT);
    }
    //PEC错误
    if(I2C_GetITStatus(I2C1,I2C_IT_PECERR)!=RESET)
    {
        I2C_ClearITPendingBit(I2C1,I2C_IT_PECERR);
    }
    //上溢/下溢
    if(I2C_GetITStatus(I2C1,I2C_IT_OVR)!=RESET)
    {
        I2C_ClearITPendingBit(I2C1,I2C_IT_OVR);
    }
    //应答失败
    if(I2C_GetITStatus(I2C1,I2C_IT_AF)!=RESET)
    {
        I2C_ClearITPendingBit(I2C1,I2C_IT_AF);
    }
    //仲裁丢失（主模式）
    if(I2C_GetITStatus(I2C1,I2C_IT_ARLO)!=RESET)
    {
        I2C_ClearITPendingBit(I2C1,I2C_IT_ARLO);
    }
    //总线错误
    if(I2C_GetITStatus(I2C1,I2C_IT_BERR)!=RESET)
    {
        I2C_ClearITPendingBit(I2C1,I2C_IT_BERR);
    }
}

// void I2C_ReadByte