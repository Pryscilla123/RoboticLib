#include "libadr.h"


int *open_device(int addr, int lenght){
   int fd = open("/dev/mem", O_RDWR | O_SYNC);
   int * pinconf = (int *) malloc(sizeof(int));
   if(pinconf == NULL){
      exit(1024);
   }
   pinconf = (int *) mmap(NULL, lenght, PROT_READ | PROT_WRITE, MAP_SHARED, fd, addr);

   return pinconf;
}



void set_pin_mode(int *pinconf, int pin, int mode){
   if(mode){
      pinconf[OE_ADDR/4] |= 1 << pin; // input
   }else{
      pinconf[OE_ADDR/4] &= ~(1 << pin); // output
   }
}


void digital_write(int *pinconf, int pin, int state){

   if(state){
      pinconf[GPIO_DATAOUT/4] |= (1 << pin);
   }
   else{
      pinconf[GPIO_DATAOUT/4] &= ~(1 << pin);
   }
}


int digital_read(int *pinconf, int pin){
   return (pinconf[GPIO_DATAIN/4] >> pin) & 0x1;
}


void close_device(int **pinconf){

   int *p = *pinconf;

   free(p);
}


void step_enable(int *pinconf, int num){
   u_int32_t reg_config = 0;

   for(int i = 0; i < num; i++){
        reg_config |= 1 << (i+1);
   }

   pinconf[ADC_TSC_STEPENABLE/4] = reg_config;
}


unsigned int get_step_register_value(step_config config){
   return (config.range_check << 27) | (config.fifo_select << 26) |
          (config.diff_cntrl << 25) | (config.sel_rfm_swc_1_0 << 23) |
          (config.sel_inp_swc_3_0 << 19) | (config.sel_inm_swc_3_0 << 15) |
          (config.sel_rfp_swc_2_0 << 12) | (config.wpnsw_swc << 11) |
          (config.ypnsw_swc << 10) | (config.xnpsw_swc << 9) |
          (config.ynnsw_swc << 8) | (config.yppsw_swc << 7) |
          (config.xnnsw_swc << 6) | (config.xppsw_swc << 5) |
          (config.averaging << 2) | (config.mode);
}


unsigned int get_idle_register_value(idle_config config){
   return (config.diff_cntrl << 25) | (config.sel_rfm_swc_1_0 << 23) |
          (config.sel_inp_swc_3_0 << 19) | (config.sel_inm_swc_3_0 << 15) |
          (config.sel_rfp_swc_2_0 << 12) | (config.wpnsw_swc << 11) |
          (config.ypnsw_swc << 10) | (config.xnpsw_swc << 9) |
          (config.ynnsw_swc << 8) | (config.yppsw_swc << 7) |
          (config.xnnsw_swc << 6) | (config.xppsw_swc << 5);
}


unsigned int get_delay_register_value(delay_config config){
   return (config.sample_delay << 24) | (config.open_delay);
}


void init_adc_config(int *pinconf, delay_config delays[], step_config steps[], idle_config idle, int num){
   pinconf[ADC_TSC_IDLECONFIG/4] = get_idle_register_value(idle);

   /**
    * @brief Os passos de configuração e delay são alternados entre sí
    * com um intervalo de 0x4 entre cada um e um intervalo de 0x8 entre cada passo 
    * por exemplo passo 1 e 2 e o mesmo com o delay.
    */

   for(int i = 0; i < num; i++){
      pinconf[(STEP_CONFIG1 + (0x8 * i))/4] = get_step_register_value(steps[i]);
      pinconf[(STEP_DELAY1 + (0x8 * i))/4] = get_delay_register_value(delays[i]);
   }
}

int read_analog(int *pinconf, int fifo_addr){
   return pinconf[fifo_addr/4] & 0xFFF;
}
