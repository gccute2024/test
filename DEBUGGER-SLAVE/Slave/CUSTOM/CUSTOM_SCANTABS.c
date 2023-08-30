/*
 * @Author: Iversu
 * @LastEditors: dejavudwh
 * @LastEditTime: 2023-05-26 20:30:11
 * @Description: RC Table(s) for Scan Matix
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

#include "CUSTOM_SCANTABS.H"
#include "CUSTOM_POWER.H"



//列行.（Column row）
const BYTE  Rc_ROM_Tables[][8] =   
{
	#ifdef KBD_SN1211
    /*    0    1    2    3    4    5    6    7*/
    /*0*/0x24,0x04,0x23,0x0C,0x21,0x00,0x06,0x26,
    /*1*/0x2D,0x2C,0x2B,0x34,0x2A,0x32,0x2E,0x25,
    /*2*/0x3C,0x35,0x3B,0x33,0x3A,0x31,0x36,0x3D,
    /*3*/0x43,0x5B,0x42,0x0B,0x41,0x00,0x55,0x3E,
    /*4*/0x44,0x80,0x4B,0x00,0x49,0x84,0x0A,0x46,
    /*5*/0xBB,0x00,0x81,0x98,0x9F,0x9A,0x94,0x95,
    /*6*/0xB2,0xB6,0xBA,0xBD,0xB3,0xB7,0x96,0x97,
    /*7*/0xB1,0xB5,0xB9,0xBC,0xBE,0x9B,0xC2,0x00,
    /*8*/0xB0,0xB4,0xB8,0x29,0x77,0x99,0xC0,0x00,
    /*9*/0x00,0x88,0x89,0xA2,0xA1,0x9C,0x9D,0xAD,
    /*10*/0x00,0x66,0x5D,0x78,0x5A,0x07,0x01,0x09,
    /*11*/0x4D,0x54,0x4C,0x52,0x00,0x4A,0x4E,0x45,
    /*12*/0x00,0x00,0x8E,0x8A,0x00,0x8B,0x00,0xC3,
    /*13*/0x91,0x85,0xC4,0x86,0x8D,0x87,0x8C,0x03,
    /*14*/0xA3,0x82,0xA7,0xA8,0xA6,0xA9,0xA0,0xA4,
    /*15*/0x00,0xAA,0x27,0xAC,0x9E,0xAB,0xA5,0x00,
    /*16*/0x1D,0x58,0x1B,0x00,0x22,0x00,0x05,0x1E,
    /*17*/0x15,0x0D,0x1C,0x76,0x1A,0x00,0x0E,0x16,
    /*18*/0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x26,
    #elif (defined(KBD_8_16))//超越 keyboard code
    /*    0    1    2    3    4    5    6    7*/
    /*0*/0xE7,0xE2,0xE6,0x00,0xE4,0x5D,0xE5,0xEB,
    /*1*/0xE3,0xE0,0x00,0xEC,0x52,0x5B,0x66,0xE8,
    /*2*/0x8A,0x8B,0x00,0x00,0x00,0x00,0x00,0x00,
    /*3*/0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    /*4*/0x8C,0x8D,0x00,0x00,0x00,0x00,0x00,0x00,
    /*5*/0x88,0x89,0x00,0x00,0x00,0x00,0x00,0x00,
    /*6*/0x00,0xAA,0xAB,0xA7,0x54,0x55,0xEA,0xE1,
    /*7*/0x29,0x41,0xA8,0x00,0xA6,0xA3,0x4E,0xE9,
    /*8*/0x31,0xA4,0xA5,0xA2,0x9E,0x9F,0xB8,0xB7,
    /*9*/0x32,0x33,0xA1,0xA0,0x9C,0x9D,0xB6,0xB5,
    /*10*/0x00,0x2A,0x34,0x35,0x2C,0x36,0xB4,0xB3,
    /*11*/0x00,0x2B,0x23,0x2D,0x24,0x2E,0xB2,0xB1,
    /*12*/0x21,0x00,0x1B,0x1D,0x25,0x26,0xB0,0xAF,
    /*13*/0x22,0x1A,0x1C,0x15,0x1E,0x16,0xAE,0xAD,
    /*14*/0x00,0x00,0x84,0x8E,0x58,0x0D,0x0E,0x76,
    /*15*/0x82,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    /*16*/0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    /*17*/0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    /*18*/0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	#endif

};
//行列式（Row Column）
const BYTE KB_S3WakeUP_Tables[] =
{
    // Scanner Table 1 
    /*    0    1    2    3    4    5    6    7*/
    /*0*/0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
    /*1*/0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
    /*2*/0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
    /*3*/0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
    /*4*/0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
    /*5*/0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
    /*6*/0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
    /*7*/0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
    /*8*/0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
    /*9*/0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
    /*10*/0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
    /*11*/0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
    /*12*/0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
    /*13*/0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
    /*14*/0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
    /*15*/0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
};

const BYTE   EtKey_S3WakeUP_Tables[] =
{	// 19X8 Scan Table 
	/*16*/0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
	/*17*/0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
	/*18*/0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
};

//----------------------------------------------------------------------------
// The function of hotkey Fn + F1
//----------------------------------------------------------------------------
void Do_HotKey_FnF1(BYTE  event)
{
#ifdef KBD_8_16//超越 keyboard code

//	  if(IS_MASK_SET(SYS_MISC1,ACPI_OS)) 				  //sleep
//	 {

		if ( event == MAKE_EVENT )
		{
		    	KBS_Buffer_Input(0xE0);	/* Buffer E0h prefix */
		    	KBS_Buffer_Input(0x3F);	/* Buffer base code */
//			ECQEvent(Sleep_Event, SCIMode_Normal);
		 }
		else
		{
			if ( event == BREAK_EVENT )
			{
	    			KBS_Buffer_Input(0xE0);	/* Buffer E0h prefix */
	    			KBS_Buffer_Input(0xF0);	/* Buffer base code */
	    			KBS_Buffer_Input(0x3F);	/* Buffer base code */
			 }
		}
//		if ( event == MAKE_EVENT )
//		{
//			Oem_TriggerS0S3();
//		}
#endif	

}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F2
//----------------------------------------------------------------------------
void Do_HotKey_FnF2(BYTE  event)
{

}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F3
//----------------------------------------------------------------------------
void Do_HotKey_FnF3(BYTE  event)
{
    #ifdef KBD_8_16//超越 keyboard code

//	if(IS_MASK_SET(SYS_MISC1,ACPI_OS)) 	//mute
//	{
	  if ( event == MAKE_EVENT )
		{
		    	KBS_Buffer_Input(0xE0);	/* Buffer E0h prefix */
		    	KBS_Buffer_Input(0x23);	/* Buffer base code */
		}
		else
		{
			if ( event == BREAK_EVENT )
			{
	    			KBS_Buffer_Input(0xE0);	/* Buffer E0h prefix */
	    			KBS_Buffer_Input(0xF0);	/* Buffer base code */
	    			KBS_Buffer_Input(0x23);	/* Buffer base code */
			 }
		}
#endif
}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F4
//----------------------------------------------------------------------------
void Do_HotKey_FnF4(BYTE  event)
{
//		if ( event == MAKE_EVENT )
//		{
//		    	KBS_Buffer_Input(0xE0);	/* Buffer E0h prefix */
//		    	KBS_Buffer_Input(0x08);	/* Buffer base code */
//		 }
//		else
//		{
//			if ( event == BREAK_EVENT )
//			{
//	    			KBS_Buffer_Input(0xE0);	/* Buffer E0h prefix */
//	    			KBS_Buffer_Input(0xF0);	/* Buffer base code */
//	    			KBS_Buffer_Input(0x08);	/* Buffer base code */
//			 }
//		}
//			if ( event == MAKE_EVENT )
//		{
//		    	KBS_Buffer_Input(0xE0);	/* Buffer E0h prefix */
//		    	KBS_Buffer_Input(0x1F);	/* Buffer base code */ //WIN
//			
//					KBS_Buffer_Input(0x04);	/* Buffer base code */  //F3

//		 }
//		else
//		{
//					
//			
//					KBS_Buffer_Input(0xF0);	/* Buffer base code */
//	    		KBS_Buffer_Input(0x04);	/* Buffer base code */
//			
//					KBS_Buffer_Input(0xE0);	/* Buffer E0h prefix */
//					KBS_Buffer_Input(0xF0);	/* Buffer base code */
//					KBS_Buffer_Input(0x1F);	/* Buffer base code */
//		}

}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F5
//----------------------------------------------------------------------------
void Do_HotKey_FnF5(BYTE  event)
{
    #ifdef KBD_8_16//超越 keyboard code

//	 if(IS_MASK_SET(SYS_MISC1,ACPI_OS)) 	//Volume +
//	 {
		if ( event == MAKE_EVENT )
		{
		    	KBS_Buffer_Input(0xE0);	/* Buffer E0h prefix */
		    	KBS_Buffer_Input(0x32);	/* Buffer base code */
		}
		else
		{
			if ( event == BREAK_EVENT )
			{
	    			KBS_Buffer_Input(0xE0);	/* Buffer E0h prefix */
	    			KBS_Buffer_Input(0xF0);	/* Buffer base code */
	    			KBS_Buffer_Input(0x32);	/* Buffer base code */
			 }
		}
#endif
}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F6
//----------------------------------------------------------------------------
void Do_HotKey_FnF6(BYTE  event)
{	
    #ifdef KBD_8_16//超越 keyboard code

//	if(IS_MASK_SET(SYS_MISC1,ACPI_OS)) 	//Vloume -
//	{
    if ( event == MAKE_EVENT )
		{
		    	KBS_Buffer_Input(0xE0);	/* Buffer E0h prefix */
		    	KBS_Buffer_Input(0x21);	/* Buffer base code */
		}
		else
		{
      if ( event == BREAK_EVENT )
			{
	    			KBS_Buffer_Input(0xE0);	/* Buffer E0h prefix */
	    			KBS_Buffer_Input(0xF0);	/* Buffer base code */
	    			KBS_Buffer_Input(0x21);	/* Buffer base code */
			}
		}
#endif
}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F7
//----------------------------------------------------------------------------
void Do_HotKey_FnF7(BYTE  event)
{
#ifdef KBD_8_16//超越 keyboard code
    // Increase brightness
//		if(BrgLevel < MAX_LEVEL-1)
//		{	
//			if(event == MAKE_EVENT)
//			{

//					BrgLevel++;
//					KBS_Buffer_Input(0xE0);	/* Buffer E0h prefix */
//					KBS_Buffer_Input(0x02);	/* Buffer base code */
//		//				ECQEvent(Bright_Inc_Event,SCIMode_Normal);

//			}

//		}
//		if ( event == BREAK_EVENT )
//		{
//					KBS_Buffer_Input(0xE0);	/* Buffer E0h prefix */
//					KBS_Buffer_Input(0xF0);	/* Buffer E0h prefix */
//					KBS_Buffer_Input(0x02);	/* Buffer base code */
//		}
		
		if ( event == MAKE_EVENT )
		{
					KBS_Buffer_Input(0xE0);	/* Buffer E0h prefix */
					KBS_Buffer_Input(0x02);	/* Buffer base code */
		}
		else
		{
      if ( event == BREAK_EVENT )
			{
					KBS_Buffer_Input(0xE0);	/* Buffer E0h prefix */
					KBS_Buffer_Input(0xF0);	/* Buffer E0h prefix */
					KBS_Buffer_Input(0x02);	/* Buffer base code */
			}
		}

#endif

}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F8
//----------------------------------------------------------------------------
void Do_HotKey_FnF8(BYTE  event)
{
#ifdef KBD_8_16//超越 keyboard code
   // Decrease brightness	
	
//	if(BrgLevel > 0)
//	{
//		if(event == MAKE_EVENT)
//		{
//				BrgLevel--;
//				KBS_Buffer_Input(0xE0);	/* Buffer E0h prefix */
//				KBS_Buffer_Input(0x01);	/* Buffer base code */
//	//					ECQEvent(Bright_Dec_Event,SCIMode_Normal);
//		}

//	}
//	if ( event == BREAK_EVENT )
//	{
//				KBS_Buffer_Input(0xE0);	/* Buffer E0h prefix */
//				KBS_Buffer_Input(0xF0);	/* Buffer E0h prefix */
//				KBS_Buffer_Input(0x01);	/* Buffer base code */
//	}
	
		if ( event == MAKE_EVENT )
		{
					KBS_Buffer_Input(0xE0);	/* Buffer E0h prefix */
					KBS_Buffer_Input(0x01);	/* Buffer base code */
		}
		else
		{
      			if ( event == BREAK_EVENT )
			{
					KBS_Buffer_Input(0xE0);	/* Buffer E0h prefix */
					KBS_Buffer_Input(0xF0);	/* Buffer E0h prefix */
					KBS_Buffer_Input(0x01);	/* Buffer base code */
			}
		}

#endif

}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F9
//----------------------------------------------------------------------------
void Do_HotKey_FnF9(BYTE  event)
{
#ifdef KBD_8_16//超越 keyboard code
#endif

}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F10
//----------------------------------------------------------------------------
void Do_HotKey_FnF10(BYTE  event)
{
#ifdef KBD_8_16//超越 keyboard code
		//if(SystemIsS0 )
		{
			if ( event == MAKE_EVENT )
			{
				if(IS_MASK_SET(GPIO2_DR0,BIT(4)))
				{	
					BACKLIGHT_L();
				}
				else
				{
					BACKLIGHT_H();
				}	
				//Buffer_Key(0xE0);	/* Buffer E0h prefix */
				//Buffer_Key(0x09);	/* Buffer base code */
			}
			else
			{
				if ( event == BREAK_EVENT )
				{
						//Buffer_Key(0xE0);	/* Buffer E0h prefix */
						//Buffer_Key(0xF0);	/* Buffer E0h prefix */
						//Buffer_Key(0x09);	/* Buffer base code */
				}
			}
		}


#endif

}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F11
//----------------------------------------------------------------------------
void Do_HotKey_FnF11(BYTE  event)
{
#ifdef KBD_8_16//超越 keyboard code
#endif

}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F12
//----------------------------------------------------------------------------
void Do_HotKey_FnF12(BYTE  event)
{
#ifdef KBD_8_16//超越 keyboard code
#endif
 	
}

//----------------------------------------------------------------------------
// The function of hotkey Fn + ESC
//----------------------------------------------------------------------------
void Do_HotKey_FnESC(BYTE  event)
{
#ifdef KBD_8_16//超越 keyboard code
#endif

}

//----------------------------------------------------------------------------
// The function of hotkey Fn + up arrow
//----------------------------------------------------------------------------
void Do_HotKey_FnUP(BYTE  event)
{

}

//----------------------------------------------------------------------------
// The function of hotkey Fn + down arrow
//----------------------------------------------------------------------------
void Do_HotKey_FnDOWN(BYTE  event)
{

}

//----------------------------------------------------------------------------
// The function of hotkey Fn + left arrow
//----------------------------------------------------------------------------
void Do_HotKey_FnLEFT(BYTE  event)
{

}

//----------------------------------------------------------------------------
// The function of hotkey Fn + right arrow
//----------------------------------------------------------------------------
void Do_HotKey_FnRIGHT(BYTE  event)
{

}

//----------------------------------------------------------------------------
// The function of hotkey Fn + Ins (Scr Lk)
//---------------------------------------------------------------------------
void Do_HotKey_FnIns(BYTE  event)
{

}

//----------------------------------------------------------------------------
// The function of hotkey Fn + Del (NumLk)
//---------------------------------------------------------------------------
void Do_HotKey_FnDel(BYTE  event)
{

}

const FUNCT_PTR_V_B  HotKey_Fn_Fx[] =
{
	Do_HotKey_FnF1,    		// Fn + F1
	Do_HotKey_FnF2, 		// Fn + F2
	Do_HotKey_FnF3, 		// Fn + F3
	Do_HotKey_FnF4,       	// Fn + F4
	Do_HotKey_FnF5,    		// Fn + F5
	Do_HotKey_FnF6, 		// Fn + F6
	Do_HotKey_FnF7, 		// Fn + F7
	Do_HotKey_FnF8,       	// Fn + F8
	Do_HotKey_FnF9,    		// Fn + F9
	Do_HotKey_FnF10, 		// Fn + F10
	Do_HotKey_FnF11, 		// Fn + F11
	Do_HotKey_FnF12,    	// Fn + F12	
	Do_HotKey_FnESC,    	// Fn + ESC	
	Do_HotKey_FnUP,    		// Fn + up arrow	
	Do_HotKey_FnDOWN,    	// Fn + down arrow	
	Do_HotKey_FnLEFT,    	// Fn + left arrow	
	Do_HotKey_FnRIGHT,  	// Fn + right arrow	
	//Do_HotKey_FnIns,		// Fn + Ins (Scr Lk)
	//Do_HotKey_FnDel,		// Fn + Del (NumLk)
};


//---------------------------------------------------------------------------
//	Numpad 7 Home	[B0h]
//	Numpad 8 UP  	[B1h]
//	Numpad 9 PGUp	[B2h]
//	Numpad *   		[B3h]
//	Numpad 4 Left	[B4h]
//	Numpad 5 		[B5h]
//	Numpad 6 Right	[B6h]
//	Numpad -  		[B7h]
//	Numpad 1 End	[B8h]
// 	Numpad 2 Down	[B9h]
//	Numpad 3 PGDn	[BAh]
//	Numpad + 		[BBh]
//	Numpad 0 Ins	[BCh]
// 	Numpad . Del	[BDh]
//	Numpad /      	[BEh]
//---------------------------------------------------------------------------

const BYTE  ss2_9C[] =	// Numpad 7 
{
    0x3D,   // Normal 		// 7
    0xB0,   // Numlock 
    0xB0,   // Fn 
    0xB0    // Fn+Numlock 
};

const BYTE  ss2_9D[] =	// Numpad 8   
{
    0x3E,   // Normal 		// 8
    0xB1,   // Numlock 
    0xB1,   // Fn 
    0xB1    // Fn+Numlock 
};

const BYTE  ss2_9E[] = 	// Numpad 9  
{
    0x46,   // Normal 		// 9
    0xB2,   // Numlock 
    0xB2,   // Fn 
    0xB2    // Fn+Numlock 
};

const BYTE  ss2_9F[] = 	// Numpad 0  
{
    0x45,   // Normal 		// 0
    0xB3,   // Numlock 
    0xB3,   // Fn 
    0xB3    // Fn+Numlock 
};

const BYTE  ss2_A0[] = 	// Numpad U  
{
    0x3C,   // Normal 		// U
    0xB4,   // Numlock 
    0xB4,   // Fn 
    0xB4    // Fn+Numlock 
};

const BYTE  ss2_A1[] = 	// Numpad I  
{
    0x43,   // Normal 		// I
    0xB5,   // Numlock 
    0xB5,   // Fn 
    0xB5    // Fn+Numlock 
};

const BYTE  ss2_A2[] =	// Numpad O 
{
    0x44,   // Normal 		// O
    0xB6,   // Numlock 
    0xB6,   // Fn 
    0xB6    // Fn+Numlock 
};

const BYTE  ss2_A3[] =	// Numpad P 
{
    0x4D,   // Normal 		// P
    0xB7,   // Numlock 
    0xB7,   // Fn 
    0xB7    // Fn+Numlock 
};

const BYTE  ss2_A4[] = 	// Numpad J 
{
    0x3B,   // Normal 		// J
    0xB8,   // Numlock 
    0xB8,   // Fn 
    0xB8    // Fn+Numlock 
};

const BYTE  ss2_A5[] = 	// Numpad K 
{
    0x42,   // Normal 		// K
    0xB9,   // Numlock 
    0xB9,   // Fn 
    0xB9    // Fn+Numlock 
};

const BYTE  ss2_A6[] =	// Numpad L 
{
    0x4B,   // Normal 		// L
    0xBA,   // Numlock 
    0xBA,   // Fn 
    0xBA    // Fn+Numlock 
};

const BYTE  ss2_A7[] = 	// Numpad ;:  
{
    0x4C,   // Normal 		// ;:
    0xBB,   // Numlock 
    0xBB,   // Fn 
    0xBB    // Fn+Numlock 
};

const BYTE  ss2_A8[] =	// Numpad M  
{
    0x3A,   // Normal 		// M
    0xBC,   // Numlock 
    0xBC,   // Fn 
    0xBC    // Fn+Numlock 
};

const BYTE  ss2_A9[] = 	// Numpad ,<  
{
    0x41,   // Normal 		// ,< 
    0x41,   // Numlock 
    0x41,   // Fn 
    0x41    // Fn+Numlock 
};

const BYTE  ss2_AA[] = 	// Numpad .>   
{
    0x49,   // Normal 		// .> 
    0xBD,   // Numlock 
    0xBD,   // Fn 
    0xBD    // Fn+Numlock 
};

const BYTE  ss2_AB[] = 	// Numpad /?      
{
    0x4A,   // Normal 		// /? 
    0xBE,   // Numlock 
    0xBE,   // Fn 
    0xBE    // Fn+Numlock 
};

const BYTE  ss2_AC[] =   
{
    0x5A,   // Normal 
    0x81,   // Fn 
};
//----------------------------------------------------------------
// Function key F1 ~ F12
//----------------------------------------------------------------
const BYTE  ss2_AD[] = 	// F1
{
    0x05,   // Normal 
    0xD0,   // Fn 
};

const BYTE  ss2_AE[] = 	// F2 	 
{
    0x06,   // Normal 
    0xD1,   // Fn 
};

const BYTE  ss2_AF[] = 	// F3  
{
    0x04,   // Normal 
    0xD2,   // Fn 
};

const BYTE  ss2_B0[] =	// F4   
{
    0x0C,   // Normal 
    0xD3,   // Fn 
};

const BYTE  ss2_B1[] = 	// F5  
{
    0x03,   // Normal 
    0xD4,   // Fn 
};

const BYTE  ss2_B2[] = 	// F6  
{
    0x0B,   // Normal 
    0xD5,   // Fn 
};

const BYTE  ss2_B3[] = 	// F7  
{
    0x80,   // Normal 
    0xD6,   // Fn 
};

const BYTE  ss2_B4[] = 	// F8  
{
    0x0A,   // Normal 
    0xD7,   // Fn 
};

const BYTE  ss2_B5[] = 	// F9  
{
    0x01,   // Normal 
    0xD8,   // Fn 
    //0x01,   // Normal 
};

const BYTE  ss2_B6[] = 	// F10  
{
    0x09,   // Normal 
	//0x09,   // Fn 
    0xD9,   // Fn 
    //0x09,   // Normal 
};

const BYTE  ss2_B7[] = 	// F11  
{
    0x78,   // Normal 
    0xDA,   // Fn 
    //0x78,   // Normal 
};

const BYTE  ss2_B8[] = 	// F12  
{
    0x07,   // Normal 
    0xDB,   // Fn 
    //0x07,
};

/* ----------------------------------------------------------------------------
 * FUNCTION: sskey2_A2_table
 *
 * The A2_TABLE structure contains the following.
 *
 * BYTE  comb;  Bit   Setting Combination Key
 *            ---   -----------------------
 *             7    Scratch_Get_Index_Comb_BIT7
 *             6    Scratch_Get_Index_Comb_BIT6
 *             5    Fn
 *             4    Numlock
 *             3    Ctrl
 *             2    Alt
 *             1    Shift
 *             0    reserved
 *
 * BYTEP pntr; Pointer to array for key.
 * ------------------------------------------------------------------------- */
const A2_TABLE sskey2_A2_table[] =
{											// Keyboard matrix index
	// Keypad -->
	{0x30, ss2_9C },    /* index = 00h */ 	// 0x9C	
    {0x30, ss2_9D },    /* index = 01h */	// 0x9D
    {0x30, ss2_9E },    /* index = 02h */	// 0x9E	
    {0x30, ss2_9F },    /* index = 03h */	// 0x9F	
    {0x30, ss2_A0 },    /* index = 04h */	// 0xA0
    {0x30, ss2_A1 },    /* index = 05h */	// 0xA1
    {0x30, ss2_A2 },    /* index = 06h */	// 0xA2
    {0x30, ss2_A3 },    /* index = 07h */	// 0xA3
    {0x30, ss2_A4 },    /* index = 08h */	// 0xA4
    {0x30, ss2_A5 },    /* index = 09h */	// 0xA5
    {0x30, ss2_A6 },    /* index = 0Ah */	// 0xA6
    {0x30, ss2_A7 },    /* index = 0Bh */	// 0xA7
    {0x30, ss2_A8 },    /* index = 0Ch */	// 0xA8
    {0x30, ss2_A9 },  	/* index = 0Dh */	// 0xA9
    {0x30, ss2_AA },  	/* index = 0Eh */	// 0xAA
    {0x30, ss2_AB },   	/* index = 0Fh */	// 0xAB
    {0x20, ss2_AC },  	/* index = 10h */	// 0xAC
	// Keypad <--
	
	// F1~F12 hotkeys -->
    {0x20, ss2_AD },    /* index = 11h */	// 0xAD
    {0x20, ss2_AE },    /* index = 12h */	// 0xAE
    {0x20, ss2_AF },    /* index = 13h */	// 0xAF
    {0x20, ss2_B0 },    /* index = 14h */	// 0xB0
    {0x20, ss2_B1 },    /* index = 15h */	// 0xB1
    {0x20, ss2_B2 },    /* index = 16h */	// 0xB2
    {0x20, ss2_B3 },    /* index = 17h */	// 0xB3
    {0x20, ss2_B4 },    /* index = 18h */	// 0xB4
    {0x20, ss2_B5 },  	/* index = 19h */	// 0xB5
    {0x20, ss2_B6 },  	/* index = 1Ah */	// 0xB6
    {0x20, ss2_B7 },   	/* index = 1Bh */	// 0xB7
    {0x20, ss2_B8 }     /* index = 1Ch */	// 0xB8	
    // F1~F12 hotkeys <--
};
											// Reserved 0xB9 ~ 0xDF
 
/* ----------------------------------------------------------------------------
 * FUNCTION: sskey2_overlay_table
 * ------------------------------------------------------------------------- */
const BYTE  sskey2_overlay_table[] =
{               //	Normal   	Fn   			// Keyboard matrix index
/*	0x9A,	0x9A,	//0xE0	←	←
	0x77,	0x7E,	//0xE1	Num LK	Scr LK
	0x98,	0x98,	//0xE2	↑	↑
	0x99,	0x99,	//0xE3	↓	↓ 
	0x97,	0x97,	//0xE4	PgDn	PgDn
	0x96,	0x96,	//0xE5	PgUp	PgUp
	0x95,	0x95,	//0xE6	End	End
	0x9B,	0x9B,	//0xE7	→ 	→ 
	0xC0,	0xC1,	//0xE8	DEL	Sys Rq
	0x91,	0x92,	//0xE9	Pause	Break
	0xC2,	0xC3,	//0xEA	Inset	Prt Scr
	0x94,	0x94,	//0xEB	Home	Home
	0x5A,	0x81,	//0xEC	Enter	 Num-Enter
*/
	0x9A,	0x9D,	//0xE0	←	Pre
	0xC2,	0x77,	//0xE1	Insert	Num Lk
	0x98,	0x9E,	//0xE2	↑	Stop
	0x99,	0x9F,	//0xE3	↓	Play/Pause
	0x97,	0x97,	//0xE4	PgDn	PgDn
	0x96,	0x96,	//0xE5	PgUp	PgUp
	0x95,	0x95,	//0xE6	End	End
	0x9B,	0x9C,	//0xE7	→ 	Next
#ifdef KBD_8_16//超越 keyboard code
	0xC0,	0xC6,	//0xE8	DEL	Reserved   // see the sskey3_80_table index
	//0xC0,	0xC0,	//0xE8	DEL	Scr Lk   // 0xE8	DEL	DEL   
    0xC3,	0xC1,	//0xE9	Prt Scr	Sys Rq	//change the sequence of ptrscreen and paus/break according to the right 
	0x91,	0x92,	//0xEA	Pause	Break
#else
	0xC0,	0x7E,	//0xE8	DEL	Scr Lk
	0x91,	0x92,	//0xE9	Pause	Break
	0xC3,	0xC1,	//0xEA	Prt Scr	Sys Rq
#endif
	0x94,	0x94,	//0xEB	Home	Home
	0x5A,	0x81,	//0xEC	Enter	 Num-Enter

};

// Reserved 0xEE ~ 0xFF								

/* ----------------------------------------------------------------------------
 * Insyde Software Key Number 2
 *
 * 00h         Null
 * 01h - 7Fh   Same as IBM Scan Code, Set2
 * 80h - BFh   Pre-Index for generation Scan Code, Set2
 * C0h - DFh   Pre-Index for PPK function
 * C0h - DFh   Pre-Index for SMI function
 * ------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------
 * FUNCTION: sskey3_80_table - Insyde Software Key Number 2
 * 80h - BFh   Pre-Index for generation Scan Code, Set2
 * ------------------------------------------------------------------------- */
const BYTE   sskey3_80_table[] =
{
	//-----------------------------------------------------------------
	// Index from keyboard matrix
	//-----------------------------------------------------------------	
									// Keyboard matrix index
	0x83, 	DO_SIMPLE_CODE,       	// [80h] F7 (118) 
	0x5A, 	DO_E0_PREFIX_CODE,    	// [81h] Numpad Enter (108) 
    0x1F, 	DO_E0_PREFIX_CODE,    	// [82h] Windows Left 
    0x27, 	DO_E0_PREFIX_CODE,    	// [83h] Windows Right 
    0x2F, 	DO_E0_PREFIX_CODE,    	// [84h] Application 
    0x37, 	DO_E0_PREFIX_CODE,    	// [85h] Power event 
    0x3F, 	DO_E0_PREFIX_CODE,    	// [86h] Sleep event 
    0x5E, 	DO_E0_PREFIX_CODE,		// [87h] Wake event 
    LShift, DO_CONTROL_EFFECT,		// [88h] 12 Shift(L) 
    RShift, DO_CONTROL_EFFECT,		// [89h] 59 Shift(R) 
    LAlt, 	DO_CONTROL_EFFECT,    	// [8Ah] 11 Alt(L) 
    RAlt, 	DO_CONTROL_EFFECT_E0,	// [8Bh] E0 11 Alt(R) 
    LCtrl, 	DO_CONTROL_EFFECT, 		// [8Ch] 14 Crtl(L) 
    RCtrl, 	DO_CONTROL_EFFECT_E0,	// [8Dh] E0 14 Ctrl(R) 
    FN, 	DO_CONTROL_EFFECT,    	// [8Eh] Fn 
    OVLAY,	DO_CONTROL_EFFECT,		// [8Fh] Overlay 
    0x00, 	DO_SIMPLE_CODE,       	// [90h] Reserved 
    0x00, 	DO_SPECIAL_CODE,      	// [91h] Pause (126) 
    0x01, 	DO_SPECIAL_CODE,      	// [92h] Break 
    0x84, 	DO_SIMPLE_CODE,       	// [93h] (124) Alt-Case (SysRq) 
	0x6C, 	DO_E0_PREFIX_CODE,    	// [94h] Home 
    0x69, 	DO_E0_PREFIX_CODE,    	// [95h] End 
    0x7D, 	DO_E0_PREFIX_CODE,  	// [96h] Page up 
    0x7A, 	DO_E0_PREFIX_CODE,    	// [97h] Page down 
	0x75, 	DO_E0_PREFIX_CODE,    	// [98h] UP arrow 
    0x72, 	DO_E0_PREFIX_CODE,    	// [99h] Down arrow 
    0x6B, 	DO_E0_PREFIX_CODE,  	// [9Ah] Left arrow 
    0x74, 	DO_E0_PREFIX_CODE,   	// [9Bh] Right arrow 
    
	//-----------------------------------------------------------------
	// For MicroSoft enhance keyboard feature.
	//-----------------------------------------------------------------
 	0x4D, DO_E0_PREFIX_CODE,    	// [9Ch] Next Track event 
    0x15, DO_E0_PREFIX_CODE,    	// [9Dh] Prev Track event 
    0x3B, DO_E0_PREFIX_CODE,    	// [9Eh] Stop event 
    0x34, DO_E0_PREFIX_CODE,    	// [9Fh] Play/Pause event 
    0x23, DO_E0_PREFIX_CODE,    	// [A0h] Mute event 
    0x32, DO_E0_PREFIX_CODE,    	// [A1h] Volume Up event 
    0x21, DO_E0_PREFIX_CODE,    	// [A2h] Volume Down event 
    0x48, DO_E0_PREFIX_CODE,    	// [A3h] Mail event 
    0x10, DO_E0_PREFIX_CODE,    	// [A4h] Search event 
    0x3A, DO_E0_PREFIX_CODE,    	// [A5h] Web/Home event 
    0x38, DO_E0_PREFIX_CODE,    	// [A6h] Back event 
    0x30, DO_E0_PREFIX_CODE,    	// [A7h] Forward event 
    0x28, DO_E0_PREFIX_CODE,    	// [A8h] Stop event 
    0x20, DO_E0_PREFIX_CODE,    	// [A9h] Refresh event 
    0x18, DO_E0_PREFIX_CODE,    	// [AAh] Favorites event 
    0x2B, DO_E0_PREFIX_CODE,    	// [ABh] Caluator event 
    0x40, DO_E0_PREFIX_CODE,    	// [ACh] My Computer event 
    0x50, DO_E0_PREFIX_CODE,    	// [ADh] Media event 
    0x00, DO_SIMPLE_CODE,       	// [AEh] Reserved 
    0x00, DO_SIMPLE_CODE,      		// [AFh] Reserved 
    
	//-----------------------------------------------------------------
	// Index from sskey2_A2_table
	//-----------------------------------------------------------------
	0x6C, DO_SIMPLE_CODE,     		// Numpad 7 Home	[B0h]
  	0x75, DO_SIMPLE_CODE,      		// Numpad 8 UP  	[B1h]
 	0x7D, DO_SIMPLE_CODE,       	// Numpad 9 PGUp	[B2h]
  	0x7C, DO_SIMPLE_CODE,       	// Numpad *   		[B3h]
	0x6B, DO_SIMPLE_CODE,       	// Numpad 4 Left	[B4h]
 	0x73, DO_SIMPLE_CODE,       	// Numpad 5 		[B5h]
	0x74, DO_SIMPLE_CODE,       	// Numpad 6 Right	[B6h]
	0x7B, DO_SIMPLE_CODE,       	// Numpad -  		[B7h]
	0x69, DO_SIMPLE_CODE,       	// Numpad 1 End		[B8h]
 	0x72, DO_SIMPLE_CODE,       	// Numpad 2 Down	[B9h]
	0x7A, DO_SIMPLE_CODE,       	// Numpad 3 PGDn	[BAh]
	0x79, DO_SIMPLE_CODE,       	// Numpad + 		[BBh]
	0x70, DO_SIMPLE_CODE,       	// Numpad 0 Ins		[BCh]
	0x71, DO_SIMPLE_CODE,      	 	// Numpad . Del		[BDh]
	0x4A, DO_E0_PREFIX_CODE, 		// Numpad /      	[BEh]
	0x00, DO_SIMPLE_CODE,      		// Reserved 		[BFh]
    0x71, DO_E0_PREFIX_CODE,  		// Delete 			[C0h]  
    0x84, DO_SIMPLE_CODE,    		// SysRq			[C1h]  
    0x70, DO_E0_PREFIX_CODE,    	// Insert			[C2h]  
    0x7C, DO_E0_PREFIX_CODE,  		// Print Screen		[C3h] 
	0x41, DO_E0_PREFIX_CODE,     	// Euro Sign 		[C4h]
	0x49, DO_E0_PREFIX_CODE,    	// Dollar Sign 		[C5h]
	0x00, DO_SIMPLE_CODE,      		// Reserved 		[C6h]
	0x00, DO_SIMPLE_CODE,      		// Reserved 		[C7h]
	0x00, DO_SIMPLE_CODE,      		// Reserved 		[C8h]
	0x00, DO_SIMPLE_CODE,      		// Reserved 		[C9h]
	0x00, DO_SIMPLE_CODE,      		// Reserved 		[CAh]
	0x00, DO_SIMPLE_CODE,      		// Reserved 		[CBh]
	0x00, DO_SIMPLE_CODE,      		// Reserved 		[CCh]
	0x00, DO_SIMPLE_CODE,      		// Reserved 		[CDh]
	0x00, DO_SIMPLE_CODE,      		// Reserved 		[CEh]
	0x00, DO_SIMPLE_CODE,      		// Reserved 		[CFh]
	
	// Function key F1 ~ F12     
    0	, DO_COSTOMER_FUNCTION,		// Function key F1	[D0h]  
    1	, DO_COSTOMER_FUNCTION,		// Function key F2	[D1h]  
    2	, DO_COSTOMER_FUNCTION,		// Function key F3	[D2h] 
    3	, DO_COSTOMER_FUNCTION,		// Function key F4	[D3h] 
    4	, DO_COSTOMER_FUNCTION,		// Function key F5	[D4h] 
    5	, DO_COSTOMER_FUNCTION,		// Function key F6	[D5h] 
    6	, DO_COSTOMER_FUNCTION,		// Function key F7	[D6h] 
    7	, DO_COSTOMER_FUNCTION,		// Function key F8	[D7h] 
    8	, DO_COSTOMER_FUNCTION,		// Function key F9	[D8h] 
    9	, DO_COSTOMER_FUNCTION,		// Function key F10	[D9h] 
    10	, DO_COSTOMER_FUNCTION,		// Function key F11	[DAh]  
    11	, DO_COSTOMER_FUNCTION,		// Function key F12	[DBh] 
	12	, DO_COSTOMER_FUNCTION,   	// Function key Esc	[DCh]
	13	, DO_COSTOMER_FUNCTION,   	// Function key Up arrow	[DDh]
	14	, DO_COSTOMER_FUNCTION,   	// Function key Down arrow	[DEh]
	15	, DO_COSTOMER_FUNCTION,  	// Function key Left arrow	[DFh]
	16	, DO_COSTOMER_FUNCTION,  	// Function key Right arrow	[E0h]	
};

// Reserved 0xE0 ~ 0xFF		

//-----------------------------------------------------------------
// The function of checking boot hotkey
//-----------------------------------------------------------------
#define Crisiskey01        0x8E   	// Fn
#define Crisiskey02        0xEC  	// ESC

void Check_HotKey_01(BYTE  event)
{
    if(event == BREAK_EVENT)
    {
        CRISIS_KEY_STATUS=0x00;
    }
    else if(event == MAKE_EVENT)
    {
        CRISIS_KEY_STATUS++;
    }
}

void Check_HotKey_02(BYTE  event)
{
    if(event == BREAK_EVENT)
    {
        CRISIS_KEY_STATUS=0x00;
    }
    else if(event == MAKE_EVENT)
    {
        CRISIS_KEY_STATUS++;
    }
}

#if 1
const sBootHotKeyStruct  asBootHotKeyStruct[]=
{
	{ Crisiskey01, Check_HotKey_01	},
	{ Crisiskey02, Check_HotKey_02	},
};
#endif

void Check_HotKey_Boot(BYTE  matrix,BYTE  event)
{
	BYTE  index;
    for(index=0x00; index<(sizeof(asBootHotKeyStruct)/sizeof(sBootHotKeyStruct)); index++)
    {
		if(matrix==asBootHotKeyStruct[index].matrix)
	    {
	        (asBootHotKeyStruct[index].pfunction)(event);
            return;
        }
    }
    CRISIS_KEY_STATUS=0x00;
}

//-----------------------------------------------------------------------------
// Keyboard wake up system from S3
//-----------------------------------------------------------------------------
void Check_KB_Wake_S3(BYTE  BKSI,BYTE  BKSO)
{
	BYTE  wuv;

    wuv = ((BKSI << 4)|BKSO);
    Tmp_code_pointer = (VBYTE *)KB_S3WakeUP_Tables;  //zb 10.27 add (VBYTE *)

    if(*(Tmp_code_pointer + wuv)==0x55)
    {
	PowerSequence_Step = 1;
	PowerSequence_Delay = 0x00;
	System_PowerState=SYSTEM_S3_S0;
    }
}

void Ex_Check_KB_Wake_S3(BYTE  BKSI,BYTE  BKSO)
{
	BYTE  wuv;

    wuv = ((BKSI*3)+BKSO);
    Tmp_code_pointer = (VBYTE *)EtKey_S3WakeUP_Tables;  //zb 10.27 add (VBYTE *)

    if(*(Tmp_code_pointer + wuv)==0x55)
    {
	PowerSequence_Step = 1;
	PowerSequence_Delay = 0x00;
	System_PowerState=SYSTEM_S3_S0;
    }
}

//----------------------------------------------------------------------------
// table_entry : value of keyboard matrix table. for example Rc_ROM_Tables[]
// event : key MAKE_EVENT or BREAK_EVENT or REPEAT_EVENT
//----------------------------------------------------------------------------
void Check_Send_Key(BYTE  table_entry, BYTE  event)
{
	#if 0
		if(SystemNotS0)
		{  	
			Check_HotKey_Boot(table_entry,event);
		}
	#endif
}

//----------------------------------------------------------------------------
// Return : 0xFF --> Transmit_Key function will be break; (no any KBD_SCAN code to host)
//----------------------------------------------------------------------------
BYTE  Skip_Send_Key(void)
{
    return(0x00);
}

//-----------------------------------------------------------------------------
// The function of setting GPIO KBD_SCAN pin to high
//-----------------------------------------------------------------------------
void Set_GPIO_ScanPin_To_H(void)
{

}   

//-----------------------------------------------------------------------------
// The function of setting GPIO KBD_SCAN pin to low
//-----------------------------------------------------------------------------
void Set_GPIO_ScanPin_To_L(void)
{

}

//-----------------------------------------------------------------------------
// The function of setting GPIO KBD_SCAN pin output
//-----------------------------------------------------------------------------
void Set_GPIO_ScanPin_Output(void)
{
}

//-----------------------------------------------------------------------------
// Fn key make
//-----------------------------------------------------------------------------
void Scratch_Fn_Key_Make(void)
{

}

//-----------------------------------------------------------------------------
// Fn key break
//-----------------------------------------------------------------------------
void Scratch_Fn_Key_Break(void)
{

}

//-----------------------------------------------------------------------------
// Return : 0xFF --> sskey2_A2_table contains bit6
//-----------------------------------------------------------------------------
BYTE  Scratch_Get_Index_Comb_BIT6(void)
{
    return(0x00);
}

//-----------------------------------------------------------------------------
// Return : 0xFF --> sskey2_A2_table contains bit7
//-----------------------------------------------------------------------------
BYTE  Scratch_Get_Index_Comb_BIT7(void)
{
    return(0x00);
}