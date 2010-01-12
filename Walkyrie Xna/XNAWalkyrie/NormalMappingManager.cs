
using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace XNAWalkyrie
{
    public partial class Utility
    {
        /// <summary>
        /// Custom vertex structure used for normal mapping.
        /// </summary>
        public struct NormalMappedVertex
        {
            public static readonly VertexElement[] VertexElements =
            {
                new VertexElement(0, 0,
                                  VertexElementFormat.Vector3,
                                  VertexElementMethod.Default,
                                  VertexElementUsage.Position, 0),

                new VertexElement(0, sizeof(float) * 3,
                                  VertexElementFormat.Vector2,
                                  VertexElementMethod.Default,
                                  VertexElementUsage.TextureCoordinate, 0),

                new VertexElement(0, sizeof(float) * 5,
                                  VertexElementFormat.Vector3,
                                  VertexElementMethod.Default,
                                  VertexElementUsage.Normal, 0),

                new VertexElement(0, sizeof(float) * 8,
                                  VertexElementFormat.Vector4,
                                  VertexElementMethod.Default,
                                  VertexElementUsage.Tangent, 0)
            };

            public Vector3 Position;
            public Vector2 TexCoord;
            public Vector3 Normal;
            public Vector4 Tangent;

            public NormalMappedVertex(Vector3 position,
                                      Vector2 texCoord,
                                      Vector3 normal,
                                      Vector4 tangent)
            {
                Position = position;
                TexCoord = texCoord;
                Normal = normal;
                Tangent = tangent;
            }

            public static int SizeInBytes
            {
                get { return sizeof(float) * 12; }
            }

            /// <summary>
            /// Given the 3 vertices (position and texture coordinate) and the
            /// face normal of a triangle calculate and return the triangle's
            /// tangent vector. This method is designed to work with XNA's default
            /// right handed coordinate system and clockwise triangle winding order.
            /// Undefined behavior will result if any other coordinate system
            /// and/or winding order is used. The handedness of the local tangent
            /// space coordinate system is stored in the tangent's w component.
            /// </summary>
            /// <param name="pos1">Triangle vertex 1 position</param>
            /// <param name="pos2">Triangle vertex 2 position</param>
            /// <param name="pos3">Triangle vertex 3 position</param>
            /// <param name="texCoord1">Triangle vertex 1 texture coordinate</param>
            /// <param name="texCoord2">Triangle vertex 2 texture coordinate</param>
            /// <param name="texCoord3">Triangle vertex 3 texture coordinate</param>
            /// <param name="normal">Triangle face normal</param>
            /// <param name="tangent">Calculated tangent vector</param>
            public static void CalcTangent(ref Vector3 pos1,
                                           ref Vector3 pos2,
                                           ref Vector3 pos3,
                                           ref Vector2 texCoord1,
                                           ref Vector2 texCoord2,
                                           ref Vector2 texCoord3,
                                           ref Vector3 normal,
                                           out Vector4 tangent)
            {
                // Create 2 vectors in object space.
                // edge1 is the vector from vertex positions pos1 to pos3.
                // edge2 is the vector from vertex positions pos1 to pos2.
                Vector3 edge1 = pos3 - pos1;
                Vector3 edge2 = pos2 - pos1;

                edge1.Normalize();
                edge2.Normalize();

                // Create 2 vectors in tangent (texture) space that point in the
                // same direction as edge1 and edge2 (in object space).
                // texEdge1 is the vector from texture coordinates texCoord1 to texCoord3.
                // texEdge2 is the vector from texture coordinates texCoord1 to texCoord2.
                Vector2 texEdge1 = texCoord3 - texCoord1;
                Vector2 texEdge2 = texCoord2 - texCoord1;

                texEdge1.Normalize();
                texEdge2.Normalize();

                // These 2 sets of vectors form the following system of equations:
                //
                //  edge1 = (texEdge1.x * tangent) + (texEdge1.y * bitangent)
                //  edge2 = (texEdge2.x * tangent) + (texEdge2.y * bitangent)
                //
                // Using matrix notation this system looks like:
                //
                //  [ edge1 ]     [ texEdge1.x  texEdge1.y ]  [ tangent   ]
                //  [       ]  =  [                        ]  [           ]
                //  [ edge2 ]     [ texEdge2.x  texEdge2.y ]  [ bitangent ]
                //
                // The solution is:
                //
                //  [ tangent   ]        1     [ texEdge2.y  -texEdge1.y ]  [ edge1 ]
                //  [           ]  =  -------  [                         ]  [       ]
                //  [ bitangent ]      det A   [-texEdge2.x   texEdge1.x ]  [ edge2 ]
                //
                //  where:
                //        [ texEdge1.x  texEdge1.y ]
                //    A = [                        ]
                //        [ texEdge2.x  texEdge2.y ]
                //
                //    det A = (texEdge1.x * texEdge2.y) - (texEdge1.y * texEdge2.x)
                //
                // From this solution the tangent space basis vectors are:
                //
                //    tangent = (1 / det A) * ( texEdge2.y * edge1 - texEdge1.y * edge2)
                //  bitangent = (1 / det A) * (-texEdge2.x * edge1 + texEdge1.x * edge2)
                //     normal = cross(tangent, bitangent)

                Vector3 t;
                Vector3 b;
                float det = (texEdge1.X * texEdge2.Y) - (texEdge1.Y * texEdge2.X);

                if ((float)Math.Abs(det) < 1e-6f)    // almost equal to zero
                {
                    t = Vector3.UnitX;
                    b = Vector3.UnitY;
                }
                else
                {
                    det = 1.0f / det;

                    t.X = (texEdge2.Y * edge1.X - texEdge1.Y * edge2.X) * det;
                    t.Y = (texEdge2.Y * edge1.Y - texEdge1.Y * edge2.Y) * det;
                    t.Z = (texEdge2.Y * edge1.Z - texEdge1.Y * edge2.Z) * det;

                    b.X = (-texEdge2.X * edge1.X + texEdge1.X * edge2.X) * det;
                    b.Y = (-texEdge2.X * edge1.Y + texEdge1.X * edge2.Y) * det;
                    b.Z = (-texEdge2.X * edge1.Z + texEdge1.X * edge2.Z) * det;

                    t.Normalize();
                    b.Normalize();
                }

                // Calculate the handedness of the local tangent space.
                // The bitangent vector is the cross product between the triangle face
                // normal vector and the calculated tangent vector. The resulting bitangent
                // vector should be the same as the bitangent vector calculated from the
                // set of linear equations above. If they point in different directions
                // then we need to invert the cross product calculated bitangent vector. We
                // store this scalar multiplier in the tangent vector's 'w' component so
                // that the correct bitangent vector can be generated in the normal mapping
                // shader's vertex shader.

                Vector3 bitangent = Vector3.Cross(normal, t);
                float handedness = (Vector3.Dot(bitangent, b) < 0.0f) ? -1.0f : 1.0f;

                tangent.X = t.X;
                tangent.Y = t.Y;
                tangent.Z = t.Z;
                tangent.W = handedness;
            }
        }

        public abstract class NormalMappedObject
        {
            protected Texture2D nullTexture;
            public Texture2D textureColorMap;
            public Texture2D textureNormalMap;
            public Texture2D textureHeightMap;

            public ShadowMap ShadowMapNear;
            public ShadowMap ShadowMapFar;
            public float FocaleCameraNear;

            protected Effect effectParrallalaxMapping;

            protected bool disableColorMap;
            protected bool disableParallax;
            protected Vector2 scaleBias;

            protected Light light;
            protected Material material;

            protected VertexDeclaration vertexDeclaration;
            protected VertexBuffer vertexBuffer;

            protected NormalMappedVertex[] vertices;

            public Matrix World;

            public NormalMappedObject(ref Light l, ref Material m)
            {
                light = l;
                material = m;
            }

            

            public virtual void Initialize()
            {

                disableColorMap = false;
                disableParallax = false;
                World = Matrix.Identity;
                scaleBias = new Vector2(0.04f, -0.03f);

                vertexDeclaration = new VertexDeclaration(GraphicsDevice, NormalMappedVertex.VertexElements);

                vertexBuffer = new VertexBuffer(GraphicsDevice,
                                NormalMappedVertex.SizeInBytes * vertices.Length,
                                BufferUsage.WriteOnly);

                vertexBuffer.SetData<NormalMappedVertex>(vertices);


            }

            public virtual void LoadContent()
            {

                //effectParrallalaxMapping = Utility.Game.Content.Load<Effect>(@"Effects\parallax_normal_mapping");
                effectParrallalaxMapping = Utility.Game.Content.Load<Effect>(@"Effects\parallax_normal_mapping_DoubleMap"); 
                
                nullTexture = new Texture2D(Utility.Game.GraphicsDevice, 1, 1, 0, TextureUsage.None, SurfaceFormat.Color);

                Color[] pixels = { Color.White };

                nullTexture.SetData(pixels);
                textureColorMap = nullTexture;
                textureNormalMap = nullTexture;
                textureHeightMap = nullTexture;

                
            }

            public virtual void Update(GameTime gameTime)
            {

                // Set the shader camera position parameter.
                effectParrallalaxMapping.Parameters["cameraPos"].SetValue(Utility.Camera.Position);

                // Set the shader global ambiance parameters.
                effectParrallalaxMapping.Parameters["globalAmbient"].SetValue(0.0f);


                effectParrallalaxMapping.Parameters["lightViewProjectionNear"].SetValue(ShadowMapNear.LightViewProjectionMatrix);
                effectParrallalaxMapping.Parameters["textureScaleBiasNear"].SetValue(ShadowMapNear.TextureScaleBiasMatrix);
                effectParrallalaxMapping.Parameters["depthBiasNear"].SetValue(ShadowMapNear.DepthBias);
                effectParrallalaxMapping.Parameters["shadowMapNear"].SetValue(ShadowMapNear.ShadowMapTexture);

                effectParrallalaxMapping.Parameters["lightViewProjectionFar"].SetValue(ShadowMapFar.LightViewProjectionMatrix);
                effectParrallalaxMapping.Parameters["textureScaleBiasFar"].SetValue(ShadowMapFar.TextureScaleBiasMatrix);
                effectParrallalaxMapping.Parameters["depthBiasFar"].SetValue(ShadowMapFar.DepthBias);
                effectParrallalaxMapping.Parameters["shadowMapFar"].SetValue(ShadowMapFar.ShadowMapTexture);

                effectParrallalaxMapping.Parameters["depthNear"].SetValue(FocaleCameraNear);
                effectParrallalaxMapping.Parameters["texelSizeNear"].SetValue(ShadowMapNear.TexelSize);
                effectParrallalaxMapping.Parameters["texelSizeFar"].SetValue(ShadowMapFar.TexelSize);
            }

            public virtual void Draw(GameTime gameTime)
            {
                if (vertexBuffer != null)
                {
                    GraphicsDevice.VertexDeclaration = vertexDeclaration;
                    GraphicsDevice.Vertices[0].SetSource(vertexBuffer, 0, NormalMappedVertex.SizeInBytes);

                    effectParrallalaxMapping.Parameters["worldMatrix"].SetValue(World);
                    effectParrallalaxMapping.Parameters["worldInverseTransposeMatrix"].SetValue(Matrix.Invert(World));
                    effectParrallalaxMapping.Parameters["worldViewProjectionMatrix"].SetValue(World * Utility.Camera.ViewProjectionMatrix);

                    effectParrallalaxMapping.Parameters["colorMapTexture"].SetValue((disableColorMap) ? nullTexture : textureColorMap);
                    effectParrallalaxMapping.Parameters["normalMapTexture"].SetValue(textureNormalMap);
                    effectParrallalaxMapping.Parameters["heightMapTexture"].SetValue(textureHeightMap);

                    // Set the shader parallax scale and bias parameter.
                    effectParrallalaxMapping.Parameters["scaleBias"].SetValue(scaleBias);

                    // Set the shader lighting parameters.
                    effectParrallalaxMapping.Parameters["light"].StructureMembers["dir"].SetValue(light.Direction);
                    effectParrallalaxMapping.Parameters["light"].StructureMembers["pos"].SetValue(light.Position);
                    effectParrallalaxMapping.Parameters["light"].StructureMembers["ambient"].SetValue(light.Ambient.ToVector4());
                    effectParrallalaxMapping.Parameters["light"].StructureMembers["diffuse"].SetValue(light.Diffuse.ToVector4());
                    effectParrallalaxMapping.Parameters["light"].StructureMembers["specular"].SetValue(light.Specular.ToVector4());
                    effectParrallalaxMapping.Parameters["light"].StructureMembers["spotInnerCone"].SetValue(light.SpotInnerConeRadians);
                    effectParrallalaxMapping.Parameters["light"].StructureMembers["spotOuterCone"].SetValue(light.SpotOuterConeRadians);
                    effectParrallalaxMapping.Parameters["light"].StructureMembers["radius"].SetValue(light.Radius);

                    // Set the shader material parameters.
                    effectParrallalaxMapping.Parameters["material"].StructureMembers["ambient"].SetValue(material.Ambient.ToVector4());
                    effectParrallalaxMapping.Parameters["material"].StructureMembers["diffuse"].SetValue(material.Diffuse.ToVector4());
                    effectParrallalaxMapping.Parameters["material"].StructureMembers["emissive"].SetValue(material.Emissive.ToVector4());
                    effectParrallalaxMapping.Parameters["material"].StructureMembers["specular"].SetValue(material.Specular.ToVector4());
                    effectParrallalaxMapping.Parameters["material"].StructureMembers["shininess"].SetValue(material.Shininess);

                    // Select the shader based on light type.
                    switch (light.Type)
                    {
                        case Light.LightType.DirectionalLight:
                            if (disableParallax)
                                effectParrallalaxMapping.CurrentTechnique = effectParrallalaxMapping.Techniques["NormalMappingDirectionalLighting"];
                            else
                                effectParrallalaxMapping.CurrentTechnique = effectParrallalaxMapping.Techniques["ParallaxNormalMappingDirectionalLighting"];
                            break;

                        case Light.LightType.PointLight:
                            if (disableParallax)
                                effectParrallalaxMapping.CurrentTechnique = effectParrallalaxMapping.Techniques["NormalMappingPointLighting"];
                            else
                                effectParrallalaxMapping.CurrentTechnique = effectParrallalaxMapping.Techniques["ParallaxNormalMappingPointLighting"];
                            break;

                        case Light.LightType.SpotLight:
                            if (disableParallax)
                                effectParrallalaxMapping.CurrentTechnique = effectParrallalaxMapping.Techniques["NormalMappingSpotLighting"];
                            else
                                effectParrallalaxMapping.CurrentTechnique = effectParrallalaxMapping.Techniques["ParallaxNormalMappingSpotLighting"];
                            break;

                        default:
                            break;
                    }

                    effectParrallalaxMapping.CommitChanges();

                    // Draw the scene geometry.

                    effectParrallalaxMapping.Begin(SaveStateMode.SaveState);

                    // Draw the walls.


                    foreach (EffectPass pass in effectParrallalaxMapping.CurrentTechnique.Passes)
                    {
                        pass.Begin();
                        graphicsDevice.DrawPrimitives(PrimitiveType.TriangleList, 0, vertices.Length / 3);
                        pass.End();
                    }

                    effectParrallalaxMapping.End();
                }

            }


            /// <summary>
            /// Returns the quad's vertex list.
            /// </summary>
            public NormalMappedVertex[] Vertices
            {
                get { return vertices; }
            }
        }


        /// <summary>
        /// The NormalMappedQuad class is used to procedurally generate a quad
        /// made up of two triangles. This class generates geometry using the
        /// NormalMappedVertex structure.
        /// </summary>
        public class NormalMappedQuad : NormalMappedObject
        {
            
            public NormalMappedQuad(ref Light l, ref Material m, 
                                    Vector3 origin, Vector3 normal, Vector3 up,
                                    float width, float height,
                                    float uTile, float vTile): base(ref l, ref m)
            {
                Generate(origin, normal, up, width, height, uTile, vTile);


            }

            /// <summary>
            /// Procedurally generate a quad. The quad is made using 2 triangles.
            /// </summary>
            /// <param name="origin">The center position of the quad.</param>
            /// <param name="normal">The quad's surface normal.</param>
            /// <param name="up">A vector pointing at the top of the quad.</param>
            /// <param name="width">Width of the quad.</param>
            /// <param name="height">Height of the quad.</param>
            /// <param name="uTile">Horizontal texture tiling factor.</param>
            /// <param name="vTile">Vertical texture tiling factor.</param>
            public void Generate(Vector3 origin, Vector3 normal, Vector3 up,
                                 float width, float height,
                                 float uTile, float vTile)
            {
                Vector3 left = Vector3.Cross(normal, up);
                Vector3 posUpperCenter = (up * height / 2.0f) + origin;
                Vector3 posUpperLeft = posUpperCenter + (left * width / 2.0f);
                Vector3 posUpperRight = posUpperCenter - (left * width / 2.0f);
                Vector3 posLowerLeft = posUpperLeft - (up * height);
                Vector3 posLowerRight = posUpperRight - (up * height);
                /*
                Vector2 textureUpperLeft = new Vector2(0.0f, 0.0f);
                Vector2 textureUpperRight = new Vector2(1.0f * uTile, 0.0f);
                Vector2 textureLowerLeft = new Vector2(0.0f, 1.0f * vTile);
                Vector2 textureLowerRight = new Vector2(1.0f * uTile, 1.0f * vTile);
                */

                Vector2 textureUpperLeft = new Vector2(1.0f * uTile, 0.0f);
                Vector2 textureUpperRight = new Vector2(1.0f * uTile, 1.0f * vTile);
                Vector2 textureLowerLeft = new Vector2(0.0f, 0.0f);
                Vector2 textureLowerRight = new Vector2(0.0f, 1.0f * vTile);
                Vector4 tangent;

                vertices = new NormalMappedVertex[6];

                NormalMappedVertex.CalcTangent(
                    ref posUpperLeft, ref posUpperRight, ref posLowerLeft,
                    ref textureUpperLeft, ref textureUpperRight, ref textureLowerLeft,
                    ref normal, out tangent);

                vertices[0] = new NormalMappedVertex(posUpperLeft, textureUpperLeft, normal, tangent);
                vertices[1] = new NormalMappedVertex(posUpperRight, textureUpperRight, normal, tangent);
                vertices[2] = new NormalMappedVertex(posLowerLeft, textureLowerLeft, normal, tangent);

                NormalMappedVertex.CalcTangent(
                    ref posLowerLeft, ref posUpperRight, ref posLowerRight,
                    ref textureLowerLeft, ref textureUpperRight, ref textureLowerRight,
                    ref normal, out tangent);

                vertices[3] = new NormalMappedVertex(posLowerLeft, textureLowerLeft, normal, tangent);
                vertices[4] = new NormalMappedVertex(posUpperRight, textureUpperRight, normal, tangent);
                vertices[5] = new NormalMappedVertex(posLowerRight, textureLowerRight, normal, tangent);
            }


        }


        public class NormalMappedModel : NormalMappedObject
        {

            public NormalMappedModel(ref Light l, ref Material m, ref Model model) : base(ref l,ref m)
            {
                GenerateModel(model);

            }

            private void GenerateModel(Model model)
            {
                Vector3[] VerticesModel = model.GetFlattenedPositionArray();
                int[] IndicesModel = model.GetFlattenedIndexArray();
                Vector2[] TextCoordModel = model.GetTexCoordArray(0);

                vertices = new NormalMappedVertex[IndicesModel.Length];

                for (int x = 0; x < IndicesModel.Length / 3; x++)
                {
                    Vector3 A = VerticesModel[IndicesModel[x * 3 + 0]];
                    Vector3 B = VerticesModel[IndicesModel[x * 3 + 1]];
                    Vector3 C = VerticesModel[IndicesModel[x * 3 + 2]];

                    Vector3 AB = B - A;
                    Vector3 BC = C - B;
                    AB.Normalize();
                    BC.Normalize();

                    Vector3 normal = Vector3.Cross(BC, AB);
                    normal.Normalize();

                    Vector2 ATexCoord = TextCoordModel[IndicesModel[x * 3 + 0]];
                    Vector2 BTexCoord = TextCoordModel[IndicesModel[x * 3 + 1]];
                    Vector2 CTexCoord = TextCoordModel[IndicesModel[x * 3 + 2]];

                    Vector4 tangent;
                    NormalMappedVertex.CalcTangent(
                                                    ref A,
                                                    ref B,
                                                    ref C,
                                                    ref ATexCoord,
                                                    ref BTexCoord,
                                                    ref CTexCoord,
                                                    ref normal,
                                                    out tangent);

                    vertices[x * 3 + 0] = new NormalMappedVertex(A, ATexCoord, normal, tangent);
                    vertices[x * 3 + 1] = new NormalMappedVertex(B, BTexCoord, normal, tangent);
                    vertices[x * 3 + 2] = new NormalMappedVertex(C, CTexCoord, normal, tangent);
                }

            }

        }

    }
}