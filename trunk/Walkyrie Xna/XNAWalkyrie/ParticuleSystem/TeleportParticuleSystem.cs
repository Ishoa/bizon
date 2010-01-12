#region Using Statements
using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
#endregion

namespace XNAWalkyrie
{
    class TeleportParticuleSystem : ParticleSystem
    {

        public TeleportParticuleSystem(Game game, ContentManager content)
            : base(game, content)
        { }


        protected override void InitializeSettings(ParticleSettings settings)
        {
            settings.TextureName = @"Textures\Particules\flare";

            settings.MaxParticles = 600;

            settings.Duration = TimeSpan.FromSeconds(1.7);

            settings.MinHorizontalVelocity = 0;
            settings.MaxHorizontalVelocity = 1;

            settings.MinVerticalVelocity = 0;
            settings.MaxVerticalVelocity = 1;

            // Create a wind effect by tilting the gravity vector sideways.
            settings.Gravity = new Vector3(0, 50, 0);

            settings.EndVelocity = 0.75f;

            settings.MinColor = Color.Blue;
            settings.MaxColor = Color.Blue;

            settings.MinRotateSpeed = -1;
            settings.MaxRotateSpeed = 1;

            settings.MinStartSize = 8;
            settings.MaxStartSize = 10;

            settings.MinEndSize = 8;
            settings.MaxEndSize = 10;

            // Use additive blending.
            settings.SourceBlend = Blend.SourceAlpha;
            settings.DestinationBlend = Blend.One;
        }
    }
}
