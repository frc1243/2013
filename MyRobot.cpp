#include "WPILib.h"
#include<cmath>

//==================================================================

//The following will be changed manually through the code

float shoot_goal= 3000;


//==================================================================

//The following will be modified by the code itself, but does not contribute to physical outputs

int shoot_error_count= 0;
bool shoot_error= false;
bool shoot_enable= false;
float shoot_diff= 0;
float shoot_last= 0;
bool shoot_auto= false;
bool shoot_locked= false;
float shoot_volt= 0.00;

//==================================================================


class RobotDemo : public SimpleRobot          //Here we are defining all of the hardware
{
	Joystick stick,stick1,stick2,stick3;      //Joysticks on the driverstation, left to right
	Victor shoot_motor;				    		  //This is the controller for the different motors
	Encoder shoot_encode;
	PCVideoServer *pc;
	DriverStation *ds;			    		  //defines the space on the driver station that we can use to display data


public:
	RobotDemo(void):
		stick(1),
		stick1(2),
		stick2(3),
		stick3(4),
		
		shoot_motor(1),
		
		shoot_encode(1,2,true),
		
		
		ds(DriverStation::GetInstance())
	{
		shoot_encode.Start();
	}
	void Autonomous(void)                     //Initiate auto code. This is the code that will be executed at the start of a match 
	{


	}
	

	void OperatorControl(void)				  //Initiate Tele-Op code. Here is the 2 minutes after auto
	{
		
//==================================================================
		//Shooter Code
/*

		if((shoot_enable == true) and (shoot_auto == true))
		{
			shoot_motor.SetSpeed(shoot_volt);
		}	

*/
		
//==================================================================
		//Shooter Error Checking
		shoot_last = shoot_encode.GetRate();			// Saves the current encoder reading for future use
		
		if(shoot_encode.GetRate() == shoot_last)		// Encoder readings should be jumpy, so if it's the same exact value twice in a row....
		{
			
			shoot_error_count = shoot_error_count + 1;	
			
			if(shoot_error_count >= 10);			// is the error count greater or equal to 10
			shoot_error = true;				
			
			shoot_auto = false;				// enables driver to manually controll the shooter
		}
		
//		shoot_diff = abs(shoot_goal - abs(shoot_encode.GetRate()));
		
	}

};

START_ROBOT_CLASS(RobotDemo);
