using System;
using System.Reflection;
using Improbable.Worker;

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
    }
}