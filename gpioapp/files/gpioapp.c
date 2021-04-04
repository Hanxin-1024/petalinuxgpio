#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include "gpioapp.h"

#define XPAR_BRAM_0_BASEADDR 0x40000000
#define XPAR_BRAM_1_BASEADDR 0x42000000
#define XPAR_BRAM_2_BASEADDR 0x44000000
#define XPAR_BRAM_3_BASEADDR 0x46000000
#define XPAR_BRAM_4_BASEADDR 0x48000000
#define XPAR_BRAM_5_BASEADDR 0x4A000000
#define XPAR_BRAM_6_BASEADDR 0x4C000000
#define XPAR_BRAM_7_BASEADDR 0x4E000000
#define XPAR_BRAM_8_BASEADDR 0x50000000
#define DATA_LEN    1024



volatile unsigned int *map_base0;
volatile unsigned int *map_base1;
volatile unsigned int *map_base2;
volatile unsigned int *map_base3;
volatile unsigned int *map_base4;
volatile unsigned int *map_base5;
volatile unsigned int *map_base6;
volatile unsigned int *map_base7;
volatile unsigned int *map_base8;

int gpio_fd;
int gpio_fd1;
int gpio_fd2;
int gpio_fd3;
int gpio_fd4;
int gpio_fd5;
int gpio_fd6;
int gpio_fd7;


static int gpio_export(int pin);
static int gpio_unexport(int pin);
static int gpio_direction(int pin, int dir);
static int gpio_write(int pin, int value);
static int gpio_read(int pin);
static int gpio_edge(int pin, int edge);




void XBram_Out32(unsigned int * Addr, unsigned int Value)
{
	volatile unsigned int *LocalAddr = (volatile unsigned int *)Addr;
	*LocalAddr = Value;
}
 unsigned int * XBram_In32(unsigned int * Addr)
{
	return *(volatile unsigned int *) Addr;
}
static int gpio_export(int pin)  
{  
    char buffer[64];  
    int len;  
    int fd;  
  
    fd = open("/sys/class/gpio/export", O_WRONLY);  
    if (fd < 0) 
    {  
        MSG("Failed to open export for writing!\n");  
        return(-1);  
    }  
  
    len = snprintf(buffer, sizeof(buffer), "%d", pin);  
    printf("%s,%d,%d\n",buffer,sizeof(buffer),len);
    if (write(fd, buffer, len) < 0) 
    {  
        MSG("Failed to export gpio!");  
        return -1;  
    }  
     
    close(fd);  
    return 0;  
}  
static int gpio_unexport(int pin)  
{  
    char buffer[64];  
    int len;  
    int fd;  
  
    fd = open("/sys/class/gpio/unexport", O_WRONLY);  
    if (fd < 0) 
    {  
        MSG("Failed to open unexport for writing!\n");  
        return -1;  
    }  
  
    len = snprintf(buffer, sizeof(buffer), "%d", pin);  
    if (write(fd, buffer, len) < 0) 
    {  
        MSG("Failed to unexport gpio!");  
        return -1;  
    }  
     
    close(fd);  
    return 0;  
} 
//dir: 0输入, 1输出
static int gpio_direction(int pin, int dir)  
{  
    static const char dir_str[] = "in\0out";  
    char path[64];  
    int fd;  
  
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/direction", pin);  
    fd = open(path, O_WRONLY);  
    if (fd < 0) 
    {  
        MSG("Failed to open gpio direction for writing!\n");  
        return -1;  
    }  
  
    if (write(fd, &dir_str[dir == 0 ? 0 : 3], dir == 0 ? 2 : 3) < 0) 
    {  
        MSG("Failed to set direction!\n");  
        return -1;  
    }  
  
    close(fd);  
    return 0;  
}  
//value: 0-->LOW, 1-->HIGH
static int gpio_write(int pin, int value)  
{  
    static const char values_str[] = "01";  
    char path[64];  
    int fd;  
  
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);  
    fd = open(path, O_WRONLY);  
    if (fd < 0) 
    {  
        MSG("Failed to open gpio value for writing!\n");  
        return -1;  
    }  
  
    if (write(fd, &values_str[value == 0 ? 0 : 1], 1) < 0) 
    {  
        MSG("Failed to write value!\n");  
        return -1;  
    }  
  
    close(fd);  
    return 0;  
}
static int gpio_read(int pin)  
{  
    char path[64];  
    char value_str[3];  
    int fd;  
  
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);  
    fd = open(path, O_RDONLY);  
    if (fd < 0) 
    {  
        MSG("Failed to open gpio value for reading!\n");  
        return -1;  
    }  
  
    if (read(fd, value_str, 3) < 0)
    {  
        MSG("Failed to read value!\n");  
        return -1;  
    }  
  
    close(fd);  
    return (atoi(value_str));
}  
// none表示引脚为输入，不是中断引脚
// rising表示引脚为中断输入，上升沿触发
// falling表示引脚为中断输入，下降沿触发
// both表示引脚为中断输入，边沿触发
// 0-->none, 1-->rising, 2-->falling, 3-->both
static int gpio_edge(int pin, int edge)
{
const char dir_str[] = "none\0rising\0falling\0both"; 
char ptr;
char path[64];  
    int fd; 
switch(edge)
{
    case 0:
        ptr = 0;
        break;
    case 1:
        ptr = 5;
        break;
    case 2:
        ptr = 12;
        break;
    case 3:
        ptr = 20;
        break;
    default:
        ptr = 0;
} 
  
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/edge", pin);  
    fd = open(path, O_WRONLY);  
    if (fd < 0) 
    {  
        MSG("Failed to open gpio edge for writing!\n");  
        return -1;  
    }  
  
    if (write(fd, &dir_str[ptr], strlen(&dir_str[ptr])) < 0) 
    {  
        MSG("Failed to set edge!\n");  
        return -1;  
    }  
  
    close(fd);  
    return 0;  
}
int GpioInit()
{

    gpio_unexport(958);
    gpio_unexport(959);
    gpio_unexport(960);
    gpio_unexport(961);
    gpio_unexport(962);
    gpio_unexport(963);
    gpio_unexport(964);
    gpio_unexport(965);

    /*********************rx handle**************/ 
    gpio_export(958);
    gpio_direction(958, 0);//
    gpio_edge(958,1);
 
    gpio_fd = open("/sys/class/gpio/gpio958/value",O_RDONLY);
    if(gpio_fd < 0)
    {
        MSG("Failed to open value!\n");  
        return -1;  
    }
    else
    printf("success open958\r\n");

    gpio_export(959);
    gpio_direction(959, 0);//
    gpio_edge(959,1);
 
    gpio_fd1 = open("/sys/class/gpio/gpio959/value",O_RDONLY);
    if(gpio_fd1 < 0)
    {
        MSG("Failed to open value1!\n");  
        return -1;  
    }
    else
    printf("success open959\r\n");

    gpio_export(960);
    gpio_direction(960, 0);//
    gpio_edge(960,1);
 
    gpio_fd2 = open("/sys/class/gpio/gpio960/value",O_RDONLY);
    if(gpio_fd2 < 0)
    {
        MSG("Failed to open value2!\n");  
        return -1;  
    }
    else
    printf("success open960\r\n");

    gpio_export(961);
    gpio_direction(961, 0);//
    gpio_edge(961,1);
 
    gpio_fd3 = open("/sys/class/gpio/gpio961/value",O_RDONLY);
    if(gpio_fd3 < 0)
    {
        MSG("Failed to open value3!\n");  
        return -1;  
    }
    else
    printf("success open961\r\n");

    gpio_export(962);
    gpio_direction(962, 0);//
    gpio_edge(962,1);
 
    gpio_fd4 = open("/sys/class/gpio/gpio962/value",O_RDONLY);
    if(gpio_fd4 < 0)
    {
        MSG("Failed to open value4!\n");  
        return -1;  
    }
    else
    printf("success open962\r\n");

    gpio_export(963);
    gpio_direction(963, 0);//
    gpio_edge(963,1);
 
    gpio_fd5 = open("/sys/class/gpio/gpio963/value",O_RDONLY);
    if(gpio_fd5 < 0)
    {
        MSG("Failed to open value5!\n");  
        return -1;  
    }
    else
    printf("success open963\r\n");

    gpio_export(964);
    gpio_direction(964, 0);//
    gpio_edge(964,1);
 
    gpio_fd6 = open("/sys/class/gpio/gpio964/value",O_RDONLY);
    if(gpio_fd6 < 0)
    {
        MSG("Failed to open value6!\n");  
        return -1;  
    }
    else
    printf("success open964\r\n");

    gpio_export(965);
    gpio_direction(965, 0);//
    gpio_edge(965,1);
 
    gpio_fd7 = open("/sys/class/gpio/gpio965/value",O_RDONLY);
    if(gpio_fd7 < 0)
    {
        MSG("Failed to open value7!\n");  
        return -1;  
    }
    else
    printf("success open965\r\n");

    
}

int BramInit()
{
/********************BRAMinit****************************************************/
    GpioInit();
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) {
        printf("can not open /dev/mem \n");
        return (-1);
    }   
    printf("/dev/mem is open \n");
    /*
    mmap（阿巴阿巴阿巴）
    第二个参数表示内存映射的大小、
    第三个参数是一个 flag标志， PROT_READ | PROT_WRITE 的组合表示映射的内存空间是可读可写的、
    第四个参数MAP_SHARED、
    第五个参数表示文件描述符 fd。
     mmap 函数的返回值就等于映射之后得到的实际地址
    */
    map_base0 = mmap(NULL, BRAM_MAX_SIZE * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, XPAR_BRAM_0_BASEADDR);
    map_base1 = mmap(NULL, BRAM_MAX_SIZE * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, XPAR_BRAM_1_BASEADDR);
    map_base2 = mmap(NULL, BRAM_MAX_SIZE * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, XPAR_BRAM_2_BASEADDR);
    map_base3 = mmap(NULL, BRAM_MAX_SIZE * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, XPAR_BRAM_3_BASEADDR);
    map_base4 = mmap(NULL, BRAM_MAX_SIZE * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, XPAR_BRAM_4_BASEADDR);
    map_base5 = mmap(NULL, BRAM_MAX_SIZE * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, XPAR_BRAM_5_BASEADDR);
    map_base6 = mmap(NULL, BRAM_MAX_SIZE * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, XPAR_BRAM_6_BASEADDR);
    map_base7 = mmap(NULL, BRAM_MAX_SIZE * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, XPAR_BRAM_7_BASEADDR);
    map_base8 = mmap(NULL, BRAM_MAX_SIZE * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, XPAR_BRAM_8_BASEADDR);
    if (map_base0 == 0 || map_base1 == 0|| map_base2 == 0|| map_base3 == 0|| map_base4 == 0||
                          map_base5 == 0|| map_base6 == 0|| map_base7 == 0|| map_base8 == 0 ) { 
        printf("NULL pointer\n");
    }   
    else {
        printf("mmap successful\n");
    }   
     close(fd);
     return 0;
}
 int BramExit()
{
  munmap(map_base0, DATA_LEN);
  munmap(map_base1, DATA_LEN);
  munmap(map_base2, DATA_LEN);
  munmap(map_base3, DATA_LEN);
  munmap(map_base4, DATA_LEN);
  munmap(map_base5, DATA_LEN);
  munmap(map_base6, DATA_LEN);
  munmap(map_base7, DATA_LEN);
}
/*******************************************************
 * 
 * 
 * 
 * 
 * 
 * *****************************************************/
 int SendBram0(unsigned char *sbuffer,unsigned int length)
{
    int m,n;

    int Status;
    int i;
    unsigned int addr;
    unsigned int content;
	unsigned char *bufferrptr;
    bufferrptr = (unsigned char *)map_base2;

    if(sbuffer == NULL)
    {
        return -1;
    }

    m = length/BRAM_MAX_SIZE;
    n = length%BRAM_MAX_SIZE;

    for(i = 0;i < m;i++)
    {
        memcpy(bufferrptr,&sbuffer[i*BRAM_MAX_SIZE],BRAM_MAX_SIZE);
        XBram_Out32(map_base0+1,BRAM_MAX_SIZE);
        XBram_Out32(map_base0+2,0x1);
        usleep(15);
        XBram_Out32(map_base0+2,0);//清除发送中断,实际地址加8

    }

    memcpy(bufferrptr,&sbuffer[m*BRAM_MAX_SIZE],n);
    XBram_Out32(map_base0+1,n);
    XBram_Out32(map_base0+2,0x1);
    usleep(15);
    XBram_Out32(map_base0+2,0);//清除发送中断,实际地址加8

    return 0;

}

int SendBram1(unsigned char *sbuffer,unsigned int length)
{

    int m,n;

    int Status;
    int i;
    unsigned int addr;
    unsigned int content;
	unsigned char *bufferrptr;
    bufferrptr = (unsigned char *)map_base3;

    if(sbuffer == NULL)
    {
        return -1;
    }

    m = length/BRAM_MAX_SIZE;
    n = length%BRAM_MAX_SIZE;

    for(i = 0;i < m;i++)
    {
        memcpy(bufferrptr,&sbuffer[i*BRAM_MAX_SIZE],BRAM_MAX_SIZE);
        XBram_Out32(map_base0+5,BRAM_MAX_SIZE);
        XBram_Out32(map_base0+6,0x1);
        usleep(15);
        XBram_Out32(map_base0+6,0);//清除发送中断,实际地址加24

    }

    memcpy(bufferrptr,&sbuffer[m*BRAM_MAX_SIZE],n);
    XBram_Out32(map_base0+5,n);
    XBram_Out32(map_base0+6,0x1);
    usleep(15);
    XBram_Out32(map_base0+6,0);//清除发送中断,实际地址加24

    return 0;

}
int SendBram2(unsigned char *sbuffer,unsigned int length)
{
    int m,n;

    int Status;
    int i;
    unsigned int addr;
    unsigned int content;
	unsigned char *bufferrptr;
    bufferrptr = (unsigned char *)map_base5;

    if(sbuffer == NULL)
    {
        return -1;
    }

    m = length/BRAM_MAX_SIZE;
    n = length%BRAM_MAX_SIZE;

    for(i = 0;i < m;i++)
    {
        memcpy(bufferrptr,&sbuffer[i*BRAM_MAX_SIZE],BRAM_MAX_SIZE);
        XBram_Out32(map_base0+9,BRAM_MAX_SIZE);
        XBram_Out32(map_base0+10,0x1);
        usleep(15);
        XBram_Out32(map_base0+10,0);//清除发送中断,实际地址加40

    }

    memcpy(bufferrptr,&sbuffer[m*BRAM_MAX_SIZE],n);
    XBram_Out32(map_base0+9,n);
    XBram_Out32(map_base0+10,0x1);
    usleep(15);
    XBram_Out32(map_base0+10,0);//清除发送中断,实际地址加40

    return 0;
}
int SendBram3(unsigned char *sbuffer,unsigned int length)
{
    int m,n;

    int Status;
    int i;
    unsigned int addr;
    unsigned int content;
	unsigned char *bufferrptr;
    bufferrptr = (unsigned char *)map_base7;

    if(sbuffer == NULL)
    {
        return -1;
    }

    m = length/BRAM_MAX_SIZE;
    n = length%BRAM_MAX_SIZE;

    for(i = 0;i < m;i++)
    {
        memcpy(bufferrptr,&sbuffer[i*BRAM_MAX_SIZE],BRAM_MAX_SIZE);
        XBram_Out32(map_base0+13,BRAM_MAX_SIZE);
        XBram_Out32(map_base0+14,0x1);
        usleep(15);
        XBram_Out32(map_base0+14,0);

    }

    memcpy(bufferrptr,&sbuffer[m*BRAM_MAX_SIZE],n);
    XBram_Out32(map_base0+13,n);
    XBram_Out32(map_base0+14,0x1);
    usleep(15);
    XBram_Out32(map_base0+14,0);

    return 0;

}
 int ReadBram0(unsigned char *rbuffer)
{
    int i;
    int Length;
    int Status;
    unsigned int addr;
    unsigned int content;

    Length = XBram_In32(map_base0+2);
     

    memcpy(rbuffer,(unsigned char*)map_base1,Length);
    XBram_Out32(map_base0,0x1);
    XBram_Out32(map_base0,0);//清除接收中断
    // XBram_Out32(map_base0+2,0);//清除发送中断,实际地址加8

    return Length;
}
int ReadBram1(unsigned char *rbuffer)
{
    int i;
    int Length;
    int Status;
    unsigned int addr;
    unsigned int content;

    Length = XBram_In32(map_base0+2+1);
    

    memcpy(rbuffer,(unsigned char*)map_base4,Length);
    XBram_Out32(map_base0+4,0x1);
    XBram_Out32(map_base0+4,0);//清除接收中断
    // XBram_Out32(map_base0+2+4,0);//清除发送中断,实际地址加8

    return Length;
}
int ReadBram2(unsigned char *rbuffer)
{
    int i;
    int Length;
    int Status;
    unsigned int addr;
    unsigned int content;

    Length = XBram_In32(map_base0+2+1+1);
    

    memcpy(rbuffer,(unsigned char*)map_base6,Length);
    XBram_Out32(map_base0+4+4,0x1);
    XBram_Out32(map_base0+4+4,0);//清除接收中断
    // XBram_Out32(map_base0+2+4+4,0);//清除发送中断,实际地址加8

    return Length;
}
int ReadBram3(unsigned char *rbuffer)
{
    int i;
    int Length;
    int Status;
    unsigned int addr;
    unsigned int content;

    Length = XBram_In32(map_base0+2+1+1+1);
 

    memcpy(rbuffer,(unsigned char*)map_base8,Length);
    XBram_Out32(map_base0+4+4+4,0x1);
    XBram_Out32(map_base0+4+4+4,0);//清除接收中断
    // XBram_Out32(map_base0+2+4+4+4,0);//清除发送中断,实际地址加8

    return Length;
}
