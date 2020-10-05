using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Robot.CommandLine.Common
{
    public class ConsoleData
    {
        static public List<string> consoleData = new List<string>();

        static public void AddInfo(byte[] _stream, int _length, bool _send)
        {
            StringBuilder builder = new StringBuilder();
            for (int i = 0; i < _length; i++)
            {
                builder.Append(string.Format("{0:X2} ", _stream[i]));
            }
            consoleData.Add((_send ? "Send: " : "Recieve: ") + builder.ToString().Trim());
        }
    }
}
