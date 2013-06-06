#include "WPILib.h" //IMPORTANT
#include <cmath> //IMPORTANT

static 		DriverStationLCD *dsLCD = DriverStationLCD::GetInstance();
static   	AxisCamera &camera = AxisCamera::GetInstance("10.12.43.11");

//---------INIT VARIABLES

	//Auton Mode, change default here
	//1 = back left
	//10 = back right
	//3 = back center
	//4 = front left*
	//5 = front right*
	//6 = front center*
	//* = not programmed, expect error
	//0 & x>7 will display error
	float autonmode = 10;

	//Disk Shot Times
	//will determins what time period in auton
	//disks will be fired
	//!!REMEMBER!! to keep values at least 2 seconds
	//apart to give time for cylinder to respond
	int disk1 = 4;
	int disk2 = 6;
	int disk3 = 8;

	//rpm setpoint
	float balanced = 0;
	float rpmgain= 400;
	float shootv = 0;
	float egainval = 3600;
	float errorgain = 0;
	//bias
	float shotset = 0;

	//drivetrain
	bool decide1 = true;
	bool decide2 = false;
	bool frontflip = false;

	//global variables
	//you should not have to change these
	float rpmraw = 0;
	float anglepoint = 0;
	bool lastpress = false;
	float debug = 0;
	float lightbal = .09;
	float abalanced = 0;
	float shooterset = 0;
	float shootset  = 0;
	
	//gyro calibration variables
	bool button6 = true;
	bool button7 = false;
	float potset = 0;
	float bias = 1.1932;
	float brads = 0;

class RobotDemo : public SimpleRobot
{
	Joystick leftjoy, rightjoy, copjoy;
	Talon left1, right1, left2, right2, climber;
	Victor angle, shooter;
	Gyro sgyro;
	AnalogChannel gyro, anglepot;
	Solenoid highgear, lowgear, clawopen, clawclosed, kickin, shoothigh, shootlow;
	Encoder leftdrive, rightdrive, shooterenc, climberenc;
	Compressor comp;
	Timer autontimer, rpmtime, foottime, shoottime, randomtimer, gyroreset, lighttime, gcal, teletimer, errortimer;
	DriverStation *ds;
	Dashboard *db;

public:
	RobotDemo(void):
		//Joystick
		leftjoy(1),
		rightjoy(2),
		copjoy(3),
		//Talon
		left1(1),
		right1(2),
		left2(3),
		right2(4),
		climber(5),
		//Victor
		angle(6),
		shooter(7),
		//Gyro
		sgyro(1),
		//Analog
		gyro(2),
		anglepot(3),
		//Solenoid
		highgear(1),
		lowgear(2),
		clawopen(3),
		clawclosed(4),
		kickin(5),
		shoothigh(8),
		shootlow(7),
		//Encoder
		leftdrive(2, 4),
		rightdrive(1, 3),
		shooterenc(8, 9),
		climberenc(6, 7),
		comp(1, 1),
		//Timer
		autontimer(),
		rpmtime(),
		foottime(),
		shoottime(),
		randomtimer(),
		gyroreset(),
		lighttime(),
		gcal(),
		teletimer(),
		errortimer(),
		//DriverStation
		ds(DriverStation::GetInstance())

	{
		clawclosed.Set(1);
		clawopen.Set(0);
		//dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "AutonMode:%1.0f", autonmode);
		dsLCD->UpdateLCD();
	}

	void Autonomous(void)
	{
		autontimer.Reset();
		autontimer.Stop();
		shoottime.Start();
		//gcal.Start();
		shooterenc.Start();
		rpmtime.Start();
		comp.Stop();
		//float potbal = .65;
		dsLCD->UpdateLCD();
		float autonsetpoint = (2.9400);
		float autongain = 2.6;
		
		shoothigh.Set(0);
		shootlow.Set(1);
		while (IsAutonomous()) {
			float atimerval = (autontimer.Get());
			
			if (copjoy.GetRawButton(2)){
				autongain = autongain -.0005;
			}
			if (copjoy.GetRawButton(3)){
				autongain = autongain +.0005;
			}
			
			
			autontimer.Start();
			shooter.Set(1);
			
			potset = ((autonsetpoint-anglepot.GetVoltage())*autongain);
			if (potset>=1){
				potset= 1;
			}
			if (potset<=-1){
				potset= -1;
			}
			

			if ((atimerval>=disk1) and (atimerval<disk1+.25)){
				kickin.Set(1);
				
			}else if ((atimerval>=disk2) and (atimerval<=disk2+.25)){
				kickin.Set(1);
			
			}else if ((atimerval>=disk3) and (atimerval<=disk3+.25)){
				kickin.Set(1);
				
			}else{
				kickin.Set(0);
			
			}
			
			if (atimerval > disk3 +2){
				comp.Start();
			}
			
			angle.Set(potset*-1);
			
			dsLCD->Printf(DriverStationLCD::kUser_Line2, 1, "anglevoltage%f", (angle.Get()));
			dsLCD->Printf(DriverStationLCD::kUser_Line3, 1, "potvolt%f", (anglepot.GetVoltage()));
			dsLCD->Printf(DriverStationLCD::kUser_Line4, 1, "errorpot%f", potset);
			dsLCD->Printf(DriverStationLCD::kUser_Line5, 1, "autonsetpoint%f", autonsetpoint);
			dsLCD->Printf(DriverStationLCD::kUser_Line6, 1, "autongain%f", autongain);
				
			dsLCD->UpdateLCD();
		
		}
		while (IsDisabled())
		{
			dsLCD->UpdateLCD();

		}
		
	}



	void OperatorControl(void)
	{
		shoothigh.Set(0);
		shootlow.Set(1);
		errorgain = 0;
		teletimer.Start();
		errortimer.Reset();
		//float angle2last = 0;
		float left=0;
		float right=0;

		shoottime.Start();
		shooterenc.Start();
		rpmtime.Start();
		randomtimer.Reset();
		climberenc.Start();
		gyroreset.Start();

		highgear.Set(1);
		lowgear.Set(0);
		kickin.Set(0);
		clawopen.Set(0);
		clawclosed.Set(1);
		gcal.Start();
		//float potkeep;
		float potsetpoint = anglepot.GetVoltage();
		float cop3axis = .5;
		
		bool lastkick = false;
		comp.Start();
		
		while (IsOperatorControl())
		{

			//------------------Compresor------------------------------
			
			if (copjoy.GetRawButton(7) == true) {
				comp.Stop();
			}
			if (copjoy.GetRawButton(8) == true) {
				comp.Start();
			}
			
			if (teletimer.Get() < .001){
				shooter.Set(0);
			}
			
			
			if (leftjoy.GetRawButton(6) == true) {
				button6 = true;
				button7 = false;
			}
			if (leftjoy.GetRawButton(7) == true) {
				button6 = false;
				button7 = true;
			}
			if (button6) {
				highgear.Set(1);
				lowgear.Set(0);
				//dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "High Gear");
			}
			if (button7) {
				highgear.Set(0);
				lowgear.Set(1);
				//dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "Low Gear");
			}

			cop3axis = (((copjoy.GetRawAxis(4)-1)/2)*-1);
			float cop2axis = (copjoy.GetRawAxis(3)*1.125);
			//float cop2axis2 = (copjoy.GetRawAxis(2));

			if (ds->GetDigitalIn(7) == true) {
				//----------------------Shooter Selection-----------------------
			
				if (ds->GetDigitalIn(1) == false) {
					//balanced = 0;
					//shooterset = 0;
					potsetpoint = 2.6400;
					ds->SetDigitalOut(6, true);
					ds->SetDigitalOut(5, false);
					ds->SetDigitalOut(4, false);
					ds->SetDigitalOut(3, false);
					ds->SetDigitalOut(2, false);

				}
				if (ds->GetDigitalIn(2) == false) {
					//balanced = 2600;
					///shooterset = .5;
					potsetpoint = 2.5600;
					ds->SetDigitalOut(6, false);
					ds->SetDigitalOut(5, true);
					ds->SetDigitalOut(4, false);
					ds->SetDigitalOut(3, false);
					ds->SetDigitalOut(2, false);

				}
				if (ds->GetDigitalIn(3) == false) {
					//shooterset = 1.0;
					potsetpoint = 3.2000;
				
					ds->SetDigitalOut(6, false);
					ds->SetDigitalOut(5, false);
					ds->SetDigitalOut(4, true);
					ds->SetDigitalOut(3, false);
					ds->SetDigitalOut(2, false);
				}
				if (ds->GetDigitalIn(4) == false){
					//shooterset = 1.0;
					potsetpoint = 2.9500;

					ds->SetDigitalOut(6, false);
					ds->SetDigitalOut(5, false);
					ds->SetDigitalOut(4, false);
					ds->SetDigitalOut(3, true);
					ds->SetDigitalOut(2, false);
				}
				if (ds->GetDigitalIn(5) == false){
					//shooterset = 1.0;
					potsetpoint = 2.9500;
					ds->SetDigitalOut(6, false);
					ds->SetDigitalOut(5, false);
					ds->SetDigitalOut(4, false);
					ds->SetDigitalOut(3, false);
					ds->SetDigitalOut(2, true);
				}
				
				//shotset = ((rightjoy.GetRawAxis(3)-1)/2);
				
				if (copjoy.GetRawButton(6)){
					shoothigh.Set(1);
					shootlow.Set(0);
				}
				if (copjoy.GetRawButton(5)){
					shoothigh.Set(0);
					shootlow.Set(1);
				}
	
				/*
				if (cop2axis2 > .6){
					cop2axis = (.6 * 1.125);
				} else if (cop2axis2 < -.65){
					cop2axis = (-.65 * 1.125);
				}
				*/
				if (cop2axis > 0.15){
					potset = (cop2axis*cop2axis*cop2axis/2);
					randomtimer.Start();
				}
				if (cop2axis < -0.15){
					potset = (cop2axis*cop2axis*cop2axis/2);
					randomtimer.Start();
				}
												
					if ((cop2axis > -0.15) and (cop2axis < 0.15)){
						if (randomtimer.Get() >= 0.1){
							potsetpoint = anglepot.GetVoltage();
							randomtimer.Stop();
							randomtimer.Reset();
						}
			
						potset = (potsetpoint-anglepot.GetVoltage());
						
						if ((potset > .005) or (potset < -.005)){
							errortimer.Start();
							if (errortimer.Get() >= .1){
								if (potset < 0){
									potset = potset -.005;	
									errortimer.Stop();
									errortimer.Reset();
									
								}
								if (potset > 0){
									potset = potset +.005;
									errortimer.Stop();
									errortimer.Reset();
									
								}
								
							}
							
						}
						if (potset>=1){
							potset= 1;
						}
						if (potset<=-1){
							potset= -1;
						}
						}
					
					angle.Set(potset*-1);
					
				} else if (ds->GetDigitalIn(7) == false){
				
				if (copjoy.GetRawButton(6)){
					shoothigh.Set(1);
					shootlow.Set(0);
				}
				if (copjoy.GetRawButton(5)){
					shoothigh.Set(0);
					shootlow.Set(1);
				}
				
					shotset = ((rightjoy.GetRawAxis(3)-1)/-2);
					
				//angle.Set(((copjoy.GetRawAxis(2)*copjoy.GetRawAxis(2)*copjoy.GetRawAxis(2))/2)*-1);
					angle.Set((copjoy.GetRawAxis(3)*copjoy.GetRawAxis(3)*copjoy.GetRawAxis(3)));
			
			}

		//-----------------drivetrain--------------------------	
				if (rightjoy.GetRawButton(11) == true){
					frontflip=false;	
				}
				if (rightjoy.GetRawButton(10) == true)
				{
					frontflip=true;
				}

				left = (leftjoy.GetRawAxis(2));
				right = (rightjoy.GetRawAxis(2));

				if (frontflip == false) {
					left1.Set(left*-1);
					left2.Set(left*-1);
					right1.Set(right*0.974);
					right2.Set(right*0.974);
				}
				if (frontflip == true) {
					left1.Set(right);
					left2.Set(right);
					right1.Set(left*-0.974);
					right2.Set(left*-0.974);
				}



			//---------------Shooter-----------------------
			//rpm calc
				
			if (rpmtime.Get() >= 0.1) {
				rpmraw = (((shooterenc.GetDistance()/250)/rpmtime.Get())*60);
				rpmtime.Reset();
				shooterenc.Reset();
			}

			if (copjoy.GetRawButton(4) == true){
				clawopen.Set(1);
				clawclosed.Set(0);
			}
			if (copjoy.GetRawButton(3) == true){
				clawopen.Set(0);
				clawclosed.Set(1);
			}
			//------Fire Disk-------
			if (copjoy.GetRawButton(1) == true){
				kickin.Set(1);
				lastkick = true;
				}
			else{
				kickin.Set(0);
				lastkick = false;
			}

/*
			//if autoshooting is enabled

			if (ds->GetDigitalIn(7)==true){

			float highpoint = (balanced*.2);

			if (rpmraw<highpoint){
				shootv = 0.4;
			}else{

			//--------------PID for precise control    ////////spellfail
				errorgain = ((balanced-rpmraw)/egainval);

				if (errorgain < 0){
					errorgain = (errorgain*-1);
				}

				errorgain=errorgain/rpmgain;

				if ((balanced>rpmraw))
					{
					shootv= (shootv+errorgain);
					if (shoottime.Get()>0.001)
						{
							shootv= (shootv+errorgain);
							shoottime.Reset();
						}
					}
				if ((balanced<rpmraw)){
					if (shoottime.Get()>0.001){
						shootv= (shootv-errorgain);
						shoottime.Reset();
					}
				}
	

			}
			
			if (shootv>=1){
				shootv= 1;
			}
			if (shootv<=0){
				shootv= 0;
			}
			if (balanced<=0){
				shootv= 0;
			}
			if (shooterenc.Get()< 0){
				shootv = 0;
				balanced = 0;
			}
			

		//	shooter.SetSpeed(shootv);
			}
*/
			float shooterset = cop3axis;
			
			if (shooterset <= 0){
				shooterset = 0;
			}

			shooter.Set(shotset);
			
			
			//--------------DriverStationLCD----------------

				//-------------Shooter Debugging---------------
				dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "shooterset%f", shotset);
				dsLCD->Printf(DriverStationLCD::kUser_Line5, 1, "potset%f", potsetpoint);
				dsLCD->Printf(DriverStationLCD::kUser_Line3, 1, "potvolt%f", (anglepot.GetVoltage()));
				dsLCD->Printf(DriverStationLCD::kUser_Line6, 1, "anglevoltage%f", angle.Get());
				dsLCD->Printf(DriverStationLCD::kUser_Line2, 1, "ShooterRPM%f", rpmraw);
				dsLCD->Printf(DriverStationLCD::kUser_Line4, 1, "copjoy%f", cop2axis);

				dsLCD->UpdateLCD();

			Wait(0.005);
		}
		
		while (IsDisabled())
		{
			potsetpoint = anglepot.GetVoltage();

		}
	}
	

	void Test() {

	}

};

START_ROBOT_CLASS(RobotDemo);
