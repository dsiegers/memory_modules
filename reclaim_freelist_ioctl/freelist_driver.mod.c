#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xc582a679, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x5c60758a, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0xc2c2ef08, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x4b30c4c, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0xe3e3fa4b, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x2b12c8f2, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0xf7b02c20, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0x7e543941, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0x441bb2e, __VMLINUX_SYMBOL_STR(__free_pages) },
	{ 0x362ef408, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0xb44ad4b3, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0x7da52caf, __VMLINUX_SYMBOL_STR(alloc_pages_current) },
	{ 0x7adeb8d4, __VMLINUX_SYMBOL_STR(ktime_get) },
	{ 0xeb4cf53c, __VMLINUX_SYMBOL_STR(freelistTry) },
	{ 0x4796f886, __VMLINUX_SYMBOL_STR(freelistEndif) },
	{ 0x654e7de5, __VMLINUX_SYMBOL_STR(freelistIf) },
	{ 0x53569707, __VMLINUX_SYMBOL_STR(this_cpu_off) },
	{ 0x2ff2f414, __VMLINUX_SYMBOL_STR(freelistCounter) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "CBFB26C8F5CE1913F4C2F5E");
