
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define MSG(args...) printf(args) 
#define BRAM_MAX_SIZE    2048
#define DATA_LEN    1024

int GpioInit();
int BramInit();

int BramExit();

int SendBram0(unsigned char *sbuffer,unsigned int length);
int ReadBram0(unsigned char *rbuffer);
int SendBram1(unsigned char *sbuffer,unsigned int length);
int ReadBram1(unsigned char *rbuffer);
int SendBram2(unsigned char *sbuffer,unsigned int length);
int ReadBram2(unsigned char *rbuffer);
int SendBram3(unsigned char *sbuffer,unsigned int length);
int ReadBram3(unsigned char *rbuffer);
