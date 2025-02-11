Link para o video de demonstração: https://youtube.com/shorts/NFlSaUjk264?si=s-jJYy0uB-_jTbs1


O código implementa um sistema embarcado usando o Raspberry Pi Pico para controlar uma matriz de LEDs WS2812 de 5x5, um display OLED SSD1306 via I2C, LEDs RGB e dois botões de entrada. O sistema também oferece comunicação serial e gerenciamento de interrupções para os botões. Abaixo está uma explicação detalhada sobre cada parte do código.

### 1. **Inclusão de bibliotecas e definições de pinos**
O código começa com a inclusão de diversas bibliotecas necessárias para o funcionamento do sistema, como o gerenciamento de GPIOs, interrupções, I2C, UART, e o controle do display SSD1306 e matriz WS2812. As constantes para os pinos são definidas no início, incluindo os pinos para o controle dos LEDs RGB, botões, pinos I2C e a matriz WS2812.

### 2. **Configuração dos LEDs RGB**
Os LEDs RGB são configurados para controle através de pinos específicos (`PIN_LED_GREEN`, `PIN_LED_BLUE`, e `PIN_LED_RED`). Inicialmente, os LEDs são configurados como saídas, e o estado inicial de cada um é definido (o verde e azul são configurados de acordo com o estado atual, enquanto o vermelho é desligado).

### 3. **Configuração dos botões**
Os dois botões (`PIN_BUTTON_A` e `PIN_BUTTON_B`) são configurados como entradas com resistência pull-up interna. O código habilita interrupções para detectar quando um botão é pressionado (quando ocorre uma borda de descida, ou seja, quando o botão é pressionado e o sinal vai de alto para baixo). A função `gpio_callback` é chamada quando uma interrupção é gerada.

### 4. **Debounce para os botões**
A função `gpio_callback` gerencia os botões e implementa a técnica de **debounce** para evitar leituras incorretas causadas por ruídos ao pressionar o botão. O tempo de debounce é definido como 200 ms. Dependendo do botão pressionado, o estado do LED verde ou azul é alterado. Além disso, uma mensagem sobre o estado do LED é exibida no display OLED e também é enviada via UART para o console.

### 5. **Controle da matriz WS2812**
A matriz de LEDs WS2812 é controlada por meio de um programa de máquina de estado (SM) no PIO (Programmable Input/Output) do Raspberry Pi Pico. A função `ws2812_init` configura o PIO para controlar a matriz de LEDs, enquanto as funções `ws2812_clear`, `ws2812_set_pixel` e `ws2812_show` são usadas para apagar, configurar e exibir os LEDs, respectivamente.

### 6. **Exibição de dígitos na matriz WS2812**
A função `display_digit_on_matrix` é usada para desenhar números (de 0 a 9) na matriz de LEDs. Para cada dígito, um padrão específico de LEDs é iluminado, usando cores diferentes (verde, azul, vermelho) para representar cada número de forma visual na matriz de 5x5. Cada número tem um padrão de LEDs acesos que formam a forma correspondente.

### 7. **Configuração do display SSD1306**
O display SSD1306 é configurado via I2C e usado para exibir informações sobre os botões pressionados, o estado dos LEDs e a entrada do usuário. A função `ssd1306_init` inicializa o display, e a função `ssd1306_draw_string` é utilizada para desenhar textos na tela. O display é atualizado sempre que um botão é pressionado ou quando um dígito é recebido via UART.

### 8. **Loop Principal**
O loop principal espera pela entrada do usuário através da UART (porta serial). Se um caractere for recebido, ele é exibido no display e na matriz de LEDs WS2812. Além disso, o programa exibe o número correspondente na matriz de LEDs usando a função `display_digit_on_matrix`.

### 9. **Comunicação via UART**
A comunicação via UART é usada para enviar e receber caracteres entre o Raspberry Pi Pico e um terminal serial. Quando um dígito de 0 a 9 é recebido, ele é mostrado na matriz WS2812, e uma mensagem sobre o caractere recebido é exibida no display OLED.

### Resumo do funcionamento:
- O código configura e controla LEDs RGB e uma matriz de LEDs WS2812 de 5x5.
- O sistema usa dois botões para alterar o estado de LEDs RGB, e um display OLED SSD1306 para mostrar o estado atual.
- A comunicação UART permite que o sistema receba entradas de caracteres, exibindo-os na matriz WS2812.
- O debounce é aplicado para garantir que as pressões dos botões sejam detectadas corretamente.
