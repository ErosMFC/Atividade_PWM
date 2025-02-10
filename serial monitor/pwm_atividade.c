#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "ssd1306.h"
#include "ws2812.h"

// Definições de pinos
#define I2C_PORT i2c1
#define SDA_PIN 14
#define SCL_PIN 15
#define WS2812_PIN 7
#define NUM_LEDS 25
#define LED_R 11
#define LED_G 12
#define LED_B 13
#define BUTTON_A 5
#define BUTTON_B 6

// Inicializa o display
ssd1306_t display;

// Padrões binários para os números de 0 a 9
static const uint8_t number_patterns[10][25] = {
    { 0, 0, 1, 1, 1,  0, 0, 1, 0, 1,  0, 0, 1, 0, 1,  0, 0, 1, 0, 1,  0, 0, 1, 1, 1 },
    { 0, 0, 0, 1, 1,  0, 0, 0, 0, 1,  0, 0, 0, 0, 1,  0, 0, 0, 0, 1,  0, 0, 0, 0, 1 },
    { 0, 0, 1, 1, 1,  0, 0, 0, 0, 1,  0, 0, 1, 1, 1,  0, 0, 1, 0, 0,  0, 0, 1, 1, 1 },
    { 0, 0, 1, 1, 1,  0, 0, 0, 0, 1,  0, 0, 0, 1, 1,  0, 0, 0, 0, 1,  0, 0, 1, 1, 1 },
    { 0, 0, 1, 0, 1,  0, 0, 1, 0, 1,  0, 0, 1, 1, 1,  0, 0, 0, 0, 1,  0, 0, 0, 0, 1 },
    { 0, 0, 1, 1, 1,  0, 0, 1, 0, 0,  0, 0, 1, 1, 1,  0, 0, 0, 0, 1,  0, 0, 1, 1, 1 },
    { 0, 0, 1, 1, 1,  0, 0, 1, 0, 0,  0, 0, 1, 1, 1,  0, 0, 1, 0, 1,  0, 0, 1, 1, 1 },
    { 0, 0, 1, 1, 1,  0, 0, 0, 0, 1,  0, 0, 0, 0, 1,  0, 0, 0, 0, 1,  0, 0, 0, 0, 1 },
    { 0, 0, 1, 1, 1,  0, 0, 1, 0, 1,  0, 0, 1, 1, 1,  0, 0, 1, 0, 1,  0, 0, 1, 1, 1 },
    { 0, 0, 1, 1, 1,  0, 0, 1, 0, 1,  0, 0, 1, 1, 1,  0, 0, 0, 0, 1,  0, 0, 1, 1, 1 }
};

// Exibe um caractere no display SSD1306
void display_character_on_oled(char c) {
    ssd1306_clear(&display);
    char str[2] = {c, '\0'};
    ssd1306_draw_string(&display, 32, 24, 2, str);
    ssd1306_show(&display);
}

// Exibe um número na matriz WS2812
void display_number_on_matrix(int num) {
    uint32_t pattern[25];
    for (int i = 0; i < 25; i++) {
        pattern[i] = number_patterns[num][i] ? 0xff0000ff : 0x00000000;
    }
    for (int i = 0; i < 25; i++) {
        ws2812_set_pixel(i, pattern[i]);
    }
    ws2812_show();
}

// Configura LEDs RGB
void set_rgb_led(bool r, bool g, bool b) {
    gpio_put(LED_R, r);
    gpio_put(LED_G, g);
    gpio_put(LED_B, b);
}

int main() {
    stdio_init_all();
    
    // Inicializa os pinos I2C
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    // Inicializa o display SSD1306
    ssd1306_init(&display, I2C_PORT, 0x3C, 128, 64);
    ssd1306_clear(&display);
    ssd1306_show(&display);

    // Inicializa a matriz WS2812
    ws2812_init(WS2812_PIN);

    // Configura LEDs RGB
    gpio_init(LED_R);
    gpio_init(LED_G);
    gpio_init(LED_B);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_set_dir(LED_B, GPIO_OUT);

    // Configura botões
    gpio_init(BUTTON_A);
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_pull_up(BUTTON_B);

    printf("Sistema iniciado. Digite um caractere:\n");

    while (true) {
        char c = getchar();  // Lê caractere do Serial Monitor
        printf("Recebido: %c\n", c);
        
        display_character_on_oled(c); // Mostra no display
        
        if (c >= '0' && c <= '9') {  // Se for um número
            int num = c - '0';
            display_number_on_matrix(num);
        }

        // Controle do LED RGB baseado no botão pressionado
        if (!gpio_get(BUTTON_A)) {
            set_rgb_led(1, 0, 0); // LED vermelho
            printf("Botão A pressionado: LED vermelho\n");
        } else if (!gpio_get(BUTTON_B)) {
            set_rgb_led(0, 1, 0); // LED verde
            printf("Botão B pressionado: LED verde\n");
        } else {
            set_rgb_led(0, 0, 1); // LED azul (padrão)
        }
    }

    return 0;
}
v