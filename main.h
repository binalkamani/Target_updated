#ifndef __MAIN_H__
#define __MAIN_H__

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_timer.h"

#include "driverlib/timer.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"

#include "utils/cpu_usage.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//******************************************************************
#define MAX_BALL_CNT (150)
#define MAX_LINE_CNT (2)
#define PI (3.141592654)
#define g_val (-9.8)

//global variables
extern double eqn1, eqn2, eqn3;
extern int no_of_balls;                 //Number of Balls
//extern int gBall_Period;              //Period of Balls Appearance
extern double time_val;                 //Timer Resolution for Ball Position Updation
extern double urat_freq;                //in milli sec
extern tboolean Ball_Tracker[MAX_BALL_CNT];


typedef struct ball_params{
	int no_of_balls;
	int period;
	int x_init_pos;
	int y_init_pos;

	int x_cur_pos;
	int y_cur_pos;
	int angle;
	double int_velocity;
	float Vx;
	float Vy;
	int after_collision_flag;
    tboolean collision_detection_flag; //Collision Detection Flag
}ball_params;

//Structure for Line Parameters
typedef struct line_params
{
		float x1;
		float y1;
		float x2;
		float y2;

        double  Slope;                  //Inclination Parameters
        double  Theta;

        double  eqn1;                              //Equation Parameters
        double  eqn2;
        double  eqn3;
}line_Params;


int no_of_balls;
char rx_char;
char rx_buff[10];
unsigned int rx_cnt=0;
unsigned int rx_param=0;
unsigned int val=0;
char runningSts;
void sendDouble(int ballNo, double xc, double yc);
void init(void);

#endif //  __MAIN_H__
