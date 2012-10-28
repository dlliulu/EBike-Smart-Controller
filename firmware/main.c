/*
 * EBike Smart Controller
 *
 * Copyright (C) Jorge Pinto aka Casainho, 2012.
 *
 *   casainho [at] gmail [dot] com
 *     www.casainho.net
 *
 * Released under the GPL License, Version 3
 */

#include "lpc210x.h"
#include "system.h"
#include "isrsupport.h"
#include "timers.h"
#include "pwm.h"
#include "main.h"
#include "config.h"
#include "ios.h"
#include "adc.h"
#include "bldc.h"
#include "motor.h"

void initialize (void)
{
  system_init (); // initialize the LPC2103 (clocks, flash memory, etc)
  ios_init (); // configure IO pins

  pwm_init (); // initialize PWM (uses timer1)
  timer0_capture_init (); // intialize Timer0, use it for capture the Hall sensors signal time and BLDC control
  //timer3_init (); // intialize timer3 (used for delay function)
  enableIRQ (); // enable interrupts

  adc_init (CURRENT); // init the ADC for current measure
}

int main (void)
{
  unsigned int initial_duty_cycle;
  unsigned int duty_cycle;

  float motor_current;

  initialize ();

  initial_duty_cycle = duty_cycle = 100;
  //motor_set_duty_cycle (initial_duty_cycle);
  //motor_start ();

  while (1)
  {
    motor_current = motor_get_current ();
    if (motor_current > MOTOR_MAX_CURRENT)
    {
      if (duty_cycle > 0)
      {
        duty_cycle -= 1;
        motor_set_duty_cycle (duty_cycle);
      }
    }
    else if ((motor_get_current () < MOTOR_MAX_CURRENT) & (duty_cycle < initial_duty_cycle))
    {
      if (duty_cycle < 1000)
      {
        duty_cycle += 1;
        motor_set_duty_cycle (duty_cycle);
      }
    }
  }
}
