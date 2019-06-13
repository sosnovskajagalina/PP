using System;
using System.Diagnostics;
using System.Threading;

namespace pplab4
{
    public class CriticalSectionImpl : ICriticalSection
    {
        private int Locked = 1;
        private int spinCount;
        private int enterWaitTime = 10;

        public CriticalSectionImpl(int count)
        {
            this.spinCount = count;
        }

        public void Enter()
        {
            while (true)
            {
                for (int i = 0; i < spinCount; i++)
                {
                    if (Interlocked.CompareExchange(ref Locked, 1, 0) == 0)
                    {
                        return;
                    }
                }
                Thread.Sleep(enterWaitTime);
            }
        }

        public void SetSpinCount(int count)
        {
            spinCount = count;
        }

        public bool TryEnter(int timeout)
        {
            var time = new Stopwatch();
            time.Start();
            do
            {
                for (int i = 0; i < spinCount; ++i)
                {
                    if (Interlocked.CompareExchange(ref Locked, 1, 0) == 0)
                    {
                        return true;
                    }
                }
                Thread.Sleep(enterWaitTime);
            } while (time.ElapsedMilliseconds < timeout);

            return false;
        }


        public void Leave()
        {
            if (Interlocked.CompareExchange(ref Locked, 1, 2) != 2)
            {
                throw new Exception("Can't out from this section.");
            }
        }

    }
}