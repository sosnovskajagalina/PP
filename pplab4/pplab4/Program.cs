using System;
using System.Diagnostics;
using System.Threading;

namespace pplab4
{
    class Program
    {
        private const int NUMBER_OF_THREADS = 3;
        private static ICriticalSection criticalSection;

        private static double pi = 0.0;

        private static int timeout;

        public static double GetPi(int numberOfThreads, int numberOfSteps, int numberOfSpins, int time, bool isEnter)
        {
            Thread[] threads = new Thread[numberOfThreads];
            int steps = numberOfSteps;
            timeout = time;
            criticalSection = new CriticalSectionImpl(numberOfSpins);
            int iterations = numberOfSteps / numberOfThreads;
            pi = 0;

            double step = 1.0 / steps;
            for (int i = 0; i < numberOfThreads; ++i)
            {
                Thread thread = isEnter ? new Thread(CalculateWithEnter) : new Thread(CalculateWithTryEnter);
                threads[i] = thread;

                ArgsThread argsThread = new ArgsThread(i * iterations, (i + 1) * iterations, step);
                thread.Start(argsThread);
            }

            for (int i = 0; i < numberOfThreads; ++i)
            {
                threads[i].Join();
            }

            return pi;

        }

        private static void CalculateWithEnter(object arg)
        {
            if (arg.GetType() != typeof(ArgsThread))
            {
                throw new Exception("Incorrect type");
            }
            ArgsThread argsThread = (ArgsThread)arg;
            for (long i = argsThread.left; i < argsThread.right; ++i)
            {
                double x = (i + 0.5) * argsThread.step;
                double value = 4.0 / (1 + x * x) * argsThread.step;

                criticalSection.Enter();
                pi += value;
                criticalSection.Leave();
            }
        }

        private static void CalculateWithTryEnter(object arg)
        {
            if (arg.GetType() != typeof(ArgsThread))
            {
                throw new Exception("Incorrect type");
            }
            ArgsThread argsThread = (ArgsThread)arg;
            for (long i = argsThread.left; i < argsThread.right;)
            {
                double x = (i + 0.5) * argsThread.step;
                double value = 4.0 / (1 + x * x) * argsThread.step;

                if (criticalSection.TryEnter(timeout))
                {
                    pi += value;
                    criticalSection.Leave();
                    ++i;
                }
            }
        }
        static void Main(string[] args)
        {
            string Enter = "Enter:";
            string EnterTime = "Enter Time:";
            string EnterPI = "Enter Time:";
            string TryEnter = "TryEnter:";
            string TryEnterTime = "Enter Time:";
            string TryEnterPI = "Enter Time:";


            if (args.Length != 3)
            {
                Console.WriteLine("The number og arguments uncorrect u need at less 3 arguments");
                return;
            }
        
            int numberOfSteps = Int32.Parse(args[0]);
            int timeout = Int32.Parse(args[1]);
            int numberOfSpins = Int32.Parse(args[2]);

            Stopwatch time = new Stopwatch();

            Console.WriteLine(Enter);
            time.Start();
            double pi = GetPi(NUMBER_OF_THREADS, numberOfSteps, numberOfSpins, timeout, true);
            time.Stop();
            Console.WriteLine(EnterTime + time.ElapsedMilliseconds);
            Console.WriteLine(EnterPI + pi);

            time.Reset();
            Console.WriteLine(TryEnter);
            time.Start();
            pi = GetPi(NUMBER_OF_THREADS, numberOfSteps, numberOfSpins, timeout, false);
            time.Stop();
            Console.WriteLine(TryEnterTime + time.ElapsedMilliseconds);
            Console.WriteLine(TryEnterPI + pi);
        }
    }
}
