#region Using Statements
using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Storage;
#endregion

namespace FlatShadows
{
    /// <summary>
    /// This is the main type for your game
    /// </summary>
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        ContentManager content;

        
        Model ground;
        Model scene;
        Model mirror;
        
        
        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            content = new ContentManager(Services);

            graphics.PreferredDepthStencilFormat = SelectStencilMode();

        }

        private DepthFormat SelectStencilMode()
        {
            // Check stencil formats
            GraphicsAdapter adapter = GraphicsAdapter.DefaultAdapter;
            SurfaceFormat format = adapter.CurrentDisplayMode.Format;
            if (adapter.CheckDepthStencilMatch(DeviceType.Hardware, format, format, DepthFormat.Depth24Stencil8))
                return DepthFormat.Depth24Stencil8;
            else if (adapter.CheckDepthStencilMatch(DeviceType.Hardware, format, format, DepthFormat.Depth24Stencil8Single))
                return DepthFormat.Depth24Stencil8Single;
            else if (adapter.CheckDepthStencilMatch(DeviceType.Hardware, format, format, DepthFormat.Depth24Stencil4))
                return DepthFormat.Depth24Stencil4;
            else if (adapter.CheckDepthStencilMatch(DeviceType.Hardware, format, format, DepthFormat.Depth15Stencil1))
                return DepthFormat.Depth15Stencil1;
            else
                throw new ApplicationException("Could Not Find Stencil Buffer for Default Adapter");
        }

        /// <summary>
        /// Allows the game to perform any initialization it needs to before starting to run.
        /// This is where it can query for any required services and load any non-graphic
        /// related content.  Calling base.Initialize will enumerate through any components
        /// and initialize them as well.
        /// </summary>
        protected override void Initialize()
        {
            // TODO: Add your initialization logic here
            base.Initialize();
        }


        /// <summary>
        /// Load your graphics content.  If loadAllContent is true, you should
        /// load content from both ResourceManagementMode pools.  Otherwise, just
        /// load ResourceManagementMode.Manual content.
        /// </summary>
        /// <param name="loadAllContent">Which type of content to load.</param>
        protected override void LoadGraphicsContent(bool loadAllContent)
        {
            if (loadAllContent)
            {
                ground = content.Load<Model>("content/ground");

                scene = content.Load<Model>("content/scene");

                mirror = content.Load<Model>("content/mirror");
            }

            // TODO: Load any ResourceManagementMode.Manual content
        }


        /// <summary>
        /// Unload your graphics content.  If unloadAllContent is true, you should
        /// unload content from both ResourceManagementMode pools.  Otherwise, just
        /// unload ResourceManagementMode.Manual content.  Manual content will get
        /// Disposed by the GraphicsDevice during a Reset.
        /// </summary>
        /// <param name="unloadAllContent">Which type of content to unload.</param>
        protected override void UnloadGraphicsContent(bool unloadAllContent)
        {
            if (unloadAllContent)
            {
                content.Unload();
            }

            // TODO: Unload any ResourceManagementMode.Manual content
        }


        /// <summary>
        /// Allows the game to run logic such as updating the world,
        /// checking for collisions, gathering input and playing audio.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            // TODO: Add your update logic here
            
            base.Update(gameTime);
        }


        private void DrawModel(Model m,Matrix world, Matrix view, Matrix projection,bool lit)
        {
            Matrix[] transforms = new Matrix[m.Bones.Count];
            m.CopyAbsoluteBoneTransformsTo(transforms);

            foreach (ModelMesh mesh in m.Meshes)
            {
                foreach (BasicEffect effect in mesh.Effects)
                {
                    if(lit)
                    {
                        effect.LightingEnabled = true;
                        effect.EnableDefaultLighting();
                        effect.TextureEnabled = true;
                        effect.DiffuseColor = new Vector3(1, 1, 1);
                        effect.DirectionalLight0.Enabled = true;
                        effect.DirectionalLight1.Enabled = true;
                        effect.DirectionalLight2.Enabled = true;
                    }
                    else
                    {
                        effect.LightingEnabled = false;
                        effect.DiffuseColor = Vector3.Zero;
                        effect.DirectionalLight0.Enabled = false;
                        effect.DirectionalLight1.Enabled = false;
                        effect.DirectionalLight2.Enabled = false;
                        effect.TextureEnabled = false;
                    }
                    effect.View = view;
                    effect.Projection = projection;
                    effect.World = transforms[mesh.ParentBone.Index] * world;

                    effect.CommitChanges();
                }
                mesh.Draw();
            }
        }


        struct VertexPositionNormal
        {
            public Vector3 Position;
            public Vector3 Normal;

        }

        void RenderScene(GameTime gameTime)
        {

            Matrix view = Matrix.CreateLookAt(Vector3.Left * 100.0f +
                                                  Vector3.Up * 100.0f +
                                                  Vector3.Backward * 50.0f,
                                              Vector3.Zero,
                                              Vector3.Up);

            Matrix proj = Matrix.CreatePerspectiveFieldOfView(
                        MathHelper.PiOver2,
                        (float)graphics.GraphicsDevice.Viewport.Width / (float)graphics.GraphicsDevice.Viewport.Height,
                        5,
                        1500);

            Matrix world = Matrix.Identity;
            
            RenderWorld(ref view, ref proj, ref world);

            graphics.GraphicsDevice.RenderState.FillMode = FillMode.WireFrame;
            DrawModel(mirror,
                    world,
                    view,
                    proj, false);
            graphics.GraphicsDevice.RenderState.FillMode = FillMode.Solid;

            //START MIRROR CODE
            graphics.GraphicsDevice.Clear(ClearOptions.Stencil, Color.Black, 0, 0);
            
            
            graphics.GraphicsDevice.RenderState.StencilEnable = true;
            graphics.GraphicsDevice.RenderState.ColorWriteChannels = ColorWriteChannels.None;
            graphics.GraphicsDevice.RenderState.ReferenceStencil = 0;
            graphics.GraphicsDevice.RenderState.StencilFunction = CompareFunction.Equal;
            graphics.GraphicsDevice.RenderState.StencilPass = StencilOperation.Increment;
            graphics.GraphicsDevice.RenderState.DepthBufferEnable = false;
            
            world = Matrix.Identity;

            DrawModel(mirror,
                    world,
                    view,
                    proj, false);
            
            graphics.GraphicsDevice.RenderState.DepthBufferEnable = true;
            graphics.GraphicsDevice.RenderState.ColorWriteChannels = ColorWriteChannels.All;
            graphics.GraphicsDevice.RenderState.ReferenceStencil = 1;
            graphics.GraphicsDevice.RenderState.StencilFunction = CompareFunction.LessEqual;
            graphics.GraphicsDevice.RenderState.CullMode = CullMode.CullClockwiseFace;

            world = CalculateReflection(mirror);

            RenderWorld(ref view, ref proj, ref world);
            
            graphics.GraphicsDevice.RenderState.CullMode = CullMode.CullCounterClockwiseFace;
            graphics.GraphicsDevice.RenderState.StencilEnable = false;

            base.Draw(gameTime);
        }

        private Matrix CalculateReflection(Model m)
        {
            VertexPositionNormal[] verts = new VertexPositionNormal[m.Meshes[0].MeshParts[0].NumVertices];
            m.Meshes[0].VertexBuffer.GetData<VertexPositionNormal>(verts);

            short[] indices = new short[m.Meshes[0].IndexBuffer.SizeInBytes / sizeof(short)];
            m.Meshes[0].IndexBuffer.GetData<short>(indices);

            Matrix[] transforms = new Matrix[m.Bones.Count];
            m.CopyAbsoluteBoneTransformsTo(transforms);

            Plane p = new Plane(Vector3.Transform(verts[indices[0]].Position, transforms[0]),
                                Vector3.Transform(verts[indices[1]].Position, transforms[0]),
                                Vector3.Transform(verts[indices[2]].Position, transforms[0]));

            return Matrix.CreateReflection(p);
        }

        private void RenderWorld(ref Matrix view, ref Matrix proj, ref Matrix world)
        {
            DrawModel(ground,
                    world,
                    view,
                    proj, true);

            DrawModel(scene,
                    world,
                    view,
                    proj, true);
        }

        

        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            graphics.GraphicsDevice.Clear(Color.CornflowerBlue);
            
            RenderScene(gameTime);
        }
    }
}
