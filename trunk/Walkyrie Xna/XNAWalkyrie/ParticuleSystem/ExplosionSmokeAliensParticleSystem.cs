﻿#region Using Statements
using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
#endregion

namespace XNAWalkyrie.ParticuleSystem
{

    class ExplosionSmokeAliensParticleSystem : ParticleSystem
    {
        public ExplosionSmokeAliensParticleSystem(Game game, ContentManager content)
            : base(game, content)
        { }

        protected override void InitializeSettings(ParticleSettings settings)
        {
            //settings.TextureName = @"Textures\Particules\explosion";
            settings.TextureName = @"Textures\Particules\Fire2";

            settings.MinColor = Color.Green;
            settings.MaxColor = Color.Green;

            settings.MaxParticles = 2000;

            settings.Duration = TimeSpan.FromSeconds(2);
            settings.DurationRandomness = 1;

            settings.MinHorizontalVelocity = 20;
            settings.MaxHorizontalVelocity = 30;

            settings.MinVerticalVelocity = -20;
            settings.MaxVerticalVelocity = 20;

            settings.EndVelocity = 0;

            //settings.MinColor = Color.DarkGray;
            //settings.MaxColor = Color.Gray;

            settings.MinRotateSpeed = -1;
            settings.MaxRotateSpeed = 1;

            settings.MinStartSize = 10;
            settings.MaxStartSize = 10;

            settings.MinEndSize = 50;
            settings.MaxEndSize = 100;

            // Use additive blending.
            settings.SourceBlend = Blend.SourceAlpha;
            settings.DestinationBlend = Blend.One;
        }
    }
}