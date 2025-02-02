#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/uart.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"
#include "hardware/pwm.h"
#include "pico/bootrom.h"  //
#include "hardware/flash.h"
#include "hardware/structs/systick.h" 
#include "pio_matrix.pio.h"

//definição pinos
#define LED_RED 11
#define LED_GREEN 12
#define LED_BLUE 13
#define BOTAO_A 5
#define BOTAO_B 6
#define BOTAO_C 22
#define OUT_PIN 7
#define BUZZER_A_PIN 10
#define BUZZER_B_PIN 21

//variaveis universais
//variaveis pio
PIO pio_controlador = pio0;
bool status_init_pio = false;
uint state_machine =0;
uint status_init_pio=0
//variaveis operação
volatile _Atomic uint numero_display=0;
volatile _Atomic uint cor_atual =1;//apenas 1, 2 e 3 
volatile _Atomic uint brilho =1; // 1 a 4

//struc com 

//callback()
void tratar_botoes(uint gpio)
{
    debounce();
    if(gpio==BOTAO_A)
    {
        //fazer acrecimo 
        numero_display=(numero_display+1)%10;
        return;
    }else if( gpio ==BOTAO_B)
    {
        //fazer decrescimo
        numero_display=(numero_display-1)%10;
        return;
    }else if(gpio==BOTAO_C)
    {
        entrarModoBootloader();
    }

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
uint32_t matrix_rgbFlag(double b, double r, double g)
{
    r=fmax(0.0, fmin(1.0, r));
    g=fmax(0.0, fmin(1.0, g));
    b=fmax(0.0, fmin(1.0, b));
  unsigned char R, G, B;
  R = (unsigned char)(r * 255.0);
  G = (unsigned char)(g * 255.0);
  B = (unsigned char)(b * 255.0);
  return  (G << 24)| (R << 16) | (B << 8)|0xFF;
}

//debounce()
void debounce()
{
    sleep_ms(50);
}
//atualiza_matrix
void atualiza_matrix()
{

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
    //a
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_RISE, true, &tratar_botoes);
    //b
    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);
    gpio_set_irq_enabled_with_callback(BOTAO_B, GPIO_IRQ_EDGE_RISE, true, &tratar_botoes);
    //c
    gpio_init(BOTAO_C);
    gpio_set_dir(BOTAO_C, GPIO_IN);
    gpio_pull_up(BOTAO_C);
    gpio_set_irq_enabled_with_callback(BOTAO_C, GPIO_IRQ_EDGE_RISE, true, &tratar_botoes);

    



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
    pio_matrix_program_init(pio_controlador, statte_machine, deslocamento, OUT_PIN);
}
void entrarModoBootloader() {

    tocar_tom_buzzer(1000, 200); 
    sleep_ms(100);              
    tocar_tom_buzzer(1500, 200); 
    sleep_ms(100);               
    tocar_tom_buzzer(2000, 300);
    sleep_ms(100);               
    tocar_tom_buzzer(1000, 200);
    sleep_ms(50);                
    tocar_tom_buzzer(1500, 200); 
    sleep_ms(50);                
    tocar_tom_buzzer(2000, 400);
    reset_usb_boot(0, 0); // Reinicia no modo bootloader
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



int main()
{
    inicia_hardware();
  
    while (true) {
        acende_led();
        sleep_ms(150);
        apaga_led();
    }
}
