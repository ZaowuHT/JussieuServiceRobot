using Robot.CommandLine.Common;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Robot.CommandLine
{
    public partial class FormMain : Form
    {
        public FormMain()
        {
            InitializeComponent();
        }

        private void RefreshCOMList()
        {
            comboBoxCOM.Items.Clear();
            comboBoxCOM.Items.AddRange(SerialManager.GetCOMList().ToArray());
        }

        private void RefreshConnectButton()
        {
            if (Communication.portConnected) buttonConnect.Text = "断开";
            else
            {
                buttonConnect.Text = "连接";
                buttonConnect.Enabled = !(comboBoxCOM.Text == string.Empty);
            }
        }

        private void UpdateDataDisplay()
        {
            // Set Speed
            string _setspeed = string.Empty;
            _setspeed += Common.Robot.speedX.ToString();
            _setspeed += ", ";
            _setspeed += Common.Robot.speedY.ToString();
            _setspeed += ", ";
            _setspeed += Common.Robot.speedYaw.ToString();
            textBoxSetSpeed.Invoke(new EventHandler(delegate
            {
                textBoxSetSpeed.Text = _setspeed;
            }));

            // Motor Speed
            string _motorspeed = string.Empty;
            _motorspeed += Common.Robot.motorCurrentSpeedPPS[Common.Robot.MOTOR_LEFT].ToString();
            _motorspeed += ", ";
            _motorspeed += Common.Robot.motorTargetSpeedPPS[Common.Robot.MOTOR_LEFT].ToString();
            _motorspeed += ", ";
            _motorspeed += Common.Robot.motorCurrentSpeedPPS[Common.Robot.MOTOR_RIGHT].ToString();
            _motorspeed += ", ";
            _motorspeed += Common.Robot.motorTargetSpeedPPS[Common.Robot.MOTOR_RIGHT].ToString();
            textBoxMotorSpeed.Invoke(new EventHandler(delegate
            {
                textBoxMotorSpeed.Text = _motorspeed;
            }));

            // Displacement
            string _displacement = string.Empty;
            _displacement += Common.Robot.displacementX.ToString();
            _displacement += ", ";
            _displacement += Common.Robot.displacementY.ToString();
            _displacement += ", ";
            _displacement += Common.Robot.displacementYaw.ToString();
            textBoxDisplacement.Invoke(new EventHandler(delegate
            {
                textBoxDisplacement.Text = _displacement;
            }));

            // Servo Angle
            string _servoangle = string.Empty;
            for (int i = 0; i < Common.Robot.SERVO_NUMBER; i++)
            {
                _servoangle += Common.Robot.servoData[i].ToString();
                if (i != Common.Robot.SERVO_NUMBER - 1) _servoangle += ", ";
            }
            textBoxServoAngle.Invoke(new EventHandler(delegate
            {
                textBoxServoAngle.Text = _servoangle;
            }));

            // Sonar Data
            string _sonardata = string.Empty;
            for (int i = 0; i < Common.Robot.SONAR_NUMBER; i++)
            {
                _sonardata += Common.Robot.sonarData[i].ToString();
                if (i != Common.Robot.SONAR_NUMBER - 1) _sonardata += ", ";
            }
            textBoxSonarData.Invoke(new EventHandler(delegate
            {
                textBoxSonarData.Text = _sonardata;
            }));

            // Battery Data
            string _batterydata = string.Empty;
            _batterydata += Common.Robot.batteryState.ToString();
            _batterydata += ", ";
            _batterydata += Common.Robot.batteryVoltage.ToString();
            _batterydata += ", ";
            _batterydata += Common.Robot.batteryPercent.ToString();
            textBoxBattery.Invoke(new EventHandler(delegate
            {
                textBoxBattery.Text = _batterydata;
            }));

            // Slamcore State
            string _slamcore = string.Empty;
            _slamcore += Common.Robot.slamcoreState.ToString();
            textBoxSlamcore.Invoke(new EventHandler(delegate
            {
                textBoxSlamcore.Text = _slamcore;
            }));
        }

        private void FormMain_Load(object sender, EventArgs e)
        {
            UpdateDataDisplay();
            Communication.InitCommunication();
            Communication.DataUpdateEvent += UpdateDataDisplay;
            RefreshCOMList();
            RefreshConnectButton();
        }

        private void buttonRefresh_Click(object sender, EventArgs e)
        {
            RefreshCOMList();
        }

        private void buttonConnect_Click(object sender, EventArgs e)
        {
            if (!Communication.portConnected)
            {
                Communication.StartCommunicaiton(comboBoxCOM.Text, 115200);
                timerConsole.Enabled = true;
            }
            else
            {
                Communication.EndCommunication();
                timerConsole.Enabled = false;
            }
            RefreshConnectButton();
        }

        private void comboBoxCOM_SelectedIndexChanged(object sender, EventArgs e)
        {
            RefreshConnectButton();
        }

        private void buttonBeep_Click(object sender, EventArgs e)
        {
            Common.Robot.beepDuration = Int32.Parse(textBoxBeep.Text);
        }

        private void buttonServo_Click(object sender, EventArgs e)
        {
            Common.Robot.servoTargetData[0] = Int32.Parse(textBoxServo0.Text);
            Common.Robot.servoTargetData[1] = Int32.Parse(textBoxServo1.Text);
            Common.Robot.servoTargetData[2] = Int32.Parse(textBoxServo2.Text);
            Common.Robot.servoTargetData[3] = Int32.Parse(textBoxServo3.Text);
        }

        private void buttonForward_Click(object sender, EventArgs e)
        {
            Common.Robot.speedX = 50;
        }

        private void buttonBackward_Click(object sender, EventArgs e)
        {
            Common.Robot.speedX = -50;
        }

        private void buttonLeft_Click(object sender, EventArgs e)
        {
            Common.Robot.speedYaw = 5;
        }

        private void buttonRight_Click(object sender, EventArgs e)
        {
            Common.Robot.speedYaw = -5;
        }

        private void timerConsole_Tick(object sender, EventArgs e)
        {
            listBoxConsole.Invoke(new EventHandler(delegate
            {
                listBoxConsole.Items.Clear();
                listBoxConsole.Items.AddRange(Common.ConsoleData.consoleData.ToArray());
                listBoxConsole.SelectedIndex = listBoxConsole.Items.Count - 1;
            }));
            // if (Common.ConsoleData.consoleData.Count > 8) Common.ConsoleData.consoleData.Clear();
            this.Text = "机器人控制台 [数据：" + Common.Communication.dataCount.ToString() + "]";
        }

        private void buttonStop_Click(object sender, EventArgs e)
        {
            Common.Robot.speedX = 0;
            Common.Robot.speedYaw = 0;
        }
    }
}
