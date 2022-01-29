#include "libadr.h"

/**
 * @brief Inicializa um ponteiro que recebe a primeira posição 
 * do ambiente virtual criado pelo mmap.
 * 
 * @param addr endereço da posição de memória que será acessada.
 * @param lenght tamanho do espaço consumido na memória pelo addr.
 * @return int* retorna o ponteiro.
 */
int *open_device(int addr, int lenght){
   int fd = open("/dev/mem", O_RDWR | O_SYNC);
   int * pinconf = (int *) malloc(sizeof(int));
   if(pinconf == NULL){
      exit(1024);
   }
   pinconf = (int *) mmap(NULL, lenght, PROT_READ | PROT_WRITE, MAP_SHARED, fd, addr);

   return pinconf;
}


/**
 * @brief Função que altera o modo do pino de saída digital 
 * para entrada (0) ou saída (1).
 * 
 * @param pinconf ponteiro com a memória alocada pelo mmap.
 * @param pin pino que será alterado. Para saber mais sobre a numeração dos pinos
 * de acordo com o GPIO, acesse: http://www.minorw.net/data/ExploringBeagleBone.pdf
 * cápitulo 6.
 * @param mode entrada ou saída. 
 */
void set_pin_mode(int *pinconf, int pin, int mode){
   if(mode){
      pinconf[OE_ADDR/4] |= 1 << pin; // input
   }else{
      pinconf[OE_ADDR/4] &= ~(1 << pin); // output
   }
}

/**
 * @brief Coloca em baixo (0V) ou em alto (3,3V) o pino desejado.
 * 
 * @param pinconf ponteiro com a memória alocada pelo mmap.
 * @param pin pino que será colocado em baixo ou alto.
 * @param state estado baixo(0) ou alto(1).
 */
void digital_write(int *pinconf, int pin, int state){

   if(state){
      pinconf[GPIO_DATAOUT/4] |= (1 << pin);
   }
   else{
      pinconf[GPIO_DATAOUT/4] &= ~(1 << pin);
   }
}

/**
 * @brief Ler a tensão do pino desejado.
 * 
 * @param pinconf ponteiro com a memória alocada pelo mmap.
 * @param pin pino que terá a tensão lida.
 * @return int retorna 0 caso o pino esteja recebendo 0V ou
 * 1 caso o pino esteja recebendo 3,3V.
 */
int digital_read(int *pinconf, int pin){
   return (pinconf[GPIO_DATAIN/4] >> pin) & 0x1;
}

/**
 * @brief desaloca a memoria de um ponteiro.
 * 
 * @param pinconf ponteiro alocado virtualmente pelo mmap para digital.
 */
void close_device(int *pinconf){
   free(pinconf);
}

/**
 * @brief Função que habilita os steps de conversão analógica.
 * 
 * @param pinconf ponteiro com a memória alocada pelo mmap para analógico.
 * @param num número de passos serão habilitados.
 */
void step_enable(int *pinconf, int num){
   u_int32_t reg_config = 0;

   for(int i = 0; i < num; i++){
        reg_config |= 1 << (i+1);
   }

   pinconf[ADC_TSC_STEPENABLE/4] = reg_config;
}

/**
 * @brief Recebe um struct do tipo Step que contem todos os bits 
 * de configuração do registrador stepConfigX. Informações sobre os bits
 * acesse: https://www.ti.com/lit/ug/spruh73q/spruh73q.pdf?ts=1630008076742
 * capítulo 12. 
 * 
 * @param config struct com as configurações de um passo de conversão
 * analógica.
 * @return unsigned int retorna uma palavra de 32 bits com a instrução da
 * configuração do passo. 
 */
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

/**
 * @brief Recebe um struct do tipo Idle (passo ocioso que ocorre entre as conversões) 
 * que contem todos os bits de configuração do registrador stepIdle. 
 * Informações sobre os bits acesse: https://www.ti.com/lit/ug/spruh73q/spruh73q.pdf?ts=1630008076742
 * capítulo 12. 
 * 
 * @param config struct com as configurações do passo ocioso.
 * @return unsigned int retorna uma palavra de 32 bits com a instrução da
 * configuração do passo. 
 */
unsigned int get_idle_register_value(idle_config config){
   return (config.diff_cntrl << 25) | (config.sel_rfm_swc_1_0 << 23) |
          (config.sel_inp_swc_3_0 << 19) | (config.sel_inm_swc_3_0 << 15) |
          (config.sel_rfp_swc_2_0 << 12) | (config.wpnsw_swc << 11) |
          (config.ypnsw_swc << 10) | (config.xnpsw_swc << 9) |
          (config.ynnsw_swc << 8) | (config.yppsw_swc << 7) |
          (config.xnnsw_swc << 6) | (config.xppsw_swc << 5);
}

/**
 * @brief  Recebe um struct do tipo Delay (passo que configura o tempo entre uma conversão e outra).
 * Informações sobre os bits acesse: https://www.ti.com/lit/ug/spruh73q/spruh73q.pdf?ts=1630008076742
 * capítulo 12. 
 * 
 * @param config struct com as configurações do delay de conversão.
 * @return unsigned int retorna uma palavra de 32 bits com o tempo entre um a conversão e outra.
 */
unsigned int get_delay_register_value(delay_config config){
   return (config.sample_delay << 24) | (config.open_delay);
}

/**
 * @brief coloca as palavras dadas pelas funções getStepRegisterValue(),
 * getIdleRegisterValue() e getDelayRegisterValue() e coloca respectivamente nos
 * registradores STEPCONFIGX, ADC_TSC_IDLECONFIG e STEPDELAYX. Essa função serve para
 * inicializar as configurações da conversão analógica antes da função stepEnable() ser usada.
 * 
 * @param pinconf ponteiro para o registrador do analógico
 * @param delays vetor com os delays de cada passo, ou seja o passo 1 tem o delay 1 e assim por diante.
 * @param steps vetor com os passos de conversão podendo ir de 1 até 16.
 * @param idle variável única do modo oscioso que é o modo onde quando não tem nenhum paso ativado 
 * então a conversão vai para as configurações do modo oscioso.
 * @param num numero de passos ativados.
 */
void init(int *pinconf, delay_config delays[], step_config steps[], idle_config idle, int num){
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

/**
 * @brief Função para a leitura do analógico.
 * 
 * @param pinconf ponteiro para o registrador do analógico.
 * @param fifo_addr o offset de qual fifo foi escolhido na configuração do Step.
 * @return int retorna valor entre 0 e 4096 que é o valor da voltagem passada pelo 
 * analógico.
 */
int read_analog(int *pinconf, int fifo_addr){
   return pinconf[fifo_addr/4] & 0xFFF;
}
