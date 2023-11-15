/////////////////////////////////////////////////////////////////////////////
// Course Number: CECS 347
// Assignment: Example project for Hardware PWM controlled Car
// Description: 
/////////////////////////////////////////////////////////////////////////////

//////////////////////1. Pre-processor Directives Section////////////////////
#include "tm4c123gh6pm.h"
#include "BitControlled.h"
#include "PWM.h"
#include "GPIO.h"
/////////////////////////////////////////////////////////////////////////////

//////////////////////2. Declarations Section////////////////////////////////

////////// Local Global Variables //////////


unsigned char wheelspeed = 0;
unsigned int speed = 8000;

////////// Function Prototypes //////////
void DisableInterrupts(void);
void EnableInterrupts(void);
void WaitForInterrupt(void);
void Delay(void);

//////////////////////3. Subroutines Section/////////////////////////////////
// MAIN: This main is meant for the command configuration of the hc05.
int main(void){ 
  LED_Init();
	UART_Init(); // Initialize UART1
  PortF_Init(); // Initilize for the three onboard LEDs
	UART0_OutString((unsigned char *)">>> Welcome to Bluetooth Controlled LED App <<<\n\r");
  Car_Dir_Init();
  PWM_PB76_Init();
  PWM_PB76_Duty(START_SPEED, START_SPEED);
	unsigned char control_symbol; // for bluetooth controlled LEDs
	
  while(1){
		
		control_symbol = UART1_InChar();
    UART0_OutChar(control_symbol);
		UART0_OutChar(CR);
    UART0_OutChar(LF);

    switch (control_symbol){
      case 'F':
      case 'f': 
        LED = Green;
				WHEEL_DIR = FORWARD;
				PWM0_ENABLE_R |= 0x00000003; // enable both wheels
        break; 
      case 'B':
      case 'b':
        LED = Blue;
				WHEEL_DIR = BACKWARD;
				PWM0_ENABLE_R |= 0x00000003; // enable both wheels
        break; 
      case 'L':
      case 'l':
        LED = Yellow;
				if (WHEEL_DIR==FORWARD) {
					PWM0_ENABLE_R |= 0x00000002; // Enable right wheel
					PWM0_ENABLE_R &= ~0x00000001; // Disable left wheel
				}else if(WHEEL_DIR==BACKWARD) {
					PWM0_ENABLE_R &= ~0x00000002; // Enable right wheel
					PWM0_ENABLE_R |= 0x00000001; // Disable left wheel
				}
        break; 
      case 'R':
      case 'r': 
        LED = Purple;
				if (WHEEL_DIR==FORWARD) {	
					PWM0_ENABLE_R &= ~0x00000002; // Enable right wheel
					PWM0_ENABLE_R |= 0x00000001; // Disable left wheel
				}else if(WHEEL_DIR==BACKWARD) {
					PWM0_ENABLE_R |= 0x00000002; // Enable right wheel
					PWM0_ENABLE_R &= ~0x00000001; // Disable left wheel
				}
        break; 
      case 'S':
      case 's':
        LED = Dark;
				PWM0_ENABLE_R &= ~0x00000003; // stop both wheels
        break; 
      case 'U':
      case 'u':
        // LED stays the same
				// wheel direction is the same	
				speed = speed + 1000;
				if(speed >= 16000) speed = 8000;
				PWM_PB76_Duty(speed,speed);
        break; 
      case 'D':
      case 'd':
        // LED stays the same
				// wheel direction is the same
				speed = speed - 1000;
				if(speed <= 2000) speed = 8000;
				PWM_PB76_Duty(speed,speed);
        break; 
      default:
        break;
    }
	
	}
}

