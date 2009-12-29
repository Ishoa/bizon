using System;

namespace OCTreeTest
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static void Main(string[] args)
        {
            using (GameFPS game = new GameFPS())
            {
                game.Run();
            }
        }
    }
}

