#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/sched/signal.h> // for counting the number of running tasks

MODULE_AUTHOR("SUNGMIN RYU");

#define PROC_NAME "hw1"
#define BUFSIZE 3500
#define NUM_OF_TASKS 5

// check policy whether the task is CFS or RT
static inline int fair_policy(int policy)
{
    return policy == SCHED_NORMAL || policy == SCHED_BATCH;
}

static inline int rt_policy(int policy)
{
    return policy == SCHED_FIFO || policy == SCHED_RR;
}

struct proc_dir_entry *entry;

struct hw1_task {
    char comm[TASK_COMM_LEN];
    int pid, prio; 
	unsigned int policy;
    unsigned long weight;
	unsigned long long lasttime;		
    u64 starttime, exectime, vruntime;
};

struct hw1_cpu {
	struct hw1_task hwtask[5]; 
};

extern struct hw1_cpu hwcpu[8]; // From core.c

static int print_bar(char *s) {
    int i;
    for (i = 0; i < 100; i++)
        sprintf(s + i, "-");
    i += sprintf(s + i, "\n");
	return i;
}	

static ssize_t hw1_read_proc(struct file *filp, char __user *ubuf, size_t count, loff_t *offset) 
{
	char buf[BUFSIZE]; // buffer to print out data
    int len = 0; // lenght for checking buffer address 
	int cpu, task; // cpu number and task count
	unsigned int num_of_cpus = num_online_cpus(); // get the number of running CPUS
	unsigned int num_of_tasks = 0; // count task in hwcpu

	// for counting the number of running tasks
	struct task_struct *task_list; 
	// Count the running tasks
	for_each_process(task_list) {
		++num_of_tasks;
	}
		
	if(*offset > 0 || count < BUFSIZE)
		return 0;
	// Print out hwcpu data from core.c
	len += print_bar(buf);
    len += sprintf(buf + len, "[System Programming Assignment 1]\n");
	len += sprintf(buf + len, "ID: 2015142213, Name: Ryu, Sungmin\n");
    len += sprintf(buf + len, "Total %d tasks, %dHz\n", num_of_tasks, HZ);
	len += print_bar(buf + len);
	
	for (cpu=0; cpu<num_of_cpus; cpu++) {
		len += sprintf(buf + len, "CPU %d\n", cpu);
		len += print_bar(buf + len);
		len += sprintf(buf + len, "\t    COMM    PID       START_TIME        LAST_TIME  PRIO    EXEC_TIME   WEIGHT     VRUNTIME\n");
		for (task=0; task < NUM_OF_TASKS; task++){
			struct hw1_task ht = hwcpu[cpu].hwtask[task];
			if (fair_policy(ht.policy)) // CFS
				len += sprintf(buf + len, "%16s %6d %16lld %16lld %5d %12lld %8ld %12lld\n", ht.comm, ht.pid, ht.starttime, ht.lasttime, ht.prio, ht.exectime, ht.weight, ht.vruntime);
			else if (rt_policy(ht.policy)) // RT
				len += sprintf(buf + len, "%16s %6d %16lld %16lld %5d %12lld\n", ht.comm, ht.pid, ht.starttime, ht.lasttime, ht.prio, ht.exectime);
		}
		len += print_bar(buf + len);
	}

	*offset = len;
	if(raw_copy_to_user(ubuf,buf,len))
		return -EFAULT;
	printk(KERN_DEBUG "checkpoint len: %d\n",len);	
	return len;
}

static const struct file_operations proc_fops = {
	owner: THIS_MODULE,
	read: hw1_read_proc
};

static int __init hw1_init(void) // initialization of hw_module
{
	entry = proc_create(PROC_NAME, 0, NULL, &proc_fops);
	

	if(entry == NULL){
		remove_proc_entry(PROC_NAME, NULL);
		return -EACCES;
	}

	printk(KERN_INFO "hw1 module has been loaded\n");
	return 0;
}

static void __exit hw1_exit(void) { // remove hw_module
	remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "hw1 module has been unloaded\n");
}

module_init(hw1_init);
module_exit(hw1_exit);
