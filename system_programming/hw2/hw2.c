#include "hw2.h"

static void hw2_tasklet_handler(unsigned long flag);
DECLARE_TASKLET(hw2_tasklet, hw2_tasklet_handler, 0);

static void hw2_timer_func(struct timer_list *list_timer){
	tasklet_schedule(&hw2_tasklet);
}

static void hw2_tasklet_handler(unsigned long flag)
{
	char s_pid[5];
	struct task_struct *task; // for counting the number of running tasks
	pid_t pid;
	int i = 0;
	unsigned int mask = 0xfff;
	
	remove_proc_entry(PROC_NAME, NULL);
	dir = proc_mkdir(PROC_NAME, NULL);

	if (dir == NULL){
		printk("Unable to create /proc/hw2\n");
        return;
    }
	// Count the running tasks
	for_each_process(task) {
		if (task->mm != NULL) {
			pgd_t *pgd;
			p4d_t *p4d;
			pud_t *pud;
			pmd_t *pmd;
			pte_t *pte;
			struct vm_area_struct *vm_mmap;

			pid = task->pid; // get pid
			hw2_data[pid].pid = pid;
			sprintf(hw2_data[pid].comm, task->comm);
			
			/* addresses */
			hw2_data[pid].start_code = task->mm->start_code;
			hw2_data[pid].end_code = task->mm->end_code;
			hw2_data[pid].start_data = task->mm->start_data;
			hw2_data[pid].end_data = task->mm->end_data;
			hw2_data[pid].start_brk = task->mm->start_brk;
			hw2_data[pid].end_brk = task->mm->brk;
			hw2_data[pid].start_stack = task->mm->start_stack;

			vm_mmap = task->mm->mmap;

			while (vm_mmap != NULL){
				if (vm_mmap->vm_end <= task->mm->start_brk){
					hw2_data[pid].start_bss = vm_mmap->vm_start;
					hw2_data[pid].end_bss = vm_mmap->vm_end;
				}
				else if (vm_mmap->vm_prev && (vm_mmap->vm_prev->vm_end == task->mm->brk)){
					hw2_data[pid].start_shared = vm_mmap->vm_start;
				}
				else if (vm_mmap->vm_next && (vm_mmap->vm_next->vm_start == task->mm->start_stack)){
					hw2_data[pid].end_shared = vm_mmap->vm_end;
				}
				else if (vm_mmap->vm_end == task->mm->start_stack){
					hw2_data[pid].end_stack = vm_mmap->vm_start;
				}	
				vm_mmap = vm_mmap->vm_next;
			}

			/* PGD */ 
			hw2_data[pid].pgd_base = (unsigned long)task->mm->pgd;
    		pgd = pgd_offset(task->mm, task->mm->start_code);
			hw2_data[pid].pgd_code = (unsigned long)pgd;
   			hw2_data[pid].pgd_value = pgd_val(*pgd);
			hw2_data[pid].pgd_value = (hw2_data[pid].pgd_value << 1) >> 1;
			hw2_data[pid].pgd_pfn = hw2_data[pid].pgd_value >> PAGE_SHIFT;
			/* PUD */
			p4d = p4d_offset(pgd, task->mm->start_code);
			pud = pud_offset(p4d, task->mm->start_code);
			hw2_data[pid].pud_code = (unsigned long)pud;
   			hw2_data[pid].pud_value = pud_val(*pud);
			hw2_data[pid].pud_pfn = hw2_data[pid].pud_value >> PAGE_SHIFT;
			/* PMD */
			pmd = pmd_offset(pud, task->mm->start_code);
			hw2_data[pid].pmd_code = (unsigned long)pmd;
   			hw2_data[pid].pmd_value = pmd_val(*pmd);
			hw2_data[pid].pmd_pfn = hw2_data[pid].pmd_value >> PAGE_SHIFT;
			/* PTE */
			pte = pte_offset_kernel(pmd, task->mm->start_code);
			hw2_data[pid].pte_code = (unsigned long)pte;
			hw2_data[pid].pte_value = pte_val(*pte);
			hw2_data[pid].pte_pfn = hw2_data[pid].pte_value >> PAGE_SHIFT;
			/* Physical address */
			hw2_data[pid].phy_add = hw2_data[pid].pte_pfn << PAGE_SHIFT;
			hw2_data[pid].phy_add = hw2_data[pid].phy_add + (task->mm->start_code & mask);

			sprintf(s_pid, "%d", task->pid);
			entry = proc_create(s_pid, 0, dir, &proc_fops);
			
			if (entry == NULL){
				printk("Unable to create /proc/hw2/process\n");
				remove_proc_entry(s_pid, NULL);
				return;
			}
		}
		i++;
	}
	printk("HW2 Tasklet | Total # of processes: %d\n", i);
	msec = jiffies_to_msecs(jiffies);
	hw2_timer.expires = jiffies + HZ * period;
	hw2_timer.function = hw2_timer_func;
	
	add_timer(&hw2_timer);
}

static ssize_t hw2_read_proc(struct file *filp, char __user *ubuf, size_t count, loff_t *offset) 
{
	char buf[BUFSIZE]; // buffer to print out data
    int len = 0; // length for checking buffer address 
	pid_t pid;

	char *pgd_ps;
	char *pgd_ab;
	char *pgd_cdb;
	char *pgd_pwt;
	char *pgd_usb;
	char *pgd_rwb;
	char *pgd_ppb;

	char *pte_db;
	char *pte_ab;
	char *pte_cdb;
	char *pte_pwt;
	char *pte_usb;
	char *pte_rwb;
	char *pte_ppb;

	if(kstrtoint(filp->f_path.dentry->d_name.name, 10, &pid) != 0){
		return 0;
	}

	if(*offset > 0 || count < BUFSIZE)
		return 0;
	
	len += print_bar(buf);
    len += sprintf(buf + len, "Virtual Memory Address Information\n");
	len += sprintf(buf + len, "Process (%15s:%u)\n", hw2_data[pid].comm, hw2_data[pid].pid);
    len += sprintf(buf + len, "Last update time %u ms\n", msec);
	len += print_bar(buf + len);

	// print info about each area
	len += sprintf(buf + len, "0x%08lx - 0x%08lx: Code Area, %lu page(s)\n", 
					hw2_data[pid].start_code, hw2_data[pid].end_code, (hw2_data[pid].end_code - hw2_data[pid].start_code)/4096);
	len += sprintf(buf + len, "0x%08lx - 0x%08lx: Data Area, %lu page(s)\n", 
					hw2_data[pid].start_data, hw2_data[pid].end_data, (hw2_data[pid].end_data - hw2_data[pid].start_data)/4096);
	len += sprintf(buf + len, "0x%08lx - 0x%08lx: BSS Area, %lu page(s)\n", 
					hw2_data[pid].start_bss, hw2_data[pid].end_bss, (hw2_data[pid].end_bss - hw2_data[pid].start_bss)/4096);
	len += sprintf(buf + len, "0x%08lx - 0x%08lx: Heap Area, %lu page(s)\n", 
					hw2_data[pid].start_brk, hw2_data[pid].end_brk, (hw2_data[pid].end_brk - hw2_data[pid].start_brk)/4096);
	len += sprintf(buf + len, "0x%08lx - 0x%08lx: Shared Libraries Area, %lu page(s)\n", 
					hw2_data[pid].start_shared, hw2_data[pid].end_shared, (hw2_data[pid].end_shared - hw2_data[pid].start_shared)/4096);
	len += sprintf(buf + len, "0x%08lx - 0x%08lx: Stack Area, %lu page(s)\n", 
					hw2_data[pid].start_stack, hw2_data[pid].end_stack, (hw2_data[pid].end_stack - hw2_data[pid].start_stack)/4096);
	
	// 1 level paging (PGD Info)
	len += print_bar(buf + len);
	len += sprintf(buf + len, "1 Level Paging: Page Global Directory Entry Information\n");
	len += print_bar(buf + len);
	
	len += sprintf(buf + len,"PGD     Base Address            : 0x%08lx\n", hw2_data[pid].pgd_base);
	len += sprintf(buf + len,"code    PGD Address             : 0x%08lx\n", hw2_data[pid].pgd_code);
	len += sprintf(buf + len, "        PGD Value               : 0x%08lx\n", hw2_data[pid].pgd_value);
	len += sprintf(buf + len, "        +PFN Address            : 0x%08lx\n", hw2_data[pid].pgd_pfn);

	pgd_ps = (hw2_data[pid].pgd_value >> 7) % 2 == 0  ? "4KB" : "4MB";
	pgd_ab = (hw2_data[pid].pgd_value >> 5) % 2 == 0 ? "0" : "1";
	pgd_cdb = (hw2_data[pid].pgd_value >> 4) % 2 == 0 ? "enable" : "disable";
	pgd_pwt = (hw2_data[pid].pgd_value >> 3) % 2 == 0 ? "write-back" : "write-through";
	pgd_usb = (hw2_data[pid].pgd_value >> 2) % 2 == 0 ? "supervisor" : "user";
	pgd_rwb = (hw2_data[pid].pgd_value >> 1) % 2 == 0 ? "read-only" : "read-wirte";
	pgd_ppb = (hw2_data[pid].pgd_value) % 2 == 0 ? "0" : "1";
	
	len += sprintf(buf + len, "        +Page Size              : %s\n", pgd_ps);
	len += sprintf(buf + len, "        +Accessed Bit           : %s\n", pgd_ab);
	len += sprintf(buf + len, "        +Cache Disable Bit      : %s\n", pgd_cdb);
	len += sprintf(buf + len, "        +Page Write-Through     : %s\n", pgd_pwt);
	len += sprintf(buf + len, "        +User/Supervisor Bit    : %s\n", pgd_usb);
	len += sprintf(buf + len, "        +Read/Write Bit         : %s\n", pgd_rwb);
	len += sprintf(buf + len, "        +Page Present Bit       : %s\n", pgd_ppb);
	
	// 2 level paging (PUD Info)
	len += print_bar(buf + len);
	len += sprintf(buf + len, "2 Level Paging: Page Upper Directory Entry Information \n");
	len += print_bar(buf + len);
	
	len += sprintf(buf + len, "code    PUD Address             : 0x%08lx\n", hw2_data[pid].pud_code);
	len += sprintf(buf + len, "        PUD Value               : 0x%08lx\n", hw2_data[pid].pud_value);
	len += sprintf(buf + len, "        +PFN Address            : 0x%08lx\n", hw2_data[pid].pud_pfn);
	
	// 3 level paging (PMD Info)
	len += print_bar(buf + len);
	len += sprintf(buf + len, "3 Level Paging: Page Middle Directory Entry Information \n");
	len += print_bar(buf + len);

	len += sprintf(buf + len, "code    PMD Address             : 0x%08lx\n", hw2_data[pid].pmd_code);
	len += sprintf(buf + len, "        PMD Value               : 0x%08lx\n", hw2_data[pid].pmd_value);
	len += sprintf(buf + len, "        +PFN Address            : 0x%08lx\n", hw2_data[pid].pmd_pfn);

	// 4 level paging (PTE Info)
	len += print_bar(buf + len);
	len += sprintf(buf + len, "4 Level Paging: Page Table Entry Information \n");
	len += print_bar(buf + len);

	len += sprintf(buf + len, "code    PTE Address             : 0x%08lx\n", hw2_data[pid].pte_code);
	len += sprintf(buf + len, "        PTE Value               : 0x%08lx\n", hw2_data[pid].pte_value);
	len += sprintf(buf + len, "        +Page Base Address      : 0x%08lx\n", hw2_data[pid].pte_pfn);

	pte_db = (hw2_data[pid].pte_value >> 6) % 2 == 0  ? "0" : "1";
	pte_ab = (hw2_data[pid].pte_value >> 5) % 2 == 0 ? "0" : "1";
	pte_cdb = (hw2_data[pid].pte_value >> 4) % 2 == 0 ? "enable" : "disable";
	pte_pwt = (hw2_data[pid].pte_value >> 3) % 2 == 0 ? "write-back" : "write-through";
	pte_usb = (hw2_data[pid].pte_value >> 2) % 2 == 0 ? "supervisor" : "user";
	pte_rwb = (hw2_data[pid].pte_value >> 1) % 2 == 0 ? "read-only" : "read-wirte";
	pte_ppb = (hw2_data[pid].pte_value) % 2 == 0 ? "0" : "1";	

	len += sprintf(buf + len, "        +Dirty Bit              : %s\n", pte_db);
	len += sprintf(buf + len, "        +Accessed Bit           : %s\n", pte_ab);
	len += sprintf(buf + len, "        +Cache Disable Bit      : %s\n", pte_cdb);
	len += sprintf(buf + len, "        +Page Write-Through     : %s\n", pte_pwt);
	len += sprintf(buf + len, "        +User/Supervisor        : %s\n", pte_usb);
	len += sprintf(buf + len, "        +Read/Write Bit         : %s\n", pte_rwb);
	len += sprintf(buf + len, "        +Page Present Bit       : %s\n", pte_ppb);

	len += print_bar(buf + len);
	len += sprintf(buf + len, "Start of Physical Address       : 0x%08lx\n", hw2_data[pid].phy_add);
	len += print_bar(buf + len);

	*offset = len;
	if(raw_copy_to_user(ubuf,buf,len))
		return -EFAULT;
	printk(KERN_DEBUG "checkpoint len: %d\n",len);	
	return len;
}

static int __init hw2_init(void) // initialization of hw_module
{
	tasklet_schedule(&hw2_tasklet);
	printk(KERN_INFO "hw2 module has been loaded\n");
	return 0;
}

static void __exit hw2_exit(void) { // remove hw_module
	del_timer_sync(&hw2_timer);
	remove_proc_entry(PROC_NAME, NULL);
	tasklet_kill(&hw2_tasklet);
    printk(KERN_INFO "hw2 module has been unloaded\n");
}

module_init(hw2_init);
module_exit(hw2_exit);