using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace XNAWalkyrie
{
        public sealed class WalkyrieFog
        {
            public static Color FogColor;
            public static bool FogEnable;
            public static float FogStart;
            public static float FogEnd;
            public static FogMode FogTableMode;

            public static void Draw(GraphicsDevice myDevice, GameTime gameTime)
            {
                if (myDevice.RenderState.FogEnable != FogEnable)
                    myDevice.RenderState.FogEnable = FogEnable;

                if (FogEnable)
                {
                    if (myDevice.RenderState.FogColor != FogColor)
                        myDevice.RenderState.FogColor = FogColor;
                    if (myDevice.RenderState.FogStart != FogStart)
                        myDevice.RenderState.FogStart = FogStart;
                    if (myDevice.RenderState.FogEnd != FogEnd)
                        myDevice.RenderState.FogEnd = FogEnd;
                    if (myDevice.RenderState.FogTableMode != FogTableMode)
                        myDevice.RenderState.FogTableMode = FogTableMode;
                }
            }
        }
}
