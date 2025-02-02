#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/uart.h"

#include "blink.pio.h"
//definição pinos
#define LED_RED 11
#define LED_GREEN 12
#define LED_BLUE 13
#define BOTAO_A 5
#define BOTAO_B 6
#define OUT_PIN 7
#define

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

//acende_led()

//apaga_led()

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

//atualiza_matrix

//inicia_HARDWARE()
void inicia_hardware()
{
init_pio_routine();

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



int main()
{
    stdio_init_all();

    // PIO Blinking example
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &blink_program);
    printf("Loaded program at %d\n", offset);
    
    #ifdef PICO_DEFAULT_LED_PIN
    blink_pin_forever(pio, 0, offset, PICO_DEFAULT_LED_PIN, 3);
    #else
    blink_pin_forever(pio, 0, offset, 6, 3);
    #endif
    // For more pio examples see https://github.com/raspberrypi/pico-examples/tree/master/pio

    // Set up our UART
    uart_init(UART_ID, BAUD_RATE);
    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    
    // Use some the various UART functions to send out data
    // In a default system, printf will also output via the def
    
    // Send out a string, with CR/LF conversions
    uart_puts(UART_ID, " Hello, UART!\n");
    
    // For more examples of UART use see https://github.com/raspberrypi/pico-examples/tree/master/uart

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
