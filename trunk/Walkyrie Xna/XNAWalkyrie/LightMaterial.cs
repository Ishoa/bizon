using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace XNAWalkyrie
{
    public struct Light
    {
        public enum LightType
        {
            DirectionalLight,
            PointLight,
            SpotLight
        }

        public LightType Type;
        public Vector3 Direction;
        public Vector3 Position;
        public Color Ambient;
        public Color Diffuse;
        public Color Specular;
        public float SpotInnerConeRadians;
        public float SpotOuterConeRadians;
        public float Radius;
    }


    /// <summary>
    /// A material. This material structure is the same as the one defined
    /// in the parallax_normal_mapping.fx file. We use the Color type here
    /// instead of a four element floating point array.
    /// </summary>
    public struct Material
    {
        public Color Ambient;
        public Color Diffuse;
        public Color Emissive;
        public Color Specular;
        public float Shininess;
    }
}
