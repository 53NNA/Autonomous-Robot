#include "main.h"
#include "pros/misc.h"
#include <fstream>

pros::Motor Turn(-1);
pros::Motor Rear1(-20);
pros::Motor Rear2(-19);
pros::MotorGroup RearWheelAids({20, -19});
pros::Controller master(pros::E_CONTROLLER_MASTER);
std::ofstream logFile("/usd/button_log.csv", std::ios::app);
int counterStraight = 0;
int counterTurn = 0;
bool pressedButtonStraight = false;
bool pressedButtonTurn = false;
void buttonCheck() {
    while (true) {
        bool buttonStraight = master.get_digital(pros::E_CONTROLLER_DIGITAL_X);
        bool buttonTurn = master.get_digital(pros::E_CONTROLLER_DIGITAL_Y);
        if (buttonStraight && !pressedButtonStraight) {
            pressedButtonStraight = true;
            logFile << "RearWheelAids.move(128)\n";
            RearWheelAids.move(100);
        } else if (buttonStraight && pressedButtonStraight) {
            counterStraight += 5;
        } else if (!buttonStraight && pressedButtonStraight) {
            logFile << "counterStraight+" << counterStraight << "\n";
            pressedButtonStraight = false;
            RearWheelAids.move(0);
        }
        if (buttonTurn && !pressedButtonTurn) {
            pressedButtonTurn = true;
            logFile << "Turn.move(50)\n";
            Turn.move(70);
        } else if (buttonTurn && pressedButtonTurn) {
            counterTurn += 5;
        } else if (!buttonTurn && pressedButtonTurn) {
            logFile << "counterTurn+" << counterTurn << "\n";
            pressedButtonTurn = false;
            Turn.move(0);
        }

        pros::delay(5);
    }
}

void autonomous() {
    pros::Task task1(buttonCheck);
    pros::delay(1000000);
    task1.remove();
    logFile.close();
}

void opcontrol() {
    autonomous();
}
