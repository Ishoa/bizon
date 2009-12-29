#region File Description
//-----------------------------------------------------------------------------
// Projectile.cs
//
// Microsoft XNA Community Game Platform
// Copyright (C) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#endregion

#region Using Statements
using System;
using Microsoft.Xna.Framework;
using System.Collections.Generic;
#endregion

namespace XNAWalkyrie
{
    /// <summary>
    /// This class demonstrates how to combine several different particle systems
    /// to build up a more sophisticated composite effect. It implements a rocket
    /// projectile, which arcs up into the sky using a ParticleEmitter to leave a
    /// steady stream of trail particles behind it. After a while it explodes,
    /// creating a sudden burst of explosion and smoke particles.
    /// </summary>
    public class Projectile
    {
        #region Constants

        const float trailParticlesPerSecond = 300;
        const int numExplosionParticles = 30;
        const int numExplosionSmokeParticles = 50;

        const float projectileLifespan = 10.0f;
        const float sidewaysVelocityRange = 60;
        const float verticalVelocityRange = 40;

        const float DistanceExplosion = 20.0f;

        #endregion

        #region Fields

        ParticuleManager.ListParticuleSystem explosionParticles;
        ParticuleManager.ListParticuleSystem explosionSmokeParticles;
        ParticleEmitter trailEmitter;

        Vector3 position;
        Vector3 velocity;

        Vector3 initialposRef;

        bool bcollision = false;
        bool bExplode = false;

        public int KillActeur = -1;

        Vector3 Collision;

        float age;
        
        static Random random = new Random();


        #endregion


        /// <summary>
        /// Constructs a new projectile.
        /// </summary>
        public Projectile(ParticuleManager.ListParticuleSystem explosionParticles,
                          ParticuleManager.ListParticuleSystem explosionSmokeParticles,
                          ParticuleManager.ListParticuleSystem trailEmitter,
                          Vector3 InitialPosition,
                          Vector3 Direction,
                          OCTree m_Octree)
        {
            this.explosionParticles = explosionParticles;
            this.explosionSmokeParticles = explosionSmokeParticles;

            // Start at the origin, firing in a random (but roughly upward) direction.
            position = InitialPosition;


            velocity = Direction * 600.0f;

            Ray direction = new Ray(InitialPosition,Direction);

            OCTreeIntersection ResultatIntersection;

            m_Octree.GetIntersectingPolygon(ref direction, out ResultatIntersection);

            if (ResultatIntersection.IntersectType != OCTreeIntersectionType.None)
            {
                bcollision = true;
                Collision = ResultatIntersection.IntersectionPoint;
                initialposRef = Collision - InitialPosition;
                initialposRef.Normalize();

            }
            else
            {
                bcollision = false;
                Collision = Vector3.Zero;
            }

            age = 0.0f;

            // Use the particle emitter helper to output our trail particles.
            this.trailEmitter = new ParticleEmitter(trailEmitter, trailParticlesPerSecond, position);
        }


        /// <summary>
        /// Updates the projectile.
        /// </summary>
        public bool Update(GameTime gameTime, List<BoundingSphere> ListCollideSphere)
        {
            float elapsedTime = (float)gameTime.ElapsedGameTime.TotalSeconds;

            // Simple projectile physics.
            age += elapsedTime;
            Vector3 vel = velocity;
            vel.Normalize();
            if (!bExplode)
            {
                position += velocity * elapsedTime;
                // Update the particle emitter, which will create our particle trail.
                trailEmitter.Update(gameTime, position);
            }



            // If enough time has passed, explode! Note how we pass our velocity
            // in to the AddParticle method: this lets the explosion be influenced
            // by the speed and direction of the projectile which created it.
            if (age > projectileLifespan)
            {
                return false;
            }

            if (ListCollideSphere.Count > 0)
            {
                for (int i = 0; i < ListCollideSphere.Count; i++)
                {
                    if (Vector3.Distance(ListCollideSphere[i].Center, position) <= ListCollideSphere[i].Radius)
                    {
                        bExplode = true;
                        KillActeur = i;
                        break;
                    }
                }
            }

            if (bExplode || (bcollision && (Vector3.DistanceSquared(position, Collision) < DistanceExplosion || Vector3.Dot(initialposRef, vel) > 1.57f)))
            {

                for (int i = 0; i < numExplosionParticles; i++)
                    Utility.ParticuleManager.AddParticule(explosionParticles,position, Vector3.Zero);

                for (int i = 0; i < numExplosionSmokeParticles; i++)
                    Utility.ParticuleManager.AddParticule(explosionSmokeParticles, position, Vector3.Zero);

                return false;
            }

            return true;
        }
    }
}
