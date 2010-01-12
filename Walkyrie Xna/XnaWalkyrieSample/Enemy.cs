using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

using XNAWalkyrie;
using SkinnedModel;
using Microsoft.Xna.Framework.Input;

namespace XnaWalkyrieSample
{
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

        public float Endurance = 100;
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


        public void Update(GameTime gameTime, OCTree sceneOct)
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
                if (Utility.Camera.CurrentBehavior == Camera.Behavior.FirstPerson)
                    Direction = Utility.Camera.Position - VectEnemyPosition;
                else if (Utility.Camera.CurrentBehavior == Camera.Behavior.ThirdPerson)
                    Direction = Utility.Camera.OrbitTarget - VectEnemyPosition;
                Direction = new Vector3(Direction.X, 0.0f, Direction.Z);
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
                Utility.VectorRenderer.DrawLine(VectEnemyPosition, VectEnemyDestination);

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
                animationEnemy.StartClip(Damage, false);

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
                    animationEnemy.StartClip(Iddle, false);
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

}
