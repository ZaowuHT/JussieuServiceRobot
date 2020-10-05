using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Robot.CommandLine.Common
{
    public class SerialManager
    {
        static public SerialPort serialPort = new SerialPort();

        static public List<string> GetCOMList()
        {
            List<string> retValue = new List<string>();
            retValue.AddRange(SerialPort.GetPortNames());
            return retValue;
        }

        static public void SetISRLength(int _len)
        {
            serialPort.ReceivedBytesThreshold = _len;
        }

        static public bool Connect(string _com, int _baud)
        {
            bool retValue = false;
            try
            {
                SetISRLength(1);
                serialPort.BaudRate = _baud;
                serialPort.PortName = _com;
                serialPort.ReadBufferSize = 512;
                serialPort.Open();
                retValue = true;
            }
            catch (Exception) { };
            return retValue;
        }

        static public void SendStream(byte[] _stream, int _len)
        {
            try
            {
                if (serialPort.IsOpen)
                {
                    ConsoleData.AddInfo(_stream, _len, true);
                    serialPort.Write(_stream, 0, _len);
                }
            }
            catch (Exception) { };
        }
    };
};
