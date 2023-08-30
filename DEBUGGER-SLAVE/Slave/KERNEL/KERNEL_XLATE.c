/*
 * @Author: Iversu
 * @LastEditors: dejavudwh
 * @LastEditTime: 2023-05-26 20:51:39
 * @Description: This file is used for Code xlate
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

#include "KERNEL_XLATE.H"

#include "AE_PRINTF.H"
#include "CUSTOM_POWER.H"

BYTE press_times = 0;
/* BREAK_EVENT key string for Convert_Cursor and Numlock_Corsor. */
	const BYTE  csr_brk1[] = {0xE0,0xF0,0x00};
// Key Code Process Table.
const FUNCT_PTR_V_B_B  kcp_vector_table[] =
{
    Single_Code,        // index 00h DO_SIMPLE_CODE IBM key number 83,84 
    Add_E0_Code,     // index 01h DO_E0_PREFIX_CODE
    Convert_Cursor,       // index 02h DO_CURSOR_SHIFT
    Numlock_Corsor,     // index 03h DO_CURSOR_NUMLOCK
    Particular_Code,       // index 04h DO_SPECIAL_CODE IBM key number 126 
    Generate_Scan_Code,     // index 05h DO_CONTROL_EFFECT Shift, Ctrl, Alt, Numlock 
    Generate_Scan_Code_E0,	// index 06h DO_CONTROL_EFFECT_E0 Ctrl, Alt 
    HotKey_Fn_Function	// index 07h DO_COSTOMER_FUNCTION costomer function key，fn功能键处理函数
};
/* ----------------------------------------------------------------------------
 * FUNCTION: KBS_Xlate_Code2
 *
 * Input: key   = Row/Column number
 *                key.index.kso = 0 - 15
 *                key.index.ksi  = 0 - 7
 *
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 *
 *        Scanner_State byte
 *
 *
 * On return, Scanner_State byte   are updated.
 *
 * Combination flag for each row/column
 *     0 = Insyde Software key number 2 in Row/Column Table
 *     1 = index for control_ptr_table in Row/Column Table
 *
 *     01-7F = Same as Scan Code, Set 2
 *     80-FF = E0-prefix, cursor, printscreen, pause/break, special key
 * ------------------------------------------------------------------------- */
void KBS_Xlate_Code2(uKEY key, BYTE event)
{
    #ifdef KBD_SN1211//sn1211扫描ksi值（row值）为反值，此问题受限于键盘硬件排线有关
    key.index.ksi=7-key.index.ksi;
    #endif 
    // Hardware KBD_SCAN keyboard
    //*******************************************************
    _R4 = Rc_ROM_Tables[key.index.kso][key.index.ksi];
    #if KB_MS_DEBUG
    dprint("KBD code is 0x%x\n",_R4);
    #endif
    /*2： Keyboard wake up system 唤醒系统的操作，暂时不需要*/
    //Check_KB_IN_S3(key.index.ksi, key.index.kso);
    if(SystemIsS3)
	{
		Check_KB_Wake_S3(key.index.ksi,key.index.kso);
	}

    /*3：将从数组中查找到的按键码和按键的事件发送下去*/
    Transmit_Key(_R4, event);
}
//*****************************************************************************
//
//  The function for checking Fn key bundled flag.
//
//  parameter :
//      KBD_code:按键的行列组合
//      event ：具体的事件 MAKE_EVENT | BREAK_EVENT | REPEAT_EVENT
//
//  return :
//      Fn_bundled :  0 --> no fn key status
//                      1 --> with fn key status
//
//  
//  
//*****************************************************************************
BYTE Check_FnKey_Related_Flag(BYTE KBD_code, BYTE event)
{
    BYTE Fn_bundled = 0x00;// pre-set no bundled fn key
    BYTE buffer_index= ((KBD_code-0x90)/8);  // index of buffer "Fn_BUNDLED_FLAG"
    BYTE buffer_mask= ((KBD_code-0x90)%8);   // mask of buffer "Fn_BUNDLED_FLAG"
    if(KBD_code >= SSKEY2_SPE_CODE)
    {
		// Key break按键break事件
        if(event==BREAK_EVENT)                    
        {
            if(Fn_BUNDLED_FLAG[buffer_index]&BIT(buffer_mask)) //with fn key but no bundled fn key flag.
            {
                Fn_bundled = 0x01;            // with fn key and bundled fn key flag.
            }
            CLEAR_MASK(Fn_BUNDLED_FLAG[buffer_index], BIT(buffer_mask));
        }
        else                                        // Key make or repeat
        {
            if(KBD_SCAN_STATE.index.Fn&&event==MAKE_EVENT)        // Fn key
                    Fn_BUNDLED_FLAG[buffer_index]|=BIT(buffer_mask);
            if(KBD_SCAN_STATE.index.Fn || (Fn_BUNDLED_FLAG[buffer_index]& BIT(buffer_mask)))
                Fn_bundled = 0x01;                // with fn key or bundled fn key flag.
        }
    }
    return(Fn_bundled);
}
/* ----------------------------------------------------------------------------
 * FUNCTION: Transmit_Key
 table_entry:按键从数组中查到的操作码
 event：具体的按键事件，make还是break
 * ------------------------------------------------------------------------- */
void Transmit_Key(BYTE table_entry, BYTE event)
{
    if(Skip_Send_Key()==0xFF)return;
    BYTE temp;
    BYTE temp_table_entry = table_entry;
    Check_Send_Key(table_entry,event);
    KBD_SCAN.mark = KBD_SCAN.tail;   /*标记缓冲区防止溢出*/
    KBD_SCAN_STATE.byte = Scanner_State;/*组合键的flag，这个flag在KBD_SCAN_STATE.byte   修改*/
    if (table_entry >= SSKEY2_OVL_CODE)				// Fn key + any key.
    {   
		/*特殊功能按键的处理*/
        temp = (table_entry - SSKEY2_OVL_CODE)<<1;
        if(Check_FnKey_Related_Flag(temp_table_entry, event)==0x01) temp++; // 递增索引以获得表项的奇数字节
        table_entry = sskey2_overlay_table[temp];	// Get a sskey2 value. 
        #ifdef KBD_8_16
        // if((table_entry==0x77)&&(event==MAKE_EVENT))
		// {
		// 	dprint("num lock\n");
		// 	NumLockKey|=NUM_LOCK;
		// }
		// else
		if((table_entry==0x77)&&(event==BREAK_EVENT))
		{
			dprint("num unlock\n");
			NumLockKey = !NumLockKey;
		}
        #endif
    }
    #ifdef KBD_8_16
    else if(table_entry>=SSKEY2_SPE_CODE)
        {
            temp = table_entry-SSKEY2_SPE_CODE;
            table_entry=sskey2_A2_table[temp].comb;
            BYTE *pntr = (BYTE *)sskey2_A2_table[temp].pntr;
            pntr+=Get_Comb_Index(table_entry,temp_table_entry,event);
            table_entry = *pntr;
        }
    #endif
    Process_Sskey3(table_entry, event);    		// Generate KBD_SCAN code, set 2.
	Scanner_State = KBD_SCAN_STATE.byte;	// Update scanner state. 
}
/* ----------------------------------------------------------------------------
 * FUNCTION: Process_Sskey3
 *
 * Input: sskey2 = Insyde Software Key 2
 *        event =  Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 *
 * Note: FUNCT_PTR_V_B_B is a pointer to a function that returns nothing (V for
 *       void) and takes a BYTE and a BYTE for parameters (B for BYTE, S for
 *       BYTE).
 * ------------------------------------------------------------------------- */
 void Process_Sskey3(BYTE kbd_code2, BYTE event)
{
    if (kbd_code2 == 0){}				// Null code 
    else if ((kbd_code2 & 0x80) == 0)
    {
    	/*普通按键的处理，传递给buffer缓存*/
        #if KB_MS_DEBUG
        dprint("the kbd_code2 = %x,event = %x\n",kbd_code2,event);
        #endif
     	Single_Code(kbd_code2, event);	// 01h through 7Fh = KBD_SCAN code. 
    }
    else  							// 80h through FFh. 
    {   
        kbd_code2 = kbd_code2 << 1;
		/*
		code_byte：再次转化按键的code码
		index：处理函数的索引
		*/
        BYTE code_byte = sskey3_80_table[kbd_code2+0];
        BYTE index = sskey3_80_table[kbd_code2+1];
		dprint("the code_byte = %x,index = %x\n",code_byte,index);		
        (kcp_vector_table[index])(code_byte, event); // Do procedure 
    } 
}
/* ----------------------------------------------------------------------------
 * FUNCTION: Particular_Code
 *
 * For IBM Key Number 126. This key generates only make code.
 *
 * Input: code
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 * ------------------------------------------------------------------------- */
void Particular_Code(BYTE code_byte, BYTE event)
{
    static const BYTE  key126_normal_mk[] = {0xE1,0x14,0x77,0xE1,0xF0,0x14,0xF0,0x77,0x00};
    static const BYTE  key126_ctrl_mk[]   = {0xE0,0x7E,0xE0,0xF0,0x7E,0x00};
    if(code_byte == 2)                  // Print Scr
    {
            if(event == BREAK_EVENT)    // Break
            {
                Add_E0_Code(0x7C, event);
                Add_E0_Code(0x12, event);
            }
            else                        // Make or repeat
            {
                Add_E0_Code(0x12, event);
                Add_E0_Code(0x7C, event);
            }
    }
    else
    {
        if (event == MAKE_EVENT)
        {
            if(Scanner_State_CONTROL)
                KBS_Buffer_puts(key126_ctrl_mk);	    // Buffer Ctrl case string.
            else
                KBS_Buffer_puts(key126_normal_mk);    // Buffer normal code string.
        }
    }
}
/* ----------------------------------------------------------------------------
 * FUNCTION: Single_Code
 *
 * Input: scan_code, Set 2
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 * ------------------------------------------------------------------------- */
void Single_Code(BYTE scan_code, BYTE event)
{
    if(event == BREAK_EVENT)
        KBS_Buffer_Input(0xF0);	// Buffer break prefix for break contact. F0h is break prefix. 
    KBS_Buffer_Input(scan_code);  // Buffer base code. 
}
//*****************************************************************************
//
//  The function for checking additional key.
//
//  parameter :
//      scan_code_a : scan_code, Set 2
//
//  return :
//      0x00 : not additional key
//      0x01 : additional key [group]
//      0x02 : additional key [Numeric / on US keyboards]
//
//*****************************************************************************
BYTE Check_Others_Key(BYTE scan_code_a)
{
    if(scan_code_a==0x70        // Insert
        ||scan_code_a==0x71     // Delete
        ||scan_code_a==0x6B     // Left Arrow
        ||scan_code_a==0x6C     // Home
        ||scan_code_a==0x69     // End
        ||scan_code_a==0x75     // Up Arrow
        ||scan_code_a==0x72     // Dn Arrow
        ||scan_code_a==0x7D     // Page Up
        ||scan_code_a==0x7A     // Page Down
        ||scan_code_a==0x74)    // Right Arrow
        return(0x01);
    else if(scan_code_a==0x4A)  // Numeric / on US keyboards
        return(0x02);           
    else
        return(0x00);
}
/* ----------------------------------------------------------------------------
 * FUNCTION: Add_E0_Code
 *
 * Input: scan_code, Set 2
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 * ------------------------------------------------------------------------- */
void Add_E0_Code(BYTE scan_code, BYTE event)
{
    BYTE additional_key = Check_Others_Key(scan_code);
    static const BYTE kbd_buf_left0[]={0xE0,0xF0,0x12};
    static const BYTE kbd_buf_RIGHT0[]={0xE0,0xF0,0x59};
    static const BYTE kbd_buf_left1[]={0xE0,0x12};
    static const BYTE kbd_buf_RIGHT1[]={0xE0,0x59};
    //-------------------------------------------
    // To check make and repeat
    //-------------------------------------------
    if(event != BREAK_EVENT)    // Make. Repeat
    {
        if(additional_key==0x01)
        {
            if(Scanner_State_NUM_LOCK)  // Num Lock ON
            {
                if((!Scanner_State_LEFT)&&(!Scanner_State_RIGHT)) // LShift || Rshift
                    KBS_Buffer_puts(kbd_buf_left1);
            }
            else                        // Num Lock OFF
            {
                if(Scanner_State_LEFT)KBS_Buffer_puts(kbd_buf_left0);   //Precede Base Make code with (E0 f0 12)
                if(Scanner_State_RIGHT)KBS_Buffer_puts(kbd_buf_RIGHT0);   //Precede Base Make code with (E0 f0 59) 
            }
        }
        else if(additional_key==0x02)
        {
            if(Scanner_State_LEFT)KBS_Buffer_puts(kbd_buf_left0);   //Precede Base Make code with (E0 f0 12)
            if(Scanner_State_RIGHT)KBS_Buffer_puts(kbd_buf_RIGHT0);   //Precede Base Make code with (E0 f0 59) 
        }
    }
    KBS_Buffer_Input(0xE0);   	// Buffer E0h prefix. 
    if (event == BREAK_EVENT)   // Buffer break prefix for break contact. F0h is break prefix. 
        KBS_Buffer_Input(0xF0);
    KBS_Buffer_Input(scan_code);  // Buffer base code. 

    //-------------------------------------------
    // To check break
    //-------------------------------------------
    if(event == BREAK_EVENT)    // Break
    {
        if(additional_key==0x01)
        {
            if(Scanner_State_NUM_LOCK)  // Num Lock ON
            {
                if((!Scanner_State_LEFT)&&(!Scanner_State_RIGHT)) // LShift || Rshift
                    KBS_Buffer_puts(kbd_buf_left1);
            }
            else                        // Num Lock OFF
            {
                if(Scanner_State_LEFT)
                    KBS_Buffer_puts(kbd_buf_left1);
                if(Scanner_State_RIGHT)
                    KBS_Buffer_puts(kbd_buf_RIGHT1);
            }
        }
        else if(additional_key==0x02)
        {
            if(Scanner_State_LEFT)
                KBS_Buffer_puts(kbd_buf_left1);
            if(Scanner_State_RIGHT)
                KBS_Buffer_puts(kbd_buf_RIGHT1);
        }
    }
}
/* ----------------------------------------------------------------------------
 * FUNCTION: Convert_Cursor
 *
 * Input: scan_code, Set 2
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 *        KBD_SCAN_STATE
 * ------------------------------------------------------------------------- */
 void Convert_Cursor(BYTE scan_code, BYTE event)
{
    static const BYTE  csr_sftl_mk[]   = {0xE0,0xF0,0x12,0x00};
    static const BYTE  csr_sftr_mk[]   = {0xE0,0xF0,0x59,0x00};
    static const BYTE  csr_sftl_brk2[] = {0xE0,0x12,0x00};
    static const BYTE  csr_sftr_brk2[] = {0xE0,0x59,0x00};
    if (event == BREAK_EVENT)   /* Key has just been released. This is a "break event". */
        KBS_Buffer_puts(csr_brk1);    		/* Buffer pre-string. */
    else if (event == MAKE_EVENT)
    {   /* Key is pressed for the first time, a "make event". */
        									/* Left and/or Right SHIFT is pressed. */
        if (KBD_SCAN_STATE.index.lShift) 									/* Left SHIFT is pressed. */
            KBS_Buffer_puts(csr_sftl_mk); 	/* Buffer pre-string. */
        if (KBD_SCAN_STATE.index.rShift)									/* Right SHIFT is pressed. */
            KBS_Buffer_puts(csr_sftr_mk); 	/* Buffer pre-string. */
        KBS_Buffer_Input(0xE0);       			/* Buffer end of pre-string. */
    }
    else
    {   /* Key is being held pressed for a length of time, a "repeat event". */
        KBS_Buffer_Input(0xE0);       			/* Buffer E0h prefix */
    } 
    KBS_Buffer_Input(scan_code);      		/* Buffer base code. */
    if (event == BREAK_EVENT)   /* Key has just been released. This is a "break event". */
    {
            if (KBD_SCAN_STATE.index.lShift)  									/* Left shift has been pressed. */
            KBS_Buffer_puts(csr_sftl_brk2);   /* Buffer tail-string. */
        if (KBD_SCAN_STATE.index.rShift)									/* Right shift has been pressed. */
            KBS_Buffer_puts(csr_sftr_brk2);   /* Buffer tail-string. */
    }

}
/* ----------------------------------------------------------------------------
 * FUNCTION: Numlock_Corsor
 *
 * Input: scan_code, Set 2
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 * ------------------------------------------------------------------------- */
 void Numlock_Corsor(BYTE scan_code, BYTE event)
{
    static const BYTE  csr_numlock_mk[]   = {0xE0,0x12,0xE0,0x00};
    static const BYTE  csr_numlock_brk2[] = {0xE0,0xF0,0x12,0x00};

    if (event == BREAK_EVENT)KBS_Buffer_puts(csr_brk1);            // Buffer pre-string. 
    else if (event == MAKE_EVENT)   KBS_Buffer_puts(csr_numlock_mk);  	// Buffer pre-string. 
    else KBS_Buffer_Input(0xE0);       			// Buffer E0h prefix.

    KBS_Buffer_Input(scan_code);              // Buffer base code. 
    if (event == BREAK_EVENT)KBS_Buffer_puts(csr_numlock_brk2);    // Buffer tail-string. 
}
/* ----------------------------------------------------------------------------
 * FUNCTION: HotKey_Fn_Function
 * The function of hotkey Fn + Fx
 * Input: code
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 * ------------------------------------------------------------------------- */
 void HotKey_Fn_Function(BYTE code_byte, BYTE event)
{
	(HotKey_Fn_Fx[code_byte])(event);
}
/* ----------------------------------------------------------------------------
 * FUNCTION: Generate_Scan_Code
 *
 * Generate KBD_SCAN code set 2 and update scanner status.
 *
 * Input: state flags
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 *        KBD_SCAN_STATE
 *
 *  The following bits are defined for state flags and KBD_SCAN_STATE:
 *       NUM_LOCK
 *       LEFT
 *       RIGHT
 *       ALT
 *       CONTROL
 *       OVERLAY
 *       FN
 *       
 *
 * Returns: Updated KBD_SCAN_STATE
 * ------------------------------------------------------------------------- */
 void Generate_Scan_Code(BYTE state, BYTE event)
{

	/*发送这些按键的按键码*/
    if ((event == MAKE_EVENT)||(event == BREAK_EVENT))
    {
    	if (state & LShift)		    Single_Code(0x12, event);
 		else if (state & LAlt)      Single_Code(0x11, event);
		else if (state & LCtrl)     Single_Code(0x14, event);
    	else if (state & RShift)    Single_Code(0x59, event);
		else if (state & FN)        Add_E0_Code(0x63, event);
        	/*根据不同的事件，将相应的flag置位*/


        if (event == MAKE_EVENT)		 	
        {   
            if (state & FN) Scratch_Fn_Key_Make();
            KBD_SCAN_STATE.byte |= state;
        }
        else if (event == BREAK_EVENT)
        {   
            if (state & FN) Scratch_Fn_Key_Break();
            KBD_SCAN_STATE.byte &= ~state;
        }
    }

}
/* ----------------------------------------------------------------------------
 * FUNCTION: Generate_Scan_Code_E0
 *
 * Generate KBD_SCAN code set 2 and update scanner status.
 * Only for Alt-Right and Ctrl-Right.
 *
 * Input: state flags
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 *        KBD_SCAN_STATE
 *
 * The following bits are defined for state_flags and KBD_SCAN_STATE:
 *       NUM_LOCK
 *       LEFT
 *       RIGHT
 *       ALT
 *       CONTROL
 *       OVERLAY
 *       FN
 *
 * Return: Updated KBD_SCAN_STATE
 * ------------------------------------------------------------------------- */
 void Generate_Scan_Code_E0(BYTE state, BYTE event)
{   //   只有Alt-R and Ctrl-R.这两个按键才会走这个流程
    if ((event == MAKE_EVENT)||(event == BREAK_EVENT))
    {
    	// First setup to generate KBD_SCAN code set 2. 
    	if (state & RAlt) Add_E0_Code(0x11, event);
		if (state & RCtrl) Add_E0_Code(0x14, event);
        /*根据事件设置和清除相应的flag*/
        if (event == MAKE_EVENT)   	KBD_SCAN_STATE.byte |= state;
        else if (event == BREAK_EVENT)KBD_SCAN_STATE.byte &= ~state;
	}
}
/* ----------------------------------------------------------------------------
 * FUNCTION: Get_Comb_Index
 *
 * Input: combination setting
 *             Bit   Meaning
 *             ---   --------
 *             7     reserved
 *             6     reserved
 *             5     Fn
 *             4     Num Lock
 *             3     Ctrl
 *             2     Alt
 *             1     Shift
 *             0     reserved
 *
 *       KBD_SCAN_STATE
 *
 * Return: offset.
 * ------------------------------------------------------------------------- */

BYTE Get_Comb_Index(BYTE comb, BYTE table_entry, BYTE event)
{
    BYTE offset;
    BYTE bit_num;

    offset  = 0;
    bit_num = 0;

    if (comb & BIT(1))
    {   						// Combination has Shift. 
    	if (KBD_SCAN_STATE.index.lShift | KBD_SCAN_STATE.index.rShift)
		{   								// Either left or right shift is pressed. 
            offset |= (1<<bit_num);
        }
        bit_num++;  						// Increment bit position. 
    }

    if (comb & BIT(2))
    {   									// Combination has Alt. 
		if (KBD_SCAN_STATE.index.tALT)
        {   								// Alt is pressed. 
            offset |= (1<<bit_num);
        }
        bit_num++;  						// Increment bit position. 
    }

   	if (comb & BIT(3))
    {   									// Combination has Ctrl. 
		if (KBD_SCAN_STATE.index.tCtrl)
        {  	 								// Ctrl is pressed. 
            offset |= (1<<bit_num);
        }
        bit_num++;  						// Increment bit position. 
    }

    if (comb & BIT(4))
    {   									// Combination has Num Lock. 
        // if (KBD_SCAN_STATE.index.num_lock)
        // {   								// NumLock has been pressed. 
        //     offset |= (1<<bit_num);
        // }
        // bit_num++; 	 						// Increment bit position. 
		if (NumLockKey)
        {   								// Fn is pressed. 
           offset |= (1<<bit_num);
        }
        bit_num++; 
	}

    if (comb & BIT(5))
    {   								// Combination has Fn. 
        //if (KBD_SCAN_STATE.index.Fn)
        //{   								// Fn is pressed. 
        //    offset |= (1<<bit_num);
        //}

        if(Check_FnKey_Related_Flag(table_entry, event)==0x01)
        {
            offset |= (1<<bit_num);
        }
        bit_num++;  						// Increment bit position. 
    }

    if (comb & BIT(6))
    {   									// Combination has Hook_calc_index_comb_BIT6 
        if(Scratch_Get_Index_Comb_BIT6()==0xFF)
        {
            offset |= (1<<bit_num);
        }
        bit_num++;  						// Increment bit position. 
    }

    if (comb & BIT(7))
    {   									// Combination has Hook_calc_index_comb_BIT7 
        if(Scratch_Get_Index_Comb_BIT7()==0xFF)
        {
            offset |= (1<<bit_num);
        }
        bit_num++;  						// Increment bit position. 
    }
    
    return(offset);
}
