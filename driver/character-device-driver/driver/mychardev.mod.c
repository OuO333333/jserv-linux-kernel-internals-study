#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xe2964344, "__wake_up" },
	{ 0x87a21cb3, "__ubsan_handle_out_of_bounds" },
	{ 0xe2c17b5d, "__SCT__might_resched" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0x1000e51, "schedule" },
	{ 0x8c26d495, "prepare_to_wait_event" },
	{ 0x92540fbf, "finish_wait" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x15ba50a6, "jiffies" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x9c6febfc, "add_uevent_var" },
	{ 0x3fd78f3b, "register_chrdev_region" },
	{ 0x1399bb1, "class_create" },
	{ 0xa6f7a612, "cdev_init" },
	{ 0x9166fada, "strncpy" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0xf4407d6b, "cdev_add" },
	{ 0xd3044a78, "device_create" },
	{ 0xf7be671b, "device_destroy" },
	{ 0xfab2eeaa, "class_unregister" },
	{ 0x92ce99, "class_destroy" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x122c3a7e, "_printk" },
	{ 0xba8fbd64, "_raw_spin_lock" },
	{ 0xb5b54b34, "_raw_spin_unlock" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x2fa5cadd, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "57240D69FADABDE3B49919B");
