using System;
using System.Reflection;
using Improbable.Worker;

namespace Example
{
    internal class Startup
    {
        private const string WorkerType = "Example";
        private const string LoggerName = "Startup.cs";

        private static int Main(string[] args)
        {
            Assembly.Load("GeneratedCode");
            var loop = new Example.Loop();
            var e = Astrum.Item.Manager.Create(0);
            Astrum.Item.Manager.Publish(e, loop);
            loop.Connect(args[1], Convert.ToUInt16(args[2]), WorkerType);
            return 0;
        }
    }
}