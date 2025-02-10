#include <stdio.h>
#include "pico/stdlib.h"
#include "ws2818b.pio.h"
#include "hardware/pio.h"
#include "fonte.h"
#include "write_matrix.h"
#include "pico/bootrom.h"
#include "inc/ssd1306.h"
#include "inc/font.h"

#define LED_MATRIZ 7
#define LED_RED 13
#define LED_GREEN 11
#define LED_BLUE 12
#define BUTTON_A 5
#define BUTTON_B 6
#define BUTTON_BOOTSEL 22
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C


void rgb_clean_except(int exception_led){
    for (int i=11; i<13; i++){
        if (i != exception_led)
            gpio_put(i, 0);
    }
}

uint count_numbers = 0;
uint32_t last_time;
bool led_green_up = false;
bool led_blue_up = false;
void gpio_irq_handler(uint gpio, uint32_t event_mask) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    if (current_time - last_time > 200000){
        if (gpio == BUTTON_A) {
            rgb_clean_except(LED_GREEN);
            gpio_put(LED_GREEN, !gpio_get(LED_GREEN));
            printf("O LED_GREEN foi alterado!\n");
            led_green_up = !led_green_up;
            led_blue_up = false;
            last_time = current_time;
        }
        if (gpio == BUTTON_B) {
            rgb_clean_except(LED_BLUE);
            gpio_put(LED_BLUE, !gpio_get(LED_BLUE));
            printf("O LED_BLUE foi alterado!\n");
            led_blue_up = !led_blue_up;
            led_green_up = false;
            last_time = current_time;

        }
        if (!gpio_get(BUTTON_BOOTSEL)) {
            rom_reset_usb_boot(0, 0);
        }
        
    }

}

int main()
{
    stdio_init_all();

    npInit(LED_MATRIZ);
    npClear();
    npWrite();

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA); // Pull up the data line
    gpio_pull_up(I2C_SCL); // Pull up the clock line
    ssd1306_t ssd; // Inicializa a estrutura do display
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);

    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);

    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);

    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    gpio_init(BUTTON_BOOTSEL);
    gpio_set_dir(BUTTON_BOOTSEL, GPIO_IN);
    gpio_pull_up(BUTTON_BOOTSEL);

    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_BOOTSEL, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    char c;
    char last;
    bool cor = true;
    while (true) {

        c = getchar_timeout_us(0);
        if (c != 254){
            last = c;
        }
        cor = !cor;
        // Atualiza o conteúdo do display com animações
        ssd1306_fill(&ssd, !cor); // Limpa o display
        ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
        ssd1306_draw_string(&ssd, "0 LEDS ACESOS", 8, 10); // Desenha uma string
        if (led_blue_up){
            ssd1306_draw_string(&ssd, "AZUL ACESO   ", 8, 10); // Desenha uma string
        }
        if (led_green_up){
            ssd1306_draw_string(&ssd, "VERDE ACESO   ", 8, 10); // Desenha uma string
        }
        ssd1306_draw_string(&ssd, "CARACTER ", 8, 30); // Desenha uma string  
        ssd1306_draw_char(&ssd, last, 80, 30); // Desenha uma string
        ssd1306_draw_string(&ssd, "Rodrigo Piropo", 8, 48); // Desenha uma string      
        // Desenha uma string
        ssd1306_send_data(&ssd); // Atualiza o display
        printf("Ultimo caractere presionado foi: '%c' em ASCI: %d \n", last, last);
        desenha_numeros(c-48);
        sleep_ms(200);
    }
}
