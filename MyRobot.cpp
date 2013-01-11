#include "WPILib.h"
#include<cmath>

//==================================================================

//The following will be changed manually through the code

float shoot_goal= .7;

//==================================================================

//The following will be modified by the code itself, but does not contribute to physical outputs

int shoot_error_count= 0;
bool shoot_encode_error= false;
bool shoot_enable= false;
float shoot_diff= 0;

//==================================================================

class RobotDemo : public SimpleRobot          //Here we are defining all of the hardware
{
	Joystick stick,stick1,stick2,stick3;      //Joysticks on the driverstation, left to right
	Victor shooter;				    		  //This is the controller for the different motors
	Encoder shoot_motor;
	PCVideoServer *pc;
	DriverStation *ds;			    		  //defines the space on the driver station that we can use to display data


public:
	RobotDemo(void):
		stick(1),
		stick1(2),
		stick2(3),
		stick3(4),
		
		shooter(1),
		
		shoot_motor(1,2,true),
		
		
		ds(DriverStation::GetInstance())
	{
		shoot_motor.Start();
	}
	void Autonomous(void)                     //Initiate auto code. This is the code that will be executed at the start of a match 
	{


	}

	void OperatorControl(void)				  //Initiate Tele-Op code. Here is the 2 minutes after auto
	{
		
		if(shoot_enable == true)
		{
			shooter.SetSpeed(.5);
		}	


		
		shoot_diff = shoot_goal - shoot_motor.Get();
	
		if(shoot_diff != 0)
		{
			
		}
		
	}

};

START_ROBOT_CLASS(RobotDemo);
