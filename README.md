Multi-Container Runtime
Overview

This project implements a lightweight multi-container runtime system in C along with a Linux kernel module for monitoring container memory usage.

The runtime allows users to start, list, and stop containers, while the kernel module continuously tracks memory usage and enforces limits.

Aim

To design and implement a basic container runtime that can manage multiple containers and integrate it with a Linux kernel module to monitor and control memory usage using soft and hard limits.

Tools & Technologies Used
C Programming Language
Linux Operating System (Ubuntu 22.04/24.04)
Linux Kernel Module Development
GCC Compiler
Makefile
System Calls (fork, exec, kill, etc.)
VirtualBox
Implementation
1. Container Runtime (engine.c)

The user-space runtime provides a CLI interface with the following commands:

start → Starts a container
ps → Lists all running containers
stop → Stops a container
Features:
Uses fork() and exec() to create container processes
Supports multiple containers simultaneously
Tracks containers using PID
Stores container information in a file
Ensures proper cleanup of processes
2. Kernel Module (monitor.c)

The Linux kernel module monitors memory usage of containers.

Features:
Tracks processes using PID
Maintains a linked list of monitored containers
Uses timer callbacks for periodic monitoring
Implements:
Soft limit → logs warning
Hard limit → terminates process
Uses ioctl for communication between user-space and kernel-space
Ensures thread-safe operations using mutex
3. Memory Monitoring Logic
Memory usage (RSS) is calculated for each process
If memory exceeds:
Soft limit → warning logged
Hard limit → process is terminated
Monitoring is performed periodically using kernel timers
Execution Steps
Build the project
make
Load kernel module
sudo insmod monitor.ko
Verify module
lsmod | grep monitor
Start containers
sudo ./engine start c1 rootfs-base /bin/sh
sudo ./engine start c2 rootfs-base /bin/sh
List containers
./engine ps
Stop containers
./engine stop c1
./engine stop c2
Remove module
sudo rmmod monitor
Screenshots
Kernel module loaded
<img width="753" height="71" alt="image" src="https://github.com/user-attachments/assets/edacd534-b405-4bb6-826d-16c5deddd7bd" />



Container started
<img width="753" height="142" alt="image" src="https://github.com/user-attachments/assets/326a4763-36a1-42fb-809c-e30eb977129a" />



Multiple containers running
<img width="753" height="91" alt="image" src="https://github.com/user-attachments/assets/98111db8-b48e-40a2-8a75-151f7fb83b00" />



Stopping containers
<img width="753" height="50" alt="image" src="https://github.com/user-attachments/assets/5bbf54af-4c39-4a5f-bccb-45bdd5f33aa5" />
<img width="753" height="62" alt="image" src="https://github.com/user-attachments/assets/94e58c69-3b8f-45d3-b7e0-39a256c5c644" />


Final state
<img width="753" height="110" alt="image" src="https://github.com/user-attachments/assets/29aec8b5-c425-48b7-93c6-16351c847c59" />



Explanation

This project demonstrates how user-space container management can be integrated with kernel-space monitoring. The runtime handles process lifecycle, while the kernel module ensures efficient resource control and enforcement.

Conclusion

This project successfully implements a basic multi-container runtime with kernel-level memory monitoring. It demonstrates key operating system concepts such as process management, kernel modules, memory tracking, and user-kernel interaction. The system is efficient, extensible, and provides a strong foundation for understanding container internals.
