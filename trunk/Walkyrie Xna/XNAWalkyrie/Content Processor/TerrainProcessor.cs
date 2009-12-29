#region File Description
//-----------------------------------------------------------------------------
// TerrainProcessor.cs
//
// Microsoft XNA Community Game Platform
// Copyright (C) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#endregion

#region Using Statements
using System.IO;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content.Pipeline;
using Microsoft.Xna.Framework.Content.Pipeline.Graphics;
using Microsoft.Xna.Framework.Content.Pipeline.Processors;
#endregion

namespace XNAWalkyrie
{
    /// <summary>
    /// Custom content processor for creating terrain meshes. Given an
    /// input heightfield texture, this processor uses the MeshBuilder
    /// class to programatically generate terrain geometry.
    /// </summary>
    [ContentProcessor]
    public class TerrainProcessor : ContentProcessor<Texture2DContent, ModelContent>
    {
        const float terrainScale = 1;
        const float terrainBumpiness = 1.0f;
        const float texCoordScale = 0.1f;

        /// <summary>
        /// Generates a terrain mesh from an input heightfield texture.
        /// </summary>
        public override ModelContent Process(Texture2DContent input,
                                             ContentProcessorContext context)
        {
            MeshBuilder builder = MeshBuilder.StartMesh("terrain");

            // Convert the input texture to float format, for ease of processing.
            input.ConvertBitmapType(typeof(PixelBitmapContent<float>));

            PixelBitmapContent<float> heightfield;
            heightfield = (PixelBitmapContent<float>)input.Mipmaps[0];

            
            //####################################################################
            PixelBitmapContent<float> resheightfield = new PixelBitmapContent<float>(heightfield.Width, heightfield.Height);

            // Lissage de l'image sans les bordures

            for (int j = 1; j < heightfield.Height - 1; j++)
            {
                for (int i = 1; i < heightfield.Width - 1; i++)
                {
                    resheightfield.SetPixel(i, j,
                                            (heightfield.GetPixel(i, j)
                                            + heightfield.GetPixel(i, (j - 1))
                                            + heightfield.GetPixel(i, (j + 1))
                                            + heightfield.GetPixel((i - 1), j)
                                            + heightfield.GetPixel((i + 1), j)
                                            + heightfield.GetPixel((i - 1), (j - 1))
                                            + heightfield.GetPixel((i + 1), (j + 1))
                                            + heightfield.GetPixel((i + 1), (j - 1))
                                            + heightfield.GetPixel((i - 1), (j + 1))) / 9.0f
                                            );
                }
            }
            // Lissage bordure gauche
            for (int j = 1; j < heightfield.Height - 1; j++)
            {
                resheightfield.SetPixel(0, j,
                                       (heightfield.GetPixel(0, j)
                                       + heightfield.GetPixel(0, (j - 1))
                                       + heightfield.GetPixel(0, (j + 1))
                                       + heightfield.GetPixel(1, j)
                                       + heightfield.GetPixel(1, (j - 1))
                                       + heightfield.GetPixel(1, (j + 1))) / 6.0f
                                       );

            }

            // Lissage bordure droite
            for (int j = 1; j < heightfield.Height - 1; j++)
            {
                resheightfield.SetPixel((heightfield.Width - 1), j,
                                                    (heightfield.GetPixel((heightfield.Width - 1), j)
                                                     + heightfield.GetPixel((heightfield.Width - 1), (j - 1))
                                                     + heightfield.GetPixel((heightfield.Width - 1), (j + 1))
                                                     + heightfield.GetPixel((heightfield.Width - 2), j)
                                                     + heightfield.GetPixel((heightfield.Width - 2), (j - 1))
                                                     + heightfield.GetPixel((heightfield.Width - 2), (j + 1))) / 6.0f
                                                     );
            }
            // Lissage bordure haut
            for (int i = 1; i < heightfield.Width - 1; i++)
            {
                resheightfield.SetPixel(i, 0,
                                        (heightfield.GetPixel(i, 0)
                                        + heightfield.GetPixel((i + 1), 0)
                                        + heightfield.GetPixel((i - 1), 0)
                                        + heightfield.GetPixel(i, 1)
                                        + heightfield.GetPixel((i + 1), 1)
                                        + heightfield.GetPixel((i - 1), 1)) / 6.0f
                                        );
            }

            // Lissage bordure bas
            for (int i = 1; i < heightfield.Width - 1; i++)
            {
                resheightfield.SetPixel(i, (heightfield.Height - 1),
                                                    (heightfield.GetPixel(i, (heightfield.Height - 1))
                                                     + heightfield.GetPixel((i + 1), (heightfield.Height - 1))
                                                     + heightfield.GetPixel((i - 1), (heightfield.Height - 1))
                                                     + heightfield.GetPixel(i, (heightfield.Height - 2))
                                                     + heightfield.GetPixel((i + 1), (heightfield.Height - 2))
                                                     + heightfield.GetPixel((i - 1), (heightfield.Height - 2))) / 6.0f
                                                     );
            }

            // Lissage des angles de l'image
            resheightfield.SetPixel(0, 0, (heightfield.GetPixel(0, 0) + heightfield.GetPixel(0, 1) + heightfield.GetPixel(1, 0) + heightfield.GetPixel(1, 1)) / 4.0f);
            resheightfield.SetPixel((heightfield.Width - 1), 0, (heightfield.GetPixel((heightfield.Width - 1), 0) + heightfield.GetPixel((heightfield.Width - 1), 1) + heightfield.GetPixel((heightfield.Width - 2), 0) + heightfield.GetPixel((heightfield.Width - 2), 1)) / 4.0f);
            resheightfield.SetPixel(0, (heightfield.Height - 1), (heightfield.GetPixel(0, (heightfield.Height - 1)) + heightfield.GetPixel(1, (heightfield.Height - 1)) + heightfield.GetPixel(0, (heightfield.Height - 2)) + heightfield.GetPixel(1, (heightfield.Height - 2))) / 4.0f);
            resheightfield.SetPixel((heightfield.Width - 1), (heightfield.Height - 1), (heightfield.GetPixel((heightfield.Width - 1), (heightfield.Height - 1)) + heightfield.GetPixel((heightfield.Width - 2), (heightfield.Height - 1)) + heightfield.GetPixel((heightfield.Width - 1), (heightfield.Height - 2)) + heightfield.GetPixel((heightfield.Width - 2), (heightfield.Height - 2))) / 4.0f);

            heightfield = resheightfield; 

            //####################################################################
           

            // Create the terrain vertices.
            for (int y = 0; y < heightfield.Height; y++)
            {
                for (int x = 0; x < heightfield.Width; x++)
                {
                    Vector3 position;

                    position.X = (x - heightfield.Width / 2) * terrainScale;
                    position.Z = (y - heightfield.Height / 2) * terrainScale;

                    position.Y = (heightfield.GetPixel(x, y)) * terrainBumpiness;

                    builder.CreatePosition(position);
                }
            }

            // Create a material, and point it at our terrain texture.
            BasicMaterialContent material = new BasicMaterialContent();

            string directory = Path.GetDirectoryName(input.Identity.SourceFilename);

            builder.SetMaterial(material);

            // Create a vertex channel for holding texture coordinates.
            int texCoordId0 = builder.CreateVertexChannel<Vector2>(VertexChannelNames.TextureCoordinate(0));
            int texCoordId1 = builder.CreateVertexChannel<Vector2>(VertexChannelNames.TextureCoordinate(1));

            float xScaleText = 1.0f / heightfield.Width;
            float yScaleText = 1.0f / heightfield.Height;

            // Create the individual triangles that make up our terrain.
            for (int y = 0; y < heightfield.Height - 1; y++)
            {
                for (int x = 0; x < heightfield.Width - 1; x++)
                {
                    AddVertex(builder, texCoordId0, texCoordId1, heightfield.Width, x, y, xScaleText, yScaleText);
                    AddVertex(builder, texCoordId0, texCoordId1, heightfield.Width, x + 1, y, xScaleText, yScaleText);
                    AddVertex(builder, texCoordId0, texCoordId1, heightfield.Width, x + 1, y + 1, xScaleText, yScaleText);

                    AddVertex(builder, texCoordId0, texCoordId1, heightfield.Width, x, y, xScaleText, yScaleText);
                    AddVertex(builder, texCoordId0, texCoordId1, heightfield.Width, x + 1, y + 1, xScaleText, yScaleText);
                    AddVertex(builder, texCoordId0, texCoordId1, heightfield.Width, x, y + 1, xScaleText, yScaleText);
                }
            }

            // Chain to the ModelProcessor so it can convert the mesh we just generated.
            MeshContent terrainMesh = builder.FinishMesh();
            
            return context.Convert<MeshContent, ModelContent>(terrainMesh,"ModelProcessor");
        }


        /// <summary>
        /// Helper for adding a new triangle vertex to a MeshBuilder,
        /// along with an associated texture coordinate value.
        /// </summary>
        static void AddVertex(MeshBuilder builder, int texCoordId0, int texCoordId1, int w, int x, int y, float xScale, float yScale)
        {
            builder.SetVertexChannelData(texCoordId0, new Vector2(x, y) * texCoordScale);
            //builder.SetVertexChannelData(texCoordId1, new Vector2(x, y) * texCoordScale);
            builder.SetVertexChannelData(texCoordId1, new Vector2(x * xScale, y * yScale));

            builder.AddTriangleVertex(x + y * w);
        }

    }
}
