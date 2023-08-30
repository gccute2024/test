/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-07-25 10:48:37
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

/******************************************h file********************************************/
#include "AE_PRINTF.H"
#ifdef USER_AE10X_LIBC_A
const BYTE  _ctype[] = {
HEX08,HEX08,HEX08,HEX08,HEX08,HEX08,HEX08,HEX08,			/* 0-7 */
HEX08,HEX08|HEX20,HEX08|HEX20,HEX08|HEX20,HEX08|HEX20,HEX08|HEX20,HEX08,HEX08,		/* 8-15 */
HEX08,HEX08,HEX08,HEX08,HEX08,HEX08,HEX08,HEX08,			/* 16-23 */
HEX08,HEX08,HEX08,HEX08,HEX08,HEX08,HEX08,HEX08,			/* 24-31 */
HEX20|HEX80,HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,			/* 32-39 */
HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,			/* 40-47 */
HEX04,HEX04,HEX04,HEX04,HEX04,HEX04,HEX04,HEX04,			/* 48-55 */
HEX04,HEX04,HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,			/* 56-63 */
HEX10,HEX01|HEX40,HEX01|HEX40,HEX01|HEX40,HEX01|HEX40,HEX01|HEX40,HEX01|HEX40,HEX01,	/* 64-71 */
HEX01,HEX01,HEX01,HEX01,HEX01,HEX01,HEX01,HEX01,			/* 72-79 */
HEX01,HEX01,HEX01,HEX01,HEX01,HEX01,HEX01,HEX01,			/* 80-87 */
HEX01,HEX01,HEX01,HEX10,HEX10,HEX10,HEX10,HEX10,			/* 88-95 */
HEX10,HEX02|HEX40,HEX02|HEX40,HEX02|HEX40,HEX02|HEX40,HEX02|HEX40,HEX02|HEX40,HEX02,	/* 96-103 */
HEX02,HEX02,HEX02,HEX02,HEX02,HEX02,HEX02,HEX02,			/* 104-111 */
HEX02,HEX02,HEX02,HEX02,HEX02,HEX02,HEX02,HEX02,			/* 112-119 */
HEX02,HEX02,HEX02,HEX10,HEX10,HEX10,HEX10,HEX08,			/* 120-127 */
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		/* 128-143 */
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		/* 144-159 */
HEX20|HEX80,HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,   /* 160-175 */
HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,HEX10,       /* 176-191 */
HEX01,HEX01,HEX01,HEX01,HEX01,HEX01,HEX01,HEX01,HEX01,HEX01,HEX01,HEX01,HEX01,HEX01,HEX01,HEX01,       /* 192-207 */
HEX01,HEX01,HEX01,HEX01,HEX01,HEX01,HEX01,HEX10,HEX01,HEX01,HEX01,HEX01,HEX01,HEX01,HEX01,HEX02,       /* 208-223 */
HEX02,HEX02,HEX02,HEX02,HEX02,HEX02,HEX02,HEX02,HEX02,HEX02,HEX02,HEX02,HEX02,HEX02,HEX02,HEX02,       /* 224-239 */
HEX02,HEX02,HEX02,HEX02,HEX02,HEX02,HEX02,HEX10,HEX02,HEX02,HEX02,HEX02,HEX02,HEX02,HEX02,HEX02};      /* 240-255 */

/******************************************prirntf.c*************************************************************/

VBYTE print_buff[PRINT_MAX_SIZE]={0};

char *strstr (const char *s1, const char *s2)
{
    char *str1 = (char *)s1;
    if (!*s2)return str1;
    char *str2 = (char *)s2;
    while (*str1)
    {
        str2=(char *)s2;
        while(*str1&&*str1!=*str2)str1++;
        while (*str2 && !(*str1 - *str2))
            str1++, str2++;
        if (!*str2)
            return(str1);
        str1++;
    }
    return(NULL);
}
int	 strcmp (const char *str1, const char *str2)
{
  
    DWORD i=0;
    while (*(str1+i)==*(str2+i))
    {
        if(*(str1+i)=='\0')return 0;
        i++;
    }
    return ++i;
}
char *strcat(char * dest, const char * src)
{
  while(*dest!='\0')dest++;
  while (*src!='\0')
  {
    *dest++=*src++;
  }
  
  return dest;
}
char *strchr(const char *str, int c)
{
    while (*str!=c&&*str!='\0')
    {
        str++;
    }
    if(*str=='\0')return NULL;
    return (char *)str;
}
#if 0
int strcoll(const char *str1, const char *str2){return strcmp (str1, str2);}
char *strcpy (char *__restrict dst0,const char *__restrict src0)
{
    return (char *)memcpy(dst0,src0,strlen(src0));
}
size_t strcspn (const char *s1,const char *s2)
{
  const char *s = s1;
  const char *c;
  while (*s1)
    {
      for (c = s2; *c; c++)
	{
	  if (*s1 == *c)
	    break;
	}
      if (*c)
	break;
      s1++;
    }

  return s1 - s;
}
size_t strlen (const char *str)
{
    DWORD i=0;
    while (*(str+i))
    {
        i++;
    }
    return i;
    
}
char *strpbrk (const char *s1,const char *s2)
{
  const char *c = s2;
  if (!*s1)
    return (char *) NULL;

  while (*s1)
    {
      for (c = s2; *c; c++)
	{
	  if (*s1 == *c)
	    break;
	}
      if (*c)
	break;
      s1++;
    }

  if (*c == '\0')
    s1 = NULL;

  return (char *) s1;
}
char *strrchr(const char *s, int i)
{
      const char *last = NULL;

  if (i)
    {
      while ((s=strchr(s, i)))
	{
	  last = s;
	  s++;
	}
    }
  else
    {
      last = strchr(s, i);
    }
		  
  return (char *) last;
}

size_t strspn(const char *s1, const char *s2)
{
  const char *s = s1;
  const char *c;

  while (*s1)
    {
      for (c = s2; *c; c++)
	{
	  if (*s1 == *c)
	    break;
	}
      if (*c == '\0')
	break;
      s1++;
    }

  return s1 - s;
}
#endif
int puts(const char *str)/*字符串输出*/
{
    #ifdef CONFIG_ONE_PUT
    while (*str)putchar(*str++);
    #else
    while(*str)
    {
        if (*str == '\n') 
		{
			while (!(PRINTF_LSR & UART_LSR_TEMP)) ;
			PRINTF_TX = '\r';
		}
        while (!(PRINTF_LSR & UART_LSR_TEMP)) ;/*当此位为空发送fifo写入数据*/
        PRINTF_TX = *str;       
        str++;       
    }
    while (!(PRINTF_LSR & UART_LSR_TEMP)) ;
        PRINTF_TX = '\r';
    while (!(PRINTF_LSR & UART_LSR_TEMP)) ;
        PRINTF_TX = '\n';
    str++;
    #endif
    return 0;
}
int _isatty(int fd)
{
  if (fd == STDOUT_FILENO || fd == STDERR_FILENO)
    return 1;

  return 0;
}

ssize_t _write(int fd, const void* ptr, size_t len)
{
  const uint8_t * current = (const char *)ptr;

  if (_isatty(fd)) {
    for (size_t jj = 0; jj < len; jj++) {
      putchar(current[jj]);
    }
    return len;
  }
  return -1;
}
static int skip_atoi(const char **s)//字符串转数字
{
	int i = 0;

	while (is_digit(**s))
		i = i * 10 + *((*s)++) - '0';

	return i;
}
static size_t strnlens(const char * s, size_t count)
{
	const char *sc;

	for (sc = s; count-- && *sc != '\0'; ++sc);
	return sc - s;
}
static void string(const char *s, int field_width,
		int precision, int flags)
{
	int len, i;

	if (s == NULL)
		s = "<NULL>";

	len = strnlens(s, precision);

	if (!(flags & LEFT))
		while (len < field_width--)
			putchar(' ');
	for (i = 0; i < len; ++i)
		putchar(*s++);
	while (len < field_width--)
		putchar(' ');
}
static void number(char *buf, s32 num,int base, int size, int precision, int type)
{
	/* we are called with base 8, 10 or 16, only, thus don't need "G..."  */
	static const char digits[16] = "0123456789ABCDEF";//转换字符
	char sign;
    char buff[64];
    short i,cnt;
	sign = 0;
	if (type & SIGN) {
		if ((s32) num < 0) {
			sign = '-';
			num = -(s32) num;
			size--;
		} else if (type & PLUS) {
			sign = '+';
			size--;
		} else if (type & SPACE) {
			sign = ' ';
			size--;
		}
	}
    u32 number=(u32)num;
	if (number == 0)
		putchar('0');
	else if (base != 10) 
    { /* 8 or 16 */
		int mask = base - 1;
		int shift = 3;

		if (base == 16)
			shift = 4;
        i=0;
		do {
			buff[i++]=(digits[((BYTE )number) & mask]);
			number >>= shift;
		} while (number);
	} 
    else 
    { /* base 10 */
		do {
			buff[i++]=(digits[number%base]);
			number /= base;
		} while (number);
	}
    buff[i--]='\0';
    for (cnt = 0; cnt < i; cnt++,i--)
    {
        buff[i]^=buff[cnt];
        buff[cnt]^=buff[i];
        buff[i]^=buff[cnt];
    }

    putchar(sign);
    puts(buff);

}
static void pointer(const char *fmt, char *buf, void *ptr,
		int field_width, int precision, int flags)
{
	unsigned long long int num = (unsigned long int)ptr;

	if (field_width == -1) {
		field_width = 2*sizeof(void *);
	}
	number(buf, num, 16, field_width, precision, flags);
}

static int vprintf_internal(const char *fmt,va_list args)
{
	long long num;//数值
	int base;//进制
	char *str;//指针
	int flags;//标志		

	int field_width;//长度	
	int precision;//精度		
	int qualifier;//类型		
    {
        str = (char *)fmt;
        for (; *str ; ++str) 
        {
            if (*str != '%') 
            {
                putchar(*str);
                continue;
            }
            flags = 0;
repeat:
                ++str;		
                switch (*str) {
                case '-':
                    flags |= LEFT;
                    goto repeat;
                case '+':
                    flags |= PLUS;
                    goto repeat;
                case ' ':
                    flags |= SPACE;
                    goto repeat;
                }
            field_width = -1;
            if (is_digit(*str))
                field_width = skip_atoi((const char **)&str);

            precision = -1;
            if (*str == '.') {
                ++str;
                if (is_digit(*str))
                    precision = skip_atoi((const char **)&str);
                if (precision < 0)
                    precision = 0;
            }

            /* get the conversion qualifier */
            qualifier = -1;
            if (*str == 'h' || *str == 'l' || *str == 'L' ) 
            {
                qualifier = *str;
                ++str;
                if (qualifier == 'l' && *str == 'l') {
                    qualifier = 'L';
                    ++str;
                }
            }		/* nothing */;

            /* default base */
            base = 10;

            switch (*str) {
            case 'c':
                if (!(flags & LEFT)) {
                    while (--field_width > 0)
                        putchar(' ');
                }
                putchar((BYTE ) va_arg(args, int));
                while (--field_width > 0)
                    putchar(' ');
                continue;
            case 's':
                {
                    string(va_arg(args, char *),field_width, precision, flags);
                }
                continue;
            case 'p':
                pointer(str + 1, str,va_arg(args, void *),field_width, precision, flags);
                while (isalnum(str[1]))
                    str++;
                continue;
            case '%':
                putchar('%');
                continue;

            /* integer number formats - set up the flags and "break" */
            case 'o':
                base = 8;
                break;

            case 'x':
            case 'X':
                base = 16;
                break;

            case 'd':
            case 'i':
                flags |= SIGN;
            case 'u':
                break;
		/* nothing */;
            default:
                putchar('%');
                if (*str)
                    putchar(*str);
                else
                    --str;
                continue;
            }
            if (qualifier == 'L')  /* "quad" for 64 bit variables */
                num = va_arg(args,unsigned long long);
            else if (qualifier == 'l') {
                num = va_arg(args,unsigned long);
                if (flags & SIGN)
                    num = (signed long) num;
            } else if (qualifier == 'h') {
                num = (WORD) va_arg(args, int);
                if (flags & SIGN)
                    num = (signed short) num;
            } else {
                num = va_arg(args, DWORD);
                if (flags & SIGN)
                    num = (signed int) num;
            }
            number(str,num, base, field_width, precision,
                    flags);
        }
            //putchar('\0');
            --str;
        /* the trailing null byte doesn't count towards the total */
        return str - fmt;

    }
}

int printf(const char* fmt, ...)
{
    int i;
    va_list args;//创建形参表
	va_start(args, fmt);//初值
	i=vprintf_internal(fmt, args);
	va_end(args);//结束
    return i;
}
#elif (defined(USER_RISCV_LIBC_A))
USED int _isatty(int fd)
{
  if (fd == STDOUT_FILENO || fd == STDERR_FILENO)
    return 1;

  return 0;
}
USED ssize_t _write(int fd, const void* ptr, size_t len)
{
  const BYTEP str = (const BYTEP)ptr;

  if (isatty(fd)) {
    for (size_t j = 0; j < len; j++) {
        if (*str == '\n') 
        {
            while (!(PRINTF_LSR & UART_LSR_TEMP)) ;
            PRINTF_TX = '\r';
        }
        while (!(PRINTF_LSR & UART_LSR_TEMP)) ;/*当此位为空发送fifo写入数据*/
        PRINTF_TX = *str;
        str++;
    }
    return len;
  }
  return -1;
}
//.gloable
USED int _read(int fd,  void* ptr, size_t len)
{
     BYTEP str = (BYTEP)ptr;
  if (isatty(fd)) {
    for (size_t j = 0; j < len; j++) {
        while (!(PRINTF_LSR & UART_LSR_OE)) ;/*当此位为空发送fifo写入数据*/
        *str = PRINTF_RX;  
    }
    return len;
  }
  return -1;
}
#endif