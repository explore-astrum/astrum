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
            loop.Execute((dispatcher, connection) =>
            {
                var entity = Astrum.Item.Manager.Create(0);
                connection.SendCreateEntityRequest(entity, null, 100);

                dispatcher.OnCreateEntityResponse(op =>
                {
                    Console.WriteLine(op.EntityId);
                    Console.WriteLine(op.StatusCode);
                    Console.WriteLine("Entity created");
                });

            });
            loop.Connect(args[1], Convert.ToUInt16(args[2]), WorkerType);
            return 0;
        }
    }
}