#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/sched/signal.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/pid.h>
#include <asm/pgtable.h>
#include <asm/page.h>

MODULE_AUTHOR("SUNGMIN RYU");
MODULE_LICENSE("GPL");

#define PROC_NAME "hw2"
#define BUFSIZE 3500
#define MAX_PID 0x1fff

// get a module argument(period) 
static int period = 5;
module_param(period, int, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(period, "An integer for period");
static unsigned int msec;

static struct proc_dir_entry *dir;
static struct proc_dir_entry *entry;

static struct timer_list hw2_timer;

static int print_bar(char *s) {
    int i;
    for (i = 0; i < 60; i++)
        sprintf(s + i, "*");
    i += sprintf(s + i, "\n");
	return i;
}

static ssize_t hw2_read_proc(struct file *filp, char __user *ubuf, size_t count, loff_t *offset);

static const struct file_operations proc_fops = {
	owner: THIS_MODULE,
	read: hw2_read_proc
};

struct hw2{
    pid_t pid;
    char comm[TASK_COMM_LEN];
    /* addresses */ 
    unsigned long start_code;
    unsigned long end_code;
    unsigned long start_data;
    unsigned long end_data;
    unsigned long start_bss;
    unsigned long end_bss;
    unsigned long start_brk;
    unsigned long end_brk;
    unsigned long start_shared;
    unsigned long end_shared;
    unsigned long start_stack;
    unsigned long end_stack;
    /* PGD info */
    unsigned long pgd_base;
    unsigned long pgd_code;
    unsigned long pgd_value;
    unsigned long pgd_pfn;
    /* PUD info */
    unsigned long pud_code;
    unsigned long pud_value;
    unsigned long pud_pfn;
    /* PMD info */
    unsigned long pmd_code;
    unsigned long pmd_value;
    unsigned long pmd_pfn;
    /* PTE info */
    unsigned long pte_code;
    unsigned long pte_value;
    unsigned long pte_pfn;
    unsigned long phy_add;
};

static struct hw2 hw2_data[MAX_PID] = { 0, };