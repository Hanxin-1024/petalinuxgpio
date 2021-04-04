#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <pthread.h>

#include "gpioapp.h"


extern gpio_fd;
extern gpio_fd2;
extern gpio_fd4;
extern gpio_fd6;
static unsigned char rbuffer[BRAM_MAX_SIZE] = {0};

void *rapidio_taks_rec(void *arg)
{
    int ret = 0,i;
    unsigned int rec_len = 0;
    struct pollfd fds[1];
    char buff[10];
    static cnt = 0;


    fds[0].fd = gpio_fd;
    fds[0].events  = POLLPRI;


    ret = read(gpio_fd,buff,10);
    if( ret == -1 )
        MSG("read\n");

    while(1)
    {
      ret = poll(fds,1,-1);
      if( ret == -1 )
          MSG("poll\n");
      if( fds[0].revents & POLLPRI)
      {
        ret = lseek(gpio_fd,0,SEEK_SET);
        if( ret == -1 )
            MSG("lseek\n");
        ret = read(gpio_fd,buff,10);
        if( ret == -1 )
            MSG("read\n");

        memset(rbuffer,0,BRAM_MAX_SIZE);
        rec_len = ReadBram0(rbuffer);
        cnt ++;
        printf("\nrec_len = 0x%x,cnt = %d\n",rec_len,cnt);
        printf("bram0 rx\n");

        // if(cnt == 3)
        // {
        //     for(i = 0;i<256;i++)
        //     {
        //         if(i%16 == 0)
        //         {
        //             printf("\n");
        //         }         
        //         printf("%x ",rbuffer[i]);
        //     }
        //     printf("\n");
        // }

        // if(rec_len != BRAM_MAX_SIZE)
        // {
        //     for(i = 0;i<rec_len;i++)
        //     {
        //         if(i%16 == 0)
        //         {
        //             printf("\n");
        //         }   
        //         printf("%x ",rbuffer[i]);
        //     }
        //     printf("\n");
        // }

      }
    else
    printf("poll nothing--------------------------\n");
   }

   pthread_exit(0);
}
void *rapidio_taks_rec1(void *arg)
{
    int ret = 0,i;
    unsigned int rec_len = 0;
    struct pollfd fds[1];
    char buff[10];
    static cnt = 0;

    fds[0].fd = gpio_fd2;
    fds[0].events  = POLLPRI;


    ret = read(gpio_fd2,buff,10);
    if( ret == -1 )
        MSG("read\n");

    while(1)
    {
      ret = poll(fds,1,-1);
      if( ret == -1 )
          MSG("poll\n");
      if( fds[0].revents & POLLPRI)
      {
        ret = lseek(gpio_fd2,0,SEEK_SET);
        if( ret == -1 )
            MSG("lseek\n");
        ret = read(gpio_fd2,buff,10);
        if( ret == -1 )
            MSG("read\n");

        memset(rbuffer,0,256);
        rec_len = ReadBram1(rbuffer);
        cnt ++;
        printf("\nrec_len = 0x%x,cnt = %d\n",rec_len,cnt);
        printf("bram1 rx\n");
        // for(i = 0;i<rec_len;i++)
        // {
        //     if(i%16 == 0)
        //     {
        //         printf("\n");
        //     }
         
        //     printf("%x ",rbuffer[i]);
        // }
        // printf("\n");
      }
    else
    printf("poll nothing--------------------------\n");
   }

   pthread_exit(0);
}
void *rapidio_taks_rec2(void *arg)
{
    int ret = 0,i;
    unsigned int rec_len = 0;
    struct pollfd fds[1];
    char buff[10];
    static cnt = 0;


    fds[0].fd = gpio_fd4;
    fds[0].events  = POLLPRI;


    ret = read(gpio_fd4,buff,10);
    if( ret == -1 )
        MSG("read\n");

    while(1)
    {
      ret = poll(fds,1,-1);
      if( ret == -1 )
          MSG("poll\n");
      if( fds[0].revents & POLLPRI)
      {
        ret = lseek(gpio_fd4,0,SEEK_SET);
        if( ret == -1 )
            MSG("lseek\n");
        ret = read(gpio_fd4,buff,10);
        if( ret == -1 )
            MSG("read\n");

        memset(rbuffer,0,256);
        rec_len = ReadBram2(rbuffer);
       cnt ++;
        printf("\nrec_len = 0x%x,cnt = %d\n",rec_len,cnt);
        printf("bram2 rx\n");
        // for(i = 0;i<rec_len;i++)
        // {
        //     if(i%16 == 0)
        //     {
        //         printf("\n");
        //     }
         
        //     printf("%x ",rbuffer[i]);
        // }
        // printf("\n");
      }
    else
    printf("poll nothing--------------------------\n");
   }

   pthread_exit(0);
}
void *rapidio_taks_rec3(void *arg)
{
    int ret = 0,i;
    unsigned int rec_len = 0;
    struct pollfd fds[1];
    char buff[10];
    static cnt = 0;


    fds[0].fd = gpio_fd6;
    fds[0].events  = POLLPRI;


    ret = read(gpio_fd6,buff,10);
    if( ret == -1 )
        MSG("read\n");

    while(1)
    {
      ret = poll(fds,1,-1);
      if( ret == -1 )
          MSG("poll\n");
      if( fds[0].revents & POLLPRI)
      {
        ret = lseek(gpio_fd6,0,SEEK_SET);
        if( ret == -1 )
            MSG("lseek\n");
        ret = read(gpio_fd6,buff,10);
        if( ret == -1 )
            MSG("read\n");

        memset(rbuffer,0,256);
        rec_len = ReadBram3(rbuffer);
       cnt ++;
        printf("\nrec_len = 0x%x,cnt = %d\n",rec_len,cnt);

        printf("bram3 rx\n");
        for(i = 0;i<(rec_len);i++)
        {
            if(i%16 == 0)
            {
                printf("\n");
            }
         
           printf("0x%02x ",rbuffer[i]);//注意大小端转换
        }
        printf("\n");
        SendBram3(rbuffer,rec_len); 
      }
      
    else
    printf("poll nothing--------------------------\n");
   }

   pthread_exit(0);
}
void *rapidio_taks_send(void *arg)
{

    int i;
    unsigned char sbuffer[256] = {0};

    for(i = 0;i < 256;i++)
    {
        sbuffer[i]=i+1;
    }

    while(1)
    {
        sleep(10);
        SendBram0(sbuffer,0x500); 
        
    }

   pthread_exit(0);

   
}
void *rapidio_taks_send1(void *arg)
{
    int i;
    unsigned char sbuffer[256] = {0};

    for(i = 0;i < 256;i++)
    {
        sbuffer[i]=i+1;
    }

    while(1)
    {
        sleep(10);
        SendBram1(sbuffer,0x100); 
        
    }

   pthread_exit(0);

}
void *rapidio_taks_send2(void *arg)
{

    
    int i;
    unsigned char sbuffer[256] = {0};

    for(i = 0;i < 256;i++)
    {
        sbuffer[i]=i+2;
    }

    while(1)
    {
        sleep(10);
        SendBram2(sbuffer,0x200); 
        
    }

   pthread_exit(0);

}
void *rapidio_taks_send3(void *arg)
{
int i;
    unsigned char sbuffer[256] = {0};

    for(i = 0;i < 256;i++)
    {
        sbuffer[i]=i+3;
    }

    while(1)
    {
        sleep(10);
        // SendBram3(sbuffer,0x51); 
        
    }

   pthread_exit(0);
}


int main(int argc, char **argv)
{

    int error;
    int ret;

    pthread_t rapidio_sid;
    pthread_t rapidio_rid;
    pthread_t rapidio_sid1;
    pthread_t rapidio_rid1;
    pthread_t rapidio_sid2;
    pthread_t rapidio_rid2;
    pthread_t rapidio_sid3;
    pthread_t rapidio_rid3;
    printf("enter app --------\n");

    BramInit();

    error=pthread_create(&rapidio_rid, NULL, &rapidio_taks_rec,NULL);
    if(error != 0)
    {
        printf("pthreadrx_create fail\n");
        return -1;
    }
    error=pthread_create(&rapidio_sid, NULL, &rapidio_taks_send,NULL);
    if(error != 0)
    {
        printf("pthreadtx_create fail\n");
        return -1;
    }
    error=pthread_create(&rapidio_rid1, NULL, &rapidio_taks_rec1,NULL);
    if(error != 0)
    {
        printf("pthreadrx1_create fail\n");
        return -1;
    }
    error=pthread_create(&rapidio_sid1, NULL, &rapidio_taks_send1,NULL);
    if(error != 0)
    {
        printf("pthreadtx1_create fail\n");
        return -1;
    }
    error=pthread_create(&rapidio_rid2, NULL, &rapidio_taks_rec2,NULL);
    if(error != 0)
    {
        printf("pthreadrx2_create fail\n");
        return -1;
    }
    error=pthread_create(&rapidio_sid2, NULL, &rapidio_taks_send2,NULL);
    if(error != 0)
    {
        printf("pthreadtx2_create fail\n");
        return -1;
    }
    error=pthread_create(&rapidio_rid3, NULL, &rapidio_taks_rec3,NULL);
    if(error != 0)
    {
        printf("pthreadrx3_create fail\n");
        return -1;
    }
    error=pthread_create(&rapidio_sid3, NULL, &rapidio_taks_send3,NULL);
    if(error != 0)
    {
        printf("pthreadtx3_create fail\n");
        return -1;
    }
    pthread_detach(rapidio_rid);
    pthread_detach(rapidio_sid);
    pthread_detach(rapidio_rid1);
    pthread_detach(rapidio_sid1);
    pthread_detach(rapidio_rid2);
    pthread_detach(rapidio_sid2);
    pthread_detach(rapidio_rid3);
    pthread_detach(rapidio_sid3);
 
    while(1);

    //BramExit();
    return 0;
}