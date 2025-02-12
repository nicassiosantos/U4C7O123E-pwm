# Controle de Servo Motor com Raspberry Pi Pico

Este projeto utiliza o **Raspberry Pi Pico** para controlar um **servo motor** através de sinais **PWM (Pulse Width Modulation)**, permitindo o ajuste preciso da angulação em 0°, 90° e 180°. Após posicionar o servo nestes ângulos, o código ativa uma rotina de interrupção para variar continuamente o ângulo, criando um movimento suave.

---

## Funcionalidades do Projeto

1. **Controle de Posições Fixas do Servo Motor:**
   - Posiciona o servo motor em **0 graus**, **90 graus** e **180 graus**, permanecendo em cada ângulo por 5 segundos.
2. **Rotina de Interrupção para Movimento Contínuo:**
   - Após ajustar as posições fixas, uma **interrupção PWM** é ativada para variar continuamente o ângulo do servo, gerando um movimento fluido de vai e vem.

---

## video do Projeto
- **[Ver Video](https://drive.google.com/file/d/1L20wqLJ6ExRvqR8A_QHNRRN3DfRTPWUk/view?usp=sharing)**
  
## Frequência e Cálculo do PWM

### Parâmetros Utilizados:
- **Frequência do PWM**: 50 Hz  
- **Divisor de Clock (PWM_DIVISER)**: 100
- **Período de Wrap (WRAP_PERIOD)**: 25000

### Cálculo da Frequência:
A frequência do PWM é calculada pela fórmula:

f_PWM = F_Clock / (PWM_DIVISER * WRAP_PERIOD)

Substituindo com os valores utilizados:

f_PWM = 125000000 / (100 * 25000) = 50 Hz 

### Cálculo do Duty Cycle para o Servo:
- 0 graus: Pulso de **0,5 ms** → 2,5% de Duty Cycle  
- 90 graus: Pulso de **1,47 ms** → 7,35% de Duty Cycle  
- 180 graus: Pulso de **2,4 ms** → 12% de Duty Cycle  

| Ângulo | Tempo do Pulso | Duty Cycle | Nível PWM |
|--------|----------------|------------|-----------|
| 0°     | 0,5 ms         | 2,5%       | 625       |
| 90°    | 1,47 ms        | 7,35%      | 1837.5    |
| 180°   | 2,4 ms         | 12%        | 3000      |

---

## Componentes Necessários

- **Raspberry Pi Pico**
- **Servo motor** (compatível com controle por PWM)
- **Jumpers** para conexão
- **Fonte de alimentação** (se necessário, dependendo do consumo do servo)

---

## Esquema de Conexão

- **GPIO 22 do Raspberry Pi Pico** → **Pino de Sinal do Servo Motor**
- **VCC do Servo Motor** → **3.3V ou 5V** (conforme especificação do servo)
- **GND do Servo Motor** → **GND do Raspberry Pi Pico**

---

## Estrutura do Código

O código está dividido nas seguintes funções:

### 1. `pwm_setup()`
- Configura o **GPIO 22** como saída PWM.
- Define o **divisor de clock** e o **período de wrap** para o PWM.
- Inicia o PWM com um nível inicial de 12% (correspondente a 180 graus).

### 2. `graus_180()`, `graus_90()`, `graus_0()`
- Ajustam o ciclo de trabalho do PWM para posicionar o servo em **180 graus**, **90 graus** e **0 graus**, respectivamente.
- Cada função mantém o servo no ângulo definido por **5 segundos** antes de prosseguir para o próximo ângulo.

### 3. `pwm_setup_irq()`
- Configura a **interrupção de PWM** para o canal correspondente ao GPIO utilizado.
- Define o **handler de interrupção** para variar continuamente o ângulo do servo após a execução das posições fixas.

### 4. `wrapHandler()`
- **Função de Interrupção** chamada quando o PWM atinge o valor de wrap.
- Varia o nível de PWM de forma crescente e decrescente, criando um movimento suave de vai e vem no ângulo do servo.

# Autor:
- **[Nicassio Santos.](https://github.com/nicassiosantos)**



