obj-m += proc_list.o

proc_list-objs := main.o add_for_current.o add_to_list.o alloc.o print_list.o purge_list.o
 
PWD := $(CURDIR)
 
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 
 
clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean