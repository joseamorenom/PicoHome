/**
 * \file        keypad.h
 * \brief
 * \details
 * \author      PicoHome
 * \version     0.0.2
 * \date        03/05/2024
 * \copyright   Unlicensed
 */

#ifndef __KEYPAD_POLLING_IRQ_
#define __KEYPAD_POLLING_IRQ_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "pico/time.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/timer.h"

#include "app.h"

/**
 * \typedef key_pad_t
 * \brief Data strcuture to manage a matrix keypad of 16 keys
 */
typedef struct{
    struct key{
    uint8_t ckey    : 8;        ///< captured key with position coding
    uint8_t dkey    : 5;        ///< captured key with decimal coding
    uint8_t rlsb    : 4;        ///< The rows LSB position for keypad rows, the four gpios for rows must be consecutives starting in position rlsb
    uint8_t clsb    : 4;        ///< The cols LSB position for keypad cols, the four gpios for cols must be consecutives starting in position clsb
    uint8_t en      : 1;        ///< Enable keypad processing
    uint8_t dzero   : 1;        ///< Flag for double zero
    uint8_t nkey    : 1;        ///< Flag that indicates that a key was pressed
    uint8_t dbnc    : 1;        ///< Flag that indicates that debouncer is active
    }KEY;                       ///< All key related information              
    uint8_t history[10];        ///< The last 10 pressed keys
    uint32_t dbnc_time;         ///< Debouncer time
    uint32_t rows;              ///< Value of the gpios connected to the keypad rows
    uint32_t cols;              ///< Value of the gpios connected to the keypad cols
    uint8_t pwm_slice;          ///< PWM slice number
}key_pad_t;


/**
 * @brief This method initializes the keypad data structure
 * 
 * @param kpad pointer to keypad data structure
 * @param rlsb LSB position of the first row GPIO
 * @param clsb LSB position of the first col GPIO
 * @param dbnc_time period for keypad debouncer
 * @param en True if keypad start enabled
 */
void kp_init(key_pad_t *kpad, uint8_t rlsb, uint8_t clsb, uint32_t dbnc_time, bool en);

/** 
 * \brief This method decodes the postional coding of the key to its actual value in decimal
 * \param kpad   Pointer to keypad data structure
 */
void kp_decode(key_pad_t *kpad);

/**
 * \brief This method captures the currently pressed key. 
 */
void kp_capture(key_pad_t *kpad);

/**
 * @brief This method enables or disables the keypad IRQs
 * 
 * @param kpad 
 * @param cols if true, the columns IRQs are enabled or disabled, if false, the rows IRQs are enabled or disabled
 * @param en 
 */
static inline void kp_set_irq_enabled(key_pad_t *kpad, bool cols ,bool en)
{
    if (cols) {
        for (int i = 0; i < 4; i++){
            gpio_set_irq_enabled(kpad->KEY.clsb + i, GPIO_IRQ_EDGE_RISE, en);
        }
    }else {
        for (int i = 0; i < 4; i++){
            gpio_set_irq_enabled(kpad->KEY.rlsb + i, GPIO_IRQ_EDGE_RISE, en);
        }
    }
}

/**
 * @brief Configures the keypad to capture the columns interrupts as rising edge
 * 
 * @param kpad 
 */
void kp_set_irq_cols(key_pad_t *kpad);

/**
 * @brief Configures the keypad to capture the rows interrupts as level high
 * 
 * @param kpad 
 */
void kp_set_irq_rows(key_pad_t *kpad);

/** 
 * \brief This method returns the value of the last pressed key in decimal coding
 * \param kpad   Pointer to keypad data structure
 */
static inline uint8_t kp_get_key(key_pad_t *kpad){
    kpad->KEY.nkey = false;
    return kpad->history[0];
}

/** 
 * \brief This method allows to access the last 10 pressed keys
 * \param kpad   Pointer to keypad data structure
 */
static inline uint8_t kp_get_keyh(key_pad_t *kpad, uint8_t n){
    return kpad->history[n%10];
}

/** 
 * \brief This method returns the value of the key with its positional coding
 * \param kpad   Pointer to keypad data structure
 */
static inline uint8_t kp_get_keyc(key_pad_t *kpad){
    return kpad->KEY.ckey;
}

/** 
 * \brief This method sets to one the flag that indicates that a zero was detected on keypad columns
 * \param kpad   Pointer to keypad data structure
 */
static inline void kp_set_zflag(key_pad_t *kpad){
    kpad->KEY.dzero = 1;
}
/** 
 * \brief This method clears the flag that indicates that a zero was detected on keypad columns
 * \param kpad   Pointer to keypad data structure
 */
static inline void kp_clr_zflag(key_pad_t *kpad){
    kpad->KEY.dzero = 0;
}

/** 
 * \brief This method returns true if a first zero was detected on keypad columns
 * \param kpad   Pointer to keypad data structure
 */
static inline bool kp_is_2nd_zero(key_pad_t *kpad){
    return kpad->KEY.dzero;
}

#endif // __KEYPAD_POLLING_IRQ_