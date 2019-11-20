/* File: TB6612FNG.h
 * Author: Robert Abad      Copyright (c) 2013
 *
 * Desc: driver for TB6612FNG Motor Driver.  For further details see
 *       header file, TB6612FNG.h
 */

#include "mbed.h"
#include "TB6612FNG.h"

#define SIGNAL_HIGH     (1)
#define SIGNAL_LOW      (0)

TB6612FNG::TB6612FNG( PinName pinPwmA, PinName pinAin1, PinName pinAin2,
                      PinName pinPwmB, PinName pinBin1, PinName pinBin2
                      ) :
pwmA(pinPwmA),
Ain1(pinAin1),
Ain2(pinAin2),
pwmB(pinPwmB),
Bin1(pinBin1),
Bin2(pinBin2)
{
    Ain1 = SIGNAL_LOW;
    Ain2 = SIGNAL_LOW;
    Bin1 = SIGNAL_LOW;
    Bin2 = SIGNAL_LOW;
    pwmA.period(TB6612FNG_PWM_PERIOD_DEFAULT);
    pwmA = TB6612FNG_PWM_PULSEWIDTH_DEFAULT;
    pwmB.period(TB6612FNG_PWM_PERIOD_DEFAULT);
    pwmB = TB6612FNG_PWM_PULSEWIDTH_DEFAULT;
}

void TB6612FNG::setPwmA(float fPeriod, float fPulsewidth)
{
    pwmA.period(fPeriod);
    pwmA = fPulsewidth;
}

void TB6612FNG::setPwmAperiod(float fPeriod)
{
    pwmA.period(fPeriod);
}

void TB6612FNG::setPwmApulsewidth(float fPulsewidth)
{
    pwmA = fPulsewidth;
}

void TB6612FNG::setPwmB(float fPeriod, float fPulsewidth)
{
    pwmB.period(fPeriod);
    pwmB = fPulsewidth;
}

void TB6612FNG::setPwmBperiod(float fPeriod)
{
    pwmB.period(fPeriod);
}

void TB6612FNG::setPwmBpulsewidth(float fPulsewidth)
{
    pwmB = fPulsewidth;
}

//void TB6612FNG::standby(void)
//{
//    nStby = SIGNAL_LOW;
//}

void TB6612FNG::forward(void)
{
    Ain1 = SIGNAL_LOW;
    Ain2 = SIGNAL_HIGH;
    // nStby = SIGNAL_HIGH;
    Bin1 = SIGNAL_LOW;
    Bin2 = SIGNAL_HIGH;
    // nStby = SIGNAL_HIGH;
}

void TB6612FNG::right(void)
{
    Ain1 = SIGNAL_LOW;
    Ain2 = SIGNAL_HIGH;
    // nStby = SIGNAL_HIGH;
    Bin1 = SIGNAL_HIGH;
    Bin2 = SIGNAL_LOW;
    // nStby = SIGNAL_HIGH;        
}

void TB6612FNG::left(void)
{
    Ain1 = SIGNAL_HIGH;
    Ain2 = SIGNAL_LOW;
    // nStby = SIGNAL_HIGH;
    Bin1 = SIGNAL_LOW;
    Bin2 = SIGNAL_HIGH;
    // nStby = SIGNAL_HIGH;       
}

void TB6612FNG::backward(void)
{
    Ain1 = SIGNAL_HIGH;
    Ain2 = SIGNAL_LOW;
    // nStby = SIGNAL_HIGH;
    Bin1 = SIGNAL_HIGH;
    Bin2 = SIGNAL_LOW;
    // nStby = SIGNAL_HIGH;    
}

void TB6612FNG::stop(void)
{
    Ain1 = SIGNAL_LOW;
    Ain2 = SIGNAL_LOW;
    // nStby = SIGNAL_HIGH;
    Bin1 = SIGNAL_LOW;
    Bin2 = SIGNAL_LOW;
    // nStby = SIGNAL_HIGH;    
}

void TB6612FNG::motorA_stop(void)
{
    Ain1 = SIGNAL_LOW;
    Ain2 = SIGNAL_LOW;
}

void TB6612FNG::motorA_ccw(void)
{
    Ain1 = SIGNAL_LOW;
    Ain2 = SIGNAL_HIGH;
    // nStby = SIGNAL_HIGH;
}

void TB6612FNG::motorA_cw(void)
{
    Ain1 = SIGNAL_HIGH;
    Ain2 = SIGNAL_LOW;
    // nStby = SIGNAL_HIGH;
}

void TB6612FNG::motorB_stop(void)
{
    Bin1 = SIGNAL_LOW;
    Bin2 = SIGNAL_LOW;
}

void TB6612FNG::motorB_ccw(void)
{
    Bin1 = SIGNAL_LOW;
    Bin2 = SIGNAL_HIGH;
    // nStby = SIGNAL_HIGH;
}

void TB6612FNG::motorB_cw(void)
{
    Bin1 = SIGNAL_HIGH;
    Bin2 = SIGNAL_LOW;
    // nStby = SIGNAL_HIGH;
}

void TB6612FNG::user_forward(float l, float r)
{
    pwmB=l;
    pwmA=r;
    Ain1 = SIGNAL_LOW;
    Ain2 = SIGNAL_HIGH;
    // nStby = SIGNAL_HIGH;
    Bin1 = SIGNAL_LOW;
    Bin2 = SIGNAL_HIGH;
}

void TB6612FNG::user_left(float l, float r)
{
    pwmB=l;
    pwmA=r;
    Ain1 = SIGNAL_HIGH;
    Ain2 = SIGNAL_LOW;
    // nStby = SIGNAL_HIGH;
    Bin1 = SIGNAL_LOW;
    Bin2 = SIGNAL_HIGH;
}
void TB6612FNG::user_right(float l, float r)
{
    pwmB=l;
    pwmA=r;
    Ain1 = SIGNAL_LOW;
    Ain2 = SIGNAL_HIGH;
    // nStby = SIGNAL_HIGH;
    Bin1 = SIGNAL_HIGH;
    Bin2 = SIGNAL_LOW;
}

void TB6612FNG::user_backward(float l, float r)
{
    pwmB=l;
    pwmA=r;
    Ain1 = SIGNAL_HIGH;
    Ain2 = SIGNAL_LOW;
    Bin1 = SIGNAL_HIGH;
    Bin2 = SIGNAL_LOW;
}


