/*
riscv lds链接参考：
Dawes LinYu:
https://www.cnblogs.com/wahahahehehe/p/15149960.html
https://zhuanlan.zhihu.com/p/529271196?utm_id=0
https://nucleisys.com/product/n100/6_sdk/?v=20210901401#233
data ： 2023.1.30
*/

/*采用riscv架构体系*/
OUTPUT_ARCH( "riscv" )

/*代码入口地址定位为_start(位于START.s文件夹内标注)*/
ENTRY( vector_base )

/*--------------------------配置RAM内存空间分配--------------------------------*/
/*SRAM总大小及起始地址*/
"RAM_ADDR"=0x20000;
"RAM_SIZE"=32K;

/*c代码中公共内存分配*/
"MEM_SIZE"=2K;

/*data段内存分配*/
"DATA_SIZE"=16K;

/*bss段内存分配*/
"BSS_SIZE"=6K;

/**************动态内存段（bss后）的内存分配**************/
/*栈stack分配，如果没有定义stack大小则默认2k*/
__stack_size = DEFINED(__stack_size) ? __stack_size : 2K;
"STACK_SIZE"=__stack_size;

"HEAP_SIZE"=("STACK_ADDR"-"HEAP_ADDR");

/*地址计算*/
"MEM_ADDR"="RAM_ADDR";
"DATA_ADDR"=("MEM_ADDR"+"MEM_SIZE");
"BSS_ADDR"=("DATA_ADDR"+"DATA_SIZE");
"STACK_ADDR"=("RAM_ADDR"+"RAM_SIZE"-"STACK_SIZE");
"HEAP_ADDR"=("BSS_ADDR"+"BSS_SIZE");

/*
    内存分布声明
    定义Flash、RAM的大小，起始位置
 
    Flash 起始地址为 0x80000，长度为 256KB
    RAM   起始地址为 0x20000，长度为 32KB
  
    ------- 
 */
MEMORY
{
  rom(rxaiw) : ORIGIN = 0x10000, LENGTH = 32K
  flash (rxai!w) : ORIGIN = 0xA0000, LENGTH = 128K

  ram (wxa!ri) : ORIGIN = DATA_ADDR, LENGTH = ("RAM_SIZE"-"MEM_SIZE")
    mem (wxa!ri) : ORIGIN = MEM_ADDR, LENGTH = MEM_SIZE
    data (wxa!ri) : ORIGIN = DATA_ADDR, LENGTH = DATA_SIZE
    bss (wxa!ri) : ORIGIN = BSS_ADDR, LENGTH = BSS_SIZE
    heap(wxa!ri) : ORIGIN = HEAP_ADDR, LENGTH = HEAP_SIZE
    stack (wxa!ri) : ORIGIN = STACK_ADDR, LENGTH = STACK_SIZE
}

/*段声明*/
SECTIONS
{
 
  /*初始化段，程序咯口存放该段*/
  .init           :
  {
    *(.vtable)
    KEEP (*(SORT_NONE(.init)))/*按顺序保存*.init*/
  } >flash AT>flash 

/*
ilm是作为ejtag调试用的一个内部存储空间，和dlm相对应。

*/
/*创建一个标签为_ilm_lma，地址为ilm起始地址（ilm加载域）*/
  .ilalign         :
  {
    . = ALIGN(4);
    PROVIDE( _ilm_lma = . );
  } >flash AT>flash 
/*创建一个标签为_ilm，地址也为ilm起始地址（存储域），由于都在flash，因此地址一样*/
  .ialign         :
  {
    PROVIDE( _ilm = . );
  } >flash AT>flash 

/*代码段*/
  .text           :
  {
    *(.text.unlikely .text.unlikely.*)
    *(.text.startup .text.startup.*)
    *(.text .text.*)
    *(.gnu.linkonce.t.*)
  } >flash AT>flash 

  .fini           :
  {
    KEEP (*(SORT_NONE(.fini)))
  } >flash AT>flash 

  . = ALIGN(4);

  PROVIDE (__etext = .);
  PROVIDE (_etext = .);
  PROVIDE (etext = .);
  PROVIDE( _eilm = . );


  .preinit_array  :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >flash AT>flash 

  .init_array     :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
    KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >flash AT>flash 

  .fini_array     :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
    KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >flash AT>flash

  .ctors          :
  {
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
  } >flash AT>flash 

  .dtors          :
  {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  } >flash AT>flash 

    /*
        该段定义了全局变量_data_lma，
        此段位于前面各段末尾，用于存放data段中的保存在flash数据
        程序运行时会从该地址加载data段数据到RAM中
    */
  .lalign         :
  {
    . = ALIGN(4);
    PROVIDE( _data_lma = . );
  } >flash AT>flash 

  /*
        该段定义了全局变量 _data,
        因为从该段开始第一次声明 保存在RAM中的段，
        所以_data变量的地址为RAM的起始地址 0x20800
    */
  .dalign         :
  {
    . = ALIGN(4);
    PROVIDE( _data = . );
  } >data AT>flash 

/*该段保存已初始化的数据段*/
  .data          :
  {
    *(.rdata)
    *(.rodata .rodata.*)
    *(.gnu.linkonce.r.*)
    *(.data .data.*)
    *(.gnu.linkonce.d.*)
    . = ALIGN(8);
    PROVIDE( __global_pointer$ = . + 0x800 );
    *(.sdata .sdata.* .sdata*)
    *(.gnu.linkonce.s.*)
    . = ALIGN(8);
    *(.srodata.cst16)
    *(.srodata.cst8)
    *(.srodata.cst4)
    *(.srodata.cst2)
    *(.srodata .srodata.*)
  } >data AT>flash 

  . = ALIGN(4);
  PROVIDE( _edata = . );
  PROVIDE( edata = . );
  .balign         :
  {
    . = ALIGN(4);
    PROVIDE( _fbss = . );
    PROVIDE( __bss_start = . );
  } >bss AT>bss 
  /*该段保存未初始化的数据段*/
  .bss            :
  {

    *(.sbss*)
    *(.gnu.linkonce.sb.*)
    *(.bss .bss.*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
    . = ALIGN(4);
  } >bss AT>bss 
/*从这里开始作为动态存储段，非stack部分皆为heap段*/
  . = ALIGN(8);
  PROVIDE( _end = . );
  PROVIDE( end = . );
  PROVIDE( _heap_start = . );
  .stack  :
  {
    PROVIDE( _heap_end = . );
    . = __stack_size;
    PROVIDE( _sp = . );
  } >stack AT>stack 

    .rom_l         :
  {
    . = ALIGN(4);
    PROVIDE( _rom_head = . );
  } >flash AT>flash 
  .rom_r         :
  {
    . = ALIGN(4);
    PROVIDE( _rom = . );
    *(.rom_start)
    *(.ROM.* .rom.*)
    . = ALIGN(4);
    PROVIDE( _rom_end = . );
  }>rom AT>flash
}
