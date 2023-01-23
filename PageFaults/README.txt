AMOL CHOTALIA 
PAGE FAULT DETECTION 
make modules_install first
then *make modules* to compile the files 
*make i* (sudo insmod kprobe.ko) to insert the mod 
*make r* (sudo rmmod kprobe) to remove the mod 
*make user* (sudo ./user_space) to start the user space program 
dmesg to view kernel messages.
You will have to run dmesg before running the user space program again to see 
the results on the users side.