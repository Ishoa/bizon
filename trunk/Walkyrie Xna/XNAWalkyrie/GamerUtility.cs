using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;
using Microsoft.Xna.Framework.Net;
using Microsoft.Xna.Framework.Storage;


namespace XNAWalkyrie
{
    public partial class Utility
    {
        public static bool IsLoggedInToLive(PlayerIndex index)
        {
            for (int x = 0; x < Gamer.SignedInGamers.Count; x++)
            {
                SignedInGamer sg = Gamer.SignedInGamers[x];

                if (sg.PlayerIndex == index)
                {
                    if (sg.IsSignedInToLive == true)
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        public static bool IsLoggedInLocal(PlayerIndex index)
        {
            for (int x = 0; x < Gamer.SignedInGamers.Count; x++)
            {
                SignedInGamer sg = Gamer.SignedInGamers[x];

                if (sg.PlayerIndex == index)
                {
                    return true;
                }
            }
            return false;
        }
    }
}