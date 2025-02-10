#define LED_COUNT 25


// Definição de pixel GRB
struct pixel_t
{
    uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t" por clareza.

// Declaração do buffer de pixels que formam a matriz.
npLED_t leds[LED_COUNT];
// Variáveis para uso da máquina PIO.
PIO np_pio;
uint sm;

/**
 * Inicializa a máquina PIO para controle da matriz de LEDs.
 */
void npInit(uint pin)
{

    // Cria programa PIO.
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;

    // Toma posse de uma máquina PIO.
    sm = pio_claim_unused_sm(np_pio, false);
    if (sm < 0)
    {
        np_pio = pio1;
        sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
    }

    // Inicia programa na máquina PIO obtida.
    ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

    // Limpa buffer de pixels.
    for (uint i = 0; i < LED_COUNT; ++i)
    {
        leds[i].R = 0;
        leds[i].G = 0;
        leds[i].B = 0;
    }
}

/**
 * Atribui uma cor RGB a um LED.
 */
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b)
{
    leds[index].R = r;
    leds[index].G = g;
    leds[index].B = b;
}

/**
 * Limpa o buffer de pixels.
 */
void npClear()
{
    for (uint i = 0; i < LED_COUNT; ++i)
        npSetLED(i, 0, 0, 0);
}

/**
 * Escreve os dados do buffer nos LEDs.
 */
void npWrite()
{
    // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
    for (uint i = 0; i < LED_COUNT; ++i)
    {
        pio_sm_put_blocking(np_pio, sm, leds[i].G);
        pio_sm_put_blocking(np_pio, sm, leds[i].R);
        pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
    sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}

void desenha_numero_vermelho(int array_pixels[], int size){
    npClear();
    npWrite();
    for (int i=0;  i < size; i++){
        npSetLED(array_pixels[i], 50, 0, 0);
        sleep_us(200);
    }
    npWrite();
}

void desenha_numeros(int numero){
    switch (numero)
    {
        case 0:
            desenha_numero_vermelho(array_num_zero, 12);
            break;
        case 1:
            desenha_numero_vermelho(array_num_um, 6);
            break;
        case 2:
            desenha_numero_vermelho(array_num_dois, 8);
            break;
        case 3:
            desenha_numero_vermelho(array_num_tres, 8);
            break;
        case 4:
            desenha_numero_vermelho(array_num_quatro, 9);
            break;
        case 5:
            desenha_numero_vermelho(array_num_cinco, 8);
            break;
        case 6:
            desenha_numero_vermelho(array_num_seis, 12);
            break;
        case 7:
            desenha_numero_vermelho(array_num_sete, 6);
            break;
        case 8:
            desenha_numero_vermelho(array_num_oito, 13);
            break;
        case 9:
            desenha_numero_vermelho(array_num_nove, 10);
            break;
    }
}