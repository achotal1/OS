
#include <linux/init.h>
#include <linux/kprobes.h> //for kprobe
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h> // for misc device
#include <linux/device.h>
#include <linux/version.h> // for getting linux version
#include <asm/uaccess.h> // for copy to user and copy from user
#include <linux/ktime.h> // for handling time
#include "ioctlheader.h" // header file for ioctl
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Amol");

// declaring varibales
int var=0;
int count;
u64 nsec;
int ret,minor;
struct timespec t,time;
data d;
// opening device
static int dev_open(struct inode *i, struct file *f)
{
    return 0;
}
// releasing device
static int dev_close(struct inode *i, struct file *f)
{
    return 0;
}
// ioctl main code
// there are different definitions as per linux version
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
static int my_ioctl(struct inode *i, struct file *f, unsigned int cmd, unsigned long arg)
#else
static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
#endif
{
    id i;
    switch(cmd)
    {
    // POSTDATA is for posting the address and time of the page fault to the user space
    case POSTDATA:
        if (copy_to_user((data *)arg, &d, sizeof(data)))
        {
            return -EACCES;
        }
        break;
    // SETVARIABLES is for setting the pid from userspace to kernel
    case SETVARIABLES:
        if(copy_from_user(&i, (id *)arg, sizeof(id)))
        {
            return -EACCES;
        }
        var = i.pid;
        break;
    default:
        return -EINVAL;
    }

    return 0;
}
static struct file_operations fops =
{
    .owner = THIS_MODULE,
    .open = dev_open,
    .release = dev_close,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
    .ioctl = my_ioctl
#else
    .unlocked_ioctl = my_ioctl
#endif
};
// making a misc device
static struct miscdevice misc_device  =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "miscdev",
    .fops = &fops
};

// the pre handler for kprobe which run every time handle__mm_fault is called
static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
    if(current->pid == var)
    {
        getnstimeofday(&t);
        nsec = ktime_get_ns() - nsec;
        nsec = t.tv_nsec - time.tv_nsec + 1000000000 * (t.tv_sec - time.tv_sec);
        d.address[count] = regs->si;
        d.pid[count] = current->pid;
        d.time[count] = nsec;
        d.count = count;
        printk(KERN_ALERT "Virtual Address: 0x%lu Time(ns): %lld",d.address[count],d.time[count]);
        count++;
    }

    return 0;
}

// kprobe on handle_mm_fault
static struct kprobe kp =
{
    .symbol_name = "handle_mm_fault"

};
// initialize kernel
static int __init kprobe_init(void)
{
    count = 0;
    getnstimeofday(&time);
    nsec = ktime_get_ns();
    // register kprobe
    ret = register_kprobe(&kp);
    kp.pre_handler = handler_pre;
    if (ret < 0)
    {
        pr_err("register_kprobe failed %d\n", ret);
        return -1;
    }
    // register misc device
    minor = misc_register(&misc_device);
    if (minor<0)
    {
        printk(KERN_ALERT "Failed to register a minor number");
        return 0;
    }
    pr_info("Kprobe at %p\n",kp.addr);
    return 0;
}
// exit kernel
static void __exit kprobe_exit(void)
{
    //unregister kprobe and misc device
    unregister_kprobe(&kp);
    misc_deregister(&misc_device);
    pr_info("kprobe at %p unregistered\n", kp.addr);
}
module_init(kprobe_init)
module_exit(kprobe_exit)

