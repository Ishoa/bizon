#region Using Statements
using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
#endregion

namespace XNAWalkyrie.ParticuleSystem
{


    /// <summary>
    /// Custom particle system for leaving smoke trails behind the rocket projectiles.
    /// </summary>
    class ProjectileAlienTrailParticuleStsytem : ParticleSystem
    {
        public ProjectileAlienTrailParticuleStsytem(Game game, ContentManager content)
            : base(game, content)
        { }


        protected override void InitializeSettings(ParticleSettings settings)
        {
            settings.TextureName = @"Textures\Particules\smoke3";
            //settings.TextureName = @"Textures\Particules\Fire2";

            settings.MinColor = Color.Green;
            settings.MaxColor = Color.Green;

            settings.MaxParticles = 5000;

            settings.Duration = TimeSpan.FromSeconds(3);

            settings.DurationRandomness = 1.5f;

            settings.EmitterVelocitySensitivity = 0.1f;

            settings.MinHorizontalVelocity = 0;
            settings.MaxHorizontalVelocity = 1;

            settings.MinVerticalVelocity = -1;
            settings.MaxVerticalVelocity = 1;


            settings.MinRotateSpeed = -4;
            settings.MaxRotateSpeed = 4;

            settings.MinStartSize = 2;
            settings.MaxStartSize = 4;

            settings.MinEndSize = 5;
            settings.MaxEndSize = 15;

            settings.SourceBlend = Blend.SourceAlpha;
            settings.DestinationBlend = Blend.One;
        }
    }
}
