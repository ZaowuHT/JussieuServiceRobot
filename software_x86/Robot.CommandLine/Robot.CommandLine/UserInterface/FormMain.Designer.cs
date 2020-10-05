namespace Robot.CommandLine
{
    partial class FormMain
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.buttonStop = new System.Windows.Forms.Button();
            this.buttonRight = new System.Windows.Forms.Button();
            this.buttonLeft = new System.Windows.Forms.Button();
            this.buttonForward = new System.Windows.Forms.Button();
            this.buttonBackward = new System.Windows.Forms.Button();
            this.comboBoxCOM = new System.Windows.Forms.ComboBox();
            this.buttonRefresh = new System.Windows.Forms.Button();
            this.buttonConnect = new System.Windows.Forms.Button();
            this.textBoxServo0 = new System.Windows.Forms.TextBox();
            this.textBoxServo1 = new System.Windows.Forms.TextBox();
            this.textBoxServo2 = new System.Windows.Forms.TextBox();
            this.textBoxServo3 = new System.Windows.Forms.TextBox();
            this.buttonServo = new System.Windows.Forms.Button();
            this.textBoxBeep = new System.Windows.Forms.TextBox();
            this.buttonBeep = new System.Windows.Forms.Button();
            this.labelSetSpeed = new System.Windows.Forms.Label();
            this.textBoxSetSpeed = new System.Windows.Forms.TextBox();
            this.textBoxMotorSpeed = new System.Windows.Forms.TextBox();
            this.labelMotorSpeed = new System.Windows.Forms.Label();
            this.textBoxDisplacement = new System.Windows.Forms.TextBox();
            this.labelDisplacement = new System.Windows.Forms.Label();
            this.textBoxServoAngle = new System.Windows.Forms.TextBox();
            this.labelServoAngle = new System.Windows.Forms.Label();
            this.textBoxSonarData = new System.Windows.Forms.TextBox();
            this.labelSonarData = new System.Windows.Forms.Label();
            this.textBoxBattery = new System.Windows.Forms.TextBox();
            this.labelBattery = new System.Windows.Forms.Label();
            this.textBoxSlamcore = new System.Windows.Forms.TextBox();
            this.labelSlamcore = new System.Windows.Forms.Label();
            this.listBoxConsole = new System.Windows.Forms.ListBox();
            this.timerConsole = new System.Windows.Forms.Timer(this.components);
            this.SuspendLayout();
            // 
            // buttonStop
            // 
            this.buttonStop.Location = new System.Drawing.Point(80, 444);
            this.buttonStop.Name = "buttonStop";
            this.buttonStop.Size = new System.Drawing.Size(64, 64);
            this.buttonStop.TabIndex = 0;
            this.buttonStop.Text = "停止";
            this.buttonStop.UseVisualStyleBackColor = true;
            this.buttonStop.Click += new System.EventHandler(this.buttonStop_Click);
            // 
            // buttonRight
            // 
            this.buttonRight.Location = new System.Drawing.Point(150, 444);
            this.buttonRight.Name = "buttonRight";
            this.buttonRight.Size = new System.Drawing.Size(64, 64);
            this.buttonRight.TabIndex = 1;
            this.buttonRight.Text = "右转";
            this.buttonRight.UseVisualStyleBackColor = true;
            this.buttonRight.Click += new System.EventHandler(this.buttonRight_Click);
            // 
            // buttonLeft
            // 
            this.buttonLeft.Location = new System.Drawing.Point(10, 444);
            this.buttonLeft.Name = "buttonLeft";
            this.buttonLeft.Size = new System.Drawing.Size(64, 64);
            this.buttonLeft.TabIndex = 2;
            this.buttonLeft.Text = "左转";
            this.buttonLeft.UseVisualStyleBackColor = true;
            this.buttonLeft.Click += new System.EventHandler(this.buttonLeft_Click);
            // 
            // buttonForward
            // 
            this.buttonForward.Location = new System.Drawing.Point(80, 374);
            this.buttonForward.Name = "buttonForward";
            this.buttonForward.Size = new System.Drawing.Size(64, 64);
            this.buttonForward.TabIndex = 3;
            this.buttonForward.Text = "前进";
            this.buttonForward.UseVisualStyleBackColor = true;
            this.buttonForward.Click += new System.EventHandler(this.buttonForward_Click);
            // 
            // buttonBackward
            // 
            this.buttonBackward.Location = new System.Drawing.Point(80, 514);
            this.buttonBackward.Name = "buttonBackward";
            this.buttonBackward.Size = new System.Drawing.Size(64, 64);
            this.buttonBackward.TabIndex = 4;
            this.buttonBackward.Text = "后退";
            this.buttonBackward.UseVisualStyleBackColor = true;
            this.buttonBackward.Click += new System.EventHandler(this.buttonBackward_Click);
            // 
            // comboBoxCOM
            // 
            this.comboBoxCOM.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxCOM.FormattingEnabled = true;
            this.comboBoxCOM.Location = new System.Drawing.Point(10, 605);
            this.comboBoxCOM.Name = "comboBoxCOM";
            this.comboBoxCOM.Size = new System.Drawing.Size(113, 25);
            this.comboBoxCOM.TabIndex = 5;
            this.comboBoxCOM.SelectedIndexChanged += new System.EventHandler(this.comboBoxCOM_SelectedIndexChanged);
            // 
            // buttonRefresh
            // 
            this.buttonRefresh.Location = new System.Drawing.Point(129, 605);
            this.buttonRefresh.Name = "buttonRefresh";
            this.buttonRefresh.Size = new System.Drawing.Size(64, 25);
            this.buttonRefresh.TabIndex = 6;
            this.buttonRefresh.Text = "刷新";
            this.buttonRefresh.UseVisualStyleBackColor = true;
            this.buttonRefresh.Click += new System.EventHandler(this.buttonRefresh_Click);
            // 
            // buttonConnect
            // 
            this.buttonConnect.Location = new System.Drawing.Point(199, 605);
            this.buttonConnect.Name = "buttonConnect";
            this.buttonConnect.Size = new System.Drawing.Size(64, 25);
            this.buttonConnect.TabIndex = 7;
            this.buttonConnect.Text = "连接";
            this.buttonConnect.UseVisualStyleBackColor = true;
            this.buttonConnect.Click += new System.EventHandler(this.buttonConnect_Click);
            // 
            // textBoxServo0
            // 
            this.textBoxServo0.Location = new System.Drawing.Point(272, 468);
            this.textBoxServo0.Name = "textBoxServo0";
            this.textBoxServo0.Size = new System.Drawing.Size(168, 23);
            this.textBoxServo0.TabIndex = 8;
            this.textBoxServo0.Text = "90";
            // 
            // textBoxServo1
            // 
            this.textBoxServo1.Location = new System.Drawing.Point(272, 497);
            this.textBoxServo1.Name = "textBoxServo1";
            this.textBoxServo1.Size = new System.Drawing.Size(168, 23);
            this.textBoxServo1.TabIndex = 9;
            this.textBoxServo1.Text = "90";
            // 
            // textBoxServo2
            // 
            this.textBoxServo2.Location = new System.Drawing.Point(272, 526);
            this.textBoxServo2.Name = "textBoxServo2";
            this.textBoxServo2.Size = new System.Drawing.Size(168, 23);
            this.textBoxServo2.TabIndex = 10;
            this.textBoxServo2.Text = "90";
            // 
            // textBoxServo3
            // 
            this.textBoxServo3.Location = new System.Drawing.Point(272, 555);
            this.textBoxServo3.Name = "textBoxServo3";
            this.textBoxServo3.Size = new System.Drawing.Size(168, 23);
            this.textBoxServo3.TabIndex = 11;
            this.textBoxServo3.Text = "90";
            // 
            // buttonServo
            // 
            this.buttonServo.Location = new System.Drawing.Point(272, 605);
            this.buttonServo.Name = "buttonServo";
            this.buttonServo.Size = new System.Drawing.Size(168, 25);
            this.buttonServo.TabIndex = 12;
            this.buttonServo.Text = "设置舵机";
            this.buttonServo.UseVisualStyleBackColor = true;
            this.buttonServo.Click += new System.EventHandler(this.buttonServo_Click);
            // 
            // textBoxBeep
            // 
            this.textBoxBeep.Location = new System.Drawing.Point(272, 374);
            this.textBoxBeep.Name = "textBoxBeep";
            this.textBoxBeep.Size = new System.Drawing.Size(168, 23);
            this.textBoxBeep.TabIndex = 13;
            this.textBoxBeep.Text = "1000";
            // 
            // buttonBeep
            // 
            this.buttonBeep.Location = new System.Drawing.Point(272, 413);
            this.buttonBeep.Name = "buttonBeep";
            this.buttonBeep.Size = new System.Drawing.Size(168, 25);
            this.buttonBeep.TabIndex = 14;
            this.buttonBeep.Text = "蜂鸣器启动";
            this.buttonBeep.UseVisualStyleBackColor = true;
            this.buttonBeep.Click += new System.EventHandler(this.buttonBeep_Click);
            // 
            // labelSetSpeed
            // 
            this.labelSetSpeed.AutoSize = true;
            this.labelSetSpeed.Location = new System.Drawing.Point(9, 11);
            this.labelSetSpeed.Name = "labelSetSpeed";
            this.labelSetSpeed.Size = new System.Drawing.Size(80, 17);
            this.labelSetSpeed.TabIndex = 15;
            this.labelSetSpeed.Text = "底盘设定速度";
            // 
            // textBoxSetSpeed
            // 
            this.textBoxSetSpeed.Location = new System.Drawing.Point(12, 31);
            this.textBoxSetSpeed.Name = "textBoxSetSpeed";
            this.textBoxSetSpeed.Size = new System.Drawing.Size(430, 23);
            this.textBoxSetSpeed.TabIndex = 16;
            this.textBoxSetSpeed.Text = "0, 0, 0";
            // 
            // textBoxMotorSpeed
            // 
            this.textBoxMotorSpeed.Location = new System.Drawing.Point(12, 81);
            this.textBoxMotorSpeed.Name = "textBoxMotorSpeed";
            this.textBoxMotorSpeed.Size = new System.Drawing.Size(430, 23);
            this.textBoxMotorSpeed.TabIndex = 20;
            this.textBoxMotorSpeed.Text = "0, 0, 0, 0";
            // 
            // labelMotorSpeed
            // 
            this.labelMotorSpeed.AutoSize = true;
            this.labelMotorSpeed.Location = new System.Drawing.Point(9, 61);
            this.labelMotorSpeed.Name = "labelMotorSpeed";
            this.labelMotorSpeed.Size = new System.Drawing.Size(56, 17);
            this.labelMotorSpeed.TabIndex = 19;
            this.labelMotorSpeed.Text = "电机速度";
            // 
            // textBoxDisplacement
            // 
            this.textBoxDisplacement.Location = new System.Drawing.Point(12, 130);
            this.textBoxDisplacement.Name = "textBoxDisplacement";
            this.textBoxDisplacement.Size = new System.Drawing.Size(430, 23);
            this.textBoxDisplacement.TabIndex = 25;
            this.textBoxDisplacement.Text = "0, 0, 0";
            // 
            // labelDisplacement
            // 
            this.labelDisplacement.AutoSize = true;
            this.labelDisplacement.Location = new System.Drawing.Point(9, 110);
            this.labelDisplacement.Name = "labelDisplacement";
            this.labelDisplacement.Size = new System.Drawing.Size(56, 17);
            this.labelDisplacement.TabIndex = 24;
            this.labelDisplacement.Text = "底盘位移";
            // 
            // textBoxServoAngle
            // 
            this.textBoxServoAngle.Location = new System.Drawing.Point(12, 180);
            this.textBoxServoAngle.Name = "textBoxServoAngle";
            this.textBoxServoAngle.Size = new System.Drawing.Size(430, 23);
            this.textBoxServoAngle.TabIndex = 27;
            this.textBoxServoAngle.Text = "90, 90, 90, 90";
            // 
            // labelServoAngle
            // 
            this.labelServoAngle.AutoSize = true;
            this.labelServoAngle.Location = new System.Drawing.Point(9, 160);
            this.labelServoAngle.Name = "labelServoAngle";
            this.labelServoAngle.Size = new System.Drawing.Size(56, 17);
            this.labelServoAngle.TabIndex = 26;
            this.labelServoAngle.Text = "舵机角度";
            // 
            // textBoxSonarData
            // 
            this.textBoxSonarData.Location = new System.Drawing.Point(12, 231);
            this.textBoxSonarData.Name = "textBoxSonarData";
            this.textBoxSonarData.Size = new System.Drawing.Size(430, 23);
            this.textBoxSonarData.TabIndex = 29;
            this.textBoxSonarData.Text = "0, 0";
            // 
            // labelSonarData
            // 
            this.labelSonarData.AutoSize = true;
            this.labelSonarData.Location = new System.Drawing.Point(9, 211);
            this.labelSonarData.Name = "labelSonarData";
            this.labelSonarData.Size = new System.Drawing.Size(68, 17);
            this.labelSonarData.TabIndex = 28;
            this.labelSonarData.Text = "超声波数据";
            // 
            // textBoxBattery
            // 
            this.textBoxBattery.Location = new System.Drawing.Point(12, 278);
            this.textBoxBattery.Name = "textBoxBattery";
            this.textBoxBattery.Size = new System.Drawing.Size(430, 23);
            this.textBoxBattery.TabIndex = 31;
            this.textBoxBattery.Text = "0, 0, 0";
            // 
            // labelBattery
            // 
            this.labelBattery.AutoSize = true;
            this.labelBattery.Location = new System.Drawing.Point(9, 258);
            this.labelBattery.Name = "labelBattery";
            this.labelBattery.Size = new System.Drawing.Size(56, 17);
            this.labelBattery.TabIndex = 30;
            this.labelBattery.Text = "电池数据";
            // 
            // textBoxSlamcore
            // 
            this.textBoxSlamcore.Location = new System.Drawing.Point(12, 325);
            this.textBoxSlamcore.Name = "textBoxSlamcore";
            this.textBoxSlamcore.Size = new System.Drawing.Size(430, 23);
            this.textBoxSlamcore.TabIndex = 33;
            this.textBoxSlamcore.Text = "0";
            // 
            // labelSlamcore
            // 
            this.labelSlamcore.AutoSize = true;
            this.labelSlamcore.Location = new System.Drawing.Point(9, 305);
            this.labelSlamcore.Name = "labelSlamcore";
            this.labelSlamcore.Size = new System.Drawing.Size(86, 17);
            this.labelSlamcore.TabIndex = 32;
            this.labelSlamcore.Text = "Slamcore状态";
            // 
            // listBoxConsole
            // 
            this.listBoxConsole.FormattingEnabled = true;
            this.listBoxConsole.ItemHeight = 17;
            this.listBoxConsole.Location = new System.Drawing.Point(10, 636);
            this.listBoxConsole.Name = "listBoxConsole";
            this.listBoxConsole.Size = new System.Drawing.Size(430, 106);
            this.listBoxConsole.TabIndex = 34;
            // 
            // timerConsole
            // 
            this.timerConsole.Enabled = true;
            this.timerConsole.Tick += new System.EventHandler(this.timerConsole_Tick);
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 17F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(452, 747);
            this.Controls.Add(this.listBoxConsole);
            this.Controls.Add(this.textBoxSlamcore);
            this.Controls.Add(this.labelSlamcore);
            this.Controls.Add(this.textBoxBattery);
            this.Controls.Add(this.labelBattery);
            this.Controls.Add(this.textBoxSonarData);
            this.Controls.Add(this.labelSonarData);
            this.Controls.Add(this.textBoxServoAngle);
            this.Controls.Add(this.labelServoAngle);
            this.Controls.Add(this.textBoxDisplacement);
            this.Controls.Add(this.labelDisplacement);
            this.Controls.Add(this.textBoxMotorSpeed);
            this.Controls.Add(this.labelMotorSpeed);
            this.Controls.Add(this.textBoxSetSpeed);
            this.Controls.Add(this.labelSetSpeed);
            this.Controls.Add(this.buttonBeep);
            this.Controls.Add(this.textBoxBeep);
            this.Controls.Add(this.buttonServo);
            this.Controls.Add(this.textBoxServo3);
            this.Controls.Add(this.textBoxServo2);
            this.Controls.Add(this.textBoxServo1);
            this.Controls.Add(this.textBoxServo0);
            this.Controls.Add(this.buttonConnect);
            this.Controls.Add(this.buttonRefresh);
            this.Controls.Add(this.comboBoxCOM);
            this.Controls.Add(this.buttonBackward);
            this.Controls.Add(this.buttonForward);
            this.Controls.Add(this.buttonLeft);
            this.Controls.Add(this.buttonRight);
            this.Controls.Add(this.buttonStop);
            this.DoubleBuffered = true;
            this.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FormMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "机器人控制台";
            this.Load += new System.EventHandler(this.FormMain_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonStop;
        private System.Windows.Forms.Button buttonRight;
        private System.Windows.Forms.Button buttonLeft;
        private System.Windows.Forms.Button buttonForward;
        private System.Windows.Forms.Button buttonBackward;
        private System.Windows.Forms.ComboBox comboBoxCOM;
        private System.Windows.Forms.Button buttonRefresh;
        private System.Windows.Forms.Button buttonConnect;
        private System.Windows.Forms.TextBox textBoxServo0;
        private System.Windows.Forms.TextBox textBoxServo1;
        private System.Windows.Forms.TextBox textBoxServo2;
        private System.Windows.Forms.TextBox textBoxServo3;
        private System.Windows.Forms.Button buttonServo;
        private System.Windows.Forms.TextBox textBoxBeep;
        private System.Windows.Forms.Button buttonBeep;
        private System.Windows.Forms.Label labelSetSpeed;
        private System.Windows.Forms.TextBox textBoxSetSpeed;
        private System.Windows.Forms.TextBox textBoxMotorSpeed;
        private System.Windows.Forms.Label labelMotorSpeed;
        private System.Windows.Forms.TextBox textBoxDisplacement;
        private System.Windows.Forms.Label labelDisplacement;
        private System.Windows.Forms.TextBox textBoxServoAngle;
        private System.Windows.Forms.Label labelServoAngle;
        private System.Windows.Forms.TextBox textBoxSonarData;
        private System.Windows.Forms.Label labelSonarData;
        private System.Windows.Forms.TextBox textBoxBattery;
        private System.Windows.Forms.Label labelBattery;
        private System.Windows.Forms.TextBox textBoxSlamcore;
        private System.Windows.Forms.Label labelSlamcore;
        private System.Windows.Forms.ListBox listBoxConsole;
        private System.Windows.Forms.Timer timerConsole;
    }
}

