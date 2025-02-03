#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include <math.h>
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"
#include "hardware/pwm.h"
#include "pico/bootrom.h"  //
#include "hardware/flash.h"
#include "hardware/structs/systick.h" 
#include "pio_matrix.pio.h"
#include "numbers_n_array.h"
#include <time.h>
#include <stdlib.h>

//definição pinos
#define LED_RED 13
#define LED_GREEN 11
#define LED_BLUE 12
#define BOTAO_A 5
#define BOTAO_B 6
#define BOTAO_C 22
#define OUT_PIN 7
#define BUZZER_A_PIN 10
#define BUZZER_B_PIN 21
#define NUM_PIXELS 25
#define DEBOUNCING_TIME_US 220
//
void tocar_tom_buzzer(uint16_t frequency, uint32_t duration_ms);
void tratar_botoes(uint gpio, uint32_t eventos);
void acende_led();
void apaga_led();
uint32_t matrix_rgbFlag(double number);
void debounce();
void inicia_hardware();
void entrarModoBootloaderDois();
void init_pio_routine(void);
void desliga_tudo();
void ligar_todos_os_leds();

//

//variaveis universais
//variaveis pio
PIO pio_controlador = pio0;
bool status_init_pio = false;
uint state_machine =0;

//variaveis operação
volatile _Atomic uint numero_display=0;
volatile _Atomic uint cor_atual =1;//apenas 1, 2 e 3 
volatile _Atomic uint brilho =1; // 1 a 4
volatile _Atomic uint tempo =50; // 1 a 4
volatile uint32_t hora_presente;
volatile _Atomic uint var_teste=0;
volatile _Atomic uint saida_teste=0;
volatile _Atomic uint sobe_um=0;
volatile _Atomic uint desce_um=0;
int randon=0;
static int randonTwo=0;

//objetivo nao muito claro
static volatile uint32_t ultimo_pressionamento = 0;


//struc com 

//callback()
void tratar_botoes(uint gpio, uint32_t events)
{
    //debounce();
    
      hora_presente = to_us_since_boot(get_absolute_time());
    if(gpio==BOTAO_A)
    {
        if(hora_presente-ultimo_pressionamento>DEBOUNCING_TIME_US)
        {
            bool botao_pressionado=!gpio_get(BOTAO_A);
            if(botao_pressionado){
                    sobe_um=1;
            }
        }
    
    //

    } else if( gpio ==BOTAO_B)
    {
         if(hora_presente-ultimo_pressionamento>DEBOUNCING_TIME_US)
        {
            bool botao_pressionado=!gpio_get(BOTAO_B);
            if(botao_pressionado){
                    desce_um=1;
            }
        }
     


    //
    
    } else if(gpio==BOTAO_C)
    {
        if(hora_presente-ultimo_pressionamento>DEBOUNCING_TIME_US)
        {
            bool botao_pressionado=!gpio_get(BOTAO_C);
            if(botao_pressionado)
            {saida_teste=1;}
        }

    }
            ultimo_pressionamento=hora_presente;

}
//acende_led()
void acende_led()
{
    gpio_put(LED_RED, true);
}
//apaga_led()
void apaga_led()
{
    gpio_put(LED_RED, false);
}
//gera_rgb
uint32_t matrix_rgbFlag(double number)
{
     // Certifica-se de que 'number' esteja no intervalo [0, 1]
    number = fmax(0.0, fmin(1.0, number));
    double brilhoLeds=0;
    if(var_teste==1){
         brilhoLeds=1;
         randon=4;
    }else{
     brilhoLeds=0.25*brilho;
        randon=(rand()%4)+1;

    }


    // Definir as variáveis de cor RGB
    unsigned char R = 0, G = 0, B = 0;
if(randon>=1&&randon<=3){
    // Ajuste de cor baseado na variável 'cor_atual'
    if (randon == 1) { // Vermelho
        R = (unsigned char)(number *brilhoLeds* 255.0); // R é o máximo, G e B são 0
    } else if (randon == 2) { // Verde
        G = (unsigned char)(number *brilhoLeds*  255.0); // G é o máximo, R e B são 0
    } else if (randon == 3) { // Azul
        B = (unsigned char)(number *brilhoLeds*  255.0); // B é o máximo, R e G são 0
    }
}else if(randon==4 || randonTwo==1)//se or and for 4 o led fica branco
{
        R = (unsigned char)(number *brilhoLeds* 255.0); // R é o máximo, G e B são 0
        G = (unsigned char)(number *brilhoLeds*  255.0); // G é o máximo, R e B são 0
        B = (unsigned char)(number *brilhoLeds*  255.0); // B é o máximo, R e G são 0
    
}
randon=0;
    // Retorna o valor RGB no formato ARGB (Alpha, Red, Green, Blue)
    return (G << 24) | (R << 16) | (B << 8) | 0xFF;
}

//debounce()
void debounce()
{
    sleep_ms(50);
}
//atualiza_matrix
void atualiza_matrix()
{
 uint32_t valor_led = 0;

    tocar_tom_buzzer(1000, 200); 
    sleep_ms(20);              

 
        for (int16_t i = 0; i < NUM_PIXELS; i++) //laço pixels ou colnas
        {

            valor_led = matrix_rgbFlag(numbers[numero_display][24-i]);
          
            pio_sm_put_blocking(pio_controlador, state_machine, valor_led);
            sleep_us(50);
   
    }
        sleep_ms(50);
           
    
}
//inicia_HARDWARE()
void inicia_hardware()
{
    //inicializa pio
    init_pio_routine();
    //inicializa leds 11 
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    //inicializa botoes
        //c
    gpio_init(BOTAO_C);
    gpio_set_dir(BOTAO_C, GPIO_IN);
    gpio_pull_up(BOTAO_C);
    gpio_set_irq_enabled_with_callback(BOTAO_C, GPIO_IRQ_EDGE_FALL, true, &tratar_botoes);
    //a
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &tratar_botoes);
    //b
    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);
    gpio_set_irq_enabled_with_callback(BOTAO_B, GPIO_IRQ_EDGE_FALL, true, &tratar_botoes);

}
void init_pio_routine()
{
    //inicia o PIO
    //sem struct
    //coloca a frequência de clock para 128 MHz, facilitando a divisão pelo clock
   status_init_pio = set_sys_clock_khz(128000, false);

    stdio_init_all();

    printf("iniciando a transmissão PIO");
    if (status_init_pio){ printf("clock set to %ld\n", clock_get_hz(clk_sys));}

    //configurações da PIO
    uint deslocamento = pio_add_program(pio_controlador, &pio_matrix_program);
    state_machine= pio_claim_unused_sm(pio_controlador, true);
    pio_matrix_program_init(pio_controlador, state_machine, deslocamento, OUT_PIN);
}
void entrarModoBootloader() {

tocar_tom_buzzer(1200, 250); 
sleep_ms(100);              
tocar_tom_buzzer(1600, 250); 
sleep_ms(100);               
tocar_tom_buzzer(2200, 350);
sleep_ms(100);               
tocar_tom_buzzer(1200, 200);
sleep_ms(50);                
tocar_tom_buzzer(1600, 200); 
sleep_ms(50);                
tocar_tom_buzzer(2200, 450);
}
void entrarModoBootloaderDois() {
tocar_tom_buzzer(2000, 300);  
sleep_ms(150);                
tocar_tom_buzzer(1500, 250);  
sleep_ms(150);                
tocar_tom_buzzer(1000, 250);  
sleep_ms(200);                
tocar_tom_buzzer(800, 300);   
sleep_ms(250);                
tocar_tom_buzzer(600, 400);   
sleep_ms(300);                
tocar_tom_buzzer(400, 500);  

}

void tocar_tom_buzzer(uint16_t frequency, uint32_t duration_ms) {
    gpio_set_function(BUZZER_B_PIN, GPIO_FUNC_PWM); // Configura pino do buzzer para PWM
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_B_PIN);

    pwm_set_wrap(slice_num, 125000000 / frequency); // Período do PWM
    pwm_set_gpio_level(BUZZER_B_PIN, (125000000 / frequency) / 2); // Duty cycle 50%
    pwm_set_enabled(slice_num, true); // Ativa o PWM

    sleep_ms(duration_ms); // Toca por tempo especificado

    pwm_set_enabled(slice_num, false); // Desliga o PWM
    gpio_set_function(BUZZER_B_PIN, GPIO_FUNC_SIO);
    gpio_put(BUZZER_B_PIN, 0);
}
void ligar_todos_os_leds()
{
    uint32_t valor_led = 0;
    double ligar_todos_os_leds_20[25] =   {1.0, 1.0, 1.0, 1.0, 1.0,
                                        1.0, 1.0, 1.0, 1.0, 1.0, 
                                        1.0, 1.0, 1.0, 1.0, 1.0,
                                        1.0, 1.0, 1.0, 1.0, 1.0,
                                        1.0, 1.0, 1.0, 1.0, 1.0};

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgbFlag(ligar_todos_os_leds_20[i]);
        pio_sm_put_blocking(pio_controlador, state_machine, valor_led);
    }
}
void desliga_tudo()
{
    uint32_t valor_led = 0;

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgbFlag(0.0);
        pio_sm_put_blocking(pio_controlador, state_machine, valor_led);

    }
  
}
void operaDisplay()
{
if (sobe_um) {  // Se sobe_um for 1
        if (numero_display < 9) {
            numero_display++;
        }
        sobe_um = 0;  // Reseta a variável após uso
    }

    if (desce_um) {  // Se desce_um for 1
        if (numero_display > 0) {
            numero_display--;
        }
        desce_um = 0;  // Reseta a variável após uso
    }

}


int main()
{
    inicia_hardware();
  
    var_teste=1;
    ligar_todos_os_leds();
    entrarModoBootloader();
    sleep_ms(200);
    desliga_tudo();
    var_teste=0;
    srand(time_us_32());
    while (true) {
        if(saida_teste==1)
        {
            randonTwo=1;
            desliga_tudo();
            sleep_ms(50);
            
            ligar_todos_os_leds();
            entrarModoBootloaderDois();
            desliga_tudo();
            reset_usb_boot(0, 0);
        }
        acende_led();
        sleep_ms(tempo);
        apaga_led();
        sleep_ms(tempo);
        operaDisplay();
        atualiza_matrix();
    }
}
