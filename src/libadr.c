#include "libadr.h"

//open_device -> abre
//close_device -> fecha

//digitalWrite
//digitalRead -> retorna bool
//pinMode

int *openDevice(int addr, int lenght){
   int fd = open("/dev/mem", O_RDWR | O_SYNC);
   int * pinconf = (int *) malloc(sizeof(int));
   if(pinconf == NULL){
      exit(1024);
   }
   pinconf = (int *) mmap(NULL, lenght, PROT_READ | PROT_WRITE, MAP_SHARED, fd, addr);

   return pinconf;
}


void pinMode(int *pinconf, int pin, int mode){
   if(mode){
      pinconf[OE_ADDR/4] |= 1 << pin; // inputp
   }else{
      pinconf[OE_ADDR/4] &= ~(1 << pin); // output
   }
}

void digitalWrite(int *pinconf, int pin, int state){

   if(state){
      pinconf[GPIO_DATAOUT/4] |= (1 << pin);
   }
   else{
      pinconf[GPIO_DATAOUT/4] &= ~(1 << pin);
   }
}

int digitalRead(int *pinconf, int pin){
   return (pinconf[GPIO_DATAIN/4] >> pin) & 0x1;
}

void closeDevice(int *pinconf){
   free(pinconf);
}

void stepEnable(int *pinconf, int num){
   u_int32_t reg_config = 0;

   for(int i = 0; i < num; i++){
        reg_config |= 1 << (i+1);
   }

   pinconf[ADC_TSC_STEPENABLE/4] = reg_config;
}

unsigned int getStepRegisterValue(Step config){
   return (config.range_check << 27) | (config.fifo_select << 26) |
          (config.diff_cntrl << 25) | (config.sel_rfm_swc_1_0 << 23) |
          (config.sel_inp_swc_3_0 << 19) | (config.sel_inm_swc_3_0 << 15) |
          (config.sel_rfp_swc_2_0 << 12) | (config.wpnsw_swc << 11) |
          (config.ypnsw_swc << 10) | (config.xnpsw_swc << 9) |
          (config.ynnsw_swc << 8) | (config.yppsw_swc << 7) |
          (config.xnnsw_swc << 6) | (config.xppsw_swc << 5) |
          (config.averaging << 2) | (config.mode);
}

unsigned int getIdleRegisterValue(Idle config){
   return (config.diff_cntrl << 25) | (config.sel_rfm_swc_1_0 << 23) |
          (config.sel_inp_swc_3_0 << 19) | (config.sel_inm_swc_3_0 << 15) |
          (config.sel_rfp_swc_2_0 << 12) | (config.wpnsw_swc << 11) |
          (config.ypnsw_swc << 10) | (config.xnpsw_swc << 9) |
          (config.ynnsw_swc << 8) | (config.yppsw_swc << 7) |
          (config.xnnsw_swc << 6) | (config.xppsw_swc << 5);
}

unsigned int getDelayRegisterValue(Delay config){
   return (config.sample_delay << 24) | (config.open_delay);
}

void init(int *pinconf, Delay delays[], Step steps[], Idle idle, int num){
   pinconf[ADC_TSC_IDLECONFIG/4] = getIdleRegisterValue(idle);

   for(int i = 0; i < num; i++){
      pinconf[(STEPCONFIG1 + (0x8 * i))/4] = getStepRegisterValue(steps[i]);
      //printf("%x %x\n", (STEPCONFIG1 + (0x8 * i)), getStepRegisterValue(steps[i]));
      pinconf[(STEPDELAY1 + (0x8 * i))/4] = getDelayRegisterValue(delays[i]);
   }
}

int readAnalog(int *pinconf, int fifo_addr){
   return pinconf[fifo_addr/4] & 0xFFF;
}
