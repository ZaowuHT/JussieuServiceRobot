using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Robot.CommandLine.Common
{
    public class SystemTimer
    {
        static public int systemTimeMS = 0;
        static public System.Threading.Timer threadTimer;
        public delegate void TimerHandle();
        static public event TimerHandle TimerEvent;

        static public void InitTimer()
        {
            threadTimer = new System.Threading.Timer(new TimerCallback(TimerUp), null, 0, 20);
        }

        static private void TimerUp(object value)
        {
            systemTimeMS += 50;
            TimerEvent?.Invoke();
        }
    }
}
