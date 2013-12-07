/*
 * (C) Copyright 2002-2006
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * To match the U-Boot user interface on ARM platforms to the U-Boot
 * standard (as on PPC platforms), some messages with debug character
 * are removed from the default U-Boot build.
 *
 * Define DEBUG here if you want additional info as shown below
 * printed upon startup:
 *
 * U-Boot code: 00F00000 -> 00F3C774  BSS: -> 00FC3274
 * IRQ Stack: 00ebff7c
 * FIQ Stack: 00ebef7c
 */

#include <common.h>
#include <command.h>
#include <malloc.h>
#include <stdio_dev.h>
#include <timestamp.h>
#include <version.h>
#include <net.h>
#include <serial.h>
#include <nand.h>
#include <onenand_uboot.h>
#include <mmc.h>

#if defined(MT6516)
  #include <asm/arch/mt6516_bat.h>
  #include <asm/arch/mt6516_typedefs.h>
#else
  //#include <asm/arch/mt65xx_bat.h>
  #include <asm/arch/mt65xx_typedefs.h>
  #include <asm/arch/boot_mode.h>
#endif

#ifdef CONFIG_BITBANGMII
#include <miiphy.h>
#endif

#ifdef CONFIG_DRIVER_SMC91111
#include "../drivers/net/smc91111.h"
#endif
#ifdef CONFIG_DRIVER_LAN91C96
#include "../drivers/net/lan91c96.h"
#endif

DECLARE_GLOBAL_DATA_PTR;

ulong monitor_flash_len;


// ===========================================
// EXTERNAL DEFINITIONS
// ===========================================
extern void dbg_print(char *sz,...);
extern void mt6516_mode_select(void);
extern void video_printf(const char *fmt, ...);
extern bool rtc_boot_check(bool can_alarm_boot);

#ifdef CONFIG_HAS_DATAFLASH
extern int  AT91F_DataflashInit(void);
extern void dataflash_print_info(void);
#endif

#ifndef CONFIG_IDENT_STRING
#define CONFIG_IDENT_STRING ""
#endif

const char version_string[] =
	U_BOOT_VERSION" (" U_BOOT_DATE " - " U_BOOT_TIME ")"CONFIG_IDENT_STRING;

#ifdef CONFIG_DRIVER_RTL8019
extern void rtl8019_get_enetaddr (uchar * addr);
#endif

#if defined(CONFIG_HARD_I2C) || \
    defined(CONFIG_SOFT_I2C)
#include <i2c.h>
#endif


/************************************************************************
 * Coloured LED functionality
 ************************************************************************
 * May be supplied by boards if desired
 */
void inline __coloured_LED_init (void) {}
void coloured_LED_init (void) __attribute__((weak, alias("__coloured_LED_init")));
void inline __red_LED_on (void) {}
void red_LED_on (void) __attribute__((weak, alias("__red_LED_on")));
void inline __red_LED_off(void) {}
void red_LED_off(void) __attribute__((weak, alias("__red_LED_off")));
void inline __green_LED_on(void) {}
void green_LED_on(void) __attribute__((weak, alias("__green_LED_on")));
void inline __green_LED_off(void) {}
void green_LED_off(void) __attribute__((weak, alias("__green_LED_off")));
void inline __yellow_LED_on(void) {}
void yellow_LED_on(void) __attribute__((weak, alias("__yellow_LED_on")));
void inline __yellow_LED_off(void) {}
void yellow_LED_off(void) __attribute__((weak, alias("__yellow_LED_off")));
void inline __blue_LED_on(void) {}
void blue_LED_on(void) __attribute__((weak, alias("__blue_LED_on")));
void inline __blue_LED_off(void) {}
void blue_LED_off(void) __attribute__((weak, alias("__blue_LED_off")));

/************************************************************************
 * Init Utilities							*
 ************************************************************************
 * Some of this code should be moved into the core functions,
 * or dropped completely,
 * but let's get it working (again) first...
 */

#if defined(CONFIG_ARM_DCC) && !defined(CONFIG_BAUDRATE)
#define CONFIG_BAUDRATE 115200
#endif
static int init_baudrate (void)
{
	char tmp[64];	/* long enough for environment variables */
	int i = getenv_r ("baudrate", tmp, sizeof (tmp));
	gd->bd->bi_baudrate = gd->baudrate = (i > 0)
			? (int) simple_strtoul (tmp, NULL, 10)
			: CONFIG_BAUDRATE;

	return (0);
}

static int display_banner (void)
{
	printf ("\n\n%s\n\n", version_string);
	debug ("U-Boot code: %08lX -> %08lX  BSS: -> %08lX\n",
	       _armboot_start, _bss_start, _bss_end);
#ifdef CONFIG_MODEM_SUPPORT
	debug ("Modem Support enabled\n");
#endif
#ifdef CONFIG_USE_IRQ
	debug ("IRQ Stack: %08lx\n", IRQ_STACK_START);
	debug ("FIQ Stack: %08lx\n", FIQ_STACK_START);
#endif

	return (0);
}

/*
 * WARNING: this code looks "cleaner" than the PowerPC version, but
 * has the disadvantage that you either get nothing, or everything.
 * On PowerPC, you might see "DRAM: " before the system hangs - which
 * gives a simple yet clear indication which part of the
 * initialization if failing.
 */
static int display_dram_config (void)
{
	int i;

//#ifdef DEBUG
	puts ("RAM Configuration:\n");

	for(i=0; i<CONFIG_NR_DRAM_BANKS; i++) {
		printf ("Bank #%d: %08lx ", i, gd->bd->bi_dram[i].start);
		print_size (gd->bd->bi_dram[i].size, "\n");
	}
//#else
//	ulong size = 0;

//	for (i=0; i<CONFIG_NR_DRAM_BANKS; i++) {
//		size += gd->bd->bi_dram[i].size;
//	}
//	puts("DRAM:  ");
//	print_size(size, "\n");
//#endif

	return (0);
}

#ifndef CONFIG_SYS_NO_FLASH
static void display_flash_config (ulong size)
{
	puts ("Flash: ");
	print_size (size, "\n");
}
#endif /* CONFIG_SYS_NO_FLASH */

#if defined(CONFIG_HARD_I2C) || defined(CONFIG_SOFT_I2C)
static int init_func_i2c (void)
{
	puts ("I2C:   ");
	i2c_init (CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);
	puts ("ready\n");
	return (0);
}
#endif

#if defined(CONFIG_CMD_PCI) || defined (CONFIG_PCI)
#include <pci.h>
static int arm_pci_init(void)
{
	pci_init();
	return 0;
}
#endif /* CONFIG_CMD_PCI || CONFIG_PCI */

/*
 * Breathe some life into the board...
 *
 * Initialize a serial port as console, and carry out some hardware
 * tests.
 *
 * The first part of initialization is running from Flash memory;
 * its main purpose is to initialize the RAM so that we
 * can relocate the monitor code to RAM.
 */

/*
 * All attempts to come up with a "common" initialization sequence
 * that works for all boards and architectures failed: some of the
 * requirements are just _too_ different. To get rid of the resulting
 * mess of board dependent #ifdef'ed code we now make the whole
 * initialization sequence configurable to the user.
 *
 * The requirements for any new initalization function is simple: it
 * receives a pointer to the "global data" structure as it's only
 * argument, and returns an integer return code, where 0 means
 * "continue" and != 0 means "fatal error, hang the system".
 */
typedef int (init_fnc_t) (void);

int print_cpuinfo (void);

#if 0
init_fnc_t *init_sequence[] = {
#if defined(CONFIG_ARCH_CPU_INIT)
	arch_cpu_init,		/* basic arch cpu dependent setup */
#endif
	board_init,		/* basic board dependent setup */
#if defined(CONFIG_USE_IRQ)
	interrupt_init,		/* set up exceptions */
#endif
	timer_init,		/* initialize timer */	
#ifdef CONFIG_FSL_ESDHC
	get_clocks,
#endif
	env_init,		/* initialize environment */
	init_baudrate,		/* initialze baudrate settings */
	serial_init,		/* serial communications setup */
	console_init_f,		/* stage 1 init of console */
	display_banner,		/* say that we are here */
#if defined(CONFIG_DISPLAY_CPUINFO)
	print_cpuinfo,		/* display cpu info (and speed) */
#endif
#if defined(CONFIG_DISPLAY_BOARDINFO)
	checkboard,		/* display board info */
#endif
#if defined(CONFIG_HARD_I2C) || defined(CONFIG_SOFT_I2C)
	init_func_i2c,
#endif
	dram_init,		/* configure available RAM banks */
#if defined(CONFIG_CMD_PCI) || defined (CONFIG_PCI)
	arm_pci_init,
#endif
	display_dram_config,
	NULL,
};
#endif

init_fnc_t *init_sequence[] = {
	cpu_init,		/* basic cpu dependent setup */
	dram_init,              /* configure available RAM banks */ /*  change the original init order */
	board_init,		/* basic board dependent setup */
	interrupt_init,		/* set up exceptions */
	env_init,		/* initialize environment */
	init_baudrate,		/* initialze baudrate settings */
	serial_init,		/* serial communications setup */
	console_init_f,		/* stage 1 init of console */
	display_banner,		/* say that we are here */
#if defined(CONFIG_DISPLAY_CPUINFO)
	print_cpuinfo,		/* display cpu info (and speed) */
#endif
#if defined(CONFIG_DISPLAY_BOARDINFO)
	checkboard,		/* display board info */
#endif
	display_dram_config,
	NULL,
};


#ifdef CFG_UBOOT_PROFILING
  /* Jau add to profile boot time */
  /* boot_time is calculated form start_armboot to kernel jump */
  unsigned int boot_time = 0;
#endif

void start_armboot (void)
{
	init_fnc_t **init_fnc_ptr;
	char *s;
	ulong size;
#if defined(CONFIG_VFD) || defined(CONFIG_LCD)
	unsigned long addr;
#endif

#ifdef CFG_UBOOT_PROFILING
    unsigned int time_start_armboot = get_timer(0);
    unsigned int time_init_sequence;
    unsigned int time_misc_init;
    unsigned int time_env;
    unsigned int time_nand;
    unsigned int time_bat;
    unsigned int time_show_logo;
    unsigned int time_backlight;
    unsigned int time_sw_env;
#endif

    #ifdef CFG_UBOOT_PROFILING
    boot_time = get_timer(0);
    #endif

	/* Pointer is writable since we allocated a register for it */
	gd = (gd_t*)(_armboot_start - CONFIG_SYS_MALLOC_LEN - sizeof(gd_t));
	/* compiler optimization barrier needed for GCC >= 3.4 */
	__asm__ __volatile__("": : :"memory");

	memset ((void*)gd, 0, sizeof (gd_t));
	gd->bd = (bd_t*)((char*)gd - sizeof(bd_t));
	memset (gd->bd, 0, sizeof (bd_t));	

	monitor_flash_len = _bss_start - _armboot_start;

#ifdef CFG_UBOOT_PROFILING
      time_init_sequence = get_timer(0);
#endif
	for (init_fnc_ptr = init_sequence; *init_fnc_ptr; ++init_fnc_ptr) 
	{
		if ((*init_fnc_ptr)() != 0) {
			hang ();
		}
	}	
	/* armboot_start is defined in the board-specific linker script */
	mem_malloc_init (_armboot_start - CONFIG_SYS_MALLOC_LEN, CONFIG_SYS_MALLOC_LEN);
#ifdef CFG_UBOOT_PROFILING
      printf("[PROFILE] ------- init_sequence takes %d ms -------- \n", get_timer(time_init_sequence));
#endif

  

#ifndef CONFIG_SYS_NO_FLASH
	/* configure available FLASH banks */
	display_flash_config (flash_init ());
#endif /* CONFIG_SYS_NO_FLASH */

#ifdef CONFIG_VFD
#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif
	/*
	 * reserve memory for VFD display (always full pages)
	 */
	/* bss_end is defined in the board-specific linker script */
	addr = (_bss_end + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);
	size = vfd_setmem (addr);
	gd->fb_base = addr;
#endif /* CONFIG_VFD */

#ifdef CONFIG_LCD
  #ifndef PAGE_SIZE
    #define PAGE_SIZE 4096
  #endif
	/*
	 * reserve memory for LCD display (always full pages)
	 */
	/* bss_end is defined in the board-specific linker script */
	addr = (_bss_end + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);
	size = lcd_setmem (addr);
	gd->fb_base = addr;
#endif /* CONFIG_LCD */

#ifdef CONFIG_HAS_DATAFLASH
	AT91F_DataflashInit();
	dataflash_print_info();
#endif

#ifdef CFG_UBOOT_PROFILING
    time_env = get_timer(0);
#endif
	/* initialize environment */
	env_relocate ();
#ifdef CFG_UBOOT_PROFILING
      printf("[PROFILE] ------- env relocate takes %d ms -------- \n", get_timer(time_env));
#endif


#ifdef CONFIG_VFD
	/* must do this after the framebuffer is allocated */
	drv_vfd_init();
#endif /* CONFIG_VFD */

	/* IP Address */
	//gd->bd->bi_ip_addr = getenv_IPaddr ("ipaddr");
	stdio_init ();	/* get the devices list going. */


#ifdef CONFIG_CMC_PU2
	load_sernum_ethaddr ();
#endif /* CONFIG_CMC_PU2 */

	jumptable_init ();

	//console_init_r ();	/* fully init console as a device */ //marked by hong-rong

	
#if defined(CONFIG_CMD_NAND)
    #ifdef CFG_UBOOT_PROFILING
      time_nand= get_timer(0);
    #endif
	puts ("NAND:  ");	
	nand_init();		/* go init the NAND */
	  #ifdef CFG_UBOOT_PROFILING
      printf("[PROFILE] ------- nand init takes %d ms -------- \n", get_timer(time_nand));
    #endif	
#endif
	
#if defined(CONFIG_ARCH_MISC_INIT)
	/* miscellaneous arch dependent initialisations */
	arch_misc_init ();
#endif

#ifdef CFG_UBOOT_PROFILING
      time_misc_init= get_timer(0);
#endif
#if defined(CONFIG_MISC_INIT_R)
	/* miscellaneous platform dependent initialisations */
	misc_init_r ();
#endif
#ifdef CFG_UBOOT_PROFILING
      printf("[PROFILE] ------- misc_init takes %d ms -------- \n", get_timer(time_misc_init));
#endif
  
	
#ifdef CONFIG_GENERIC_MMC
	puts ("MMC:   ");
	mmc_initialize (gd->bd);
#endif

#ifdef CFG_POWER_CHARGING
    #ifdef CFG_UBOOT_PROFILING
      time_bat= get_timer(0);
    #endif
  #if defined(MT6516)	
	  mt6516_bat_init();
	#else
	  mt65xx_bat_init();
	#endif    
	  #ifdef CFG_UBOOT_PROFILING
      printf("[PROFILE] ------- battery init takes %d ms -------- \n", get_timer(time_bat));
    #endif
#else
    /* NOTE: if define CFG_POWER_CHARGING, will rtc_boot_check() in mt65xx_bat_init() */
    rtc_boot_check(false);
#endif

#ifdef CFG_UBOOT_PROFILING
      time_show_logo= get_timer(0);
#endif
	// Some driver refresh RAM data to LCM after sleeping out.
	// LCM must sleep out before backlight on. Or Users may see the mess data in LCM in a instance.
	mt65xx_disp_power(1);
        if(g_boot_mode != ALARM_BOOT)
        {
	mt65xx_disp_show_boot_logo();
        }
#ifdef CFG_UBOOT_PROFILING
      printf("[PROFILE] ------- show logo takes %d ms -------- \n", get_timer(time_show_logo));
#endif

#ifdef CFG_UBOOT_PROFILING
      time_show_logo= get_timer(0);
#endif
	
#ifdef CFG_UBOOT_PROFILING
      time_backlight= get_timer(0);
#endif	
	mt65xx_backlight_on();
#ifdef CFG_UBOOT_PROFILING
      printf("[PROFILE] ------- backlight takes %d ms -------- \n", get_timer(time_backlight));
#endif


#ifdef CFG_UBOOT_PROFILING
      time_sw_env= get_timer(0);
#endif		
	//*****************
	//* prepare mt65xx sw enviroment	
	#if defined(MT6516)
	  mt6516_sw_env();
	#else
	  mt65xx_sw_env();
	#endif
#ifdef CFG_UBOOT_PROFILING
      printf("[PROFILE] ------- sw_env takes %d ms -------- \n", get_timer(time_sw_env));
#endif	
	
#ifdef CFG_UBOOT_PROFILING
    printf("[PROFILE] ------- start_armboot takes %d ms -------- \n", get_timer(time_start_armboot));
#endif
    
    /* main_loop() can return to retry autoboot, if so just run it again */
	for (;;) {
		main_loop ();
	}
	
	/* NOTREACHED - no way out of command loop except booting */
}

void hang (void)
{
	puts ("### ERROR ### Please RESET the board ###\n");
	for (;;);
}
