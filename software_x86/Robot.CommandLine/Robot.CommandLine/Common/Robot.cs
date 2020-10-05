using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Robot.CommandLine.Common
{
    public class Robot
    {
        static public int MOTOR_LEFT = 1;
        static public int MOTOR_RIGHT = 0;
        static public int SERVO_NUMBER = 4;
        static public int SONAR_NUMBER = 2;

        static public float speedX = 0;
        static public float speedY = 0;
        static public float speedYaw = 0;
        static public float displacementX = 0;
        static public float displacementY = 0;
        static public float displacementYaw = 0;
        static public int[] motorCurrentSpeedPPS = new int[2] { 0, 0 };
        static public int[] motorTargetSpeedPPS = new int[2] { 0, 0 };
        static public int[] servoData = new int[4] { 90, 90, 90, 90 };
        static public int[] servoTargetData = new int[4] { 90, 90, 90, 90 };
        static public int[] sonarData = new int[4] { 90, 90, 90, 90 };
        static public int beepDuration = 0;
        static public int batteryState = 0;
        static public int batteryPercent = 0;
        static public float batteryVoltage = 0;
        static public int slamcoreState = 0;
    };
};
