/**
 * @authors Avik De <avikde@gmail.com>

  This file is part of koduino <https://github.com/avikde/koduino>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _timebase_h_
#define _timebase_h_

#include "timer.h"

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus


/** @addtogroup Timebase Timer interrupts
 *  @{
 */

// This is a general timer interrupt class, called as 
// 
// attachTimerInterrupt(i, ISR, freqHz)
// 
// where i = 0,1,2 for user functions.
// i=3 is used for pwmIn
// i=4 is used for the system clock (millis / micros)

// Make up Arduino-like functions
// Minimum freq ~= 0.061Hz, maximum ~= 36MHz)

/**
 * @brief Begin a timer interrupt
 * @details Starts calling the interrupt handler specified at the frequency specified.
 * 
 * @param i One of 0, 1, or 2 (#3 is used for pwmIn, and #4 is used for the [system clock](@ref Time))
 * @param ISR Interrupt handler (similar to usage for [external interrupts](@ref EXTI))
 * @param freqHz Frequency in Hz, between ~0.061Hz and ~36MHz.
 */
void attachTimerInterrupt(uint8_t i, ISRType ISR, int freqHz);

/**
 * @brief Disable a timer interrupt
 * @details This forgets the attached interrupt handler, i.e. to restart, attachTimerInterrupt() has to be called again!
 * 
 * @param i One of 0, 1, or 2
 */
void detachTimerInterrupt(uint8_t i);

/**
 * @brief Disable all timer interrupts
 * @details Call this to temporarily suspend all timer interrupts, and timerInterrupts() to resume them again
 */
void noTimerInterrupts();

/**
 * @brief Re-enable timer interrupts
 * @details To be called after noTimerInterrupts()
 */
void timerInterrupts();

/** @} */ // end of addtogroup


// General ISR for all timebase
static inline void timebaseISR(uint8_t i, uint8_t timer) __attribute__((always_inline, unused));
static inline void timebaseISR(uint8_t i, uint8_t timer) {
  if (TIM_GetITStatus(TIMER_MAP[timer].TIMx, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIMER_MAP[timer].TIMx, TIM_IT_Update);
    if (TIMEBASE_MAP[i].isr != 0)
      TIMEBASE_MAP[i].isr();
  }
}
// Specialized timer interrupt initialization functions
// 
// Uses a basic timer to setup up millisecond interrupts such that the period is 1000. OLD...now SysTick
// void attachSysClkInterrupt(ISRType ISR);
// Highest resolution for PWM in
// TIM_TypeDef *attachHighResClkUpdate(ISRType ISR);

/**
 * @brief Doesn't work quite right...mysterious multiples
 * @details [long description]
 * 
 * @param timer [description]
 * @param ISR [description]
 * @param usDelay [description]
 * @return [description]
 */
uint32_t attachTimerCallback(uint8_t timer, ISRType ISR, uint32_t usDelay);


#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif