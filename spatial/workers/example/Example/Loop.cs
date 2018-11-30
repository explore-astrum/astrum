using System;
using System.Reflection;
using Improbable.Worker;
using System.Threading.Tasks;
using System.Collections.Concurrent;

namespace Example
{
    public class Loop
    {

        private ConcurrentBag<Action<View, Connection>> actions = new ConcurrentBag<Action<View, Connection>>();
        public Loop()
        {
        }

        public void Connect(string hostname, ushort port, string worker_type)
        {
            var connectionParameters = new ConnectionParameters
            {
                WorkerType = worker_type,
                Network =
                {
                    ConnectionType = NetworkConnectionType.Tcp,
                    UseExternalIp = false
                }
            };

            using (var future = Connection.ConnectAsync(hostname, port, Guid.NewGuid().ToString(), connectionParameters))
            using (var connection = future.Get())
            using (var view = new View())
            {
                var isConnected = true;

                view.OnDisconnect(op =>
                {
                    Console.Error.WriteLine("[disconnect] " + op.Reason);
                    isConnected = false;
                });

                view.OnLogMessage(op =>
                {
                    if (op.Level == LogLevel.Fatal)
                    {
                        Console.Error.WriteLine("Fatal error: " + op.Message);
                        Environment.Exit(1);
                    }
                });

                while (isConnected)
                {
                    using (var opList = connection.GetOpList(100))
                    {
                        view.Process(opList);
                    }
                    Action<View, Connection> action;
                    while (!this.actions.IsEmpty)
                    {
                        this.actions.TryTake(out action);
                        action(view, connection);
                    }

                }
            }
        }

        public void Execute(Action<Dispatcher, Connection> cb)
        {
            this.actions.Add((dispatcher, connection) =>
            {
                cb(dispatcher, connection);
            });
        }
    }

}