#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h>

struct	my_node {
	struct list_head list;
	int	data;
};

void struct_example(void) {
	struct list_head my_list;
	
	/* initialize list */
	INIT_LIST_HEAD(&my_list);

	/* list element add */
	int i;
	for (i = 0; i < 100000; i++) {
		struct my_node *new = kmalloc(sizeof(struct my_node), GFP_KERNEL);
		new->data = i;
		list_add(&new->list, &my_list);
		//if (i == 0)
		//	printk("First node created !\n");
		//if (i == 99999) {
		//	printk("Last node created !\n");
		//}
	}

	printk("Nodes created !\n");

	struct my_node *current_node;
	struct list_head *p, *q;

//	list_for_each(p, &my_list) {
//		current_node = list_entry(p, struct my_node, list);
//		printk("current value %d\n", current_node->data);
//	}

	printk("Deleting 100 000 nodes\n");
	printk("Delete begins...\n");
	list_for_each_safe(p, q, &my_list) {
		current_node = list_entry(p, struct my_node, list);
		list_del(p);
		kfree(current_node);
		//printk("current value 2 : %d\n", current_node->data);
	}
	printk("Delete ends...\n");
}

int __init hello_module_init(void)
{
	struct_example();
	printk("Assignment 6 init\n");
	return(0);
}

void __exit hello_module_cleanup(void) {
	printk("Bye Module\n");
}

module_init(hello_module_init);
module_exit(hello_module_cleanup);
