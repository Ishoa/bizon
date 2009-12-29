// Using Statements
using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content;

namespace XNAWalkyrie
{
    public partial class Utility
    {

        private static BillboardManager billboardManager = null;

        public static BillboardManager BillboardManager
        {
            get
            {
                if (billboardManager == null)
                {
                    billboardManager = new BillboardManager(game);
                    game.Components.Add(BillboardManager);
                    billboardManager.Initialize();
                }
                return billboardManager;
            }
            set
            {
                if (billboardManager != null)
                {
                    RemoveService<BillboardManagerService>();
                }
                billboardManager = value;
                AddService<BillboardManagerService>(new BillboardManagerService(billboardManager));
            }
        }
    }

    public interface IBillboardManagerService
    {
        BillboardManager BillboardManager { get; }
    }

    public class BillboardManagerService : IBillboardManagerService
    {

        public BillboardManagerService(BillboardManager billboardManager)
        {
            this.billboardManager = billboardManager;
        }

        private BillboardManager billboardManager;

        public BillboardManager BillboardManager
        {
            get
            {
                return billboardManager;
            }
        }

        public static implicit operator BillboardManager(BillboardManagerService cs)
        {
            return cs.BillboardManager;
        }
    }


        public class BillboardManager : DrawableGameComponent
        {
            List<Billboard> billBoardList = new List<Billboard>();

            QuadRenderComponent quadRenderer;
            Effect BilboardEffect;

            List<Billboard> BillBoardList
            {
                get { return billBoardList; }
            }


            public BillboardManager(Game g)
                : base(g)
            {

               
            }

            public void AddBillboard(Billboard b)
            {
                billBoardList.Add(b);
            }

            public override void Initialize()
            {
                quadRenderer = new QuadRenderComponent();
                Utility.Game.Components.Add(quadRenderer);
                quadRenderer.Initialize();
                billBoardList.Clear();

                base.Initialize();
            }

            protected override void LoadContent()
            {
                BilboardEffect = Utility.Game.Content.Load<Effect>("Billboard");
                base.LoadContent();
            }

            public override void Update(GameTime gameTime)
            {

                for (int x = 0; x < billBoardList.Count; )
                {
                    Billboard b = billBoardList[x];
                    if (!b.Active)
                    {
                        billBoardList.RemoveAt(x);
                    }
                    else
                        x++;
                }

                billBoardList.Sort();
                base.Update(gameTime);
            }

            public override void Draw(GameTime gameTime)
            {

                GraphicsDevice.RenderState.AlphaBlendEnable = true;

                BilboardEffect.Begin(SaveStateMode.SaveState);
                BilboardEffect.Techniques[0].Passes[0].Begin();


                Matrix world, ViewProj, worldViewProj;
                Vector3 camPos = Utility.Camera.Position;
                Vector3 camUp = Utility.Camera.YAxis;
                Vector3 camDir = Utility.Camera.OrbitTarget - Utility.Camera.Position;
                ViewProj = Utility.Camera.ViewProjectionMatrix;
                for (int x = 0; x < BillBoardList.Count; x++)
                {
                    Billboard p = BillBoardList[x];

                    BilboardEffect.Parameters["baseTexture"].SetValue(p.Texture);



                    Matrix.CreateBillboard(ref p.Position, ref camPos,
                                           ref camUp, camDir, out world);

                    world = Matrix.CreateRotationZ(p.Rotation) * world;

                    Matrix.Multiply(ref world, ref ViewProj, out worldViewProj);

                    BilboardEffect.Parameters["worldViewProj"].SetValue(worldViewProj);

                    BilboardEffect.CommitChanges();

                    quadRenderer.Render(Vector3.Zero, p.Size, p.Couleur);
                }

                BilboardEffect.Techniques[0].Passes[0].End();
                BilboardEffect.End();

                GraphicsDevice.RenderState.AlphaBlendEnable = false;

                base.Draw(gameTime);
            }


        }

        public class Billboard : IComparable
        {
            public Vector3 Position;
            public float Size;
            public Color Couleur;
            public float Rotation;
            public Texture2D Texture;
            public bool Active;

            public Billboard(Vector3 position, float size, Color couleur)
            {
                Position = position;
                Size = size;
                Couleur = couleur;
                Rotation = 0.0f;
                Active = true;
                Texture = null;
            }

            public int CompareTo(object obj)
            {
                Billboard p = (Billboard)obj;

                float f1 = Vector3.DistanceSquared(Position, Utility.Camera.Position);
                float f2 = Vector3.DistanceSquared(p.Position, Utility.Camera.Position);

                if (f1 > f2)
                    return -1;
                else if (f1 < f2)
                    return 1;
                else
                    return 0;
            }
        }


        public class QuadRenderComponent : DrawableGameComponent
        {
            // Private Members
            Effect quadRenderEffect;
            VertexDeclaration vertexDecl = null;
            VertexPositionColorTexture[] verts = null;
            //VertexPositionTexture[] verts = null;
            short[] ib = null;


            // Constructor
            public QuadRenderComponent()
                : base(Utility.Game)
            {

            }

            public override void Initialize()
            {
                base.Initialize();
            }

            protected override void LoadContent()
            {

                quadRenderEffect = Game.Content.Load<Effect>("simpleEffect");

                vertexDecl = new VertexDeclaration(
                    GraphicsDevice, VertexPositionColorTexture.VertexElements);

                verts = new VertexPositionColorTexture[]
            {
                new VertexPositionColorTexture(
                    new Vector3(0,0,0),
                    new Color(1,1,1,1),
                    new Vector2(1,1)),
                new VertexPositionColorTexture(
                    new Vector3(0,0,0),
                    new Color(1,1,1,1),
                    new Vector2(0,1)),
                new VertexPositionColorTexture(
                    new Vector3(0,0,0),
                    new Color(1,1,1,1),
                    new Vector2(0,0)),
                new VertexPositionColorTexture(
                    new Vector3(0,0,0),
                    new Color(1,1,1,1),
                    new Vector2(1,0))
            };

                ib = new short[] { 0, 1, 2, 2, 3, 0 };

                base.LoadContent();
            }


            protected override void UnloadContent()
            {
                vertexDecl.Dispose();

                base.UnloadContent();
            }


            public void Begin()
            {
                quadRenderEffect.Begin();
                quadRenderEffect.Techniques[0].Passes[0].Begin();
            }

            public void End()
            {
                quadRenderEffect.Techniques[0].Passes[0].End();
                quadRenderEffect.End();
            }

            public void Render(Vector2 v1, Vector2 v2, Color col)
            {
                GraphicsDevice.VertexDeclaration = vertexDecl;
                verts[0].Position.X = v2.X;
                verts[0].Position.Y = v1.Y;
                verts[0].Color = col;

                verts[1].Position.X = v1.X;
                verts[1].Position.Y = v1.Y;
                verts[0].Color = col;

                verts[2].Position.X = v1.X;
                verts[2].Position.Y = v2.Y;
                verts[0].Color = col;

                verts[3].Position.X = v2.X;
                verts[3].Position.Y = v2.Y;
                verts[0].Color = col;

                GraphicsDevice.DrawUserIndexedPrimitives<VertexPositionColorTexture>
                                    (PrimitiveType.TriangleList, verts, 0, 4, ib, 0, 2);
            }


            public void Render(Vector3 v, float width, Color col)
            {

                GraphicsDevice.VertexDeclaration = vertexDecl;

                verts[0].Position.X = v.X - width / 2.0f;
                verts[0].Position.Y = v.Y - width / 2.0f;
                verts[0].Position.Z = v.Z;
                verts[0].Color = col;

                verts[1].Position.X = v.X + width / 2.0f;
                verts[1].Position.Y = v.Y - width / 2.0f;
                verts[1].Position.Z = v.Z;
                verts[1].Color = col;

                verts[2].Position.X = v.X + width / 2.0f;
                verts[2].Position.Y = v.Y + width / 2.0f;
                verts[2].Position.Z = v.Z;
                verts[2].Color = col;

                verts[3].Position.X = v.X - width / 2.0f;
                verts[3].Position.Y = v.Y + width / 2.0f;
                verts[3].Position.Z = v.Z;
                verts[3].Color = col;

                GraphicsDevice.DrawUserIndexedPrimitives<VertexPositionColorTexture>
                    (PrimitiveType.TriangleList, verts, 0, 4, ib, 0, 2);
            }
        }
}
