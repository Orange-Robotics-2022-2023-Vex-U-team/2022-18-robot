/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      FEB 10 2023                                               */
/*    Description:  Clawbot Competition Template                              */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// ChassisLR            motor         4               
// ChassisLF            motor         6               
// ChassisRF            motor         7               
// ChassisRR            motor         8               
// Left                 rotation      16              
// Right                rotation      17              
// Center               rotation      5               
// IntakeMotor          motor         11              
// TriggerMotor         motor         9               
// ShooterMotors        motor_group   18, 19          
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "robot-config.h"
#include "trigger.h"
#include "shooter.h"
#include "odometry.h"
#include "auto-controller.h"

#include "./mecanum-drive.h"
#include "./constants.h"

#include <cmath>

using namespace vex;

// A global instance of competition
competition Competition;
MecanumDrive mecDrive;
AutoController autoController;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  
  // initialization stuff
  Trigger::init();

  Inertial16.calibrate();
  odo.reset();

  autoController.init("BlitzSkills");
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) { 

  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................

  //mecDrive.driveToLocation(86, 17.39, 50);
  //wait(0.2, sec);

  //mecDrive.turnToTheta(M_PI); 
  //mecDrive.turnToHeading(-M_PI + 0.001);
  
  //mecDrive.turnPID(-2*M_PI/1); 
  
  //mecDrive.turnAndDrivePID(10, 90);

  mecDrive.goToPointPID(86, 20);
  mecDrive.shootToFarGoal();

  // Sample Usage of AutoController
  // autoController.executeRoutine();
}


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  // Controller1.ButtonR2.pressed([](){
  //   IntakeMotor.stop();
  // });

  // disabled for now in favor of hold-button control
  /*Controller1.ButtonL2.pressed([](){
    stopShooter();
  });*/
  

  // idling -- dont use duirng practice, causing overheat
  // Controller1.ButtonL1.pressed([](){
  //   Shooter::spinShooterForward(79);
  // });
  // Shooter::spinShooterForward(50);
  // Controller1.ButtonL1.released([](){
  //   Shooter::spinShooterForward(50);
  // });

  /* CONTROLS INFO
    A : launch disk / move flipper (press)
    B : unassigned
    X : unassigned
    Y : unassigned

    Up:    rotate to face far goal (press)
    Down:  rotate to face near goal (press)
    Left:  slow turn left (hold)
    Right: slow turn right (hold)

    Right Stick X (Axis 1): rotation control
    Right Stick Y (Axis 2): unassigned

    Left Stick X (Axis 4): left/right translation control
    Left Stick Y (Axis 3): forward/reverse translation control

    R1 Trigger: spin intake forward (hold)
    R2 Trigger: spin intake backward (hold)

    L1 Trigger: spin shooter flywheel (hold)
    L2 Trigger: invert translational joystick control (hold)

  */

  Controller1.ButtonA.pressed([](){ 
    Trigger::launch();
  });

  Controller1.ButtonLeft.pressed([](){
    MecanumDrive::adjustLeft(18);
    angleAdjustTimer.reset();
  });

  Controller1.ButtonRight.pressed([](){
    MecanumDrive::adjustRight(18);
    angleAdjustTimer.reset();
  });

  Controller1.ButtonR1.pressed([](){
    IntakeMotor.spin(vex::forward,100,velocityUnits::pct);
  });

  Controller1.ButtonR1.released([](){
    IntakeMotor.stop();
  });

  Controller1.ButtonR2.pressed([](){
    IntakeMotor.spin(vex::reverse, 100, velocityUnits::pct);
  });

  Controller1.ButtonR2.released([](){
    IntakeMotor.stop();
  });

  Controller1.ButtonUp.pressed([](){
    mecDrive.turnToFarGoal();
  });

  Controller1.ButtonDown.pressed([](){
    mecDrive.turnToNearGoal();
  });

  // double turnImportance = 0.5;
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................
    if (Controller1.ButtonL1.pressing()) {
      Shooter::spinShooterForward(79);
    } else {
      Shooter::stopShooter();
    }
 

    {
      if (!Controller1.ButtonLeft.pressing() && !Controller1.ButtonRight.pressing() && mecDrive.activePID == false) {
        if (!Controller1.ButtonL2.pressing()) {
          MecanumDrive::drive(Controller1.Axis3.value(), Controller1.Axis4.value(), Controller1.Axis1.value()); //inverted joystick control
        } else {
          MecanumDrive::drive(-Controller1.Axis3.value(), -Controller1.Axis4.value(), Controller1.Axis1.value()); //joystick drivetrain control code
        }
      }
    }

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.

  }
}

//
// Main will set up the competition functions and callbacks.
//

void printProcess() {
  while(true) {
    if (DEBUG_MODE) odo.printLocation();

    wait(500, msec);
  }
}

int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Separate thread for slower print updates for better performance.
  vex::thread printThread(printProcess);

  // Main process will be doing updates to the odometry regularly (roughly 100Hz)
  while (true) {
    odo.updateOdometry();
    wait(10, msec);
  }
}
