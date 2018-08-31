/*
*******************************************************************************
*    模块：BSP_GSL268x
*    描述：板级 GSL268x 功能模块驱动
           GSL268x_ADDR  0x80
*    作者：Huo
*    时间：2017.12.12
*    版本：UP-Magic-Version 1.0.0
*******************************************************************************
*/
#include "BSP_GSL268x.h"
#include "GSL268x_fw.h"

/* 内部函数声明 */
static void BSP_GSL268x_GPIO_Init(void);
static uint8_t I2C_WriteByte(uint8_t SendByte, uint8_t WriteAddress);
static uint8_t I2C_BufferWrite(uint8_t* pBuffer, uint8_t length, uint8_t WriteAddress);
static uint8_t I2C_BufferRead(uint8_t* pBuffer, uint8_t length, uint8_t ReadAddress);
static void gsl_start_core(void);
static void gsl_reset_core(void);
static void gsl_clear_reg(void);
static uint8_t check_mem_data(void);
static void gsl_fw_load(void);

/* 非精确延时函数 */
static void BSP_GSL268x_DelayMs(volatile uint16_t xMs)
{
    volatile uint32_t Count=24000;
    while(xMs--)
    {
        Count=24000;
        while(Count--);
    }
}

/*
*******************************************************************************
*	函 数 名: BSP_GSL268x_Init
*	功能说明: 板级 GSL268x 初始化函数
*	形    参: 无
*	返 回 值: 无
*******************************************************************************
*/
void BSP_GSL268x_Init(void)
{
//	BSP_MyIIC_Init();
    while(BSP_MyIIC_CheckDevice(GSL268x_ADDR)) {};
    //init gpio
    BSP_GSL268x_GPIO_Init();

    //set register
    gsl_clear_reg();
    gsl_reset_core();
    gsl_fw_load();
    gsl_start_core();
    BSP_GSL268x_DelayMs(20);
    check_mem_data();
}

/*
*******************************************************************************
*	函 数 名: BSP_GSL268x_GPIO_Init
*	功能说明: 板级 GSL268x IO口 初始化函数
*	形    参: 无
*	返 回 值: 无
*******************************************************************************
*/
static void BSP_GSL268x_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_GSL268x_INT, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
    GPIO_InitStructure.GPIO_Pin = PIN_GSL268x_INT; //对应引脚
    GPIO_Init(PORT_GSL268x_INT, &GPIO_InitStructure);
}

/*
*******************************************************************************
*	函 数 名: BSP_GSL268x_Callback
*	功能说明: 板级 GSL268x 上报点位功能函数
*	形    参: PEN 点位数组
*	返 回 值: 0-有效  1-无效
*******************************************************************************
*/
uint8_t BSP_GSL268x_Callback(uint16_t PEN[2])
{
    uint8_t TPCodebuf[4];

    if(GPIO_ReadInputDataBit(PORT_GSL268x_INT, PIN_GSL268x_INT))
    {
        I2C_BufferRead(&TPCodebuf[0],4,0x84);
        PEN[0] = ((0x0F&TPCodebuf[3])<<8)|TPCodebuf[2];  //X
        PEN[1] = TPCodebuf[1]<<8|TPCodebuf[0];           //Y

        return 0;
    }
    else
        return 1;
}

/*
*******************************************************************************
*	函 数 名: gsl_start_core  gsl_reset_core  gsl_clear_reg  check_mem_data  gsl_fw_load
*	功能说明: GSL268x寄存器操作功能函数
*	形    参:
*	返 回 值:
*******************************************************************************
*/
static void gsl_start_core(void)
{
    I2C_WriteByte(0x00,0xe0);
    BSP_GSL268x_DelayMs(1);
}
static void gsl_reset_core(void)
{
    I2C_WriteByte(0x88,0xE0);
    BSP_GSL268x_DelayMs(1);
    I2C_WriteByte(0x04,0xE4);
    BSP_GSL268x_DelayMs(1);
    I2C_WriteByte(0x00,0xbc);
    BSP_GSL268x_DelayMs(1);
}
static void gsl_clear_reg(void)
{
    I2C_WriteByte(0x88,0xE0);
    BSP_GSL268x_DelayMs(1);
    I2C_WriteByte(0x01,0x80);
    BSP_GSL268x_DelayMs(1);
    I2C_WriteByte(0x04,0xE4);
    BSP_GSL268x_DelayMs(1);
    I2C_WriteByte(0x00,0xE0);
    BSP_GSL268x_DelayMs(1);
}
static uint8_t check_mem_data(void)
{
    uint8_t read_buf[4]  = {0};

    I2C_BufferRead(read_buf,4,0xb0);
    BSP_GSL268x_DelayMs(1);
    if (read_buf[3] != 0x5a || read_buf[2] != 0x5a || read_buf[1] != 0x5a || read_buf[0] != 0x5a)
    {
#if 1
        printf("load firmware failed.!\n");
#endif
        return 0;
    }
    else
    {
#if 0
        printf("load firmware success.!\n");
#endif
        return 1;
    }
}
static void gsl_fw_load(void)
{
    uint8_t buf[5] = {0};
    uint32_t source_line = 0;
    uint8_t *cur = buf + 1;

    for (source_line = 0; source_line < TOUCH_CODE_LEN; source_line++)
    {
        buf[0]= GSL2681_FW[source_line].offset;
        buf[1] = (uint8_t)(GSL2681_FW[source_line].val&0x000000ff);
        buf[2] = (uint8_t)((GSL2681_FW[source_line].val&0x0000ff00) >> 8);
        buf[3] = (uint8_t)((GSL2681_FW[source_line].val&0x00ff0000) >> 16);
        buf[4] = (uint8_t)((GSL2681_FW[source_line].val&0xff000000) >> 24);

        if (buf[0] == 0xf0)
        {
            I2C_WriteByte(buf[1],buf[0]);
        }
        else
        {
            I2C_BufferWrite(cur, 4,buf[0]);
        }
    }
}

/*
*******************************************************************************
*	函 数 名: I2C_WriteByte  I2C_BufferWrite  I2C_BufferRead
*	功能说明: GSL268x接口操作功能函数
*	形    参:
*	返 回 值:
*******************************************************************************
*/
static uint8_t I2C_WriteByte(uint8_t SendByte, uint8_t WriteAddress)
{
    uint16_t m;

    for (m = 0; m < 1000; m++)
    {
        /* 第1步：发起I2C总线启动信号 */
        BSP_MyIIC_Start();
        /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
        BSP_MyIIC_SendByte(GSL268x_ADDR | I2C_WR);	/* 此处是写指令 */
        /* 第3步：发送一个时钟，判断器件是否正确应答 */
        if (BSP_MyIIC_WaitAck() == 0)
        {
            break;
        }
    }
    if (m  == 1000)
    {
        /* EEPROM器件写超时,发送I2C总线停止信号 */
        BSP_MyIIC_Stop();
        return 0;
    }
    /* 第4步：发送字节地址 */
    BSP_MyIIC_SendByte((uint8_t)WriteAddress);
    if (BSP_MyIIC_WaitAck() != 0)
    {
        /* EEPROM器件无应答,发送I2C总线停止信号 */
        BSP_MyIIC_Stop();
        return 0;
    }
    /* 第6步：开始写入数据 */
    BSP_MyIIC_SendByte(SendByte);
    /* 第7步：发送ACK */
    if (BSP_MyIIC_WaitAck() != 0)
    {
        /* EEPROM器件无应答,发送I2C总线停止信号 */
        BSP_MyIIC_Stop();
        return 0;
    }
    /* 命令执行成功，发送I2C总线停止信号 */
    BSP_MyIIC_Stop();
    return 1;
}
static uint8_t I2C_BufferWrite(uint8_t* pBuffer, uint8_t length, uint8_t WriteAddress)
{
    uint16_t i,m;

    for (i = 0; i < length; i++)
    {
        for (m = 0; m < 1000; m++)
        {
            /* 第1步：发起I2C总线启动信号 */
            BSP_MyIIC_Start();
            /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
            BSP_MyIIC_SendByte(GSL268x_ADDR | I2C_WR);	/* 此处是写指令 */
            /* 第3步：发送一个时钟，判断器件是否正确应答 */
            if (BSP_MyIIC_WaitAck() == 0)
            {
                break;
            }
        }
        if (m  == 1000)
        {
            /* EEPROM器件写超时,发送I2C总线停止信号 */
            BSP_MyIIC_Stop();
            return 0;
        }
        /* 第4步：发送字节地址 */
        BSP_MyIIC_SendByte((uint8_t)WriteAddress);
        if (BSP_MyIIC_WaitAck() != 0)
        {
            /* EEPROM器件无应答,发送I2C总线停止信号 */
            BSP_MyIIC_Stop();
            return 0;
        }
        /* 第6步：开始写入数据 */
        BSP_MyIIC_SendByte(pBuffer[i]);
        /* 第7步：发送ACK */
        if (BSP_MyIIC_WaitAck() != 0)
        {
            /* EEPROM器件无应答,发送I2C总线停止信号 */
            BSP_MyIIC_Stop();
            return 0;
        }
        WriteAddress++;
    }
    /* 命令执行成功，发送I2C总线停止信号 */
    BSP_MyIIC_Stop();
    return 1;
}
static uint8_t I2C_BufferRead(uint8_t* pBuffer, uint8_t length, uint8_t ReadAddress)
{
    uint16_t i;

    /* 第1步：发起I2C总线启动信号 */
    BSP_MyIIC_Start();
    /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    BSP_MyIIC_SendByte(GSL268x_ADDR | I2C_WR);	/* 此处是写指令 */
    /* 第3步：发送ACK */
    if (BSP_MyIIC_WaitAck() != 0)
    {
        /* EEPROM器件无应答,发送I2C总线停止信号 */
        BSP_MyIIC_Stop();
        return 0;
    }
    /* 第4步：发送字节地址 */
    BSP_MyIIC_SendByte((uint8_t)ReadAddress);
    if (BSP_MyIIC_WaitAck() != 0)
    {
        /* EEPROM器件无应答,发送I2C总线停止信号 */
        BSP_MyIIC_Stop();
        return 0;
    }
    /* 第5步：发送I2C总线停止信号 */
    BSP_MyIIC_Stop();
    BSP_GSL268x_DelayMs(10);		//huo 20160905

    /* 第6步：重新启动I2C总线。下面开始读取数据 */
    BSP_MyIIC_Start();
    /* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    BSP_MyIIC_SendByte(GSL268x_ADDR | I2C_RD);	/* 此处是读指令 */
    /* 第8步：发送ACK */
    if (BSP_MyIIC_WaitAck() != 0)
    {
        /* EEPROM器件无应答,发送I2C总线停止信号 */
        BSP_MyIIC_Stop();
        return 0;
    }
    /* 第9步：循环读取数据 */
    for (i = 0; i < length; i++)
    {
        pBuffer[i] = BSP_MyIIC_ReadByte();	/* 读1个字节 */

        /* 每读完1个字节后，需要发送Ack， 最后一个字节不需要Ack，发Nack */
        if (i != length - 1)
        {
            BSP_MyIIC_Ack();	/* 中间字节读完后，CPU产生ACK信号(驱动SDA = 0) */
        }
        else
        {
            BSP_MyIIC_NAck();	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
        }
    }
    /* 发送I2C总线停止信号 */
    BSP_MyIIC_Stop();
    return 1;	/* 执行成功 */
}
