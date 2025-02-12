#include <stdio.h> //biblioteca padrão da linguagem C
#include "pico/stdlib.h" //subconjunto central de bibliotecas do SDK Pico
#include "hardware/pwm.h" //biblioteca para controlar o hardware de PWM

#define PIN 22
const uint16_t WRAP_PERIOD = 25000; //valor máximo do contador - WRAP
const float PWM_DIVISER = 100; //divisor do clock para o PWM
volatile bool rotina = false;
uint slice = 0;

// frequencia pwm = 125000000/(100*25000) = 50 Hz 
// Tpwm = 1/50 = 0.02 = 20 ms

//Função para configurar o módulo PWM
void pwm_setup()
{
    gpio_set_function(PIN, GPIO_FUNC_PWM); //habilitar o pino GPIO como PWM

    slice = pwm_gpio_to_slice_num(PIN); //obter o canal PWM da GPIO

    pwm_set_clkdiv(slice, PWM_DIVISER); //define o divisor de clock do PWM

    pwm_set_wrap(slice, WRAP_PERIOD); //definir o valor de wrap

    pwm_set_gpio_level(PIN, 25000); //definir o cico de trabalho (duty cycle) do pwm

    pwm_set_enabled(slice, true); //habilita o pwm no slice correspondente
}

//Função para o tratamento da interrrupção
void wrapHandler(){ 
    static float wrap = 625; 
    static bool rise = true; //flag para elevar ou reduzir a potência entregada
    pwm_clear_irq(pwm_gpio_to_slice_num(PIN)); //resetar o flag de interrupção

    if(rise){ //caso a potência seja crescendo
        wrap += 35; //aumenta o nível de potência (28 us)
        if(wrap > 3000){ //caso o wrap seja menor que 3000
            wrap = 3000; //iguala wrap a 3000
            rise = false; //muda o flag rise para redução 
        }
    }
    else{ //caso a potência esteja caindo
        wrap -= 35; //diminui o nivel de potência (28 us)
        if(wrap < 625){ //caso o fade seja menor que 625
            wrap = 625; //iguala wrap a 625
            rise = true; //muda o flag rise para elevação no nível de iluminação
        }
    }

    pwm_set_gpio_level(PIN, wrap); //define o wrap 
    sleep_ms(10);
}

//Configuração do PWM com interrupção
void pwm_setup_irq(){

    pwm_clear_irq(slice); //resetar o flag de interrupção para o slice
    pwm_set_irq_enabled(slice, true); //habilitar a interrupção de PWM para um dado slice
    irq_set_exclusive_handler(PWM_IRQ_WRAP, wrapHandler); //Definir um tipo de interrupção.
    irq_set_enabled(PWM_IRQ_WRAP, true); //Habilitar ou desabilitar uma interrupção específica

    pwm_config config = pwm_get_default_config(); //obtem a configuração padrão para o PWM
    pwm_config_set_clkdiv(&config, PWM_DIVISER); //define o divisor de clock do PWM
    pwm_init(slice, &config, rotina); //inicializa o PWM com as configurações do objeto

}

//Função que deixa em 180 graus
void graus_180(){
    if(rotina == false){ 
        pwm_set_gpio_level(PIN, 3000); // duty cycle (12%) => 25000 * 0,12 = 3000(2.4ms)
        sleep_ms(5000);
    }
}

//Função que deixa em 90 graus
void graus_90(){
    if(rotina == false){ 
        printf("Entrou 2\n");
        pwm_set_gpio_level(PIN, 1837.5); // duty cycle (7,35%) => 25000 * 0,0735 = 1837.5(1.47ms)
        sleep_ms(5000);
    }
}

//Função que deixa em 0 graus
void graus_0(){
    if(rotina == false){ 
        pwm_set_gpio_level(PIN, 625); // duty cycle (2.5%) => 25000 * 0.025 = 625 (0.5 ms)
        sleep_ms(5000);
        rotina = true;
    }
}

//função principal
int main()
{
    stdio_init_all(); //inicializa o sistema padrão de I/O
    
    pwm_setup(); //configura o PWM

    graus_180();
    graus_90();
    graus_0();
    
    pwm_setup_irq(); //condigura o pwm

    if(rotina){
        pwm_set_irq_enabled(slice, true); //Inicializa a rotina do pwm
    }


    //loop principal
    while (true) {
        sleep_ms(100);
    }
}
