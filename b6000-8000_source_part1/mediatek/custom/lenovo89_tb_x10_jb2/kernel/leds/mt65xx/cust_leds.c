#include <cust_leds.h>
#include <mach/mt_pwm.h>
#include <mach/mt_gpio.h>
#include <linux/kernel.h>
#include <mach/pmic_mt6329_hw_bank1.h> 
#include <mach/pmic_mt6329_sw_bank1.h> 
#include <mach/pmic_mt6329_hw.h>
#include <mach/pmic_mt6329_sw.h>
#include <mach/upmu_common_sw.h>
#include <mach/upmu_hw.h>

#define CUST_BLS_PWM //need to be compatible with dct GPIO setting

extern int mtkfb_set_backlight_level(unsigned int level);
extern int mtkfb_set_backlight_pwm(int div);
extern int disp_bls_set_backlight(unsigned int level);
/*
#define ERROR_BL_LEVEL 0xFFFFFFFF

unsigned int brightness_mapping(unsigned int level)
{  
	return ERROR_BL_LEVEL;
}
*/
unsigned int brightness_mapping(unsigned int level)
{
#if !defined(CUST_BLS_PWM)

    return ((level << 6) >> 8);
#else
    unsigned int mapped_level;
    
    mapped_level = level;
       
	return mapped_level;
#endif    
}

unsigned int Cust_SetBacklight(int level, int div)
{
#if !defined(CUST_BLS_PWM)
    mtkfb_set_backlight_pwm(div);
    mtkfb_set_backlight_level(brightness_mapping(level));
#else
    disp_bls_set_backlight(brightness_mapping(level));
#endif
    return 0;
}


static struct cust_mt65xx_led cust_led_list[MT65XX_LED_TYPE_TOTAL] = {
	{"red",               MT65XX_LED_MODE_PMIC, MT65XX_LED_PMIC_NLED_ISINK0,{0}},   // have no hw, add as blue for android app need
	{"green",             MT65XX_LED_MODE_PMIC, MT65XX_LED_PMIC_NLED_ISINK1,{0}},
	{"blue",              MT65XX_LED_MODE_PMIC, MT65XX_LED_PMIC_NLED_ISINK0,{0}},
	{"jogball-backlight", MT65XX_LED_MODE_NONE, -1,{0}},
	{"keyboard-backlight",MT65XX_LED_MODE_NONE, -1,{0}},
	{"button-backlight",  MT65XX_LED_MODE_PMIC, MT65XX_LED_PMIC_BUTTON,{0}},
#if !defined(CUST_BLS_PWM)
	{"lcd-backlight",     MT65XX_LED_MODE_PWM, PWM1,{1,3,1,1,0}},
#else
	{"lcd-backlight",     MT65XX_LED_MODE_CUST_BLS_PWM, (int)disp_bls_set_backlight,{0,0x47,0,0,0}},
#endif	
};

struct cust_mt65xx_led *get_cust_led_list(void)
{
	return cust_led_list;
}

