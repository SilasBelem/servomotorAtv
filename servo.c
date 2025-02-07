#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define PWM_GPIO 22
#define WRAP_VALUE 25000  // 50Hz (20ms período)

#define PULSE_180 1920  // 180 graus
#define PULSE_90  1190  // 90 graus
#define PULSE_0   400   // 0 graus

#define SWEEP_STEP 5   // Incremento para movimento suave
#define SWEEP_DELAY 10 // Tempo entre passos (10ms)

void set_pwm_pulse(uint gpio, uint16_t pulse_width) {
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    uint16_t level = (pulse_width * WRAP_VALUE) / 20000;  
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(gpio), level);
}

int main() {
    stdio_init_all();
    gpio_set_function(PWM_GPIO, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(PWM_GPIO);

    pwm_set_wrap(slice_num, WRAP_VALUE);
    pwm_set_clkdiv(slice_num, 125.0f);
    pwm_set_enabled(slice_num, true);

    
    set_pwm_pulse(PWM_GPIO, PULSE_180);
    sleep_ms(5000);

    set_pwm_pulse(PWM_GPIO, PULSE_90);
    sleep_ms(5000);

    set_pwm_pulse(PWM_GPIO, PULSE_0);
    sleep_ms(5000);

    while (true) {
        for (uint16_t pulse = PULSE_0; pulse <= PULSE_180; pulse += SWEEP_STEP) {
            set_pwm_pulse(PWM_GPIO, pulse);
            sleep_ms(SWEEP_DELAY);
        }
        for (uint16_t pulse = PULSE_180; pulse >= PULSE_0; pulse -= SWEEP_STEP) {
            set_pwm_pulse(PWM_GPIO, pulse);
            sleep_ms(SWEEP_DELAY);
        }
    }
}
