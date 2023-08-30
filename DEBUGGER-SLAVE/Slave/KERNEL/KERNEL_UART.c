/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-07-31 18:35:30
 * @Description: 
 * 
 * 
 * The following is the Chinese and English copyright notice, encoded as UTF-8.
 * 以下是中文及英文版权同步声明，编码为UTF-8。
 * Copyright has legal effects and violations will be prosecuted.
 * 版权具有法律效力，违反必究。
 * 
 * Copyright ©2021-2023 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
 * 版权所有 ©2021-2023龙晶石半导体科技（苏州）有限公司
 */
#include "AE_INCLUDE.H"
#include "KERNEL_UART.H"

void serial_base(DWORD uart_num)
{
	switch(uart_num)
	{
		case 0:
			g_uart_base = UART0_BASE_ADDR;
			break;
        #if (defined(AE102)||defined(AE103))
		case 1:
			g_uart_base = UART1_BASE_ADDR;
			break;
		case 2:
			g_uart_base = UART2_BASE_ADDR;
			break;
		case 3:
			g_uart_base = UART3_BASE_ADDR;
			break;
		case 4:
			g_uart_base = UARTA_BASE_ADDR;
			break;
		case 5:
			g_uart_base = UARTB_BASE_ADDR;
			break;
        #endif
		default:
			g_uart_base = UARTA_BASE_ADDR;
			break;
	}
}
void serial_config(DWORD uart_num,DWORD baudrate)
{	
	VBYTEP uart_lcr = (VBYTEP )(g_uart_base + UART_LCR_OFFSET);
	VBYTEP uart_divisor_lsb = (VBYTEP )(g_uart_base+UART_DLL_OFFSET);
	VBYTEP uart_divisor_msb = (VBYTEP )(g_uart_base + UART_DLH_OFFSET);
	VBYTEP uart_fcr = (VBYTEP )(g_uart_base + UART_FCR_OFFSET);
    VBYTEP uart_ier = (VBYTEP )(g_uart_base + UART_IER_OFFSET);
	register unsigned long divisor;
	register unsigned long freq;

	
//	divisor = serial_divisor();#
#ifdef AE103
    freq=24576000;
#else 
    freq=CPU_FREQ;
#endif
    divisor=freq/baudrate+0b100;//27

    //Set DLAB to 1 in LCR
	*uart_lcr = UART_LCR_DLAB|UART_LCR_DLS_8bit;
	if ((uart_num == 0) || (uart_num == 1) || (uart_num == 2) || (uart_num == 3))
		divisor >>= 3;
	else if ((uart_num == 4) || (uart_num == 5))
	    divisor >>=4;
	*uart_divisor_lsb = divisor;
	*uart_divisor_msb = (divisor >> 8);
	//Set Line Control Register
	*uart_fcr = UART_FCR_TFTL_8BYTE|UART_FCR_FIFOEN|UART_FCR_TFR|UART_FCR_RFR;
    *uart_lcr&=~UART_LCR_DLAB;
	*uart_ier|=UART_IER_RDAIE|UART_IER_PTIME;//recived data int enable
}

int serial_init(DWORD uart_num, DWORD baudrate)
{
	serial_base(uart_num);
	serial_config(uart_num,baudrate);
	return 0;
}
BYTE  Uart_Int_Enable(BYTE  channel, BYTE  type)
{
    switch(channel)
    {
        case UART0_CHANNEL:
            UART0_IER |= (0x1<<type);
            return 0;
        #if (defined(AE102)||defined(AE103))
        case UART1_CHANNEL:
            UART1_IER |= (0x1<<type);
            return 0;
        case UART2_CHANNEL:
            UART2_IER |= (0x1<<type);
            return 0;
        case UART3_CHANNEL:
            UART3_IER |= (0x1<<type);
            return 0;
        #endif
        case UARTA_CHANNEL:
            UARTA_IER |= (0x1<<type);
            return 0;
        case UARTB_CHANNEL:
            UARTB_IER |= (0x1<<type);
            return 0;
        default:
            return -1;
    }
}

BYTE  Uart_Int_Disable(BYTE  channel, BYTE  type)
{
    switch(channel)
    {
        
        case UART0_CHANNEL:
            UART0_IER &= ~(0x1<<type);
            return 0;
        #if (defined(AE102)||defined(AE103))
        case UART1_CHANNEL:
            UART1_IER &= ~(0x1<<type);
            return 0;
        case UART2_CHANNEL:
            UART2_IER &= ~(0x1<<type);
            return 0;
        case UART3_CHANNEL:
            UART3_IER &= ~(0x1<<type);
            return 0;
        #endif
        case UARTA_CHANNEL:
            UARTA_IER &= ~(0x1<<type);
            return 0;
        case UARTB_CHANNEL:
            UARTB_IER &= ~(0x1<<type);
            return 0;
        default:
            return -1;
    }
}

BYTE  Uart_Int_Enable_Read(BYTE  channel, BYTE  type)
{
    switch(channel)
    {
        case UART0_CHANNEL:
            return ((UART0_IER & (0x1<<type)) != 0);
        #if (defined(AE102)||defined(AE103))
        case UART1_CHANNEL:
            return ((UART1_IER & (0x1<<type)) != 0);
        case UART2_CHANNEL:
            return ((UART2_IER & (0x1<<type)) != 0);
        case UART3_CHANNEL:
            return ((UART3_IER & (0x1<<type)) != 0);
        #endif
        case UARTA_CHANNEL:
            return ((UARTA_IER & (0x1<<type)) != 0);
        case UARTB_CHANNEL:
            return ((UARTB_IER & (0x1<<type)) != 0);
        default:
            return -1;
    }
}

BYTE  Uart_Int_Status(BYTE  channel, BYTE  type)
{
    switch(channel)
    {
        case UART0_CHANNEL:
            return ((UART0_IIR & (0x1<<type)) != 0);
        #if (defined(AE102)||defined(AE103))
        case UART1_CHANNEL:
            return ((UART1_IIR & (0x1<<type)) != 0);
        case UART2_CHANNEL:
            return ((UART2_IIR & (0x1<<type)) != 0);
        case UART3_CHANNEL:
            return ((UART3_IIR & (0x1<<type)) != 0);
        #endif
        case UARTA_CHANNEL:
            return ((UARTA_IIR & (0x1<<type)) != 0);
        case UARTB_CHANNEL:
            return ((UARTB_IIR & (0x1<<type)) != 0);
        default:
            return -1;
    }
}
WEAK int putchar(int ch)/*字符输出*/
{
    #if PRINTF_UART_SWITCH<6
    char str=(char)ch;
    #if SUPPORT_SLAVE_PRINTF
    if(F_Service_PUTC)
    {
        if (str == '\n') 
        {
            while (!(PRINTF_LSR & UART_LSR_TEMP)) ;
            PRINTF_TX = '\r';
        }
        while (!(PRINTF_LSR & UART_LSR_TEMP)) ;/*当此位为空发送fifo写入数据*/
        PRINTF_TX = str;
    }
    else 
    {
        if (str == '\n') 
        {
            //while (print_buff[print_cnt]!='\0')WDT_REG(0xC) =0x76;           
            print_buff[print_cnt]='\r';
            print_cnt++;
            print_number++;
            if(print_cnt>=PRINT_MAX_SIZE)print_cnt=0;

        }
        // while (print_buff[print_cnt]!='\0')WDT_REG(0xC) =0x76;
        print_buff[print_cnt]=str;
        print_cnt++;
        print_number++;
        if(print_cnt>=PRINT_MAX_SIZE)print_cnt=0;
    }
    #else
    if (str == '\n') 
    {
		while (!(PRINTF_LSR & UART_LSR_TEMP)) ;
		PRINTF_TX = '\r';
    }
    while (!(PRINTF_LSR & UART_LSR_TEMP)) ;/*当此位为空发送fifo写入数据*/
    PRINTF_TX = str;
    #endif
    return ch;
    #endif
}
void DEBUGER_putchar(char ch)
{
    #if DEBUG_UART_SWITCH<6
    while (!(DEBUGGER_UART_REG(UART_LSR_OFFSET) & 0x60)) ;/*FIFO check*/
(*((DWORD*)(DEBUGGER_UART))) = ch;//send by uart
    #endif
}