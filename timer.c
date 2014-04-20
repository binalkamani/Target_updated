//Will there be changes in timer.h?


void Timer0AIntHandler(void)
{
        float   cosine_val = 0;
        double  numerator_eqn = 0;

        int i, j;
        double dist_to_line = 0;

    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

        for(i = 0; i < no_of_balls; i++)
        {
                if(Ball_Tracker[i]) //Array to track presence of balls on screen
                {
//Compute new x, y:
                        cosine_val = (float)cos(bp.angle[i] * (PI/180.0f));
                        bp[i].x_cur_pos = (bp[i].int_velocity * cosine_val * t_val) + bp[i].x_cur_pos;
                        bp[i].y_cur_pos = (0.5 * g_val * t_val * t_val) + ((bp[i].int_velocity * sin(bp[i].angle * (PI/180.0f))) * t_val) + bp[i].y_cur_pos;

//if ball hits boundary
                        if((bp[i].x_cur_pos >= 1000) || (bp[i].y_cur_pos >= 1000) || (bp[i].x_cur_pos < 0) || (bp[i].y_cur_pos < 0))
                        {
                                Ball_Tracker[i] = 0;
                              //  num_present_balls--;	//nowhere defined??!!
                                continue;
                        }

//Compute new Vx, Vy:
                        bp[i].Vx = bp[i].int_velocity * cosine_val;
                        bp[i].Vy = (bp[i].int_velocity * sin(bp[i].angle * (PI/180.0f))) + (g_val * t_val);

//Update Velocity Magnitude, Angle:
                        bp[i].int_velocity = sqrt((bp[i].Vx * bp[i].Vx) + bp[i].Vy * bp[i].Vy));

                        if(bp[i].Vx)
                        {
                                bp[i].angle = (180.0f/PI) * atan(fabs(bp[i].Vy) / fabs(bp[i].Vx));

                                if((bp[i].Vx > 0) && (bp[i].Vy > 0))
                                {
                                        bp[i].angle = bp[i].angle;
                                }
                                else if((bp[i].Vx < 0) && (bp[i].Vy > 0))
                                {
                                        bp[i].angle = 180.0f - bp[i].angle;
                                }
                                else if((bp[i].Vx < 0) && (bp[i].Vy < 0))
                                {
                                        bp[i].angle = 180.0f + bp[i].angle;
                                }
                                else if((bp[i].Vx > 0) && (bp[i].Vy < 0))
                                {
                                        bp[i].angle = 360.0f - bp[i].angle;
                                }
                        }

//Collision Detection Part:

                        for(j = 1; j <= 2; j++)	//given number of lines
                        {
                                if((bp[i].x_cur_pos >= Line[j].x1) && (bp[i].x_cur_pos <= Line[j].x2))
                                {
                                        //dist_to_line = abs((A * Balls[i].x) + (B * Balls[i].y) + C) / sqrt((A * A) + (B * B));
                                        numerator_eqn = (Line[j].A * bp[i].x_cur_pos) + (Line[j].B * bp[i].x_cur_pos) + Line[j].C;
                                        if(numerator_eqn < 0)
                                        {
                                                numerator_eqn = numerator_eqn * (-1);
                                        }
                                        dist_to_line = numerator_eqn / sqrt((Line[j].A * Lines[j].A) + (Line[j].B * Line[j].B));

                                        bp[i].after_collision_flag = 0;
                                        if((dist_to_line - gRadius) <= 30)
                                        {
                                                bp[i].after_collision_flag = 1;
                                        }

                                        if(bp[i].collision_detection_flag)
                                        {
                                                bp[i].collision_detection_flag--;
                                        }

                                        if((bp[i].after_collision_flag == 1) && (!bp[i].collision_detection_flag))
                                        {
                                                bp[i].collision_detection_flag = 10;
                                                bp[i].angle = 90.0f + Line[j].Theta;
                                        }
                                }
                        }

#ifdef ENABLE_TRACE
                        fprintf(fp1, "%5.2f\t%5.2f\t%lf\t%lf\t%lf\t%lf\t%d\t%lf\t%lf\n", bp[i].x_cur_pos, bp[i].y_cur_pos, bp[i].Vx, bp[i].Vy, bp[i].int_velocity, bp[i].angle, bp[i].after_collision_flag, temp, dist_to_line);
#endif
                }
        }


		//This function needs to be implemented 
        UART_Send_Data();
      //  g_ulCounter1++; // used for cpu utilization
}

/* I don't understand importance of this part.

void Timer1BIntHandler(void)
{
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

        if(gNo_of_Balls_Entered < gMax_No_Balls_Given)
        {
                Ball_Tracker[gNo_of_Balls_Entered] = 1;         //New ball entry

                gNo_of_Balls_Entered++;                                         //Increment the number of balls entered on screen
                num_present_balls++;                                         //Increment the number of balls present on screen
        }

        //g_ulCounter2++;
    //printf("b%d\n", g_ulCounter2);

        return;
}

*/

void timer_init()
{
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

    TimerDisable(TIMER0_BASE, TIMER_A);
    TimerDisable(TIMER1_BASE, TIMER_A);

    //TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC);
    //TimerConfigure(TIMER1_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PERIODIC);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);
    TimerConfigure(TIMER1_BASE, TIMER_CFG_A_PERIODIC);

    TimerLoadSet(TIMER0_BASE, TIMER_A, gUART_Frequency * (SysCtlClockGet() / 1000));
    TimerLoadSet(TIMER1_BASE, TIMER_A, (gBall_Period * 1000 * (SysCtlClockGet() / 1000)));

    TimerIntRegister(TIMER0_BASE, TIMER_A, &Timer0AIntHandler);
    TimerIntRegister(TIMER1_BASE, TIMER_A, &Timer1BIntHandler);

    //IntEnable(INT_TIMER0A | INT_TIMER1B);
    IntEnable(INT_TIMER0A);
    IntEnable(INT_TIMER1A);

        TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
        TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    IntMasterEnable();

    //TimerEnable(TIMER0_BASE | TIMER1_BASE, TIMER_A | TIMER_B);
    TimerEnable(TIMER0_BASE, TIMER_A);
    TimerEnable(TIMER1_BASE, TIMER_A);
}
