#include "main.h"
#include "timer.h"

ball_params bp[MAX_BALL_CNT];
line_params Line[MAX_LINE_CNT];

//different parameters
double t_val = 0.04;
double urat_freq = 40;

int num_present_balls = 0;


/* Initialize the current position to the initial position */
void computeBallInit()
{
	int i;	
	
	for(i=0; i<bp.no_of_balls; i++)
	{
		bp.x_cur_pos[i] = bp.x_init_pos;
		bp.y_cur_pos[i] = bp.y_init_pos;
		
		bp[i].Vx = 0;
		bp[i].Vy = 0;
		bp[i].angle = 270;
	}
	
	for(i = 1; i <= 2; i++)
        {
                Line[i].Slope = (Line[i].y2 - Line[i].y1) / (Line[i].x2 - Line[i].x1);
                Line[i].Theta = (180.0f/PI) * atan(Line[i].Slope);

                Line[i].eqn1 = (Line[i].y2 - Line[i].y1);
                Line[i].eqn2 = (Line[i].x1 - Line[i].x2);
                Line[i].eqn3 = (Line[i].x2 * Line[i].y1) - (Line[i].x1 * Line[i].y2);
        }

}

void computeBall()
{
	int i;
	int j;
	float sin_angle, cos_angle;
	
	// Main loop for ball computation
	while(1)
	{
		while(runningSts == 1)
		{
		
			// Sending Data in UART remaining
			sendDouble(1,(double)sin(30 * (PI/180.0f)),(double)sin(45 * (PI/180.0f)));
		//	runningSts = 0;
			// Testing sample animation from target
//			for(i=0;(i<2) ;i++)
//			{
//				UARTprintf("<1,%d,%d>",(i*25),(i*25));
//				for(j=0;j<0xfffff;j++);
//			    UARTprintf("<2,%d,%d>",(i*50),(i*50));
//				for(j=0;j<0xfff;j++);
//			}

		}
		
	}
	
	
}

void sendDouble(int ballNo, double xc, double yc)
{
    unsigned int q,w,e,r,t,y;
    unsigned int factor =1;

    q = xc/factor;
    w = xc*10/factor;
    w = w%10;
    e = xc*100/factor;
    e = e%10;
    r = yc/factor;
    t = yc*10/factor;
    t = t%10;
    y = yc*100/factor;
    y = y%10;

    UARTprintf("<%d,%d.%d%d,%d.%d%d>",ballNo,q,w,e,r,t,y);
}


void UARTIntHandler(void)
{
	float sine_val=0;
	int i;
	unsigned long ulStatus;
	ulStatus = ROM_UARTIntStatus(UART0_BASE, true);
	ROM_UARTIntClear(UART0_BASE, ulStatus);

	while(ROM_UARTCharsAvail(UART0_BASE))
	{
		rx_char=ROM_UARTCharGetNonBlocking(UART0_BASE);
		// Initialization values from host
		switch (rx_char)
		{
			case '<':
			{
//				UARTprintf("<");
				break;
			}
			case '>':
			{
//				UARTprintf(">");
			}
			case ',':
			{
				for(i=0;i<rx_cnt;i++)
				{
					val = val*10 + (rx_buff[i] - '0');
				}
				switch(rx_param)
				{
					case 0:
						bp.no_of_balls = val;
						break;
					case 1:
						bp.period = val;
						break;
					case 2:
						bp.x_init_pos = val;
						break;
					case 3:
						bp.y_init_pos = val;
						break;
					case 4:
						bp.l1_x1 = val;
						break;
					case 5:
						bp.l1_y1 = val;
						break;
					case 6:
						bp.l1_x2 = val;
						break;
					case 7:
						bp.l1_y2 = val;
						break;
					case 8:
						bp.l2_x1 = val;
						break;
					case 9:
						bp.l2_y1 = val;
						break;
					case 10:
						bp.l2_x2 = val;
						break;
					case 11:
						bp.l2_y2 = val;
						computeBallInit();
						break;
				}
				rx_param++;
				rx_cnt=0;
				val=0;
				break;
			}
			case 'S':
			{
//				UARTprintf("S");
				runningSts = 1;
				break;
			}
			case '.':
			{
//				UARTprintf(".");
				runningSts = 0;
				break;
			}
			default:
			{
				rx_buff[rx_cnt] = rx_char;
				rx_cnt++;
				break;
			}
		}
	}
}


void init()
{
	ROM_FPUEnable();
	ROM_FPULazyStackingEnable();
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
	SYSCTL_XTAL_16MHZ);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	ROM_IntMasterEnable();
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,
	(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
	UART_CONFIG_PAR_NONE));
	ROM_IntEnable(INT_UART0);
	ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
	UARTStdioInit(0);
	runningSts = 0;
}


void main(void)
{
	init();
	timer_init();
	UARTprintf("abcd");
	computeBallInit();
	//computeBall();
}
