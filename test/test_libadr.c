#ifdef TEST

#include "unity.h"

#include "libadr.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_libadr_open_device(void)
{
    //teste do digital not null
    TEST_ASSERT_NOT_NULL(open_device(GPIO0_ADDR, 0x1000));
    TEST_ASSERT_NOT_NULL(open_device(GPIO1_ADDR, 0x1000));
    TEST_ASSERT_NOT_NULL(open_device(GPIO2_ADDR, 0x1000));
    TEST_ASSERT_NOT_NULL(open_device(GPIO3_ADDR, 0x1000));
    //teste do analogico não null
    TEST_ASSERT_NOT_NULL(open_device(ADC_TSC, 0x2000));
}

void test_libadr_digital_read(void)
{
    //prep
    int *pinconf = open_device(GPIO1_ADDR, 0x1000);

    set_pin_mode(pinconf, 28, 0);
    set_pin_mode(pinconf, 16, 1);

    digital_write(pinconf, 28, 0);

    //pinMode(pinconf, 28, 1);
    TEST_ASSERT_EQUAL_INT(0, digital_read(pinconf, 16));

    digital_write(pinconf, 28, 1);

    TEST_ASSERT_EQUAL_INT(1, digital_read(pinconf, 16));

    //closeDevice
    //close_device(&pinconf);
}

void test_libadr_get_step_register_value(void)
{
    //prep
    step_config config_step;

    config_step.range_check = DISABLE_O_R_C;
    config_step.fifo_select = FIFO0;
    config_step.diff_cntrl = SINGLE_ENDED;
    config_step.sel_rfm_swc_1_0 = VSSA_RFM;
    config_step.sel_inp_swc_3_0 = CH1_INP;
    config_step.sel_inm_swc_3_0 = ADCREFM_INM;
    config_step.sel_rfp_swc_2_0 = VDDA_ADC;
    config_step.wpnsw_swc = OFF_WPNSW;
    config_step.ypnsw_swc = OFF_YPNSW;
    config_step.xnpsw_swc = OFF_XNPSW;
    config_step.ynnsw_swc = OFF_YNNSW;
    config_step.yppsw_swc = OFF_YPPSW;
    config_step.xnnsw_swc = OFF_XNNSW;
    config_step.xppsw_swc = OFF_XPPSW;
    config_step.averaging = NONE;
    config_step.mode = SW_ONE_SHOT;

    //test
    TEST_ASSERT_EQUAL_UINT(0x40000, get_step_register_value(config_step));
}

void test_libadr_get_idle_register_value(void)
{
    //prep
    idle_config config_idle;

    config_idle.diff_cntrl = SINGLE_ENDED;
    config_idle.sel_rfm_swc_1_0 = VSSA_RFM;
    config_idle.sel_inp_swc_3_0 = CH1_INP;
    config_idle.sel_inm_swc_3_0 = ADCREFM_INM;
    config_idle.sel_rfp_swc_2_0 = VDDA_ADC;
    config_idle.wpnsw_swc = OFF_WPNSW;
    config_idle.ypnsw_swc = OFF_YPNSW;
    config_idle.xnpsw_swc = OFF_XNPSW;
    config_idle.ynnsw_swc = OFF_YNNSW;
    config_idle.yppsw_swc = OFF_YPPSW;
    config_idle.xnnsw_swc = OFF_XNNSW;
    config_idle.xppsw_swc = OFF_XPPSW;

    //test
    TEST_ASSERT_EQUAL_UINT(0x40000, get_idle_register_value(config_idle));
}

void test_libadr_get_delay_register_value(void)
{
    //prep
    delay_config config_delay;

    config_delay.sample_delay = 0x0;
    config_delay.open_delay = 0x0;

    //test
    TEST_ASSERT_EQUAL_UINT(0x0, get_delay_register_value(config_delay));
}

void test_libadr_read_analog(void)
{
    //prep
    step_config config_step[1];
    idle_config config_idle;
    delay_config config_delay[1];

    int *analogconfig = open_device(ADC_TSC, 0x2000);

    config_step[0].range_check = DISABLE_O_R_C;
    config_step[0].fifo_select = FIFO0;
    config_step[0].diff_cntrl = SINGLE_ENDED;
    config_step[0].sel_rfm_swc_1_0 = VSSA_RFM;
    config_step[0].sel_inp_swc_3_0 = CH1_INP;
    config_step[0].sel_inm_swc_3_0 = ADCREFM_INM;
    config_step[0].sel_rfp_swc_2_0 = VDDA_ADC;
    config_step[0].wpnsw_swc = OFF_WPNSW;
    config_step[0].ypnsw_swc = OFF_YPNSW;
    config_step[0].xnpsw_swc = OFF_XNPSW;
    config_step[0].ynnsw_swc = OFF_YNNSW;
    config_step[0].yppsw_swc = OFF_YPPSW;
    config_step[0].xnnsw_swc = OFF_XNNSW;
    config_step[0].xppsw_swc = OFF_XPPSW;
    config_step[0].averaging = NONE;
    config_step[0].mode = SW_ONE_SHOT;

    config_idle.diff_cntrl = SINGLE_ENDED;
    config_idle.sel_rfm_swc_1_0 = VSSA_RFM;
    config_idle.sel_inp_swc_3_0 = CH1_INP;
    config_idle.sel_inm_swc_3_0 = ADCREFM_INM;
    config_idle.sel_rfp_swc_2_0 = VDDA_ADC;
    config_idle.wpnsw_swc = OFF_WPNSW;
    config_idle.ypnsw_swc = OFF_YPNSW;
    config_idle.xnpsw_swc = OFF_XNPSW;
    config_idle.ynnsw_swc = OFF_YNNSW;
    config_idle.yppsw_swc = OFF_YPPSW;
    config_idle.xnnsw_swc = OFF_XNNSW;
    config_idle.xppsw_swc = OFF_XPPSW;

    config_delay[0].sample_delay = 0x0;
    config_delay[0].open_delay = 0x0;

    init_adc_config(analogconfig, config_delay, config_step, config_idle, 1);

    step_enable(analogconfig, 1);

    while(analogconfig[ADC_TSC_STEPENABLE/4] != 0);

    //test
    TEST_ASSERT_LESS_OR_EQUAL_UINT(4096, read_analog(analogconfig, 0x100));
    //close_device(&analogconfig);
}

#endif // TEST
