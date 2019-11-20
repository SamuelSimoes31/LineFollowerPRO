#include "mbed.h"
#include "QTRSensors.h"
#include "TB6612FNG.h"

//#define BT

// QTR SENSORS *****************************************************************
#define NUM_SENSORS     8       // Número de sensores QTR
#define TIMEOUT         2500    // Tempo máximo de espera para a leitura dos sensores
//testar se dá para usar valores menores de timeout para acelurar o processo de leitura
#define EMITTER_PIN     (p12)   // Pino de controle dos leds
QTRSensorsRC sensors((PinName[]){p5,p6,p7,p8,p9,p10,p11,p12}, NUM_SENSORS, TIMEOUT, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];   // Valores lidos ods sensores
unsigned int linePos;
DigitalOut calibratingLED(LED1); 

// PID *************************************************************************
#define SETPOINT    3500    // O centro da linha 7000/2 = 3500
#define KP          0.2     // Constante Proporcional do PID
#define KD          1       // Constante Derivativa do PID
#define MAX_SPEED   1       // Velocidade máxima dos motores
#define SET_SPEED 0.8       // A velocidade "normal/ideal" dos motores
volatile float setSpeed=0.4;// Variável de velocidade
#define MIN_SPEED   0       // Velocidade mínima
int lastError = 0;          // Backup do erro do PID
int error_pid;              //Erro do PID
int adjust;

// MOTORES *********************************************************************
#define TB6612FNG_PIN_PWMA      (p26)
#define TB6612FNG_PIN_AIN1      (p24)
#define TB6612FNG_PIN_AIN2      (p25)
#define TB6612FNG_PIN_PWMB      (p21)
#define TB6612FNG_PIN_BIN1      (p23)
#define TB6612FNG_PIN_BIN2      (p22)
//#define TB6612FNG_PIN_NSTBY     (p27)
TB6612FNG motorDriver( TB6612FNG_PIN_PWMA, TB6612FNG_PIN_AIN1, TB6612FNG_PIN_AIN2,
                       TB6612FNG_PIN_PWMB, TB6612FNG_PIN_BIN1, TB6612FNG_PIN_BIN2);
#define fPwmPeriod 0.00002f
#define INITIAL_SPEED 0.4
float speedL, speedR;
Ticker tSoftStarter;


// SEGUNDO SENSOR ***************************************************************
InterruptIn ledDir(p5);
volatile int tests=0, confirms=0;
volatile bool run = true;
volatile bool firstFall = false;
volatile bool updatedValues;
Ticker tEnd;

void confirmEndLap(){
    if(updatedValues){
        if( linePos > 3200 && linePos < 3800 && sensorValues[0] < 400 && sensorValues[7] < 400){
            confirms++;
        }
        tests++;
    }
}

void endLapRise(){
    if(firstFall && tests==0) tEnd.attach(&confirmEndLap,0.01);
}

void endLapFall(){
    if(firstFall){
        tEnd.detach();
        if( tests != 0 && (confirms/(double)tests) > 0.9){
            run = false;
            motorDriver.stop();
        }
        tests = confirms = 0;            
    }
    firstFall = true;
}


//SERIAL ***********************************************************************
#ifdef BT
Serial bt(p9,p10);
#endif

int rangeSpeed(int v){
    if(v >= MAX_SPEED) return MAX_SPEED;
    else if (v <= MIN_SPEED) return MIN_SPEED;
    return v;
}

void pid(){
    updatedValues = false;
    linePos = sensors.readLine(sensorValues);
    updatedValues = true;
    
    error_pid = SETPOINT - linePos;
    adjust = error_pid*KP + KD*(error_pid - lastError);
    adjust /= 10000;
    speedL = rangeSpeed(setSpeed - adjust);
    speedR = rangeSpeed(setSpeed + adjust);
    lastError = error_pid;
    
    motorDriver.user_forward(speedL,speedR);
    
    //SERIAL
    #ifdef BT
    bt.printf("%d %d %d %d %d %d %d %d\n",sensorValues[0],sensorValues[1],sensorValues[2],sensorValues[3],sensorValues[4],sensorValues[5],sensorValues[6],sensorValues[7]);
    bt.printf("LinePos:%d Erro:%d adjust:%d | L:%f R:%f\n",linePos,error_pid,adjust,speedL,speedR);
    #endif
}

void softStarter(){
    setSpeed += 0.1;
    if(setSpeed == SET_SPEED) tSoftStarter.detach();
}

int main()
{
    #ifdef BT
    bt.baud(115200);
    #endif
    
    calibratingLED = 1;
    for(int i=0;i<10;i++){
        sensors.calibrate(QTR_EMITTERS_ON_AND_OFF);
        wait_ms(200);
    }
    calibratingLED = 0;
    wait(1);
    
    ledDir.rise(&endLapRise);
    ledDir.fall(&endLapFall);

    motorDriver.setPwmAperiod(fPwmPeriod);
    motorDriver.setPwmBperiod(fPwmPeriod);
        
    tSoftStarter.attach(&softStarter,0.1);

    while(run){
        pid();
    }
    while(1);
}
