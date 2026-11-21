#include <linux/init.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/pid.h>

MODULE_DESCRIPTION("Show subtree of process");
MODULE_LICENSE("GPL");

static int pid = -1;
module_param(pid, int, 0444);
MODULE_PARM_DESC(pid, "Root process PID");

static int proc_dfs(struct task_struct *cur, int depth)
{
    printk(KERN_INFO "%*s%s%s (%d) [%c]\n",
        2*depth, "",
        depth == 0 ? "" : "|-- ",
        cur->comm,
        cur->pid,
        task_state_to_char(cur));

    struct task_struct *child;
    list_for_each_entry(child, &cur->children, sibling){
        proc_dfs(child, depth+1);
    }

    return 0;
}

static int print_proc_tree(void)
{
    struct pid *rpid;
    struct task_struct *root;

    rpid = find_get_pid(pid);
    if(!rpid){
        printk(KERN_ERR "PID: %d not found\n", pid);
        return -1;
    }

    root = get_pid_task(rpid, PIDTYPE_PID);
    if(!root) return -1;
    
    return proc_dfs(root, 0);
}

static int __init proc_tree_init(void)
{
    printk(KERN_INFO "proc_tree: module loaded\n");
    
    return print_proc_tree();
}

static void __exit proc_tree_exit(void)
{
    printk(KERN_INFO "proc_tree: module unloaded\n");
}

module_init(proc_tree_init);
module_exit(proc_tree_exit);