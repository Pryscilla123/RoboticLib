#include "src/libadr.h"
#include "/var/lib/gems/2.5.0/gems/ceedling-0.31.1/vendor/unity/src/unity.h"








void setUp(void)

{

}



void tearDown(void)

{

}



void test_libadr_openDevice(void)

{



    do {if ((((openDevice(0x44E07000, 0x1000))) != 

   ((void *)0)

   )) {} else {UnityFail( (((" Expected Non-NULL"))), (UNITY_UINT)((UNITY_UINT)((UNITY_UINT)(18))));}} while(0);

    do {if ((((openDevice(0x4804C000, 0x1000))) != 

   ((void *)0)

   )) {} else {UnityFail( (((" Expected Non-NULL"))), (UNITY_UINT)((UNITY_UINT)((UNITY_UINT)(19))));}} while(0);

    do {if ((((openDevice(0x481AC000, 0x1000))) != 

   ((void *)0)

   )) {} else {UnityFail( (((" Expected Non-NULL"))), (UNITY_UINT)((UNITY_UINT)((UNITY_UINT)(20))));}} while(0);

    do {if ((((openDevice(0x481AE000, 0x1000))) != 

   ((void *)0)

   )) {} else {UnityFail( (((" Expected Non-NULL"))), (UNITY_UINT)((UNITY_UINT)((UNITY_UINT)(21))));}} while(0);



    do {if ((((openDevice(0x44E0D000, 0x2000))) != 

   ((void *)0)

   )) {} else {UnityFail( (((" Expected Non-NULL"))), (UNITY_UINT)((UNITY_UINT)((UNITY_UINT)(23))));}} while(0);

}



void test_libadr_digitalRead(void)

{



    int *pinconf = openDevice(0x4804C000, 0x1000);



    pinMode(pinconf, 28, 0);

    digitalWrite(pinconf, 28, 0);





    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((digitalRead(pinconf, 28))), (

   ((void *)0)

   ), (UNITY_UINT)(35), UNITY_DISPLAY_STYLE_INT);





    digitalWrite(pinconf, 28, 1);





    UnityAssertEqualNumber((UNITY_INT)((1)), (UNITY_INT)((digitalRead(pinconf, 28))), (

   ((void *)0)

   ), (UNITY_UINT)(41), UNITY_DISPLAY_STYLE_INT);







}



void test_libadr_getStepRegisterValue(void)

{



    Step config_step;



    config_step.range_check = disable_o_r_c;

    config_step.fifo_select = FIFO0;

    config_step.diff_cntrl = single_ended;

    config_step.sel_rfm_swc_1_0 = VSSA_RFM;

    config_step.sel_inp_swc_3_0 = ch1_inp;

    config_step.sel_inm_swc_3_0 = ADCREFM_INM;

    config_step.sel_rfp_swc_2_0 = VDDA_ADC;

    config_step.wpnsw_swc = off_wpnsw;

    config_step.ypnsw_swc = off_ypnsw;

    config_step.xnpsw_swc = off_xnpsw;

    config_step.ynnsw_swc = off_ynnsw;

    config_step.yppsw_swc = off_yppsw;

    config_step.xnnsw_swc = off_xnnsw;

    config_step.xppsw_swc = off_xppsw;

    config_step.averaging = none;

    config_step.mode = SW_ONE_SHOT;





    UnityAssertEqualNumber((UNITY_INT)((0x40000)), (UNITY_INT)((getStepRegisterValue(config_step))), (

   ((void *)0)

   ), (UNITY_UINT)(70), UNITY_DISPLAY_STYLE_UINT);

}



void test_libadr_getIdleRegisterValue(void)

{



    Idle config_idle;



    config_idle.diff_cntrl = single_ended;

    config_idle.sel_rfm_swc_1_0 = VSSA_RFM;

    config_idle.sel_inp_swc_3_0 = ch1_inp;

    config_idle.sel_inm_swc_3_0 = ADCREFM_INM;

    config_idle.sel_rfp_swc_2_0 = VDDA_ADC;

    config_idle.wpnsw_swc = off_wpnsw;

    config_idle.ypnsw_swc = off_ypnsw;

    config_idle.xnpsw_swc = off_xnpsw;

    config_idle.ynnsw_swc = off_ynnsw;

    config_idle.yppsw_swc = off_yppsw;

    config_idle.xnnsw_swc = off_xnnsw;

    config_idle.xppsw_swc = off_xppsw;





    UnityAssertEqualNumber((UNITY_INT)((0x40000)), (UNITY_INT)((getIdleRegisterValue(config_idle))), (

   ((void *)0)

   ), (UNITY_UINT)(92), UNITY_DISPLAY_STYLE_UINT);

}



void test_libadr_getDelayRegisterValue(void)

{



    Delay config_delay;



    config_delay.sample_delay = 0x0;

    config_delay.open_delay = 0x0;





    UnityAssertEqualNumber((UNITY_INT)((0x0)), (UNITY_INT)((getDelayRegisterValue(config_delay))), (

   ((void *)0)

   ), (UNITY_UINT)(104), UNITY_DISPLAY_STYLE_UINT);

}



void test_libadr_readAnalog(void)

{



    Step config_step[1];

    Idle config_idle;

    Delay config_delay[1];



    int *analogconfig = openDevice(0x44E0D000, 0x2000);



    config_step[0].range_check = disable_o_r_c;

    config_step[0].fifo_select = FIFO0;

    config_step[0].diff_cntrl = single_ended;

    config_step[0].sel_rfm_swc_1_0 = VSSA_RFM;

    config_step[0].sel_inp_swc_3_0 = ch1_inp;

    config_step[0].sel_inm_swc_3_0 = ADCREFM_INM;

    config_step[0].sel_rfp_swc_2_0 = VDDA_ADC;

    config_step[0].wpnsw_swc = off_wpnsw;

    config_step[0].ypnsw_swc = off_ypnsw;

    config_step[0].xnpsw_swc = off_xnpsw;

    config_step[0].ynnsw_swc = off_ynnsw;

    config_step[0].yppsw_swc = off_yppsw;

    config_step[0].xnnsw_swc = off_xnnsw;

    config_step[0].xppsw_swc = off_xppsw;

    config_step[0].averaging = none;

    config_step[0].mode = SW_ONE_SHOT;



    config_idle.diff_cntrl = single_ended;

    config_idle.sel_rfm_swc_1_0 = VSSA_RFM;

    config_idle.sel_inp_swc_3_0 = ch1_inp;

    config_idle.sel_inm_swc_3_0 = ADCREFM_INM;

    config_idle.sel_rfp_swc_2_0 = VDDA_ADC;

    config_idle.wpnsw_swc = off_wpnsw;

    config_idle.ypnsw_swc = off_ypnsw;

    config_idle.xnpsw_swc = off_xnpsw;

    config_idle.ynnsw_swc = off_ynnsw;

    config_idle.yppsw_swc = off_yppsw;

    config_idle.xnnsw_swc = off_xnnsw;

    config_idle.xppsw_swc = off_xppsw;



    config_delay[0].sample_delay = 0x0;

    config_delay[0].open_delay = 0x0;



    init(analogconfig, config_delay, config_step, config_idle, 1);



    stepEnable(analogconfig, 1);



    while(analogconfig[0x54/4] != 0);





    UnityAssertGreaterOrLessOrEqualNumber((UNITY_INT) ((4096)), (UNITY_INT) ((readAnalog(analogconfig, 0x100))), UNITY_SMALLER_OR_EQUAL, (

   ((void *)0)

   ), (UNITY_UINT)(156), UNITY_DISPLAY_STYLE_UINT);

}
