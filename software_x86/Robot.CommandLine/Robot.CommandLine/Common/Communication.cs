using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Robot.CommandLine.Common
{
    public class Communication
    {
        public enum HostState
        { DISCONNECTED = 0, CONNECTED = 1 };

        public enum StreamState
        { WAITFORHEAD = 0, WAITFORLENGTH = 1, WAITFORPAYLOAD = 2 };

        public enum EventState
        { WAITCONNECT = 0, SEND = 1, WAITRESP = 2 };

        static public byte HOST_STREAM_HEAD = 0x50;
        static public byte HOST_STREAM_CONNECT_REQ = 0x01;
        static public byte HOST_STREAM_RESP_OK = 0x20;
        static public byte HOST_STREAM_RESP_ERROR = 0x21;
        static public byte HOST_STREAM_SPEED_REQ = 0x02;
        static public byte HOST_STREAM_SERVO_REQ = 0x03;
        static public byte HOST_STREAM_BATTERY_REQ = 0x04;
        static public byte HOST_STREAM_SLAMCORE_STATE_REQ = 0x05;
        static public byte HOST_STREAM_SONAR_REQ = 0x07;
        static public byte HOST_STREAM_BEEP_REQ = 0x08;

        static public bool portConnected = false;
        static public HostState hostState = HostState.DISCONNECTED;
        static public StreamState streamState = StreamState.WAITFORHEAD;
        static public bool streamRecieved = false;
        static public byte[] streamBuffer = new byte[256];
        static public int streamBufferPos = 0;
        static public int streamLength = 0;

        public delegate void DataUpdateHandle();
        static public event DataUpdateHandle DataUpdateEvent;

        static public EventState eventState = EventState.WAITCONNECT;
        static public byte eventContext = HOST_STREAM_CONNECT_REQ;

        static public int dataCount = 0;

        static public void DataHandle(object sender, SerialDataReceivedEventArgs e)
        {
            dataCount++;
            if (streamState == StreamState.WAITFORHEAD)
            {
                if (SerialManager.serialPort.BytesToRead >= 1)
                {
                    SerialManager.serialPort.Read(streamBuffer, streamBufferPos, 1);
                    if (streamBuffer[0] == HOST_STREAM_HEAD)
                    {
                        streamBufferPos = 1;
                        streamState = StreamState.WAITFORLENGTH;
                        SerialManager.SetISRLength(2);
                    }
                }
            }
            else if (streamState == StreamState.WAITFORLENGTH)
            {
                if (SerialManager.serialPort.BytesToRead >= 2)
                {
                    SerialManager.serialPort.Read(streamBuffer, streamBufferPos, 2);
                    streamBufferPos = 3;
                    streamState = StreamState.WAITFORPAYLOAD;
                    SerialManager.SetISRLength(
                        (int) StreamBuilder.GetUnsignedInt(streamBuffer, 1, StreamBuilder.BuilderType.INT16_BYTE));
                }
            }
            else if (streamState == StreamState.WAITFORPAYLOAD)
            {
                int _length = (int)StreamBuilder.GetUnsignedInt(streamBuffer, 1, StreamBuilder.BuilderType.INT16_BYTE);
                if (SerialManager.serialPort.BytesToRead >= _length)
                {
                    streamLength = _length + 3;
                    SerialManager.serialPort.Read(streamBuffer, streamBufferPos, _length);
                    ConsoleData.AddInfo(streamBuffer, streamLength, false);
                    if (StreamBuilder.VerifyCheckSum(streamBuffer, 0, (UInt16) (streamLength - 2), streamBuffer[streamLength - 1])
                        == StreamBuilder.ChecksumResult.CHECKSUM_OK)
                    {
                        streamRecieved = true;
                    }
                }
                streamBufferPos = 0;
                streamState = StreamState.WAITFORHEAD;
            }
        }

        static public void InitCommunication()
        {
            SystemTimer.TimerEvent += Event;
            SerialManager.serialPort.DataReceived += DataHandle;
        }

        static public bool StartCommunicaiton(string _com, int _baud)
        {
            bool retValue = false;
            try
            {
                SerialManager.Connect(_com, _baud);
                if (SerialManager.serialPort.IsOpen)
                {
                    portConnected = true;
                    SystemTimer.InitTimer();
                    retValue = true;
                }
            }
            catch (Exception) { };
            return retValue;
        }

        static public bool EndCommunication()
        {
            bool retValue = false;
            try
            {
                SerialManager.serialPort.Close();
                if (!SerialManager.serialPort.IsOpen)
                {
                    portConnected = false;
                    retValue = true;
                }
            }
            catch (Exception) { };
            return retValue;
        }

        static public void ProcessStream(byte[] dataStream, byte streamType)
        {
            if (dataStream[3] == HOST_STREAM_RESP_OK)
            {
                if (streamType == HOST_STREAM_CONNECT_REQ)
                {
                    hostState = HostState.CONNECTED;
                }
                else if (streamType == HOST_STREAM_SPEED_REQ)
                {
                    Robot.motorCurrentSpeedPPS[Robot.MOTOR_LEFT] =
                        StreamBuilder.GetInt(dataStream, 4, StreamBuilder.BuilderType.INT32_BYTE);
                    Robot.motorTargetSpeedPPS[Robot.MOTOR_LEFT] =
                        StreamBuilder.GetInt(dataStream, 8, StreamBuilder.BuilderType.INT32_BYTE);
                    Robot.motorCurrentSpeedPPS[Robot.MOTOR_RIGHT] =
                        StreamBuilder.GetInt(dataStream, 12, StreamBuilder.BuilderType.INT32_BYTE);
                    Robot.motorTargetSpeedPPS[Robot.MOTOR_RIGHT] =
                        StreamBuilder.GetInt(dataStream, 16, StreamBuilder.BuilderType.INT32_BYTE);
                    Robot.displacementX =
                        StreamBuilder.GetFloat(dataStream, 20, StreamBuilder.BuilderType.FLOAT_BYTE_Q8);
                    Robot.displacementY =
                        StreamBuilder.GetFloat(dataStream, 24, StreamBuilder.BuilderType.FLOAT_BYTE_Q8);
                    Robot.displacementYaw =
                        StreamBuilder.GetFloat(dataStream, 28, StreamBuilder.BuilderType.FLOAT_BYTE_Q8);
                }
                else if (streamType == HOST_STREAM_SERVO_REQ)
                {
                    for (UInt16 i = 0; i < Robot.SERVO_NUMBER; i++)
                        Robot.servoData[i] =
                            (int)StreamBuilder.GetUnsignedInt(dataStream, (UInt16)(i + 4), StreamBuilder.BuilderType.BYTE);
                }
                else if (streamType == HOST_STREAM_BATTERY_REQ)
                {
                    Robot.batteryState =
                        (int)StreamBuilder.GetUnsignedInt(dataStream, 4, StreamBuilder.BuilderType.BYTE);
                    Robot.batteryVoltage =
                        StreamBuilder.GetUnsignedFloat(dataStream, 5, StreamBuilder.BuilderType.FLOAT_BYTE_Q16);
                    Robot.batteryPercent =
                        (int)StreamBuilder.GetUnsignedInt(dataStream, 9, StreamBuilder.BuilderType.BYTE);
                }
                else if (streamType == HOST_STREAM_SLAMCORE_STATE_REQ)
                {
                    Robot.slamcoreState =
                        (int)StreamBuilder.GetUnsignedInt(dataStream, 4, StreamBuilder.BuilderType.BYTE);
                }
                else if (streamType == HOST_STREAM_SONAR_REQ)
                {
                    for (int i = 0; i < Robot.SONAR_NUMBER; i++)
                        Robot.sonarData[i] =
                            (int)StreamBuilder.GetUnsignedInt(dataStream, (UInt16) (i * 4 + 4), StreamBuilder.BuilderType.INT32_BYTE);
                }

                DataUpdateEvent?.Invoke();
            }
        }

        static public int BuildStream(byte[] dataStream, byte streamType)
        {
            int retValue = 0;

            retValue +=
                StreamBuilder.AddUnsignedInt(HOST_STREAM_HEAD, dataStream, (UInt16)retValue, StreamBuilder.BuilderType.BYTE);

            if (streamType == HOST_STREAM_CONNECT_REQ)
            {
                retValue +=
                    StreamBuilder.AddUnsignedInt(2, dataStream, (UInt16)retValue, StreamBuilder.BuilderType.INT16_BYTE);
                retValue +=
                    StreamBuilder.AddUnsignedInt(HOST_STREAM_CONNECT_REQ, dataStream, (UInt16)retValue, StreamBuilder.BuilderType.BYTE);
            }
            else if (streamType == HOST_STREAM_SPEED_REQ)
            {
                retValue +=
                    StreamBuilder.AddUnsignedInt(10, dataStream, (UInt16)retValue, StreamBuilder.BuilderType.INT16_BYTE);
                retValue +=
                    StreamBuilder.AddUnsignedInt(HOST_STREAM_SPEED_REQ, dataStream, (UInt16)retValue, StreamBuilder.BuilderType.BYTE);
                retValue +=
                    StreamBuilder.AddFloat(Robot.speedX, dataStream, (UInt16)retValue, StreamBuilder.BuilderType.FLOAT_BYTE_Q8);
                retValue +=
                    StreamBuilder.AddFloat(Robot.speedYaw, dataStream, (UInt16)retValue, StreamBuilder.BuilderType.FLOAT_BYTE_Q8);
            }
            else if (streamType == HOST_STREAM_SERVO_REQ)
            {
                retValue +=
                    StreamBuilder.AddUnsignedInt((UInt16) (Robot.SERVO_NUMBER + 2), dataStream, (UInt16)retValue, StreamBuilder.BuilderType.INT16_BYTE);
                retValue +=
                    StreamBuilder.AddUnsignedInt(HOST_STREAM_SERVO_REQ, dataStream, (UInt16)retValue, StreamBuilder.BuilderType.BYTE);
                for (int i = 0; i < Robot.SERVO_NUMBER; i++)
                    retValue +=
                        StreamBuilder.AddUnsignedInt((UInt32)Robot.servoTargetData[i], dataStream, (UInt16)retValue, StreamBuilder.BuilderType.BYTE);
            }
            else if (streamType == HOST_STREAM_BEEP_REQ)
            {
                retValue +=
                    StreamBuilder.AddUnsignedInt(6, dataStream, (UInt16)retValue, StreamBuilder.BuilderType.INT16_BYTE);
                retValue +=
                    StreamBuilder.AddUnsignedInt(HOST_STREAM_BEEP_REQ, dataStream, (UInt16)retValue, StreamBuilder.BuilderType.BYTE);
                retValue +=
                    StreamBuilder.AddUnsignedInt((UInt32)Robot.beepDuration, dataStream, (UInt16)retValue, StreamBuilder.BuilderType.INT32_BYTE);
                if (Robot.beepDuration > 0) Robot.beepDuration = 0;
            }
            else
            {
                retValue +=
                       StreamBuilder.AddUnsignedInt(2, dataStream, (UInt16)retValue, StreamBuilder.BuilderType.INT16_BYTE);
                retValue +=
                    StreamBuilder.AddUnsignedInt(streamType, dataStream, (UInt16)retValue, StreamBuilder.BuilderType.BYTE);
            }

            retValue += 
                StreamBuilder.AddCheckSum(dataStream, 0, (UInt16)(retValue - 1), (UInt16)(retValue));

            return retValue;
        }

        static public byte GetNextContext(byte _context)
        {
            byte retValue = 0x00;
            if (_context == HOST_STREAM_SPEED_REQ) retValue = HOST_STREAM_SERVO_REQ;
            else if (_context == HOST_STREAM_SERVO_REQ) retValue = HOST_STREAM_BATTERY_REQ;
            else if (_context == HOST_STREAM_BATTERY_REQ) retValue = HOST_STREAM_SLAMCORE_STATE_REQ;
            else if (_context == HOST_STREAM_SLAMCORE_STATE_REQ) retValue = HOST_STREAM_SONAR_REQ;
            else if (_context == HOST_STREAM_SONAR_REQ) retValue = HOST_STREAM_BEEP_REQ;
            else if (_context == HOST_STREAM_BEEP_REQ) retValue = HOST_STREAM_SPEED_REQ;
            return retValue;
        }

        static public void Event()
        {
            if (SerialManager.serialPort.IsOpen && portConnected)
            {
                if (eventState == EventState.WAITCONNECT)
                {
                    eventContext = HOST_STREAM_CONNECT_REQ;
                    int _length = BuildStream(streamBuffer, eventContext);
                    streamRecieved = false;
                    streamBufferPos = 0;
                    streamState = StreamState.WAITFORHEAD;
                    SerialManager.SetISRLength(1);
                    SerialManager.SendStream(streamBuffer, _length);
                    eventState = EventState.WAITRESP;
                }
                else if (eventState == EventState.SEND)
                {
                    int _length = BuildStream(streamBuffer, eventContext);
                    streamRecieved = false;
                    streamBufferPos = 0;
                    streamState = StreamState.WAITFORHEAD;
                    SerialManager.SetISRLength(1);
                    SerialManager.SendStream(streamBuffer, _length);
                    eventState = EventState.WAITRESP;
                }
                else if (eventState == EventState.WAITRESP)
                {
                    if (streamRecieved)
                    {
                        if (hostState == HostState.CONNECTED || eventContext == HOST_STREAM_CONNECT_REQ)
                        {
                            ProcessStream(streamBuffer, eventContext);
                            if (eventContext == HOST_STREAM_CONNECT_REQ && hostState == HostState.CONNECTED)
                            {
                                eventContext = HOST_STREAM_SPEED_REQ;
                                eventState = EventState.SEND;
                            }
                            else if (eventContext != HOST_STREAM_CONNECT_REQ)
                            {
                                eventContext = GetNextContext(eventContext);
                                eventState = EventState.SEND;
                            }
                        }
                    }
                }
            }
            else
            {
                hostState = HostState.DISCONNECTED;
                eventState = EventState.WAITCONNECT;
                eventContext = HOST_STREAM_CONNECT_REQ;
                streamRecieved = false;
                streamBufferPos = 0;
                streamState = StreamState.WAITFORHEAD;
                SerialManager.SetISRLength(1);
            }
        }
    }
}
