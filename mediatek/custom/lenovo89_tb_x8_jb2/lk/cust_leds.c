//#include <platform/cust_leds.h>
#include <cust_leds.h>
#include <platform/mt_pwm.h>
#include <platform/mt_gpio.h>

#define CUST_BLS_PWM //need to be compatible with dct GPIO setting

extern int DISP_SetBacklight(int level);
extern int disp_bls_set_backlight(unsigned int level);

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

