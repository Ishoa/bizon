using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using XNAWalkyrie.ParticuleSystem;

namespace XNAWalkyrie
{
    public partial class Utility
    {

        private static ParticuleManager particuleManager = null;

        public static ParticuleManager ParticuleManager
        {
            get
            {
                if (particuleManager == null)
                {
                    particuleManager = new ParticuleManager(game);
                    game.Components.Add(particuleManager);
                    particuleManager.Initialize();
                }
                return particuleManager;
            }
            set
            {
                if (particuleManager != null)
                {
                    RemoveService<BillboardManagerService>();
                }
                particuleManager = value;
                AddService<ParticuleManagerService>(new ParticuleManagerService(particuleManager));
            }
        }
    }

    public interface IParticuleManagerService
    {
        ParticuleManager ParticuleManager { get; }
    }

    public class ParticuleManagerService : IParticuleManagerService
    {

        public ParticuleManagerService(ParticuleManager particuleManager)
        {
            this.particuleManager = particuleManager;
        }

        private ParticuleManager particuleManager;

        public ParticuleManager ParticuleManager
        {
            get
            {
                return particuleManager;
            }
        }

        public static implicit operator ParticuleManager(ParticuleManagerService cs)
        {
            return cs.ParticuleManager;
        }
    }

    public enum TypeParticule
    {
        ParticulePoint = 0,
        ParticuleBilboard,
    }

    public class Particule
    {
        public TypeParticule Type;// The type. In this sample there will only be two types of particle, but you can program in your own.

        public float Timelife;// How long it lives.
        public float TimePassed = 0.0f;// This variable will tell the particle to behave differently after it reaches a certain point in its lifespan. In this sample it will be used to simulate gravity.

        public Vector3 Velocity; // 
        public Vector3 Force;

        public delegate void UpdateParticule(Particule p, GameTime gameTime);
        public UpdateParticule updateParticule = null;

    }



    public class ParticuleBilboard : Particule
    {

        public Billboard billboard;


    }

    /*
    public class ParticulePoint : Particule, IComparable
    {
        public bool Active;
        public VertexPointParticule ParticuleData;


        public int CompareTo(object obj)
        {
            ParticulePoint p = (ParticulePoint)obj;

            float f1 = Vector3.DistanceSquared(ParticuleData.position, Utility.Camera.Position);
            float f2 = Vector3.DistanceSquared(ParticuleData.position, Utility.Camera.Position);

            if (f1 > f2)
                return -1;
            else if (f1 < f2)
                return 1;
            else
                return 0;
        }

    }

    public struct VertexPointParticule
    {
        public Vector3 position;
        public float pointSize;
        public float particuleValue;
        public Vector4 color;

        public VertexPointParticule(Vector3 position, float pointSize, float ParticuleValue, Vector4 color)
        {
            this.position = position;
            this.pointSize = pointSize;
            this.particuleValue = ParticuleValue;
            this.color = color;
        }

        public static readonly VertexElement[] VertexElements =
        {
            new VertexElement(0, 
                    0, 
                    VertexElementFormat.Vector3, 
                    VertexElementMethod.Default, 
                    VertexElementUsage.Position, 
                    0),
                    
            new VertexElement(0, 
                    sizeof(float)*3, 
                    VertexElementFormat.Single, 
                    VertexElementMethod.Default, 
                    VertexElementUsage.PointSize,
                     0),
                     
            new VertexElement(0, 
                    sizeof(float)*4,
                     VertexElementFormat.Single, 
                     VertexElementMethod.Default, 
                     VertexElementUsage.Color, 
                     0),               
                     
            new VertexElement(0, 
                    sizeof(float)*5,
                     VertexElementFormat.Vector4, 
                     VertexElementMethod.Default, 
                     VertexElementUsage.Color, 
                     1),
        };

        public static int SizeInBytes = sizeof(float) * (3 + 1 + 1 + 4);
    }
    */



    public class ParticuleManager : DrawableGameComponent
    {
        public enum ListParticuleSystem
        {
            ExplosionParticuleSystem,
            ExplosionAlienParticuleSystem,
            ExplosionSmokdeParticuleSystem,
            ExplosionSmokeAliensParticleSystem,
            FireParticuleSystem,
            SmokePlumeParticule,
            ProjectileTailParticuleSystem,
            ProjectileAlienTrailParticuleStsytem,
            SmokePlumeParticuleSystem,
            NbparticuleSystem,
        };

        private ParticleSystem[] SystemsParticules;



        private List<ParticuleBilboard> billBoardParticuleList = new List<ParticuleBilboard>();

        /*
        private List<ParticulePoint> pointParticuleList = new List<ParticulePoint>();

        private Effect myEffetPointParticule;
        private VertexDeclaration PointParticuleVertexDeclaration;

        public Texture2D nullTexture;
        public Texture2D currenTexture;
        */

        public ParticuleManager(Game game)
            : base(game)
        {

            SystemsParticules = new ParticleSystem[(int)ListParticuleSystem.NbparticuleSystem];
            
            SystemsParticules[(int)ListParticuleSystem.ExplosionParticuleSystem] = new ExplosionParticleSystem(Utility.Game,Utility.Game.Content);
            SystemsParticules[(int)ListParticuleSystem.ExplosionAlienParticuleSystem] = new ExplosionAlienParticuleSystem(Utility.Game, Utility.Game.Content);
            SystemsParticules[(int)ListParticuleSystem.ExplosionSmokeAliensParticleSystem] = new ExplosionSmokeAliensParticleSystem(Utility.Game, Utility.Game.Content);
            SystemsParticules[(int)ListParticuleSystem.ExplosionSmokdeParticuleSystem] = new ExplosionSmokeParticleSystem(Utility.Game,Utility.Game.Content);
            SystemsParticules[(int)ListParticuleSystem.FireParticuleSystem] = new FireParticleSystem(Utility.Game, Utility.Game.Content);
            SystemsParticules[(int)ListParticuleSystem.SmokePlumeParticule] = new SmokePlumeParticleSystem(Utility.Game, Utility.Game.Content);
            SystemsParticules[(int)ListParticuleSystem.ProjectileTailParticuleSystem] = new ProjectileTrailParticleSystem(Utility.Game, Utility.Game.Content);
            SystemsParticules[(int)ListParticuleSystem.SmokePlumeParticuleSystem] = new SmokePlumeParticleSystem(Utility.Game, Utility.Game.Content);
            SystemsParticules[(int)ListParticuleSystem.ProjectileAlienTrailParticuleStsytem] = new ProjectileAlienTrailParticuleStsytem(Utility.Game, Utility.Game.Content);

            
            // Set the draw order so the explosions and fire
            // will appear over the top of the smoke.
            SystemsParticules[(int)ListParticuleSystem.SmokePlumeParticuleSystem].DrawOrder = 100;
            SystemsParticules[(int)ListParticuleSystem.ExplosionAlienParticuleSystem].DrawOrder = 200;
            SystemsParticules[(int)ListParticuleSystem.ExplosionSmokdeParticuleSystem].DrawOrder = 300;
            SystemsParticules[(int)ListParticuleSystem.ExplosionSmokeAliensParticleSystem].DrawOrder = 400;
            SystemsParticules[(int)ListParticuleSystem.ProjectileTailParticuleSystem].DrawOrder = 500;
            SystemsParticules[(int)ListParticuleSystem.ProjectileAlienTrailParticuleStsytem].DrawOrder = 600;
            SystemsParticules[(int)ListParticuleSystem.ExplosionParticuleSystem].DrawOrder = 700;
            SystemsParticules[(int)ListParticuleSystem.FireParticuleSystem].DrawOrder = 800;

            for(int i = 0; i<(int)ListParticuleSystem.NbparticuleSystem;i++)
                Utility.Game.Components.Add(SystemsParticules[i]);

        }

        public int NbParticuleBilboard
        {
            get { return billBoardParticuleList.Count; } 
        }

        /*
        public int NbParticulePoint
        {
            get { return pointParticuleList.Count; }
        }
        */


        public void AddBillboardParticule(ParticuleBilboard pb)
        {
            pb.TimePassed = 0.0f;
            billBoardParticuleList.Add(pb);
            Utility.BillboardManager.AddBillboard(pb.billboard);
        }

        public int NbParticuleSystem()
        {
            int res = 0;
            for (int i = 0; i< (int)ListParticuleSystem.NbparticuleSystem; i++)
                res += SystemsParticules[i].NbActiveParticules();

            return res;
        }

        /*
        public void AddPointParticule(ParticulePoint pp)
        {
            pp.TimePassed = 0.0f;
            pointParticuleList.Add(pp);
        }
        */


        public override void Initialize()
        {

            base.Initialize();
        }



        public void AddParticule( ListParticuleSystem ParticuleSystem, Vector3 Position , Vector3 Velocity)
        {
            SystemsParticules[(int)ParticuleSystem].AddParticle(Position, Velocity);
        }

        protected override void LoadContent()
        {
            

            /*
            nullTexture = new Texture2D(Utility.Game.GraphicsDevice, 1, 1, 0, TextureUsage.None, SurfaceFormat.Color);

            Color[] pixels = { Color.Red };

            nullTexture.SetData(pixels);
            

            myEffetPointParticule = Utility.Game.Content.Load<Effect>("PointParticule");
            PointParticuleVertexDeclaration = new VertexDeclaration(GraphicsDevice, VertexPointParticule.VertexElements);
            */

            base.LoadContent();
        }

        public override void Update(GameTime gameTime)
        {
            float elapsed = (float)gameTime.ElapsedGameTime.TotalSeconds;


            for (int x = 0; x < billBoardParticuleList.Count; )
            {
                ParticuleBilboard p = billBoardParticuleList[x];

                if (p.updateParticule != null)
                {

                    p.updateParticule(p, gameTime);
                    billBoardParticuleList[x] = p;

                    if (p.TimePassed > p.Timelife)
                    {
                        billBoardParticuleList[x].billboard.Active = false;
                        billBoardParticuleList.RemoveAt(x);
                    }
                    else
                    {
                        x++;
                    }
                }
            }
            /*
            for (int k = 0; k < pointParticuleList.Count; )
            {
                ParticulePoint pp = pointParticuleList[k];

                if (pp.updateParticule != null)
                {

                    pp.updateParticule(pp, gameTime);
                    pointParticuleList[k] = pp;
                }

                if (pp.TimePassed > pp.Timelife)
                {
                    pointParticuleList[k].Active = false;
                    pointParticuleList.RemoveAt(k);
                }
                else
                {
                    k++;
                }
            }
            
            //pointParticuleList.Sort();
            */
               

            base.Update(gameTime);
        }

        public override void Draw(GameTime gameTime)
        {
            /*
            if (pointParticuleList.Count > 0)
            {
                VertexPointParticule[] VertexBufferParticulePoint = new VertexPointParticule[pointParticuleList.Count];
                for (int i = 0; i < pointParticuleList.Count; i++)
                {
                    ParticulePoint pp  = pointParticuleList[i];

                    VertexBufferParticulePoint[i] = pp.ParticuleData;

                }


                myEffetPointParticule.CurrentTechnique = myEffetPointParticule.Techniques["ParticulePoint"];
                myEffetPointParticule.Begin(SaveStateMode.SaveState);


                myEffetPointParticule.Parameters["World"].SetValue(Matrix.Identity);
                myEffetPointParticule.Parameters["View"].SetValue(Utility.Camera.ViewMatrix);
                myEffetPointParticule.Parameters["Projection"].SetValue(Utility.Camera.ProjectionMatrix);

                if(currenTexture != null)
                    myEffetPointParticule.Parameters["Texture"].SetValue(currenTexture);
                else
                    myEffetPointParticule.Parameters["Texture"].SetValue(nullTexture);




                // Set the alpha blend mode.

                GraphicsDevice.RenderState.PointSpriteEnable = true;
                GraphicsDevice.RenderState.PointSizeMax = 256.0f;
                GraphicsDevice.RenderState.DepthBufferEnable = true;

                GraphicsDevice.RenderState.AlphaBlendEnable = true;
                GraphicsDevice.RenderState.AlphaBlendOperation = BlendFunction.Add;
                GraphicsDevice.RenderState.SourceBlend = Blend.SourceAlpha;
                GraphicsDevice.RenderState.DestinationBlend = Blend.InverseSourceAlpha;

                foreach (EffectPass pass in myEffetPointParticule.CurrentTechnique.Passes)
                {
                    pass.Begin();
                    GraphicsDevice.VertexDeclaration = PointParticuleVertexDeclaration;

                    GraphicsDevice.DrawUserPrimitives(
                                PrimitiveType.PointList,
                                VertexBufferParticulePoint,
                                0,
                                VertexBufferParticulePoint.Length);

                    pass.End();
                }

                myEffetPointParticule.End();


            }
            */



            base.Draw(gameTime);
        }

    }

}
