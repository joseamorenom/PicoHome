/**
 * \file        keypad_polling_irq.c
 * \brief
 * \details
 * \author      MST_CDA
 * \version     0.0.2
 * \date        03/05/2024
 * \copyright   Unlicensed
 */

#include "keypad_irq.h"
#include "app.h"

uint32_t gDBNC_TIME;

void kp_init(key_pad_t *kpad, uint8_t rlsb, uint8_t clsb, uint32_t dbnc_time, bool en){
    // Initialize history buffer
    for(int i=0; i<10;i++){
        kpad->history[i] = 0xFF;
    }
    // Verify that the keypad's rows and colums gpios do not overlap
    assert(abs(rlsb-clsb)>=4);
    kpad->KEY.rlsb = rlsb;
    kpad->KEY.clsb = clsb;
    kpad->KEY.ckey = 0x00;
    kpad->KEY.dkey = 0x1F;
    kpad->KEY.nkey = 0;
    kpad->KEY.dbnc = 0;
    kpad->dbnc_time = dbnc_time;
    gDBNC_TIME = dbnc_time;
    kpad->KEY.dzero = 0;
    kpad->KEY.en = en;
    kpad->pwm_slice = 0;

    // Initialize keypad gpios
    gpio_init_mask(0x0000000F << kpad->KEY.rlsb); // gpios for key rows 2,3,4,5
    gpio_init_mask(0x0000000F << kpad->KEY.clsb); // gpios for key cols 6,7,8,9
    
    kp_set_irq_cols(kpad);
}

void kp_decode(key_pad_t *kpad){
    switch (kpad->KEY.ckey)
    {
    case 0x88:
        kpad->KEY.dkey = 0x01;
        break;
    case 0x48:
        kpad->KEY.dkey = 0x02;
        break;
    case 0x28:
        kpad->KEY.dkey = 0x03;
        break;
    case 0x18:
        kpad->KEY.dkey = 0x0A;
        break;
    case 0x84:
        kpad->KEY.dkey = 0x04;
        break;
    case 0x44:
        kpad->KEY.dkey = 0x05;
        break;
    case 0x24:
        kpad->KEY.dkey = 0x06;
        break;
    case 0x14:
        kpad->KEY.dkey = 0x0B;
        break;
    case 0x82:
        kpad->KEY.dkey = 0x07;
        break;
    case 0x42:
        kpad->KEY.dkey = 0x08;
        break;
    case 0x22:
        kpad->KEY.dkey = 0x09;
        break;
    case 0x12:
        kpad->KEY.dkey = 0x0C;
        break;
    case 0x81:
        kpad->KEY.dkey = 0x0E;
        break;
    case 0x41:
        kpad->KEY.dkey = 0x00;
        break;
    case 0x21:
        kpad->KEY.dkey = 0x0F;
        break;
    case 0x11:
        kpad->KEY.dkey = 0x0D;
        break;
    }
    
}

void kp_capture(key_pad_t *kpad){

    // if (!kpad->KEY.en) return;

    kpad->KEY.ckey = (kpad->cols >> 2) | (kpad->rows >> 2);
    kp_decode(kpad);
    for(int i=0;i<9;i++){
        kpad->history[9-i] = kpad->history[9-i-1];
    }
    kpad->history[0] = kpad->KEY.dkey;
    kpad->KEY.nkey = 1;
}

void kp_set_irq_cols(key_pad_t *kpad) 
{

    // Disable rows IRQs
    kp_set_irq_enabled(kpad, false, false);

    // Set gpios as inputs and outputs
    gpio_set_dir_masked(0x0000000F << kpad->KEY.rlsb, 0x0000000F << kpad->KEY.rlsb); // rows as outputs
    gpio_set_dir_masked(0x0000000F << kpad->KEY.clsb, 0x00000000 << kpad->KEY.rlsb); // cols as inputs

    // Set pull down resistors to columns
    gpio_pull_down(kpad->KEY.clsb + 0);
    gpio_pull_down(kpad->KEY.clsb + 1);
    gpio_pull_down(kpad->KEY.clsb + 2);
    gpio_pull_down(kpad->KEY.clsb + 3);

    gpio_put_masked(0x0000000F << kpad->KEY.rlsb, 0x0000000F << kpad->KEY.rlsb); // rows as high

    // Initialize interrupts
    gpio_set_irq_enabled_with_callback(kpad->KEY.clsb + 0, GPIO_IRQ_EDGE_RISE, true, gpio_cb);
    gpio_set_irq_enabled_with_callback(kpad->KEY.clsb + 1, GPIO_IRQ_EDGE_RISE, true, gpio_cb);
    gpio_set_irq_enabled_with_callback(kpad->KEY.clsb + 2, GPIO_IRQ_EDGE_RISE, true, gpio_cb);
    gpio_set_irq_enabled_with_callback(kpad->KEY.clsb + 3, GPIO_IRQ_EDGE_RISE, true, gpio_cb);

}

void kp_set_irq_rows(key_pad_t *kpad) 
{
    // Disable columns IRQs
    kp_set_irq_enabled(kpad, true, false);

    // Set gpios as inputs and outputs
    gpio_set_dir_masked(0x0000000F << kpad->KEY.rlsb, 0x00000000 << kpad->KEY.rlsb); // rows as inputs
    gpio_set_dir_masked(0x0000000F << kpad->KEY.clsb, 0x0000000F << kpad->KEY.clsb); // cols as outputs

    // Set pull down resistors to rows
    gpio_pull_down(kpad->KEY.rlsb + 0);
    gpio_pull_down(kpad->KEY.rlsb + 1);
    gpio_pull_down(kpad->KEY.rlsb + 2);
    gpio_pull_down(kpad->KEY.rlsb + 3);

    gpio_put_masked(0x0000000F << kpad->KEY.clsb, 0x0000000F << kpad->KEY.clsb); // cols as high

    // Initialize interrupts
    // gpio_set_irq_enabled_with_callback(kpad->KEY.rlsb + 0, GPIO_IRQ_LEVEL_HIGH, true, gpioCallback);
    // gpio_set_irq_enabled_with_callback(kpad->KEY.rlsb + 1, GPIO_IRQ_LEVEL_HIGH, true, gpioCallback);
    // gpio_set_irq_enabled_with_callback(kpad->KEY.rlsb + 2, GPIO_IRQ_LEVEL_HIGH, true, gpioCallback);
    // gpio_set_irq_enabled_with_callback(kpad->KEY.rlsb + 3, GPIO_IRQ_LEVEL_HIGH, true, gpioCallback);
}



