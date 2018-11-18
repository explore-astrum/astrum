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
                var entityIdReservationRequestId = connection.SendReserveEntityIdRequest(100);
                Console.WriteLine(entityIdReservationRequestId);
                var entityCreationRequestId = default(RequestId<CreateEntityRequest>);

                dispatcher.OnReserveEntityIdResponse(op =>
                {
                    Console.WriteLine(op.StatusCode);
                    if (op.RequestId == entityIdReservationRequestId && op.StatusCode == StatusCode.Success)
                    {
                        Console.WriteLine("cool");
                        var entity = new Entity();
                        // Empty ACL - should be customised.
                        entity.Add(new Improbable.EntityAcl.Data(
                          new Improbable.WorkerRequirementSet(new Improbable.Collections.List<Improbable.WorkerAttributeSet>()),
                          new Improbable.Collections.Map<uint, Improbable.WorkerRequirementSet>()));
                        // Needed for the entity to be persisted in snapshots.
                        entity.Add(new Improbable.Persistence.Data());
                        entity.Add(new Improbable.Metadata.Data("test"));
                        entity.Add(new Improbable.Position.Data(new Improbable.Coordinates(1, 2, 3)));
                        entityCreationRequestId = connection.SendCreateEntityRequest(entity, op.EntityId, 100);
                    }
                });

                dispatcher.OnCreateEntityResponse(op =>
                {
                    Console.WriteLine("Entity created");
                });

            });
            loop.Connect(args[1], Convert.ToUInt16(args[2]), WorkerType);
            return 0;
        }
    }
}