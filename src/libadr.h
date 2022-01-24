#ifndef LIB_A_D_R
#define LIB_A_D_R

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>


#define OE_ADDR 0x134
#define GPIO_DATAOUT 0x13C
#define GPIO_DATAIN 0x138
#define GPIO0_ADDR 0x44E07000
#define GPIO1_ADDR 0x4804C000
#define GPIO2_ADDR 0x481AC000
#define GPIO3_ADDR 0x481AE000

#define ADC_TSC 0x44E0D000
#define ADC_TSC_STEPENABLE 0x54
#define ADC_TSC_IDLECONFIG 0x58

enum STEPCONFIG {STEPCONFIG1 = 0x64, STEPCONFIG2 = 0x6C, STEPCONFIG3 = 0x74, STEPCONFIG4 = 0x7C, STEPCONFIG5 = 0x84,
                 STEPCONFIG6 = 0x8C, STEPCONFIG7 = 0x94, STEPCONFIG8 = 0x9C, STEPCONFIG9 = 0xA4, STEPCONFIG10 = 0xAC,
                 STEPCONFIG11 = 0xB4, STEPCONFIG12 = 0xBC, STEPCONFIG13 = 0xC4, STEPCONFIG14 = 0xCC, STEPCONFIG15 = 0xD4,
                 STEPCONFIG16 = 0xDC};

enum STEPDELAY {STEPDELAY1 = 0x68, STEPDELAY2 = 0x70, STEPDELAY3 = 0x78, STEPDELAY4 = 0x80, STEPDELAY5 = 0x88,
                 STEPDELAY6 = 0x90, STEPDELAY7 = 0x98, STEPDELAY8 = 0xA0, STEPDELAY9 = 0xA8, STEPDELAY10 = 0xB0,
                 STEPDELAY11 = 0xB8, STEPDELAY12 = 0xC0, STEPDELAY13 = 0xC8, STEPDELAY14 = 0xD0, STEPDELAY15 = 0xD8,
                 STEPDELAY16 = 0xE0};

enum RANGE_CHECK {disable_o_r_c = 0x0, compare_adc_r_c_r = 0x1};
enum FIFO_SELECT {FIFO0 = 0x0, FIFO1 = 0x1};
enum DIFF_CNTRL {single_ended = 0x0, diferencial_pair_enable = 0x1};
enum SEL_RFM_SWC_1_0 {VSSA_RFM = 0x0, XNUR_RFM = 0x1, YNLR_RFM = 0x2, VREFN_RFM = 0x3};
enum SEL_INP_SWC_3_0 {ch1_inp = 0x0, ch2_inp = 0x1, ch3_inp = 0x2, ch4_inp = 0x3, ch5_inp = 0x4,
                      ch6_inp = 0x5, ch7_inp = 0x6, ch8_inp = 0x7, VREFN_INP = 0x8};
enum SEL_INM_SWC_3_0 {ch1_inm = 0x0, ch2_inm = 0x1, ch3_inm = 0x2, ch4_inm = 0x3, ch5_inm = 0x4,
                      ch6_inm = 0x5, ch7_inm = 0x6, ch8_inm = 0x7, ADCREFM_INM = 0x8};
enum SEL_RFP_SWC_2_0 {VDDA_ADC = 0x0, XPUL_RFP = 0x1, YPLL_RFP = 0x2, VREFP = 0x3, reserved = 0x4};
enum WPNSW_SWC {off_wpnsw = 0x0, on_wpnsw = 0x1};
enum YPNSW_SWC {off_ypnsw = 0x0, on_ypnsw = 0x1};
enum XNPSW_SWC {off_xnpsw = 0x0, on_xnpsw = 0x1};
enum YNNSW_SWC {off_ynnsw = 0x0, on_ynnsw = 0x1};
enum YPPSW_SWC {off_yppsw = 0x0, on_yppsw = 0x1};
enum XNNSW_SWC {off_xnnsw = 0x0, on_xnnsw = 0x1};
enum XPPSW_SWC {off_xppsw = 0x0, on_xppsw = 0x1};
enum AVERAGING {none = 0x0, samples_2 = 0x1, samples_4 = 0x2, samples_8 = 0x3, samples_16 = 0x4};
enum MODE {SW_ONE_SHOT = 0x0, SW_CONTINUOUS = 0x1, HW_ONE_SHOT = 0x2, HW_CONTINUOUS = 0x3};

typedef struct _stepconfig {
   int range_check;
   int fifo_select;
   int diff_cntrl;
   int sel_rfm_swc_1_0;
   int sel_inp_swc_3_0;
   int sel_inm_swc_3_0;
   int sel_rfp_swc_2_0;
   int wpnsw_swc;
   int ypnsw_swc;
   int xnpsw_swc;
   int ynnsw_swc;
   int yppsw_swc;
   int xnnsw_swc;
   int xppsw_swc;
   int averaging;
   int mode;
}Step;

typedef struct _idleconfig {
   int diff_cntrl;
   int sel_rfm_swc_1_0;
   int sel_inp_swc_3_0;
   int sel_inm_swc_3_0;
   int sel_rfp_swc_2_0;
   int wpnsw_swc;
   int ypnsw_swc;
   int xnpsw_swc;
   int ynnsw_swc;
   int yppsw_swc;
   int xnnsw_swc;
   int xppsw_swc;
} Idle;

typedef struct _stepdelay{
   int sample_delay;
   int open_delay;
}Delay;

int *openDevice(int addr, int lenght);
void pinMode(int *pinconf, int pin, int mode);
void digitalWrite(int *pinconf, int pin, int state);
int digitalRead(int *pinconf, int pin);
void closeDevice(int *pinconf);

void stepEnable(int *pinconf, int num);

unsigned int getStepRegisterValue(Step config);
unsigned int getIdleRegisterValue(Idle config);
unsigned int getDelayRegisterValue(Delay config);

void init(int *pinconf, Delay delays[], Step steps[], Idle idle, int num);

int readAnalog(int *pinconf, int fifo_addr);

#endif
