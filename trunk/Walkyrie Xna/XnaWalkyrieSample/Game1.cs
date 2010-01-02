using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

using XNAWalkyrie;
using System;
using SkinnedModel;
using Microsoft.Xna.Framework.Audio;

namespace OCTreeTest
{

    public class MovingSphere
    {
        public BoundingSphere Bounds;
        public Vector3 Velocity;
    }

    public class Environnement
    {
        enum Mur
        {
            Sol = 0,
            Sol_High,
            MurDroit,
            MurGauche,
            MurAvant,
            MurArriere,
            Nb_Mur,
        }

        private Texture2D floorColorMap;
        private Texture2D floorNormalMap;
        private Texture2D floorHeightMap;

        private Texture2D WoodColorMap;
        private Texture2D WoodNormalMap;
        private Texture2D WoodHeightMap;

        private Texture2D briqueColorMap;
        private Texture2D briqueNormalMap;
        private Texture2D briqueHeightMap;

        private Light light;
        private Material material;

        private XNAWalkyrie.Utility.NormalMappedQuad[] NormalMappedQuad;
        private XNAWalkyrie.Utility.NormalMappedModel NormalMappedModel;
        private Model modelTest;


        public Environnement(ref Light l, ref Material m)
        {
            light = l;
            material = m;
        }

        public void Initialize()
        {


            NormalMappedQuad = new XNAWalkyrie.Utility.NormalMappedQuad[(int)Mur.Nb_Mur];

            //########################################### SOL #################################################################

            NormalMappedQuad[(int)Mur.Sol] = new XNAWalkyrie.Utility.NormalMappedQuad(ref light,ref material,
                                                            new Vector3(100.0f, -295.0f, -280.0f), Vector3.Up, Vector3.Forward,
                                                            1400.0f, 1200.0f, 20.0f, 15.0f);
            
            //########################################### SOL_HIGH #################################################################

            NormalMappedQuad[(int)Mur.Sol_High] = new XNAWalkyrie.Utility.NormalMappedQuad(ref light, ref material,
                                                            new Vector3(-35.5f, -293.5f, 1.0f), Vector3.Up, Vector3.Forward,
                                                            391.0f, 322.0f, 10.0f, 5.0f);


            //########################################### RIGHT #################################################################

            NormalMappedQuad[(int)Mur.MurDroit] = new XNAWalkyrie.Utility.NormalMappedQuad(ref light, ref material,
                                                            new Vector3(-528.0f, -258.0f, -280.0f), Vector3.Right, Vector3.Forward,
                                                            75.0f, 1100.0f, 20.0f, 1.0f);


            //########################################### LEFT #################################################################

            NormalMappedQuad[(int)Mur.MurGauche] = new XNAWalkyrie.Utility.NormalMappedQuad(ref light, ref material,
                                                            new Vector3(686.0f, -258.0f, -280.0f), Vector3.Left, Vector3.Forward,
                                                            75.0f, 1100.0f, 20.0f, 1.0f);


            //########################################### BEHIND #################################################################

            NormalMappedQuad[(int)Mur.MurArriere] = new XNAWalkyrie.Utility.NormalMappedQuad(ref light, ref material,
                                                            new Vector3(100.0f, -258.0f, -800.0f), Vector3.UnitZ, Vector3.Right,
                                                            75.0f, 1300.0f, 20.0f, 1.0f);


            //########################################### FORWARD #################################################################

            NormalMappedQuad[(int)Mur.MurAvant] = new XNAWalkyrie.Utility.NormalMappedQuad(ref light, ref material,
                                                            new Vector3(100.0f, -258.0f, 235.0f), -Vector3.UnitZ, Vector3.Right,
                                                            75.0f, 1300.0f, 20.0f, 1.0f);
            for (int i = 0; i < (int)Mur.Nb_Mur; i++)
            {
                NormalMappedQuad[i].Initialize();
            }

           

        }

        public void LoadContent()
        {
            floorColorMap = Utility.Game.Content.Load<Texture2D>(@"Textures\ColorNormalHeightMap\floor_color_map");
            floorNormalMap = Utility.Game.Content.Load<Texture2D>(@"Textures\ColorNormalHeightMap\floor_normal_map");
            floorHeightMap = Utility.Game.Content.Load<Texture2D>(@"Textures\ColorNormalHeightMap\floor_height_map");

            briqueColorMap = Utility.Game.Content.Load<Texture2D>(@"Textures\ColorNormalHeightMap\brick_color_map");
            briqueNormalMap = Utility.Game.Content.Load<Texture2D>(@"Textures\ColorNormalHeightMap\brick_normal_map");
            briqueHeightMap = Utility.Game.Content.Load<Texture2D>(@"Textures\ColorNormalHeightMap\brick_height_map");

            //WoodColorMap = Utility.Game.Content.Load<Texture2D>(@"Textures\wood_color_map");
            WoodColorMap = Utility.Game.Content.Load<Texture2D>(@"Textures\ColorNormalHeightMap\woodplanks");
            //WoodNormalMap = Utility.Game.Content.Load<Texture2D>(@"Textures\wood_normal_map");
            WoodNormalMap = Utility.Game.Content.Load<Texture2D>(@"Textures\ColorNormalHeightMap\woodplanks_normal");
            WoodHeightMap = Utility.Game.Content.Load<Texture2D>(@"Textures\ColorNormalHeightMap\wood_height_map");


            modelTest =  Utility.Game.Content.Load<Model>(@"Models\box");

            NormalMappedModel = new XNAWalkyrie.Utility.NormalMappedModel(ref light, ref material, ref modelTest);
            NormalMappedModel.Initialize();

            for (int i = 0; i < (int)Mur.Nb_Mur; i++)
            {
                NormalMappedQuad[i].LoadContent();
            }

            NormalMappedModel.LoadContent();

            NormalMappedModel.textureColorMap = WoodColorMap;
            NormalMappedModel.textureNormalMap = WoodNormalMap;
            NormalMappedModel.textureHeightMap = WoodHeightMap;
            NormalMappedModel.World = Matrix.CreateTranslation(-300.0f, -295.0f, -200.0f);
            NormalMappedQuad[(int)Mur.Sol].textureColorMap = floorColorMap;
            NormalMappedQuad[(int)Mur.Sol].textureNormalMap = floorNormalMap;
            NormalMappedQuad[(int)Mur.Sol].textureHeightMap = floorHeightMap;

            NormalMappedQuad[(int)Mur.Sol_High].textureColorMap = WoodColorMap;
            NormalMappedQuad[(int)Mur.Sol_High].textureNormalMap = WoodNormalMap;
            NormalMappedQuad[(int)Mur.Sol_High].textureHeightMap = WoodHeightMap;

            for (int i = (int)Mur.MurDroit; i < (int)Mur.Nb_Mur; i++)
            {
                NormalMappedQuad[i].textureColorMap = briqueColorMap;
                NormalMappedQuad[i].textureNormalMap = briqueNormalMap;
                NormalMappedQuad[i].textureHeightMap = briqueHeightMap;
            }

            
        }


        public void UnloadContent()
        {

        }

        public void Update(GameTime gameTime)
        {
            NormalMappedModel.Update(gameTime);

            for (int i = 0; i < (int)Mur.Nb_Mur; i++)
            {
                NormalMappedQuad[i].Update(gameTime);
            }
            

        }

        public void Draw(GameTime gameTime)
        {
            for (int i = 0; i < (int)Mur.Nb_Mur; i++)
            {
                NormalMappedQuad[i].Draw(gameTime);
            }
            NormalMappedModel.Draw(gameTime);
        }
    }

    public class FPSEnemy
    {
        public MovingSphere ColliderSphere;

        public Model ModelDebugColliderSphere;

        public Model modelEnemy;
        public Vector3 VectEnemyPosition;
        public Vector3 VectEnemyDestination;
        public Vector3 PreviousVectEnemyPosition;
        public Quaternion QuaternionEnemyOrientation;
        private Matrix WorldEnemy;

        public enum StateEnemy
        {
            IddleState = 0,
            RunState,
            BiteState,
            DamageState,
            DeadState
        }

        AnimationPlayer animationEnemy;
        AnimationClip Iddle, Run, Bite, Damage, Die;
        AnimationClip previousAnim;

        public float MyAngle;
        public float DistanceObjectif;
        float Movespeed = 10.0f;

        StateEnemy eStateEnemy;

        public  float Endurance = 100;
        private Vector3 dirEnemy;
        private float nextShoot = 0.0f;

        Random rand;
        private float HauteurEnemy = -270.0f;

        static int indice = 0;

        public FPSEnemy(Vector3 initpos)
        {
            VectEnemyPosition = initpos;
            VectEnemyDestination = VectEnemyPosition;

            indice++;
            rand = new Random(indice);

            eStateEnemy = StateEnemy.IddleState;
        }


        public void Update(GameTime gameTime,OCTree sceneOct)
        {
            
            PreviousVectEnemyPosition = VectEnemyPosition;
            nextShoot -= (float)gameTime.ElapsedGameTime.TotalSeconds;

            DistanceObjectif = Vector3.DistanceSquared(VectEnemyDestination, VectEnemyPosition);
            if (Endurance > 0.0f && DistanceObjectif < 10.0f)
            {
                VectEnemyDestination = new Vector3(
                                                    ((float)rand.NextDouble()) * 1010.0f - 420.0f,
                                                    HauteurEnemy,
                                                    ((float)rand.NextDouble()) * 830.0f - 700.0f
                                                    );

                Vector3 vec1, vec2;
                vec1 = new Vector3(VectEnemyPosition.X, 0.0f, VectEnemyPosition.Z);
                vec2 = new Vector3(VectEnemyDestination.X, 0.0f, VectEnemyDestination.Z);

                dirEnemy = vec2 - vec1;
                dirEnemy.Normalize();

                
                Ray direction = new Ray(VectEnemyPosition, dirEnemy);

                OCTreeIntersection ResultatIntersection;

                sceneOct.GetIntersectingPolygon(ref direction, out ResultatIntersection);


                if (ResultatIntersection.IntersectType == OCTreeIntersectionType.Inside && ResultatIntersection.IntersectType != OCTreeIntersectionType.None)
                {
                    VectEnemyDestination = new Vector3(ResultatIntersection.IntersectionPoint.X, HauteurEnemy, ResultatIntersection.IntersectionPoint.Z) - dirEnemy * (ColliderSphere.Bounds.Radius * 1.5f);

                }

                vec1 = new Vector3(VectEnemyPosition.X, 0.0f, VectEnemyPosition.Z);
                vec2 = new Vector3(VectEnemyDestination.X, 0.0f, VectEnemyDestination.Z);

                dirEnemy = vec2 - vec1;
                dirEnemy.Normalize();

                
            }

            if (DistanceObjectif > 20.0f)
                DistanceObjectif = 10.0f;

            if (eStateEnemy == StateEnemy.RunState)// && eStateEnemy != StateEnemy.BiteState && Endurance > 0.0f)
                VectEnemyPosition += dirEnemy * Movespeed * DistanceObjectif * (float)gameTime.ElapsedGameTime.TotalSeconds;

            Vector3 Direction = Vector3.One;

            if (eStateEnemy != StateEnemy.BiteState)
            {
                Direction = dirEnemy;
            }
            else
            {
                if(Utility.Camera.CurrentBehavior == Camera.Behavior.FirstPerson)
                    Direction = Utility.Camera.Position - VectEnemyPosition;
                else if(Utility.Camera.CurrentBehavior == Camera.Behavior.ThirdPerson)
                    Direction = Utility.Camera.OrbitTarget - VectEnemyPosition;
                Direction = new Vector3(Direction.X, 0.0f,Direction.Z);
                Direction.Normalize();
            }

            if (eStateEnemy != StateEnemy.IddleState && eStateEnemy != StateEnemy.DamageState)
            {
                double scalar = Vector3.Dot(Direction, new Vector3(0.0f, 0.0f, 1.0f));
                //double scalar = dir.Z;

                if (Direction.X < 0.0f)
                    MyAngle = (float)-Math.Acos(scalar);
                else
                    MyAngle = (float)Math.Acos(scalar);

            }

            QuaternionEnemyOrientation = Quaternion.CreateFromAxisAngle(new Vector3(0.0f, 1.0f, 0.0f), MyAngle);


            WorldEnemy = Matrix.CreateFromQuaternion(QuaternionEnemyOrientation) * Matrix.CreateScale(0.3f) * Matrix.CreateTranslation(new Vector3(VectEnemyPosition.X, VectEnemyPosition.Y - ColliderSphere.Bounds.Radius - 5.0f, VectEnemyPosition.Z));


            ColliderSphere.Bounds.Center = VectEnemyPosition;
            ColliderSphere.Bounds.Center.Y -= 15.0f;
            UpdateAnimation(gameTime);
        }

        public void UpdateAnimation(GameTime gameTime)
        {

            switch (eStateEnemy)
            {
                case StateEnemy.IddleState:
                    if (previousAnim != Iddle)
                    {
                        animationEnemy.StartClip(Iddle);
                        previousAnim = Iddle;
                    }

                    if (VectEnemyPosition != VectEnemyDestination)
                        eStateEnemy = StateEnemy.RunState;

                    break;

                case StateEnemy.RunState:

                        if (previousAnim != Run)
                        {
                            animationEnemy.StartClip(Run);
                            previousAnim = Run;
                        }

                        if (VectEnemyPosition == PreviousVectEnemyPosition)
                        {
                            eStateEnemy = StateEnemy.IddleState;
                        }
                    break;

                case StateEnemy.BiteState:
                    if (animationEnemy.EStateOfAnimation == AnimationPlayer.StateOfAnimation.EndAnimation)
                        eStateEnemy = StateEnemy.IddleState;
                    break;


                case StateEnemy.DamageState:
                    if (animationEnemy.EStateOfAnimation == AnimationPlayer.StateOfAnimation.EndAnimation)
                        eStateEnemy = StateEnemy.IddleState;
                    break;

                case StateEnemy.DeadState:
                    break;

                default:
                    break;
            }

            if (Endurance <= 0.0f)
                if (animationEnemy.EStateOfAnimation == AnimationPlayer.StateOfAnimation.EndAnimation)
                {
                    eStateEnemy = StateEnemy.DeadState;
                }

            animationEnemy.Update(gameTime.ElapsedGameTime, true, WorldEnemy);

        }

        public void LoadContent()
        {
            ColliderSphere = new MovingSphere();
            modelEnemy = Utility.Game.Content.Load<Model>(@"Models\EnemyBeast");

            ModelDebugColliderSphere = Utility.Game.Content.Load<Model>(@"Models\unitSphere");

            ColliderSphere.Velocity = Vector3.Zero;
            ColliderSphere.Bounds = ModelExtensions.GetBoundingSphere(modelEnemy);
            ColliderSphere.Bounds.Radius *= 0.2f;
            QuaternionEnemyOrientation = Quaternion.Identity;

            // Look up our custom skinning information.
            SkinningData skinningData = modelEnemy.Tag as SkinningData;

            if (skinningData == null)
                throw new InvalidOperationException
                    ("This model does not contain a SkinningData tag.");

            // Create an animation player, and start decoding an animation clip.
            animationEnemy = new AnimationPlayer(skinningData);

            Iddle = skinningData.AnimationClips["Idle"];
            Run = skinningData.AnimationClips["Run"];
            Bite = skinningData.AnimationClips["Bite"];
            Damage = skinningData.AnimationClips["Take Damage"];
            Die = skinningData.AnimationClips["Die"];
            animationEnemy.StartClip(Iddle);
        }

        public void Draw(GameTime gameTime)
        {
            Matrix[] bones = animationEnemy.GetSkinTransforms();

            // Render the skinned mesh.
            foreach (ModelMesh mesh in modelEnemy.Meshes)
            {
                foreach (Effect effect in mesh.Effects)
                {
                    effect.Parameters["Bones"].SetValue(bones);
                    effect.Parameters["ViewProjection"].SetValue(Utility.Camera.ViewProjectionMatrix);
                }

                mesh.Draw();
            }

            if (Utility.InputState.IsKeyDown(Keys.V))
                Utility.VectorRenderer.DrawLine(VectEnemyPosition,VectEnemyDestination);

            if (Utility.InputState.IsKeyDown(Keys.C))
                ModelDebugColliderSphere.Draw(Matrix.CreateScale(ColliderSphere.Bounds.Radius / 2.0f) * Matrix.CreateTranslation(ColliderSphere.Bounds.Center));

        }


        public Vector3 Position
        {
            get { return VectEnemyPosition; }
        }

        public bool IsDead()
        {
            return eStateEnemy == StateEnemy.DeadState;
        }


        public void TakeDamage()
        {
            if (Endurance > 0.0f)
            {
                eStateEnemy = StateEnemy.DamageState;
                animationEnemy.StartClip(Damage,false);

                previousAnim = Damage;

                Endurance -= 10.0f;
                if (Endurance <= 0.0f)
                {
                    animationEnemy.StartClip(Die, false);
                    previousAnim = Die;
                }
            }
        }

        public bool PlayerNextenemy()
        {
            if (Endurance > 0.0f && eStateEnemy != StateEnemy.DamageState)
            {
                eStateEnemy = StateEnemy.BiteState;
                if (nextShoot <= 0.0f)
                {
                    animationEnemy.StartClip(Bite, false);
                    previousAnim = Bite;
                    nextShoot = 2.0f;
                    return true;
                }
                else if (previousAnim != Iddle && animationEnemy.EStateOfAnimation == AnimationPlayer.StateOfAnimation.EndAnimation)
                {
                    animationEnemy.StartClip(Iddle,false);
                    previousAnim = Iddle;
                }
                
            }
            return false;
        }

        public Matrix MatrixEnemy
        {
            get { return WorldEnemy; }
        }

        public Model ModelEnemy
        {
            get { return modelEnemy; }
        }

        public AnimationPlayer AnimationEnemy
        {
            get { return animationEnemy; }
        }

    }

    public class FPSPlayer
    {

        private const float WEAPON_SCALE = 0.01f;
        private const float WEAPON_X_OFFSET = 0.25f;
        private const float WEAPON_Y_OFFSET = -0.3f;
        private const float WEAPON_Z_OFFSET = -0.50f;
        private const float FIRSTPERSONNYDECAL = 2.0f;

        public MovingSphere ColliderSphere;

        public Model ModelDebugColliderSphere;

        public Model modelPerso;
        public Vector3 VectPersoPosition;
        public Vector3 PreviousVectPersoPosition;
        public Quaternion QuaternionPersoOrientation;
        private Matrix WorldPerso;

        AnimationPlayer animationPlayer;
        AnimationClip Iddle, Run;
        AnimationClip previousAnim;
        SkinningData skinningData;
        Matrix[] boneTransforms;

        public Matrix MatrixWeapon;
        public Model ModelWeapon;

        public float YRotation;
        public float XRotation;

        public float XDelta;
        public float YDelta;

        public float Friction;
        

        public float MyAngle;


        public int health = 1000;
        public int maxhealth = 1000;

        public int Ammo = 50;
        public int MaxAmo = 50;

        public float Endurance = 5.0f;
        public float MaxEndurance = 5.0f;

        public FPSPlayer()
        {
            YRotation = 0;
            XRotation = 0;
            XDelta = 0.0f;
            YDelta = 0.0f;
            Friction  = 0.3f;

        }


        public void Update(GameTime gameTime)
        {
            PreviousVectPersoPosition = VectPersoPosition;

            switch (Utility.Camera.CurrentBehavior)
            {
                case Camera.Behavior.FirstPerson:

                    MatrixWeapon = Utility.Camera.WeaponWorldMatrix(WEAPON_X_OFFSET,
                                                    WEAPON_Y_OFFSET, WEAPON_Z_OFFSET, WEAPON_SCALE);

                    VectPersoPosition = Utility.Camera.Position + Vector3.Down * ColliderSphere.Bounds.Radius * 0.85f;

                    break;

                case Camera.Behavior.FreeOrbit:
                case Camera.Behavior.ThirdPerson:
                    VectPersoPosition = Utility.Camera.OrbitTarget + Vector3.Down * ColliderSphere.Bounds.Radius * 0.85f;

                    break;
            }

            Vector3 vec1, vec2;
            vec1 = new Vector3(Utility.Camera.Position.X, 0.0f, Utility.Camera.Position.Z);
            vec2 = new Vector3(Utility.Camera.OrbitTarget.X, 0.0f, Utility.Camera.OrbitTarget.Y);
            vec1.Normalize();
            vec2.Normalize();

            Vector3 dir = new Vector3(Utility.Camera.ZAxis.X, 0.0f, Utility.Camera.ZAxis.Z);
            dir.Normalize();
            double scalar = Vector3.Dot(dir, new Vector3(0.0f, 0.0f, 1.0f));
            //double scalar = dir.Z;

            Vector3 vectoriel = Vector3.Cross(dir, new Vector3(0.0f, 0.0f, 1.0f));

            if (dir.X < 0.0f)
                MyAngle = (float)-Math.Acos(scalar);
            else
                MyAngle = (float)Math.Acos(scalar);

            QuaternionPersoOrientation = Quaternion.CreateFromAxisAngle(new Vector3(0.0f, 1.0f, 0.0f), MyAngle);


            WorldPerso = Matrix.CreateRotationY(MathHelper.ToRadians(180.0f)) * Matrix.CreateFromQuaternion(QuaternionPersoOrientation) * Matrix.CreateTranslation(new Vector3(VectPersoPosition.X, VectPersoPosition.Y - ColliderSphere.Bounds.Radius, VectPersoPosition.Z));

            if (VectPersoPosition == PreviousVectPersoPosition && previousAnim != Iddle)
            {
                animationPlayer.StartClip(Iddle);
                previousAnim = Iddle;
            }
            else if (VectPersoPosition != PreviousVectPersoPosition && previousAnim != Run)
            {
                animationPlayer.StartClip(Run);
                
                previousAnim = Run;
            }
            
            animationPlayer.Update(gameTime.ElapsedGameTime, true, WorldPerso);

            // Copy the transforms into our own array, so we can safely modify the values.
            animationPlayer.GetBoneTransforms().CopyTo(boneTransforms, 0);
        }



        public void LoadContent()
        {
            ColliderSphere = new MovingSphere();
            modelPerso = Utility.Game.Content.Load<Model>(@"Models\PlayerMarine");

            ModelWeapon = Utility.Game.Content.Load<Model>(@"Models\weapon");
            ModelDebugColliderSphere = Utility.Game.Content.Load<Model>(@"Models\unitSphere");

            ColliderSphere.Velocity = Vector3.Zero;
            ColliderSphere.Bounds = ModelExtensions.GetBoundingSphere(modelPerso);
            ColliderSphere.Bounds.Radius *= 0.9f;
            QuaternionPersoOrientation = Quaternion.Identity;

            // Look up our custom skinning information.
            skinningData = modelPerso.Tag as SkinningData;

            if (skinningData == null)
                throw new InvalidOperationException
                    ("This model does not contain a SkinningData tag.");

            boneTransforms = new Matrix[skinningData.BindPose.Count];

            // Create an animation player, and start decoding an animation clip.
            animationPlayer = new AnimationPlayer(skinningData);

            Iddle = skinningData.AnimationClips["Idle"];
            Run = skinningData.AnimationClips["Run"];

            animationPlayer.SpeedAnimation = 1.5f;
            animationPlayer.StartClip(Iddle);
        }

        public void Draw(GameTime gameTime)
        {

            switch (Utility.Camera.CurrentBehavior)
            {
                case Camera.Behavior.FirstPerson:

                    ModelWeapon.Draw(MatrixWeapon);
                    break;

                case Camera.Behavior.FreeOrbit:
                case Camera.Behavior.ThirdPerson:

                    Matrix[] bones = animationPlayer.GetSkinTransforms();

                    // Render the skinned mesh.
                    foreach (ModelMesh mesh in modelPerso.Meshes)
                    {
                        foreach (Effect effect in mesh.Effects)
                        {
                            effect.Parameters["Bones"].SetValue(bones);
                            effect.Parameters["ViewProjection"].SetValue(Utility.Camera.ViewProjectionMatrix);
                        }

                        mesh.Draw();
                    }

                    // Modify the transform matrices for the head and upper left arm bones.
                    int HandIndex = skinningData.BoneIndices["R_Hand"];

                    Matrix[] worldTransforms = animationPlayer.GetWorldTransforms();

                    Matrix WeaponWorldTransform = Matrix.CreateScale(0.1f) * Matrix.CreateTranslation(-1.0f,2.0f,-4.0f)*Matrix.CreateRotationY(1.75f) * Matrix.CreateRotationX(-1.57f) *
                                               worldTransforms[HandIndex];

                    ModelWeapon.Draw(WeaponWorldTransform);

                    if(Utility.InputState.IsKeyDown(Keys.C))
                        ModelDebugColliderSphere.Draw(Matrix.CreateScale(ColliderSphere.Bounds.Radius / 2.0f) * Matrix.CreateTranslation(VectPersoPosition));
                    break;



            }
        }

        public Matrix MatrixPerso
        {
            get { return WorldPerso; }
        }

        public Model ModelPerso
        {
            get { return modelPerso; }
        }

        public AnimationPlayer AnimationPlayer
        {
            get { return animationPlayer; }
        }
    }


    public class GameFPS : Microsoft.Xna.Framework.Game
    {

        /// <summary>
        /// A light. This light structure is the same as the one defined in
        /// the parallax_normal_mapping.fx file. The only difference is the
        /// LightType enum.
        /// </summary>
        /// 

        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        SpriteFont debugFont;

        private const int NB_Enemy = 5;
        private const int NB_Caisse = 13;
        private const int NB_Palmier = 1;
        private const int NB_Bridge = 2;
        private const int NB_Teleport = 2;

        //##########################################################
        // Personnages de la scene
        FPSPlayer player;
        FPSEnemy[] enemy = new FPSEnemy[NB_Enemy];

        //##########################################################
        // Model de la scene
        Model scene;
        Model Caisse;
        Model Reservoir;
        Model Bridge;
        Model Palmier;
        Model Escalier;
        Model ScannerRoom;
        Model ScannerMonitor;
        Model Gunship;
        Model RotateBlade;

        //##########################################################
        // Texture affecté au model
        private Texture2D TextureReservoir;
        private Texture2D TextureCaisse;
        private Texture2D TextureBridge;

        //##########################################################
        // Matrix des model
        private Matrix[] MatrixBridge = new Matrix[NB_Bridge];
        private Matrix[] MatrixPalmier = new Matrix[NB_Palmier];
        private Matrix[] MatrixCaisse = new Matrix[NB_Caisse];
        private Matrix MatrixEscalier;
        private Matrix MatrixReservoir;
        private Matrix MatrixScannerRoom;
        private Matrix MatrixGunship;
        private Matrix MatrixRotateBlade;
        private Matrix MatrixTerrain;

        //##########################################################
        // Données génération du Terrain
        Model Terrain;
        float TerrainScale = 20.0f;
        float TerrainBumppiness = 512.0f;
        float HauteurOriginTerrain = -400.0f;

        //private Effect TerrainEffect;

        private Texture2D TextureTerre;
        private Texture2D Herbe;
        private Texture2D DensiteTerre;

        //##########################################################
        // Octree de la scene
        OCTree sceneOct;

        //##########################################################
        // Variable pour le calcul du FPS
        private int frames;
        private int framesPerSecond;
        private double accumElapsedTimeSec;

        //##########################################################
        // Particules
        const int numExplosionParticles = 30;
        const int numExplosionSmokeParticles = 50;

        private Texture2D Smoke;
        private Texture2D Fire;

        private float nextParticle = 0.0f;

        List<Projectile> projectilesPlayer = new List<Projectile>();
        List<Projectile> projectilesEnemy = new List<Projectile>();

        //##########################################################
        // Interfaces
        private Texture2D Interface;
        private Texture2D InterfaceDetail;
        private Texture2D InterfaceViseur;
        private Texture2D InterfaceTeleport;

        //##########################################################
        // Constante Caméra
        private const float CAMERA_FOV = 90.0f;
        private const float CAMERA_ZNEAR = 0.01f;
        private const float CAMERA_ZFAR = 3000.0f;
        private const float CAMERA_OFFSET = 1.5f;

        //##########################################################
        // Données pour la SkyBox
        XNAWalkyrie.Utility.SkyBox MySkyBox;

        private Texture2D[] texturesSky = new Texture2D[6];
        private Texture2D[] texturesSkyNight = new Texture2D[6];
        private Texture2D[] texturesSkyGrass = new Texture2D[6];

        //##########################################################
        // Données pour la génaration des ombres
        private ShadowMap shadowMapNear;
        private ShadowMap shadowMapFar;

        private bool displayShadowMap;
        private bool displayShadows;
        private float aspectRatio;
        private float FocaleCamera = 450.0f;

        private Dictionary<int, Texture2D> modelTextures;

        private ShadowTechnique shadowTechnique;
        private Effect effectLambert;
        private Effect effectTerrainLambert;

        private enum ShadowTechnique
        {
            None,
            ShadowMapping,
            ShadowMappingWith2x2PercentageCloserFiltering,
            ShadowMappingWith3x3PercentageCloserFiltering
        }

        //##########################################################
        // Données Divers ( light , materiels)
        Random rand = new Random();
        bool LightRotation;
        private Light light;
        private float lightAngle;
        private Material material;
        private float RotationBlade = 0.0f;

        //##########################################################
        // Données Téléport
        private float timeBetweenSalveTeleport = 0.3f;
        private float CurrentTimeSalve = 0.0f;
        private float RayonTeleport = 20.0f;

        private Vector3[] PositionTeleport = new Vector3[NB_Teleport];
        private float fInterfaceTeleport = 1.0f;
        private int IncrementTeleport = -1;

        //##########################################################
        // Données etat player
        enum EPlayerSituation
        {
            InsideLabo,
            Out,
            None,
        };

        EPlayerSituation ePlayerSituation;

        //##########################################################
        // Données pour la création d'un environnement texturé
        private Environnement MyParralaxEnv;

        /*##########################################################*/

        public GameFPS()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            displayShadowMap = false;
            displayShadows = false;
            LightRotation = false;

            shadowTechnique = ShadowTechnique.ShadowMappingWith3x3PercentageCloserFiltering;
            modelTextures = new Dictionary<int, Texture2D>();

            Utility.Game = this;
        }

        
        protected override void Initialize()
        {

            // Setup frame buffer.
            graphics.SynchronizeWithVerticalRetrace = false;
            graphics.PreferredBackBufferWidth = 1024;
            graphics.PreferredBackBufferHeight = 768;
            graphics.PreferMultiSampling = true;
            graphics.ApplyChanges();

            aspectRatio = (float)1024 / (float)768;

            Utility.Camera.EyeHeightStanding = 10.0f;

            Utility.Camera.Perspective(CAMERA_FOV, aspectRatio, CAMERA_ZNEAR, CAMERA_ZFAR);
            Utility.Camera.Position = new Vector3(0.0f, 200.0f, 0.0f);
            Utility.Camera.Acceleration = new Vector3(100.0f, 50.0f, 100.0f);
            Utility.Camera.CurrentForce = new Vector3(0.0f, -7.0f, 0.0f);
            Utility.Camera.VelocityWalking = new Vector3(100.0f, 100.0f, 100.0f);
            Utility.Camera.OrbitMinZoom = 20.0f;
            Utility.Camera.OrbitMaxZoom = 400.0f;
            Utility.Camera.OrbitOffsetDistance = 60.0f;
            Utility.Camera.PerformsCollisionFonction = PerformCollisionCamera;
            Utility.Camera.CurrentBehavior = Camera.Behavior.FirstPerson;

            lightAngle = 1.5f * MathHelper.Pi;

            // Initialize light settings.
            light.Type = Light.LightType.PointLight;
            light.Direction = new Vector3((float)Math.Cos(lightAngle), -1.0f, (float)Math.Sin(lightAngle));
            light.Direction.Normalize();
            light.Radius = 1200.0f;
            light.Position = new Vector3(0.0f, -100.0f, -100.0f);
            light.Ambient = Color.White;
            light.Diffuse = Color.White;
            light.Specular = Color.White;
            light.SpotInnerConeRadians = MathHelper.ToRadians(30.0f);
            light.SpotOuterConeRadians = MathHelper.ToRadians(100.0f);
            

            // Initialize material settings for the floor.
            material.Ambient = new Color(new Vector4(0.6f, 0.6f, 0.6f, 1.0f));
            material.Diffuse = new Color(new Vector4(0.8f, 0.8f, 0.8f, 1.0f));
            material.Emissive = Color.Black;
            material.Specular = Color.White;
            material.Shininess = 90.0f;

            MatrixCaisse[0] = Matrix.CreateTranslation(50.0f, -295.0f, 50.0f);
            MatrixCaisse[1] = Matrix.CreateTranslation(100.0f, -295.0f, -50.0f);
            MatrixCaisse[2] = Matrix.CreateTranslation(100.0f, -295.0f, -100.0f);
            MatrixCaisse[3] = Matrix.CreateTranslation(-50.0f, -295.0f, 100.0f);
            MatrixCaisse[4] = Matrix.CreateTranslation(-50.0f, -295.0f, -50.0f);
            MatrixCaisse[5] = Matrix.CreateTranslation(25.0f, -295.0f, 25.0f);
            MatrixCaisse[6] = Matrix.CreateTranslation(100.0f, -295.0f, 100.0f);
            MatrixCaisse[7] = Matrix.CreateTranslation(75.0f, -295.0f, 75.0f);
            MatrixCaisse[8] = Matrix.CreateTranslation(-200.0f, -295.0f, -25.0f);
            MatrixCaisse[9] = Matrix.CreateTranslation(-150.0f, -295.0f, 75.0f);
            MatrixCaisse[10] = Matrix.CreateTranslation(100.0f, -295.0f, 225.0f);
            MatrixCaisse[11] = Matrix.CreateTranslation(100.0f, -295.0f, 200.0f);
            MatrixCaisse[12] = Matrix.CreateTranslation(100.0f, -270.0f, 225.0f);

            MatrixEscalier = Matrix.CreateScale(0.1f) * Matrix.CreateTranslation(-150.0f, -300.0f, 160.0f);

            MatrixBridge[0] = Matrix.CreateScale(0.7f, 0.1f, 1.4f) * Matrix.CreateTranslation(-20.0f, -219, 470.0f);
            MatrixBridge[1] = Matrix.CreateScale(0.7f, 0.1f, 1.4f) * Matrix.CreateTranslation(-20.0f, -219, 60.0f);

            MatrixReservoir = Matrix.CreateScale(0.8f) * Matrix.CreateTranslation(-400.0f, -330.0f, -500.0f);

            MatrixPalmier[0] = Matrix.CreateScale(0.1f) * Matrix.CreateTranslation(100.0f, -300.0f, 0.0f);

            MatrixScannerRoom = Matrix.CreateScale(20.0f) * Matrix.CreateTranslation(-400.0f, -450.0f, 0.0f);

            PositionTeleport[0] = new Vector3(-400.0f, -295.0f, 0.0f);
            PositionTeleport[1] = new Vector3(-400.0f, -435.0f, 0.0f);

            MatrixGunship = Matrix.CreateScale(34.0f) * Matrix.CreateRotationX(-1.57f) * Matrix.CreateTranslation(0.0f, -300.0f, -650.0f);
            MatrixRotateBlade = Matrix.CreateRotationY(RotationBlade) * MatrixGunship;

            MatrixTerrain = Matrix.CreateScale(TerrainScale, TerrainBumppiness, TerrainScale) * Matrix.CreateTranslation(0.0f, HauteurOriginTerrain, -275.0f);
            MyParralaxEnv = new Environnement(ref light, ref material);
            MyParralaxEnv.Initialize();

            MySkyBox = new XNAWalkyrie.Utility.SkyBox(Utility.Game);
            MySkyBox.Initialize();

            shadowMapNear = new ShadowMap(GraphicsDevice, Content,1024);
            shadowMapNear.DepthBias = 0.001f;

            shadowMapFar = new ShadowMap(GraphicsDevice, Content,1024);
            shadowMapFar.DepthBias = 0.001f;

            player = new FPSPlayer();

            ePlayerSituation = EPlayerSituation.Out;

            base.Initialize();
        }


       
        protected override void LoadContent()
        {
            spriteBatch = new SpriteBatch(GraphicsDevice);
            debugFont = Content.Load<SpriteFont>(@"Fonts\DebugFont");

            Interface = Content.Load<Texture2D>(@"Textures\Interfaces\DisplaySprites");
            InterfaceDetail = Content.Load<Texture2D>(@"Textures\Interfaces\texturehud7iv");
            InterfaceViseur = Content.Load<Texture2D>(@"Textures\Interfaces\viseur3");
            InterfaceTeleport = Content.Load<Texture2D>(@"Textures\Interfaces\Teleport");

            //scene = Content.Load<Model>("scene5");
            scene = Content.Load<Model>(@"Models\testRad");
            Caisse = Content.Load<Model>(@"Models\box");
            Reservoir = Content.Load<Model>(@"Models\Water_Reservoir");
            Bridge = Content.Load<Model>(@"Models\bridge");
            Escalier = Content.Load<Model>(@"Models\Japanese Stair Case");
            Palmier = Content.Load<Model>(@"Models\Palm");
            ScannerRoom = Content.Load<Model>(@"Models\scannerroom");
            ScannerMonitor = Content.Load<Model>(@"Models\scannermonitors");
            Gunship = Content.Load<Model>(@"Models\GunshipWhole");
            RotateBlade = Content.Load<Model>(@"Models\rotablade");

            // Setup effects.
            effectLambert = Content.Load<Effect>(@"Effects\Lambert");
            effectTerrainLambert = Content.Load<Effect>(@"Effects\TerrainLambertEffect");

            TextureCaisse = Content.Load<Texture2D>(@"Textures\crate");
            TextureReservoir = Content.Load<Texture2D>(@"Textures\WHTCON");
            TextureBridge = Content.Load<Texture2D>(@"Textures\WHTCON");

            TextureTerre = Content.Load<Texture2D>(@"Textures\Terrain\Terre");
            Herbe = Content.Load<Texture2D>(@"Textures\Terrain\Herbe");
            DensiteTerre = Content.Load<Texture2D>(@"Textures\Terrain\DensiteTerre");

            Smoke = Content.Load<Texture2D>(@"Textures\Particules\smoke"); ;
            Fire = Content.Load<Texture2D>(@"Textures\Particules\Fire"); ;

            Reservoir.SetTexture(TextureReservoir);
            Caisse.SetTexture(TextureCaisse);
            Bridge.SetTexture(TextureBridge);

            //############################### Terrain ################################

            Terrain = Content.Load<Model>(@"Textures\Terrain\MapTerrain");
            //Terrain = Content.Load<Model>(@"Textures\Terrain\terrain"); 
            //TerrainEffect = Content.Load<Effect>(@"Effects\TerrainEffect");

            //TerrainEffect.Parameters["TerreTexture"].SetValue(TextureTerre);
            //TerrainEffect.Parameters["DensiteTerreTexture"].SetValue(DensiteTerre);

            Terrain.SetTexture(Herbe);

            foreach (ModelMesh mesh in Terrain.Meshes)
            {
                foreach (ModelMeshPart part in mesh.MeshParts)
                {
                    BasicEffect basicEffect = part.Effect as BasicEffect;

                    if (basicEffect != null)
                        modelTextures[mesh.GetHashCode()] = basicEffect.Texture;

                    //part.Effect = TerrainEffect;
                    //part.Effect = effectTerrainLambert;
                    part.Effect = effectLambert;
                }
            }


            //############################ Shadow Map ##############################

            foreach (ModelMesh mesh in scene.Meshes)
            {
                foreach (ModelMeshPart part in mesh.MeshParts)
                {
                    BasicEffect basicEffect = part.Effect as BasicEffect;

                    if (basicEffect != null)
                        modelTextures[mesh.GetHashCode()] = basicEffect.Texture;

                    part.Effect = effectLambert;
                }
            }


            foreach (ModelMesh mesh in Palmier.Meshes)
            {
                foreach (ModelMeshPart part in mesh.MeshParts)
                {
                    BasicEffect basicEffect = part.Effect as BasicEffect;

                    if (basicEffect != null)
                        modelTextures[mesh.GetHashCode()] = basicEffect.Texture;

                    part.Effect = effectLambert;
                }
            }

            foreach (ModelMesh mesh in Escalier.Meshes)
            {
                foreach (ModelMeshPart part in mesh.MeshParts)
                {
                    BasicEffect basicEffect = part.Effect as BasicEffect;

                    if (basicEffect != null)
                        modelTextures[mesh.GetHashCode()] = basicEffect.Texture;

                    part.Effect = effectLambert;
                }
            }

            foreach (ModelMesh mesh in Reservoir.Meshes)
            {
                foreach (ModelMeshPart part in mesh.MeshParts)
                {
                    BasicEffect basicEffect = part.Effect as BasicEffect;

                    if (basicEffect != null)
                        modelTextures[mesh.GetHashCode()] = basicEffect.Texture;

                    part.Effect = effectLambert;
                }
            }

            foreach (ModelMesh mesh in Caisse.Meshes)
            {
                foreach (ModelMeshPart part in mesh.MeshParts)
                {
                    BasicEffect basicEffect = part.Effect as BasicEffect;

                    if (basicEffect != null)
                        modelTextures[mesh.GetHashCode()] = basicEffect.Texture;

                    part.Effect = effectLambert;
                }
            }

            
            foreach (ModelMesh mesh in Bridge.Meshes)
            {
                foreach (ModelMeshPart part in mesh.MeshParts)
                {
                    BasicEffect basicEffect = part.Effect as BasicEffect;

                    if (basicEffect != null)
                        modelTextures[mesh.GetHashCode()] = basicEffect.Texture;

                    part.Effect = effectLambert;
                }
            }
            
            //############################ Construction Octree ###########################


            OCTreeBuilder builder = new OCTreeBuilder(6000, 15, true, 0.0001f, 0.05f);

            List<OCTreeBuilder.ModelAndMatrix> ListMeshScene = new List<OCTreeBuilder.ModelAndMatrix>();
            
            OCTreeBuilder.ModelAndMatrix SceneModelMatrix = new OCTreeBuilder.ModelAndMatrix();
            SceneModelMatrix.Matrix = Matrix.Identity;
            SceneModelMatrix.Model = scene;
            ListMeshScene.Add(SceneModelMatrix);

            OCTreeBuilder.ModelAndMatrix EscalierModelMatrix = new OCTreeBuilder.ModelAndMatrix();
            EscalierModelMatrix.Matrix = MatrixEscalier;
            EscalierModelMatrix.Model = Escalier;
            ListMeshScene.Add(EscalierModelMatrix);

            OCTreeBuilder.ModelAndMatrix ScannerRoomModelMatrix = new OCTreeBuilder.ModelAndMatrix();
            ScannerRoomModelMatrix.Matrix = MatrixScannerRoom;
            ScannerRoomModelMatrix.Model = ScannerRoom;
            //ListMeshScene.Add(ScannerRoomModelMatrix);

            OCTreeBuilder.ModelAndMatrix ScannerMonitorsModelMatrix = new OCTreeBuilder.ModelAndMatrix();
            ScannerMonitorsModelMatrix.Matrix = MatrixScannerRoom;
            ScannerMonitorsModelMatrix.Model = ScannerMonitor;
            //ListMeshScene.Add(ScannerMonitorsModelMatrix);

            for(int i = 0; i<NB_Bridge; i++)
            {
                OCTreeBuilder.ModelAndMatrix BridgeModelMatrix = new OCTreeBuilder.ModelAndMatrix();
                BridgeModelMatrix.Matrix = MatrixBridge[i];
                BridgeModelMatrix.Model = Bridge;

                ListMeshScene.Add(BridgeModelMatrix);
            }

            OCTreeBuilder.ModelAndMatrix TerrainModelMatrix = new OCTreeBuilder.ModelAndMatrix();
            TerrainModelMatrix.Matrix = MatrixTerrain;
            TerrainModelMatrix.Model = Terrain;
            //ListMeshScene.Add(TerrainModelMatrix);

            OCTreeBuilder.ModelAndMatrix ReservoirModelMatrix =  new OCTreeBuilder.ModelAndMatrix();
            ReservoirModelMatrix.Matrix = MatrixReservoir;
            ReservoirModelMatrix.Model = Reservoir;

            ListMeshScene.Add(ReservoirModelMatrix);

            for (int i = 0; i < NB_Caisse; i++)
            {
                OCTreeBuilder.ModelAndMatrix ObjetModelMatrix = new OCTreeBuilder.ModelAndMatrix();
                ObjetModelMatrix.Matrix = MatrixCaisse[i];
                ObjetModelMatrix.Model = Caisse;

                ListMeshScene.Add(ObjetModelMatrix);
            }

            
            
            
            sceneOct = builder.Build(ListMeshScene);


            player.LoadContent();

            for (int i = 0; i < NB_Enemy; i++)
            {
                enemy[i] = new FPSEnemy( new Vector3(0.0f , -270.0f, i * -100.0f));
                enemy[i].LoadContent();
            }

            MyParralaxEnv.LoadContent();

            /*
            texturesSky[0] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\Sky\skybox_back");
            texturesSky[1] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\Sky\skybox_front");
            texturesSky[2] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\Sky\skybox_bottom");
            texturesSky[3] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\Sky\skybox_top");
            texturesSky[4] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\Sky\skybox_right");
            texturesSky[5] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\Sky\skybox_left");
            */


            texturesSkyNight[0] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\City\city_north");
            texturesSkyNight[1] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\City\city_south");
            texturesSkyNight[2] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\City\city_down");
            texturesSkyNight[3] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\City\city_up");
            texturesSkyNight[4] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\City\city_west");
            texturesSkyNight[5] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\City\city_east");
            

            /*
            texturesSky[0] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\Calm\Calm_north");
            texturesSky[1] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\Calm\Calm_south");
            texturesSky[2] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\Calm\Calm_down");
            texturesSky[3] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\Calm\Calm_up");
            texturesSky[4] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\Calm\Calm_west");
            texturesSky[5] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\Calm\Calm_east");
            */

            texturesSkyGrass[0] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\Grass\grassCUBE_page_006");
            texturesSkyGrass[1] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\Grass\grassCUBE_page_005");
            texturesSkyGrass[2] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\Grass\grassCUBE_page_004");
            texturesSkyGrass[3] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\Grass\grassCUBE_page_003");
            texturesSkyGrass[4] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\Grass\grassCUBE_page_001");
            texturesSkyGrass[5] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\Grass\grassCUBE_page_002");

            texturesSky[0] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\Hourglass\hourglass_north");
            texturesSky[1] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\Hourglass\hourglass_south");
            texturesSky[2] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\Hourglass\hourglass_down");
            texturesSky[3] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\Hourglass\hourglass_up");
            texturesSky[4] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\Hourglass\hourglass_west");
            texturesSky[5] = Utility.Game.Content.Load<Texture2D>(@"Textures\SkyBox\Hourglass\hourglass_east");
            

            //MySkyBox.Textures = texturesSky;
            MySkyBox.Textures = texturesSkyGrass;


            base.LoadContent();

        }

        protected override void UnloadContent()
        {
            MyParralaxEnv.UnloadContent();

            base.UnloadContent();
        }


        public void PerformCollisionCamera(Vector3 OldPosition, Vector3 Velocity, out Vector3 NewPosition)
        {

            List<OCTreeIntersection> sphereColliders = new List<OCTreeIntersection>();

            player.ColliderSphere.Bounds.Center = OldPosition;

            player.ColliderSphere.Bounds.Center += Vector3.Down * player.ColliderSphere.Bounds.Radius * 0.85f;

            player.ColliderSphere.Velocity = Velocity;
            sceneOct.MoveSphere(ref player.ColliderSphere.Bounds,
                                ref player.ColliderSphere.Velocity,
                                player.Friction,
                                ref sphereColliders);

            Utility.Camera.OnGround = false;

            for (int x = 0; x < sphereColliders.Count; x++)
            {

                if (Vector3.Dot(sphereColliders[x].IntersectionNormal,

                    Vector3.Up) > 0.5f)
                {

                    Utility.Camera.OnGround = true;

                    break;

                }

            }

            if (Utility.Camera.OnGround)
                Utility.Camera.CurrentVelocity = new Vector3(Utility.Camera.CurrentVelocity.X, 0.0f, Utility.Camera.CurrentVelocity.Z) ;

            NewPosition = player.ColliderSphere.Bounds.Center + Vector3.Up * player.ColliderSphere.Bounds.Radius * 0.85f;

        }

        //############################################################
        private void ChangeCameraBehavior(Camera.Behavior behavior)
        {
            if (Utility.Camera.CurrentBehavior == behavior)
                return;

            Utility.Camera.CurrentBehavior = behavior;
        }


        public void UpdateParticuleBilboard(Particule p,GameTime gameTime)
        {
            ParticuleBilboard pb = (ParticuleBilboard)p;

            float elapsed = (float)gameTime.ElapsedGameTime.TotalSeconds;

            p.TimePassed += elapsed;
            pb.billboard.Size = 10.0f * (1.0f - p.TimePassed / p.Timelife);

            pb.billboard.Position += p.Velocity * elapsed;
            pb.billboard.Position += p.Force * elapsed;
            pb.billboard.Couleur = new Color(pb.billboard.Couleur.R, pb.billboard.Couleur.G, pb.billboard.Couleur.B, 1.0f - pb.TimePassed / pb.Timelife); ;
        }

        /*
        public void UpdateParticulePoint(Particule p, GameTime gameTime)
        {
            ParticulePoint pp = (ParticulePoint)p;

            float elapsed = (float)gameTime.ElapsedGameTime.TotalSeconds;

            p.TimePassed += elapsed;
            pp.ParticuleData.pointSize = 50.0f * (1.0f - p.TimePassed / p.Timelife);
            pp.ParticuleData.position += p.Force * elapsed;

            pp.ParticuleData.color = new Vector4(pp.ParticuleData.color.X, pp.ParticuleData.color.Y, pp.ParticuleData.color.Z, 1.0f - pp.TimePassed / pp.Timelife); ;
        }
        */

        //############################################################
        private void UpdateParticuleBilboard(GameTime gameTime)
        {
            float elapsed = (float)gameTime.ElapsedGameTime.TotalSeconds;
            Vector3 PostionFeu = new Vector3(200.0f, -295.0f,0.0f);

            nextParticle -= elapsed;
            if (nextParticle <= 0.0f)
            {
                for (int i = 0; i < 5; i++)
                {
                    ParticuleBilboard pb = new ParticuleBilboard();

                    Vector3 pos = new Vector3(PostionFeu.X +(((float)rand.NextDouble()) * 2.0f - 1.0f) * 6.0f,
                              PostionFeu.Y + 20.0f + ((((float)rand.NextDouble()) * 2.0f - 1.0f) * 6.0f),
                              PostionFeu.Z + (((float)rand.NextDouble()) * 2.0f - 1.0f) * 6.0f);

                    Billboard Test = new Billboard(pos, 1.0f, new Color(1.0f, 1.0f, 1.0f,1.0f));

                    Test.Texture = Smoke;
                    pb.billboard = Test;
                    pb.Timelife = ((float)rand.NextDouble()) * 2.0f;
                    pb.Force = new Vector3(0.0f, 50.0f, 0.0f);

                    Vector3 vel = new Vector3((((float)rand.NextDouble()) * 2.0f - 1.0f) * 2.0f,
                              (((float)rand.NextDouble()) * 2.0f - 1.0f) * 20.0f,
                              (((float)rand.NextDouble()) * 2.0f - 1.0f) * 2.0f);

                    pb.Velocity = vel;

                    pb.updateParticule = UpdateParticuleBilboard;

                    Utility.ParticuleManager.AddBillboardParticule(pb);
                }

                for (int i = 0; i < 8; i++)
                {
                    ParticuleBilboard pb = new ParticuleBilboard();

                    Vector3 pos = new Vector3(PostionFeu.X + (((float)rand.NextDouble()) * 2.0f - 1.0f) * 10.0f,
                              PostionFeu.Y  + ((((float)rand.NextDouble()) * 2.0f - 1.0f) * 5.0f),
                              PostionFeu.Z + (((float)rand.NextDouble()) * 2.0f - 1.0f) * 10.0f);

                    Billboard Test = new Billboard(pos, 1.0f, new Color(1.0f, 1.0f, 1.0f));

                    Test.Texture = Fire;
                    pb.billboard = Test;
                    pb.Timelife = ((float)rand.NextDouble()) * 2.0f;
                    pb.Force = new Vector3(0.0f, 30.0f, 0.0f);

                    Vector3 vel = new Vector3((((float)rand.NextDouble()) * 2.0f - 1.0f) * 2.0f,
                              (((float)rand.NextDouble()) * 2.0f - 1.0f) * 20.0f,
                              (((float)rand.NextDouble()) * 2.0f - 1.0f) * 2.0f);

                    pb.Velocity = vel;

                    

                    pb.updateParticule = UpdateParticuleBilboard;

                    Utility.ParticuleManager.AddBillboardParticule(pb);
                }

                nextParticle = 0.002f;


            }
        }


        //############################################################

        private void UpdateProjectiles(GameTime gameTime)
        {
            if (Utility.InputState.IsMouseButtonPress(MouseButtons.LeftButton) || Utility.InputState.IsButtonJoYstickPCDown(2))
            {
                Vector3 dir;

                if (Utility.Camera.CurrentBehavior == Camera.Behavior.FirstPerson)
                {
                    dir = Utility.Camera.ZAxis;

                }
                else
                {
                    dir = new Vector3(Utility.Camera.ZAxis.X,0.0f,Utility.Camera.ZAxis.Z);
                }
                dir.Normalize();


                Vector3 initialPosition;

                if (Utility.Camera.CurrentBehavior == Camera.Behavior.FirstPerson)
                    initialPosition = Utility.Camera.Position;
                else
                    initialPosition = Utility.Camera.OrbitTarget;


                // Create a new projectile once per second. The real work of moving
                // and creating particles is handled inside the Projectile class.
                
                projectilesPlayer.Add(new Projectile(ParticuleManager.ListParticuleSystem.ExplosionParticuleSystem,
                                               ParticuleManager.ListParticuleSystem.ExplosionSmokdeParticuleSystem,
                                               ParticuleManager.ListParticuleSystem.ProjectileTailParticuleSystem,
                                               initialPosition,
                                               - dir,
                                               sceneOct));
                
              
            }


            int i = 0;

            List<BoundingSphere> BoundingActeurScene = new List<BoundingSphere>();

            for(int j = 0; j<NB_Enemy; j++)
                //if(!enemy[j].IsDead())
                    BoundingActeurScene.Add(enemy[j].ColliderSphere.Bounds);

            while (i < projectilesPlayer.Count)
            {
                if (!projectilesPlayer[i].Update(gameTime, BoundingActeurScene))
                {
                    // Remove projectiles at the end of their life.
                    int IndiceActeur = projectilesPlayer[i].KillActeur;
                    if (IndiceActeur  >= 0)
                        enemy[IndiceActeur].TakeDamage();

                    projectilesPlayer.RemoveAt(i);

                }
                else
                {
                    // Advance to the next projectile.
                    i++;
                }
            }

            List<BoundingSphere> BoundingPlayerScene = new List<BoundingSphere>();

            BoundingPlayerScene.Add(player.ColliderSphere.Bounds);

            while (i < projectilesEnemy.Count)
            {
                if (!projectilesEnemy[i].Update(gameTime, BoundingPlayerScene))
                {
                    // Remove projectiles at the end of their life.
                    int IndiceActeur = projectilesEnemy[i].KillActeur;
                    if (IndiceActeur >= 0 && player.health > 0)
                        player.health-=50;

                    projectilesEnemy.RemoveAt(i);

                }
                else
                {
                    // Advance to the next projectile.
                    i++;
                }
            }
            
        }

        //############################################################
        protected override void Update(GameTime gameTime)
        {
            if (Utility.InputState.IsBackOrEscapePress())
                this.Exit();
            int CurrentTeleport = InTeleport();
            if (CurrentTeleport  != -1 && Utility.InputState.IsKeyPress(Keys.LeftShift))
            {
                switch (CurrentTeleport)
                {
                    case 0:
                        {
                            switch (Utility.Camera.CurrentBehavior)
                            {
                                case Camera.Behavior.FirstPerson:
                                    Utility.Camera.Position = new Vector3(-400.0f, -400.0f, 0.0f);
                                    break;
                                case Camera.Behavior.ThirdPerson:
                                    Utility.Camera.OrbitTarget = new Vector3(-400.0f, -400.0f, 0.0f);
                                    break;
                            }
                            ePlayerSituation = EPlayerSituation.InsideLabo;
                        }
                    break;

                    case 1:
                    {
                        switch (Utility.Camera.CurrentBehavior)
                        {
                            case Camera.Behavior.FirstPerson:
                                Utility.Camera.Position = new Vector3(-400.0f, -265.0f, 0.0f);
                                break;
                            case Camera.Behavior.ThirdPerson:
                                Utility.Camera.OrbitTarget = new Vector3(-400.0f, -265.0f, 0.0f);
                                break;
                        }
                        ePlayerSituation = EPlayerSituation.Out;
                    }
                    break;
                }

                
            }   
             

            ProcessInput();
            UpdateProjectiles(gameTime);

            //UpdateParticuleBilboard(gameTime);

            //Fumée
            Utility.ParticuleManager.AddParticule(ParticuleManager.ListParticuleSystem.SmokePlumeParticuleSystem, new Vector3(500.0f, -295.0f, 0.0f), Vector3.Zero);

            //Teleport 
            CurrentTimeSalve -= (float)gameTime.ElapsedGameTime.TotalSeconds;
            if(CurrentTimeSalve <= 0.0f)
            {
                for (int j = 0; j < NB_Teleport; j++)
                {
                    for (double i = 0; i < MathHelper.Pi * 2; i += MathHelper.Pi * 2 / 50.0f)
                    {
                        Vector3 posParticule = PositionTeleport[j] + new Vector3((float)(Math.Cos(i) * RayonTeleport), 0.0f, (float)(Math.Sin(i) * RayonTeleport));
                        Utility.ParticuleManager.AddParticule(ParticuleManager.ListParticuleSystem.TeleportParticuleSystem, posParticule, Vector3.Zero);
                    }
                }

                CurrentTimeSalve = timeBetweenSalveTeleport;
            }
            
            fInterfaceTeleport += IncrementTeleport * (float)gameTime.ElapsedGameTime.TotalSeconds;
            if (fInterfaceTeleport <= 0.3f || fInterfaceTeleport >= 1.0f)
                IncrementTeleport = -IncrementTeleport;

            base.Update(gameTime);

            player.Update(gameTime);
            for (int i = 0; i < NB_Enemy; i++)
            {
                
                if (!enemy[i].IsDead())
                {
                    if (Utility.Camera.CurrentBehavior == Camera.Behavior.FirstPerson)
                    {
                        if (Vector3.Distance(enemy[i].Position, Utility.Camera.Position) < 300.0f)
                            if (enemy[i].PlayerNextenemy())
                            {
                                Vector3 Direction = Vector3.One;
                                if (Utility.Camera.CurrentBehavior == Camera.Behavior.FirstPerson)
                                    Direction = Utility.Camera.Position - enemy[i].Position;
                                else if (Utility.Camera.CurrentBehavior == Camera.Behavior.ThirdPerson)
                                    Direction = Utility.Camera.OrbitTarget - enemy[i].Position;
                                Direction.Normalize();

                                projectilesEnemy.Add(new Projectile(ParticuleManager.ListParticuleSystem.ExplosionAlienParticuleSystem,
                                                       ParticuleManager.ListParticuleSystem.ExplosionSmokeAliensParticleSystem,
                                                       ParticuleManager.ListParticuleSystem.ProjectileAlienTrailParticuleStsytem,
                                                       enemy[i].Position + new Vector3(0.0f,-10.0f,0.0f),
                                                       Direction,
                                                       sceneOct));
                            }
                            
                    }
                    else if (Utility.Camera.CurrentBehavior == Camera.Behavior.ThirdPerson)
                    {
                        if (Vector3.Distance(enemy[i].Position, Utility.Camera.OrbitTarget) < 300.0f)
                            if (enemy[i].PlayerNextenemy())
                            {

                                Vector3 Direction = Vector3.One;
                                if (Utility.Camera.CurrentBehavior == Camera.Behavior.FirstPerson)
                                    Direction = Utility.Camera.Position - enemy[i].Position;
                                else if (Utility.Camera.CurrentBehavior == Camera.Behavior.ThirdPerson)
                                    Direction = Utility.Camera.OrbitTarget - enemy[i].Position;
                                Direction.Normalize();

                                projectilesEnemy.Add(new Projectile(ParticuleManager.ListParticuleSystem.ExplosionAlienParticuleSystem,
                                                       ParticuleManager.ListParticuleSystem.ExplosionSmokeAliensParticleSystem,
                                                       ParticuleManager.ListParticuleSystem.ProjectileAlienTrailParticuleStsytem,
                                                       enemy[i].Position + new Vector3(0.0f, -10.0f, 0.0f),
                                                       Direction,
                                                       sceneOct));
                            }
                    }
                }

                enemy[i].Update(gameTime, sceneOct);
            }

            MyParralaxEnv.Update(gameTime);
            MySkyBox.Update(gameTime);


            if (LightRotation)
            {
                lightAngle += 0.001f;
                if (lightAngle >= 2 * MathHelper.Pi)
                    lightAngle = 0.0f;

                light.Direction = new Vector3((float)Math.Cos(lightAngle), -1.0f, (float)Math.Sin(lightAngle));
                light.Direction.Normalize();
            }

            RotationBlade += 0.3f;
            if(RotationBlade >= 2 * MathHelper.Pi)
                RotationBlade = 0.0f;

            MatrixRotateBlade = Matrix.CreateRotationZ(RotationBlade) * MatrixGunship;

            UpdateShadowMap();

            //UpdateTerrain();
        }

        private void UpdateTerrain()
        {

        }

        //############################################################
        protected void ProcessInput()
        {
            if (Utility.InputState.IsKeyPress(Keys.D1) || Utility.InputState.IsButtonJoYstickPCDown(6))
                ChangeCameraBehavior(Camera.Behavior.FirstPerson);


            if (Utility.InputState.IsKeyPress(Keys.D2) || Utility.InputState.IsButtonJoYstickPCDown(3))
                ChangeCameraBehavior(Camera.Behavior.FreeOrbit);

            if (Utility.InputState.IsKeyPress(Keys.D3) || Utility.InputState.IsButtonJoYstickPCDown(7))
                ChangeCameraBehavior(Camera.Behavior.ThirdPerson);

            if (Utility.InputState.IsKeyPress(Keys.F5))
            {
                displayShadows = !displayShadows;
                displayShadowMap = !displayShadowMap;
                
                if (displayShadows)
                    MySkyBox.Textures = texturesSkyNight;
                else
                    //MySkyBox.Textures = texturesSky;
                    MySkyBox.Textures = texturesSkyGrass;
                 
                
            }

            if (Utility.InputState.IsKeyPress(Keys.F1))
                shadowTechnique = ShadowTechnique.ShadowMapping;

            if (Utility.InputState.IsKeyPress(Keys.F2))
                shadowTechnique = ShadowTechnique.ShadowMappingWith2x2PercentageCloserFiltering;

            if (Utility.InputState.IsKeyPress(Keys.F3))
                shadowTechnique = ShadowTechnique.ShadowMappingWith3x3PercentageCloserFiltering;


            
            if (Utility.InputState.IsKeyPress(Keys.PageUp))
                FocaleCamera += 20.0f;

            if (Utility.InputState.IsKeyPress(Keys.PageDown))
            {
                FocaleCamera -= 20.0f;
                if(FocaleCamera <= 100.0f)
                    FocaleCamera = 100.0f;
            }
            if (Utility.InputState.IsKeyPress(Keys.Add))
                shadowMapNear.DepthBias = shadowMapNear.DepthBias + 0.001f;

            if (Utility.InputState.IsKeyPress(Keys.Subtract))
            {
                
                shadowMapNear.DepthBias = shadowMapNear.DepthBias - 0.001f;

                if (shadowMapNear.DepthBias <= 0.0f)
                    shadowMapNear.DepthBias = 0.0f;
                
            }

            if (Utility.InputState.IsKeyPress(Keys.LeftControl))
            {
                LightRotation = !LightRotation;
            }

        }

        //############################################################
        private int InTeleport()
        {

            for(int i = 0; i < NB_Teleport; i++)
                if (Vector3.Distance(player.VectPersoPosition, PositionTeleport[i]) <= RayonTeleport)
                    return i;

            return -1;
        }

        //############################################################
        private void DrawRadar()
        {

            spriteBatch.Draw(Interface, // Texture
                             new Vector2(GraphicsDevice.DisplayMode.Width - 340, 120.0f), //Position
                             new Rectangle(2, 14, 167, 167), //Rectangle Source
                             Color.White, // Color
                             0f, // rotation
                             new Vector2(100, 100), // Origin
                             1.0f, // Scale
                             SpriteEffects.None,
                             1f);

            spriteBatch.Draw(Interface, // Texture
                 new Vector2(GraphicsDevice.DisplayMode.Width - 340, 120.0f), //Position
                 new Rectangle(171, 13, 170, 170), //Rectangle Source
                 Color.White, // Color
                 0f, // rotation
                 new Vector2(100, 100), // Origin
                 1.0f, // Scale
                 SpriteEffects.None,
                 1f);

        }

        //############################################################
        private void DrawJaugeLife()
        {

            spriteBatch.Draw(Interface, // Texture
                             new Vector2(20, GraphicsDevice.DisplayMode.Height - 300), //Position
                             new Rectangle(1, 1, 504, 11), //Rectangle Source
                             Color.White, // Color
                             0f, // rotation
                             new Vector2(0, 0), // Origin
                             1.0f, // Scale
                             SpriteEffects.None,
                             1f);

            for (int index = 0; index < (player.health * 490 * 1.0f / player.maxhealth); index++)
            {
                spriteBatch.Draw(Interface, // Texture
                                 new Vector2(20 + index + 5, GraphicsDevice.DisplayMode.Height - 300), //Position
                                 new Rectangle(506, 1, 1, 11), //Rectangle Source
                                 Color.White, // Color
                                 0f, // rotation
                                 new Vector2(0, 0), // Origin
                                 1.0f, // Scale
                                 SpriteEffects.None,
                                 1f);

            }


        }

        //############################################################
        private void DrawAmmo()
        {

            spriteBatch.Draw(Interface, // Texture
                             new Vector2(120, GraphicsDevice.DisplayMode.Height - 240), //Position
                             new Rectangle(351, 14, 105, 26), //Rectangle Source
                             Color.White, // Color
                             0f, // rotation
                             new Vector2(100, 100), // Origin
                             1.0f, // Scale
                             SpriteEffects.None,
                             1f);

            spriteBatch.Draw(Interface, // Texture
                             new Vector2(120, GraphicsDevice.DisplayMode.Height - 240), //Position
                             new Rectangle(351, 45, 108, 27), //Rectangle Source
                             Color.White, // Color
                             0f, // rotation
                             new Vector2(100, 100), // Origin
                             1.0f, // Scale
                             SpriteEffects.None,
                             1f);

            string text = null;
            System.Text.StringBuilder buffer = new System.Text.StringBuilder();
            buffer.AppendFormat("{0}", player.Ammo.ToString("g5"));
            text = buffer.ToString();
            spriteBatch.DrawString(debugFont, text, new Vector2(50, GraphicsDevice.DisplayMode.Height - 334), Color.Red);


        }

        //############################################################
        private void DrawJaugeEndurance()
        {

            float pasTexture = 171 / player.MaxEndurance;
            int TailleTexture = (int)(player.Endurance * pasTexture);

            spriteBatch.Draw(InterfaceDetail, // Texture
                             new Vector2(GraphicsDevice.DisplayMode.Width - 380, GraphicsDevice.DisplayMode.Height - 350), //Position
                             new Rectangle(6, 320, TailleTexture, 37), //Rectangle Source
                             Color.White, // Color
                             MathHelper.ToRadians(-90.0f), // rotation
                             new Vector2(0, 0), // Origin
                             1.0f, // Scale
                             SpriteEffects.None,
                             1f);

        }

        private void DrawViseur()
        {

            spriteBatch.Draw(InterfaceViseur, // Texture
                 new Vector2(graphics.PreferredBackBufferWidth / 2, graphics.PreferredBackBufferHeight / 2), //Position
                 new Rectangle(0, 0, InterfaceViseur.Width, InterfaceViseur.Height), //Rectangle Source
                 new Color(1.0f,1.0f,1.0f,0.5f), // Color
                 MathHelper.ToRadians(-90.0f), // rotation
                 new Vector2(0, 0), // Origin
                 1.0f, // Scale
                 SpriteEffects.None,
                 1f);
        }

        private void DrawTeleport()
        {

            spriteBatch.Draw(InterfaceTeleport, // Texture
                 new Vector2(GraphicsDevice.DisplayMode.Width - 340, 420.0f), //Position
                 new Rectangle(0, 0, InterfaceTeleport.Width, InterfaceTeleport.Height),
                 new Color(1.0f, 1.0f, 1.0f, fInterfaceTeleport), // Color
                 0f, // rotation
                 new Vector2(0, 0), // Origin
                 1.0f, // Scale
                 SpriteEffects.None,
                 1f);

        }

        //############################################################
        private void DrawInterface()
        {
            spriteBatch.Begin(SpriteBlendMode.AlphaBlend, SpriteSortMode.Immediate, SaveStateMode.SaveState);

            DrawRadar();
            DrawJaugeLife();
            DrawAmmo();
            if(Utility.Camera.CurrentBehavior == Camera.Behavior.FirstPerson)
                DrawViseur();
            if (InTeleport() != -1)
                DrawTeleport();
            //DrawJaugeEndurance();
            spriteBatch.End();
        }


        //############################################################
        void UpdateFrameRate(GameTime gameTime)
        {
            accumElapsedTimeSec += gameTime.ElapsedRealTime.TotalSeconds;

            if (accumElapsedTimeSec > 1.0)
            {
                framesPerSecond = frames;
                frames = 0;
                accumElapsedTimeSec = 0.0;
            }
            else
            {
                ++frames;
            }
        }

        //############################################################
        private void CreateShadowMap()
        {

            List<ShadowMap.ModelAndMatrix> myScene = new List<ShadowMap.ModelAndMatrix>();


            for (int i = 0; i < NB_Caisse; i++)
            {
                ShadowMap.ModelAndMatrix ModelCaisse;
                ModelCaisse.model = Caisse;
                ModelCaisse.matrix = MatrixCaisse[i];
                ModelCaisse.type = ShadowMap.TypeModel.Fixe;
                ModelCaisse.animationPlayer = null;
                myScene.Add(ModelCaisse);
            }

            for (int i = 0; i < NB_Palmier; i++)
            {
                ShadowMap.ModelAndMatrix ModelPalmier;
                ModelPalmier.model = Palmier;
                ModelPalmier.matrix = MatrixPalmier[i];
                ModelPalmier.type = ShadowMap.TypeModel.Fixe;
                ModelPalmier.animationPlayer = null;
                myScene.Add(ModelPalmier);
            }

            ShadowMap.ModelAndMatrix ModelScene;
            ModelScene.model = scene;
            ModelScene.matrix = Matrix.Identity;
            ModelScene.type = ShadowMap.TypeModel.Fixe;
            ModelScene.animationPlayer = null;
            myScene.Add(ModelScene);

            ShadowMap.ModelAndMatrix ModelEscalier;
            ModelEscalier.model = Escalier;
            ModelEscalier.matrix = MatrixEscalier;
            ModelEscalier.type = ShadowMap.TypeModel.Fixe;
            ModelEscalier.animationPlayer = null;
            myScene.Add(ModelEscalier);

            for (int i = 0; i < NB_Bridge; i++)
            {
                ShadowMap.ModelAndMatrix ModelBridge;
                ModelBridge.model = Bridge;
                ModelBridge.matrix = MatrixBridge[i];
                ModelBridge.type = ShadowMap.TypeModel.Fixe;
                ModelBridge.animationPlayer = null;
                myScene.Add(ModelBridge);
            }

            ShadowMap.ModelAndMatrix ModelReservoir;
            ModelReservoir.model = Reservoir;
            ModelReservoir.matrix = MatrixReservoir;
            ModelReservoir.type = ShadowMap.TypeModel.Fixe;
            ModelReservoir.animationPlayer = null;
            myScene.Add(ModelReservoir);

            ShadowMap.ModelAndMatrix ModelPerso;
            ModelPerso.model = player.ModelPerso;
            ModelPerso.matrix = player.MatrixPerso;
            ModelPerso.type = ShadowMap.TypeModel.Animated;
            ModelPerso.animationPlayer = player.AnimationPlayer;

            myScene.Add(ModelPerso);

            for (int i = 0; i < NB_Enemy; i++)
            {
                ShadowMap.ModelAndMatrix ModelEnemy;
                ModelEnemy.model = enemy[i].ModelEnemy;
                ModelEnemy.matrix = enemy[i].MatrixEnemy;
                ModelEnemy.type = ShadowMap.TypeModel.Animated;
                ModelEnemy.animationPlayer = enemy[i].AnimationEnemy;
                myScene.Add(ModelEnemy);
            }

            shadowMapNear.Begin(GraphicsDevice);
            shadowMapNear.Draw(myScene);
            shadowMapNear.End();

            /*
            shadowMapFar.Begin(GraphicsDevice);
            shadowMapFar.Draw(myScene);
            shadowMapFar.End();
            */

        }

        //############################################################
        protected override void Draw(GameTime gameTime)
        {
            
            if (displayShadows)
                CreateShadowMap();

            GraphicsDevice.Clear(Color.CornflowerBlue);
            
            Utility.VectorRenderer.SetWorldMatrix(Matrix.Identity);
            Utility.VectorRenderer.SetViewProjMatrix(Utility.Camera.ViewProjectionMatrix);

            GraphicsDevice.RenderState.DepthBufferEnable = false;
            GraphicsDevice.RenderState.DepthBufferWriteEnable = false;
            MySkyBox.Draw(gameTime);
            GraphicsDevice.RenderState.DepthBufferEnable = true;
            GraphicsDevice.RenderState.DepthBufferWriteEnable = true;
            
            if(Utility.InputState.IsButtonDown(Buttons.X) || Utility.InputState.IsKeyDown(Keys.X))
            {
                
                GraphicsDevice.RenderState.FillMode = FillMode.WireFrame;
                sceneOct.DrawDebugPolygons();
                GraphicsDevice.RenderState.FillMode = FillMode.Solid;
                
            }
            else
            {
                
                WalkyrieFog.FogEnable = true;

                if (displayShadows)
                {
                    WalkyrieFog.FogColor = Color.TransparentBlack;//Color.DarkGray;
                    WalkyrieFog.FogEnd = FocaleCamera + 1000.0f;
                }
                else
                {
                    WalkyrieFog.FogColor = Color.DarkGray;
                    WalkyrieFog.FogEnd = 3000f;
                }
                WalkyrieFog.FogStart = 75f;
                WalkyrieFog.FogTableMode = FogMode.Linear;

                WalkyrieFog.Draw(graphics.GraphicsDevice, gameTime);
                
                /*
                if (!displayShadows)
                {
                    MyParralaxEnv.Draw(gameTime);
                }
                */

                if (ePlayerSituation == EPlayerSituation.InsideLabo)
                {
                    ModelExtensions.Draw(ScannerRoom, MatrixScannerRoom);
                    ModelExtensions.Draw(ScannerMonitor, MatrixScannerRoom);
                }
                else
                {
                    /*
                    Matrix[] bones = Terrain.GetAboluteBoneTransforms();
                    foreach (ModelMesh mesh in Terrain.Meshes)
                    {
                        foreach (Effect eff in mesh.Effects)
                        {
                            eff.Parameters["WorldViewProjection"].SetValue(bones[mesh.ParentBone.Index] * MatrixTerrain * Utility.Camera.ViewProjectionMatrix);
                        }
                        mesh.Draw();
                    }
                    */

                    

                    DrawModelShadow(scene, Matrix.Identity);
                    
                    DrawModelShadow(Reservoir, MatrixReservoir);

                    for (int i = 0; i < NB_Caisse; i++)
                        DrawModelShadow(Caisse, MatrixCaisse[i]);

                    //DrawTerrainShadow();
                    DrawModelShadow(Terrain, MatrixTerrain);

                    for (int i = 0; i < NB_Enemy; i++)
                        enemy[i].Draw(gameTime);

                    for (int i = 0; i < NB_Bridge; i++)
                        DrawModelShadow(Bridge, MatrixBridge[i]);

                    DrawModelShadow(Escalier, MatrixEscalier);

                    for (int i = 0; i < NB_Palmier; i++)
                        DrawModelShadow(Palmier, MatrixPalmier[i]);

                    //ModelExtensions.Draw(Gunship, MatrixGunship);
                    //ModelExtensions.Draw(RotateBlade, MatrixRotateBlade);
                    //ModelExtensions.Draw(RotateBlade, Matrix.CreateRotationZ(0.75f) * MatrixRotateBlade);

                    
                }

                player.Draw(gameTime);



            }

            if (Utility.InputState.IsButtonDown(Buttons.B) || Utility.InputState.IsKeyDown(Keys.B))
            {
                sceneOct.DrawDebugBounds();
            }

            if (displayShadowMap)
                DrawShadowMap();

            this.spriteBatch.DrawStringImmediate(
                debugFont, Utility.Camera.OnGround ? "On Ground" : "Falling",
                Vector2.One * 20,
                Color.White);

            this.spriteBatch.DrawStringImmediate(
                debugFont, Utility.InputState.IsButtonJoYstickPCDown(PlayerIndex.One, 0) ? "Button Ok Down" : "Button Ok Not Down",
                new Vector2(20,40),
                Color.White);
  
            System.Text.StringBuilder buffer = new System.Text.StringBuilder();

            buffer.AppendFormat("FPS: {0}\n", framesPerSecond);

            buffer.AppendFormat("Nb Particules Bilboard: {0}\n", Utility.ParticuleManager.NbParticuleBilboard);

            //buffer.AppendFormat("Nb Particules Point: {0}\n", Utility.ParticuleManager.NbParticulePoint);
            buffer.AppendFormat("Nb Particules System: {0}\n", Utility.ParticuleManager.NbParticuleSystem());
            
            buffer.AppendFormat("  XRotation: {0} YRotation:{1}\n",
                                player.XRotation.ToString("g4"),
                                player.YRotation.ToString("g4"));
            
            buffer.AppendFormat("  XDelta: {0} YDelta:{1}\n",
                    player.XDelta.ToString("g4"),
                    player.YDelta.ToString("g4"));

            buffer.AppendFormat("  Camera Position X:{0} Y:{1} Z:{2}\n",
                    Utility.Camera.Position.X.ToString("g4"),
                    Utility.Camera.Position.Y.ToString("g4"),
                    Utility.Camera.Position.Z.ToString("g4"));

            buffer.AppendFormat("  Camera Velocity X:{0} Y:{1} Z:{2}\n",
                    Utility.Camera.CurrentVelocity.X.ToString("g4"),
                    Utility.Camera.CurrentVelocity.Y.ToString("g4"),
                    Utility.Camera.CurrentVelocity.Z.ToString("g4"));

            buffer.AppendFormat("  Perso Angle :{0}\n",
                    player.MyAngle.ToString("g4"));

            buffer.AppendFormat("  ShadowMap Bias :{0}\n",
                    shadowMapNear.DepthBias.ToString("g4"));

            for(int i = 0; i<NB_Enemy;i++)
                buffer.AppendFormat("  Enemy {0} // endurance :{1}\n",
                        i.ToString("g4"),
                        enemy[i].Endurance.ToString("g4")); 
            
            string text = buffer.ToString();


            spriteBatch.Begin(SpriteBlendMode.AlphaBlend, SpriteSortMode.Deferred, SaveStateMode.SaveState);
            spriteBatch.DrawString(debugFont, text, new Vector2(20.0f, 60.0f), Color.White);
            spriteBatch.End();

            GraphicsDevice.RenderState.DepthBufferEnable = true;
            GraphicsDevice.RenderState.AlphaBlendEnable = false;

            base.Draw(gameTime);

            DrawInterface();

            UpdateFrameRate(gameTime);
        }

        private void DrawShadowMap()
        {
            Rectangle rect = new Rectangle();

            rect.X = 0;
            rect.Y = GraphicsDevice.Viewport.Height - 384;
            rect.Width = 256;
            rect.Height = 256;

            spriteBatch.Begin(SpriteBlendMode.None, SpriteSortMode.Deferred, SaveStateMode.SaveState);
            spriteBatch.Draw(shadowMapNear.ShadowMapTexture, rect, Color.White);
            spriteBatch.End();

            /*
            rect.X = 0;
            rect.Y = GraphicsDevice.Viewport.Height - 700;

            spriteBatch.Begin(SpriteBlendMode.None, SpriteSortMode.Deferred, SaveStateMode.SaveState);
            spriteBatch.Draw(shadowMapFar.ShadowMapTexture, rect, Color.White);
            spriteBatch.End();
             */
        }

        private void UpdateShadowMap()
        {
            Utility.Camera.Perspective(CAMERA_FOV, aspectRatio, CAMERA_ZNEAR, FocaleCamera);
            shadowMapNear.Update(light.Direction, Utility.Camera.ViewProjectionMatrix);
            Utility.Camera.Perspective(CAMERA_FOV, aspectRatio, CAMERA_ZNEAR, CAMERA_ZFAR);

            //shadowMapFar.Update(light.Direction, Utility.Camera.ViewProjectionMatrix);
        }


        private void DrawModelShadow(Model model, Matrix world)
        {

            Matrix[] bones = model.GetAboluteBoneTransforms();
            foreach (ModelMesh m in model.Meshes)
            {
                foreach (Effect e in m.Effects)
                {
                    if (!displayShadows)
                    {
                        e.CurrentTechnique = e.Techniques["Lambert"];
                    }
                    else
                    {
                        switch (shadowTechnique)
                        {
                            case ShadowTechnique.ShadowMapping:
                                e.CurrentTechnique = e.Techniques["LambertWithShadows"];
                                break;

                            case ShadowTechnique.ShadowMappingWith2x2PercentageCloserFiltering:
                                e.CurrentTechnique = e.Techniques["LambertWithShadows2x2PCF"];
                                e.Parameters["texelSize"].SetValue(shadowMapNear.TexelSize);
                                break;

                            case ShadowTechnique.ShadowMappingWith3x3PercentageCloserFiltering:
                                e.CurrentTechnique = e.Techniques["LambertWithShadows3x3PCF"];
                                e.Parameters["texelSize"].SetValue(shadowMapNear.TexelSize);
                                break;
                        }

                        e.Parameters["lightViewProjection"].SetValue(shadowMapNear.LightViewProjectionMatrix);
                        e.Parameters["textureScaleBias"].SetValue(shadowMapNear.TextureScaleBiasMatrix);
                        e.Parameters["depthBias"].SetValue(shadowMapNear.DepthBias);
                        e.Parameters["shadowMap"].SetValue(shadowMapNear.ShadowMapTexture);
                    }

                    e.Parameters["world"].SetValue(bones[m.ParentBone.Index] * world);

                    e.Parameters["view"].SetValue(Utility.Camera.ViewMatrix);
                    e.Parameters["projection"].SetValue(Utility.Camera.ProjectionMatrix);


                    e.Parameters["lightDir"].SetValue(light.Direction);
                    e.Parameters["lightColor"].SetValue(light.Diffuse.ToVector4());
                    e.Parameters["materialAmbient"].SetValue(material.Ambient.ToVector4());
                    e.Parameters["materialDiffuse"].SetValue(material.Diffuse.ToVector4());
                    e.Parameters["colorMap"].SetValue(modelTextures[m.GetHashCode()]);

                    e.CommitChanges();
                }

                m.Draw();
            }
        }

        private void DrawTerrainShadow()
        {

            Matrix[] bones = Terrain.GetAboluteBoneTransforms();
            foreach (ModelMesh m in Terrain.Meshes)
            {
                foreach (Effect e in m.Effects)
                {
                    if (!displayShadows)
                    {
                        e.CurrentTechnique = e.Techniques["Lambert"];
                    }
                    else
                    {
                        switch (shadowTechnique)
                        {
                            case ShadowTechnique.ShadowMapping:
                                e.CurrentTechnique = e.Techniques["LambertWithShadows"];
                                break;

                            case ShadowTechnique.ShadowMappingWith2x2PercentageCloserFiltering:
                                e.CurrentTechnique = e.Techniques["LambertWithShadows2x2PCF"];
                                e.Parameters["texelSize"].SetValue(shadowMapNear.TexelSize);
                                break;

                            case ShadowTechnique.ShadowMappingWith3x3PercentageCloserFiltering:
                                e.CurrentTechnique = e.Techniques["LambertWithShadows3x3PCF"];
                                e.Parameters["texelSize"].SetValue(shadowMapNear.TexelSize);
                                break;
                        }

                        e.Parameters["lightViewProjection"].SetValue(shadowMapNear.LightViewProjectionMatrix);
                        e.Parameters["textureScaleBias"].SetValue(shadowMapNear.TextureScaleBiasMatrix);
                        e.Parameters["depthBias"].SetValue(shadowMapNear.DepthBias);
                        e.Parameters["shadowMap"].SetValue(shadowMapNear.ShadowMapTexture);
                    }

                    e.Parameters["world"].SetValue(bones[m.ParentBone.Index] * MatrixTerrain);

                    e.Parameters["view"].SetValue(Utility.Camera.ViewMatrix);
                    e.Parameters["projection"].SetValue(Utility.Camera.ProjectionMatrix);

                    e.Parameters["TerreTexture"].SetValue(TextureTerre);
                    e.Parameters["DensiteTerreTexture"].SetValue(DensiteTerre);

                    e.Parameters["lightDir"].SetValue(light.Direction);
                    e.Parameters["lightColor"].SetValue(light.Diffuse.ToVector4());
                    e.Parameters["materialAmbient"].SetValue(material.Ambient.ToVector4());
                    e.Parameters["materialDiffuse"].SetValue(material.Diffuse.ToVector4());
                    e.Parameters["herbeTexture"].SetValue(modelTextures[m.GetHashCode()]);

                    e.CommitChanges();
                }

                m.Draw();
            }
        }
    }
}
