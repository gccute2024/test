/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-07-03 17:00:52
 * @Description: 
 * 
 * 
 * The following is the Chinese and English copyright notice, encoded as UTF-8.
 * 以下是中文及英文版权同步声明，编码为UTF-8。
 * Copyright has legal benefits, and violations must be investigated.
 * 版权具有法律效益，违法必究。
 * 
 * Copyright ©2021-2025 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
 * 版权所有 ©2021-2025龙晶石半导体科技（苏州）有限公司
 */

#if defined(USER_AE10X_LIBC_A)||1
#include <AE_LIBC.H>
#include <AE_INIT.H>
SECTION(".flash_over") USED BYTE flash_over_val=0xff;
void __assert_func (const char *file,
	int line,
	const char *func,
	const char *failedexpr)
{
  dprint("assertion \"%s\" failed: file \"%s\", line %d%s%s\n",failedexpr, file, line,func ? ", function: " : "", func ? func : "");
  abort();
  /* NOTREACHED */
}
char *setlocale (int category, const char *locale)
{
    if (locale)
    { 
        if (strcmp (locale, "POSIX") && strcmp (locale, "C")&& strcmp (locale, ""))
        return NULL;
    }
    return "C";
}
#ifdef _SETJMP_H_
void	longjmp (jmp_buf __jmpb, int __retval)
{exit(0);while(1);}

int	setjmp (jmp_buf __jmpb)
{return 0;}
#endif

struct lconv *localeconv (void)
{
    return NULL;
}

void abort (void)
{
    exit(SIGABRT);
    while (1);
    
}
#ifdef _SIGNAL_H_
_sig_func_ptr signal (int sig, _sig_func_ptr sig_func)
{

}
int	raise (int sig)
{}
#endif
#ifdef _STDLIB_H_
int rand(void)
{}
void srand(DWORD seed)
{}
int abs(int num)
{
    if(num<0)return -num;
    else return num;
}
long labs(long x)
{
    return abs(x);
}
time_t time(time_t *t)
{
    *t=(((VDWORD)timer_1s_count)+((VDWORD)timer_1min_count)*60+((VDWORD)timer_1hours_count)*3600);
    return *t;
}
ldiv_t	ldiv (long num, long denom)
{
    ldiv_t div_n;
    div_n.quot=num/denom;
    div_n.rem=num%denom;
    return div_n;
}

div_t	div (int num, int denom)
{
    div_t div_n;
    div_n.quot=num/denom;
    div_n.rem=num%denom;
    return div_n;
}
#endif
int *__errno (void)
{
    return (int *)-1;//直接出错
}
#endif