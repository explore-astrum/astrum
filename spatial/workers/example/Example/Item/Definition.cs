using System;
using System.Reflection;
using System.Collections.Generic;
using Improbable.Worker;

namespace Astrum.Item
{
    public class Definition
    {
        public ulong Key { get; set; }
        public string Slug { get; set; }
        public string Name { get; set; }
        public string[] Tags { get; set; }
        public Definition()
        {
        }

        static Definition()
        {
            Add(new Definition()
            {
                Key = 0,
                Slug = "box",
                Name = "Box",
                Tags = new string[] { "flammable" }
            });
        }

        public static Dictionary<ulong, Definition> All = new Dictionary<ulong, Definition>();

        public static void Add(Definition definition)
        {
            All.Add(definition.Key, definition);
        }

        public static Definition Get(ulong key)
        {
            return All[key];
        }

    }

    public class Tags
    {
        public static void Add(Entity e, string[] tags)
        {
            foreach (var t in tags)
            {
                if (t == "flammable") e.Add(new Astrum.Schema.Tags.Flammable.Data(false));
            }
        }
    }
}