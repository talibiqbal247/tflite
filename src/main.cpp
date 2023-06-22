/*
 * Copyright (c) 2021 Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 */

#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/pwm.h"


#include "tflite_model.h"

#include "ml_model.h"

MLModel ml_model(tflite_model, 128 * 1024);

int8_t* my_input = nullptr;
int main( void )
{
    // initialize stdio
    stdio_init_all();

    printf("hello pico fire alarm detection\n");

    gpio_set_function(PICO_DEFAULT_LED_PIN, GPIO_FUNC_PWM);
    
    uint pwm_slice_num = pwm_gpio_to_slice_num(PICO_DEFAULT_LED_PIN);
    uint pwm_chan_num = pwm_gpio_to_channel(PICO_DEFAULT_LED_PIN);

    // Set period of 256 cycles (0 to 255 inclusive)
    pwm_set_wrap(pwm_slice_num, 256);

    // Set the PWM running
    pwm_set_enabled(pwm_slice_num, true);

    if (!ml_model.init()) {
        printf("Failed to initialize ML model!\n");
        while (1) { tight_loop_contents(); }
    }
    my_input = (int8_t*)ml_model.input_data();
    int8_t inp[] = {30, 7, 1, 4, 60, 2, 4, 2, 9, 1};
    int i=0;
    //my_input = static_cast<int8_t*>(ml_model.input_data());
    while (1) {
        // wait for new samples
        i++;
        i= i%10;

        ml_model.set_input_data((float)inp[i]);
        //int8_t *t  = (int8_t*)ml_model.input_data();
        printf("%d", inp[i]);
    
        float prediction = ml_model.predict();
        
        
        printf("\t(prediction = %f)\n\n", prediction);


        pwm_set_chan_level(pwm_slice_num, pwm_chan_num, prediction * 255);
    }

    return 0;
}
