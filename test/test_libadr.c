#ifdef TEST

#include "unity.h"

#include "libadr.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_libadr_openDevice(void)
{
    //teste do digital not null
    TEST_ASSERT_NOT_NULL(openDevice(GPIO0_ADDR, 0x1000));
    TEST_ASSERT_NOT_NULL(openDevice(GPIO1_ADDR, 0x1000));
    TEST_ASSERT_NOT_NULL(openDevice(GPIO2_ADDR, 0x1000));
    TEST_ASSERT_NOT_NULL(openDevice(GPIO3_ADDR, 0x1000));
    //teste do analogico não null
    TEST_ASSERT_NOT_NULL(openDevice(ADC_TSC, 0x2000));
}

void test_libadr_digitalRead(void)
{
    //prep
    int *pinconf = openDevice(GPIO1_ADDR, 0x1000);

    pinMode(pinconf, 28, 0);
    digitalWrite(pinconf, 28, 0);

    //pinMode(pinconf, 28, 1);
    TEST_ASSERT_EQUAL_INT(0, digitalRead(pinconf, 28));

    //pinMode(pinconf, 28, 0);
    digitalWrite(pinconf, 28, 1);

    //pinMode(pinconf, 28, 1);
    TEST_ASSERT_EQUAL_INT(1, digitalRead(pinconf, 28));

    //closeDevice
    //closeDevice(pinconf);
}

void test_libadr_getStepRegisterValue(void)
{
    //prep
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

    //test
    TEST_ASSERT_EQUAL_UINT(0x40000, getStepRegisterValue(config_step));
}

void test_libadr_getIdleRegisterValue(void)
{
    //prep
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

    //test
    TEST_ASSERT_EQUAL_UINT(0x40000, getIdleRegisterValue(config_idle));
}

void test_libadr_getDelayRegisterValue(void)
{
    //prep
    Delay config_delay;

    config_delay.sample_delay = 0x0;
    config_delay.open_delay = 0x0;

    //test
    TEST_ASSERT_EQUAL_UINT(0x0, getDelayRegisterValue(config_delay));
}

void test_libadr_readAnalog(void)
{
    //prep
    Step config_step[1];
    Idle config_idle;
    Delay config_delay[1];

    int *analogconfig = openDevice(ADC_TSC, 0x2000);

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

    while(analogconfig[ADC_TSC_STEPENABLE/4] != 0);

    //test
    TEST_ASSERT_LESS_OR_EQUAL_UINT(4096, readAnalog(analogconfig, 0x100));
}

#endif // TEST
