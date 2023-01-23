#ifndef IOCTLHEADER_H_INCLUDED
#define IOCTLHEADER_H_INCLUDED
#include <linux/ioctl.h>
#define MAX 1000
// struct for storing pid to give from userspace to kernel
typedef struct id
{
    int pid;

} id;
// struct for storing the virtual address and time
typedef struct data
{
    unsigned long address[MAX];
    long long time[MAX];
    //stores pid where you get a pf
    int pid[MAX];
    // number of page faults for a pid
    int count;

} data;

#define POSTDATA _IOR('i', 1, data *)
#define SETVARIABLES _IOW('i', 2, id *)
#endif // IOCTLHEADER_H_INCLUDED
