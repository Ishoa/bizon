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

    public class MovingSphere
    {
        public BoundingSphere Bounds;
        public Vector3 Velocity;
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
            Friction = 0.3f;

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

                    Matrix WeaponWorldTransform = Matrix.CreateScale(0.1f) * Matrix.CreateTranslation(-1.0f, 2.0f, -4.0f) * Matrix.CreateRotationY(1.75f) * Matrix.CreateRotationX(-1.57f) *
                                               worldTransforms[HandIndex];

                    ModelWeapon.Draw(WeaponWorldTransform);

                    if (Utility.InputState.IsKeyDown(Keys.C))
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
}
