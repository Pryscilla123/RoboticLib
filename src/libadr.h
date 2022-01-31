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

#define ADC_TSC 0x44E0D000
#define ADC_TSC_STEPENABLE 0x54
#define ADC_TSC_IDLECONFIG 0x58

enum gpio_addr {GPIO0_ADDR = 0x44E07000, GPIO1_ADDR = 0x4804C000, GPIO2_ADDR = 0x481AC000,
                GPIO3_ADDR = 0x481AE000};

enum step_config_x {STEP_CONFIG1 = 0x64, STEP_CONFIG2 = 0x6C, STEP_CONFIG3 = 0x74, STEP_CONFIG4 = 0x7C, STEP_CONFIG5 = 0x84,
                    STEP_CONFIG6 = 0x8C, STEP_CONFIG7 = 0x94, STEP_CONFIG8 = 0x9C, STEP_CONFIG9 = 0xA4, STEP_CONFIG10 = 0xAC,
                    STEP_CONFIG11 = 0xB4, STEP_CONFIG12 = 0xBC, STEP_CONFIG13 = 0xC4, STEP_CONFIG14 = 0xCC, STEP_CONFIG15 = 0xD4,
                    STEP_CONFIG16 = 0xDC};

enum step_delay_x {STEP_DELAY1 = 0x68, STEP_DELAY2 = 0x70, STEP_DELAY3 = 0x78, STEP_DELAY4 = 0x80, STEP_DELAY5 = 0x88,
                   STEP_DELAY6 = 0x90, STEP_DELAY7 = 0x98, STEP_DELAY8 = 0xA0, STEP_DELAY9 = 0xA8, STEP_DELAY10 = 0xB0,
                   STEP_DELAY11 = 0xB8, STEP_DELAY12 = 0xC0, STEP_DELAY13 = 0xC8, STEP_DELAY14 = 0xD0, STEP_DELAY15 = 0xD8,
                   STEP_DELAY16 = 0xE0};

enum range_check {DISABLE_O_R_C = 0x0, COMPARE_ADC_R_C_R = 0x1};

enum fifo_select {FIFO0 = 0x0, FIFO1 = 0x1};

enum diff_cntrl {SINGLE_ENDED = 0x0, DIFERENCIAL_PAIR_ENABLE = 0x1};

enum sel_rfm_swc_1_0 {VSSA_RFM = 0x0, XNUR_RFM = 0x1, YNLR_RFM = 0x2, VREFN_RFM = 0x3};

enum sel_inp_swc_3_0 {CH1_INP = 0x0, CH2_INP = 0x1, CH3_INP = 0x2, CH4_INP = 0x3, CH5_INP = 0x4,
                      CH6_INP = 0x5, CH7_INP = 0x6, CH8_INP = 0x7, VREFN_INP = 0x8};

enum sel_inm_swc_3_0 {CH1_INM = 0x0, CH2_INM = 0x1, CH3_INM = 0x2, CH4_INM = 0x3, CH5_INM = 0x4,
                      CH6_INM = 0x5, CH7_INM = 0x6, CH8_INM = 0x7, ADCREFM_INM = 0x8};

enum sel_rfp_swc_2_0 {VDDA_ADC = 0x0, XPUL_RFP = 0x1, YPLL_RFP = 0x2, VREFP = 0x3, RESERVED = 0x4};

enum wpnsw_swc {OFF_WPNSW = 0x0, ON_WPNSW = 0x1};

enum ypnsw_swc {OFF_YPNSW = 0x0, ON_YPNSW = 0x1};

enum xnpsw_swc {OFF_XNPSW = 0x0, ON_XNPSW = 0x1};

enum ynnsw_swc {OFF_YNNSW = 0x0, ON_YNNSW = 0x1};

enum yppsw_swc {OFF_YPPSW = 0x0, ON_YPPSW = 0x1};

enum xnnsw_swc {OFF_XNNSW = 0x0, ON_XNNSW = 0x1};

enum xppsw_swc {OFF_XPPSW = 0x0, ON_XPPSW = 0x1};

enum averaging {NONE = 0x0, SAMPLES_2 = 0x1, SAMPLES_4 = 0x2, SAMPLES_8 = 0x3, SAMPLES_16 = 0x4};

enum mode {SW_ONE_SHOT = 0x0, SW_CONTINUOUS = 0x1, HW_ONE_SHOT = 0x2, HW_CONTINUOUS = 0x3};

enum fifo_data {FIFO0_DATA = 0x100, FIFO1_DATA = 0x200};

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
}step_config;

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
}idle_config;

typedef struct _stepdelay{
   int sample_delay;
   int open_delay;
}delay_config;

/**
 * @brief Inicializa um ponteiro que recebe a primeira posição 
 * do ambiente virtual criado pelo mmap.
 * 
 * @param addr endereço da posição de memória que será acessada.
 * @param lenght tamanho do espaço consumido na memória pelo addr.
 * @return int* retorna o ponteiro.
 */
int *open_device(int addr, int lenght);

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
void set_pin_mode(int *pinconf, int pin, int mode);

/**
 * @brief Coloca em baixo (0V) ou em alto (3,3V) o pino desejado.
 * 
 * @param pinconf ponteiro com a memória alocada pelo mmap.
 * @param pin pino que será colocado em baixo ou alto.
 * @param state estado baixo(0) ou alto(1).
 */
void digital_write(int *pinconf, int pin, int state);

/**
 * @brief Ler a tensão do pino desejado.
 * 
 * @param pinconf ponteiro com a memória alocada pelo mmap.
 * @param pin pino que terá a tensão lida.
 * @return int retorna 0 caso o pino esteja recebendo 0V ou
 * 1 caso o pino esteja recebendo 3,3V.
 */
int digital_read(int *pinconf, int pin);

/**
 * @brief desaloca a memoria de um ponteiro.
 * 
 * @param pinconf ponteiro alocado virtualmente pelo mmap para digital.
 */
void close_device(int *pinconf);

/**
 * @brief Função que habilita os steps de conversão analógica.
 * 
 * @param pinconf ponteiro com a memória alocada pelo mmap para analógico.
 * @param num número de passos serão habilitados.
 */
void step_enable(int *pinconf, int num);

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
unsigned int get_step_register_value(step_config config);

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
unsigned int get_idle_register_value(idle_config config);

/**
 * @brief  Recebe um struct do tipo Delay (passo que configura o tempo entre uma conversão e outra).
 * Informações sobre os bits acesse: https://www.ti.com/lit/ug/spruh73q/spruh73q.pdf?ts=1630008076742
 * capítulo 12. 
 * 
 * @param config struct com as configurações do delay de conversão.
 * @return unsigned int retorna uma palavra de 32 bits com o tempo entre um a conversão e outra.
 */
unsigned int get_delay_register_value(delay_config config);

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
void init_adc_config(int *pinconf, delay_config delays[], step_config steps[], idle_config idle, int num);

/**
 * @brief Função para a leitura do analógico.
 * 
 * @param pinconf ponteiro para o registrador do analógico.
 * @param fifo_addr o offset de qual fifo foi escolhido na configuração do Step.
 * @return int retorna valor entre 0 e 4096 que é o valor da voltagem passada pelo 
 * analógico.
 */
int read_analog(int *pinconf, int fifo_addr);

#endif
