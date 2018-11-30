using System;
using System.Reflection;
using Improbable.Worker;
using System.Threading.Tasks;

namespace Astrum.Item
{
    static class Manager
    {
        public static Entity Create(ulong type)
        {
            var definition = Definition.Get(type);
            var e = new Entity();
            e.Add(new Astrum.Schema.Ownable.Data(new Schema.OwnableData()));
            e.Add(new Astrum.Schema.Item.Data(type));
            e.Add(new Improbable.Metadata.Data(definition.Slug));
            e.Add(new Improbable.Persistence.Data());
            e.Add(new Improbable.Metadata.Data(definition.Slug));
            e.Add(new Improbable.Position.Data(new Improbable.Coordinates(0, 0, 0)));
            Tags.Add(e, definition.Tags);
            return e;
        }

        public static async Task<CreateEntityResponseOp> Publish(Entity e, Example.Loop loop)
        {
            var t = new TaskCompletionSource<CreateEntityResponseOp>();
            ulong cb = 0;
            loop.Dispatch((view, connection) =>
            {
                var req = connection.SendCreateEntityRequest(e, null, 100);
                cb = view.OnCreateEntityResponse(op =>
                {
                    if (op.RequestId != req) return;
                    t.SetResult(op);
                });
            });
            var result = await t.Task;
            Console.WriteLine("Entity published " + result.EntityId);
            loop.Dispatch((view, conn) => view.Remove(cb));
            return result;
        }
    }
}