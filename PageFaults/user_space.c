// import libraries
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include "ioctlheader.h"
int main()
{
    // declaring variables
    int fd,num;

    id i;
    data d;
    // open the misc device
    fd = open("/dev/miscdev", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    if (fd < 0)
    {
        perror("Failed to open file");
    }
    // get pid from user and feed it to kernel via IOCTL system call
    printf("Enter PID: ");
    scanf("%d",&num);
    i.pid = num;
    if (ioctl(fd, SETVARIABLES, &i) < 0)
    {
        perror("ioctl set");
    }

    // get the virtual addresses and time from kernel by IOCTL
    if (ioctl(fd, POSTDATA, &d) < 0)
    {
        perror("ioctl set");
    }
    // print out data
    for(int i=0; i<d.count; i++)
    {
        printf("0x%lu           %lld            %d   %d\n",d.address[i],d.time[i],d.pid[i],d.count);

    }


    // Close file descriptor
    close(fd);
    // make a csv file to store the data so that we can plot
    FILE *fptr = fopen("sysb.csv", "w");
    for(int i=0; i<d.count; i++)
        fprintf(fptr,"%lu,%lld\n",d.address[i],d.time[i]);
    fclose(fptr);
    return 0;
}
