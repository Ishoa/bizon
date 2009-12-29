using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
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
        private static InputState inputState = null;

        public static InputState InputState
        {
            get
            {
                if (inputState == null)
                {
                    inputState = new InputState(game);
                    game.Components.Add(InputState);
                }
                return inputState;
            }
            set
            {
                if (inputState != null)
                {
                    RemoveService<InputService>();
                }
                inputState = value;
                AddService<InputService>(new InputService(inputState));
            }
        }
    }

    public interface IInputService
    {
        InputState InputState { get; }
    }

    public class InputService : IInputService
    {

        public InputService(InputState inputState)
        {
            this.inputState = inputState;
        }

        private InputState inputState;

        public InputState InputState
        {
            get
            {
                return inputState;
            }
        }

        public static implicit operator InputState(InputService cs)
        {
            return cs.InputState;
        }
    }



#if !XBOX360
        public enum MouseButtons
        {
            LeftButton,
            MiddleButton,
            RightButton,
            XButton1,
            XButton2
        }
#endif


        

        public class InputState : DrawableGameComponent
        {

            private GamePadState[] currentPadState;
            private GamePadState[] previousPadState;

            private KeyboardState currentKeyState;
            private KeyboardState previousKeyState;

            private readonly Keys[] keyArray =
                Utility.GetValuesArray<Keys>();

            private readonly PlayerIndex[] padArray =
                Utility.GetValuesArray<PlayerIndex>();

            private readonly Buttons[] buttonArray =
                Utility.GetValuesArray<Buttons>();

#if !XBOX360
            private Joystick[] MyJoystickSPlayer;
            private string[] sticks;

            private MouseState currentMouseState;
            private MouseState previousMouseState;
#endif

            public InputState(Game g) : base(g)
            {

                currentPadState = new GamePadState[4];
                previousPadState = new GamePadState[4];

                for (int i = 0; i < 4; i++)
                {
                    previousPadState[i] = currentPadState[i] =
                        GamePad.GetState(padArray[i]);
                }

                previousKeyState = Keyboard.GetState();
                currentKeyState = Keyboard.GetState();

#if !XBOX360
                previousMouseState = Mouse.GetState();
                currentMouseState = Mouse.GetState();
                mousePosition = new Vector2(currentMouseState.X,
                                            currentMouseState.Y);
#endif
            }

            protected override void LoadContent()
            {
#if !XBOX360
                

                sticks = Joystick.FindJoysticks();
                if (sticks != null)
                {
                    MyJoystickSPlayer = new Joystick[sticks.Length];
                    for (int i = 0; i < sticks.Length; i++)
                    {
                        MyJoystickSPlayer[i] = new Joystick();
                        MyJoystickSPlayer[i].AcquireJoystick(sticks[i]);
                    }
                }


#endif
                base.LoadContent();
            }


            public override void Update(GameTime gameTime)
            {
                for (int i = 0; i < currentPadState.Length; i++)
                {
                    previousPadState[i] = currentPadState[i];
                    currentPadState[i] = GamePad.GetState(padArray[i]);
                }


#if !XBOX360
                previousMouseState = currentMouseState;
                currentMouseState = Mouse.GetState();

                // We also take in the latest mouse position.
                mousePosition.X = currentMouseState.X;
                mousePosition.Y = currentMouseState.Y;
                if (sticks != null)
                {
                    for (int i = 0; i < sticks.Length; i++)
                        MyJoystickSPlayer[i].UpdateStatus();
                }

#endif

                // We set the Previous states to the Current states
                // since the Current states will be updated with
                // the ACTUAL Current states.
                previousKeyState = currentKeyState;
                currentKeyState = Keyboard.GetState();

            }

            public override void Draw(GameTime gameTime)
            {
                base.Draw(gameTime);
            }

            public bool IsBackOrEscapeDown()
            {
                return IsButtonDown(Buttons.Back) ||
                       IsKeyDown(Keys.Escape);
            }
            public bool IsBackOrEscapeDown(PlayerIndex player)
            {
                return IsButtonDown(Buttons.Back, player) ||
                       IsKeyDown(Keys.Escape);
            }

            public bool IsBackOrEscapePress()
            {
                return IsButtonPress(Buttons.Back) ||
                       IsKeyPress(Keys.Escape);
            }
            public bool IsBackOrEscapePress(PlayerIndex player)
            {
                return IsButtonPress(Buttons.Back, player) ||
                       IsKeyPress(Keys.Escape);
            }

            public bool IsOKDown()
            {
                return IsButtonDown(Buttons.A) ||
                       IsButtonDown(Buttons.Start) ||
                       IsKeyDown(Keys.Enter)
#if !XBOX360
                        || (sticks != null && MyJoystickSPlayer[0].Buttons[1])
#endif
                       ;
            }
            public bool IsOKDown(PlayerIndex player)
            {
                return IsButtonDown(Buttons.A, player) ||
                       IsButtonDown(Buttons.Start, player) ||
                       IsKeyDown(Keys.Enter);
            }

            public bool IsCancelDown()
            {
                return IsButtonDown(Buttons.B) ||
                       IsButtonDown(Buttons.Back) ||
                       IsKeyDown(Keys.Escape);
            }
            public bool IsCancelDown(PlayerIndex player)
            {
                return IsButtonDown(Buttons.B, player) ||
                       IsButtonDown(Buttons.Back, player) ||
                       IsKeyDown(Keys.Escape);
            }

            public bool IsOKPress()
            {
                return IsButtonPress(Buttons.A) ||
                       IsButtonPress(Buttons.Start) ||
                       IsKeyPress(Keys.Enter);
            }
            public bool IsOKPress(PlayerIndex player)
            {
                return IsButtonPress(Buttons.A, player) ||
                       IsButtonPress(Buttons.Start, player) ||
                       IsKeyPress(Keys.Enter);
            }

            public bool IsCancelPress()
            {
                return IsButtonPress(Buttons.B) ||
                       IsButtonPress(Buttons.Back) ||
                       IsKeyPress(Keys.Escape);
            }
            public bool IsCancelPress(PlayerIndex player)
            {
                return IsButtonPress(Buttons.B, player) ||
                       IsButtonPress(Buttons.Back, player) ||
                       IsKeyPress(Keys.Escape);
            }

            public bool IsButtonPress(Buttons button, PlayerIndex player)
            {
                return (currentPadState[(int)player].IsButtonDown(button) &&
                        previousPadState[(int)player].IsButtonUp(button));
            }

            public bool IsButtonPress(Buttons button)
            {
                for (int x = 0; x < 4; x++)
                {
                    if (currentPadState[x].IsButtonDown(button) &&
                        previousPadState[x].IsButtonUp(button))
                    {
                        return true;
                    }
                }
                return false;
            }

            public PlayerIndex GetOKButtonPlayerIndex()
            {
                for (int x = 0; x < 4; x++)
                {
                    if (IsOKPress((PlayerIndex)x))
                    {
                        return (PlayerIndex)x;
                    }
                }
                return PlayerIndex.One;
            }

            public bool IsButtonPressDirectionDown()
            {
                for (int x = 0; x < 4; x++)
                {
                    if ((currentPadState[x].IsButtonDown(Buttons.LeftThumbstickDown) &&
                        previousPadState[x].IsButtonUp(Buttons.LeftThumbstickDown)) ||
                        (currentPadState[x].IsButtonDown(Buttons.DPadDown) &&
                        previousPadState[x].IsButtonUp(Buttons.DPadDown)))
                    {
                        return true;
                    }
                }
                return false;
            }
            public bool IsButtonPressDirectionDown(PlayerIndex player)
            {
                return (currentPadState[(int)player].IsButtonDown(Buttons.LeftThumbstickDown) &&
                        previousPadState[(int)player].IsButtonUp(Buttons.LeftThumbstickDown) ||
                        currentPadState[(int)player].IsButtonDown(Buttons.DPadDown) &&
                        previousPadState[(int)player].IsButtonUp(Buttons.DPadDown));
            }

            public bool IsButtonPressDirectionUp()
            {
                for (int x = 0; x < 4; x++)
                {
                    if (currentPadState[x].IsButtonDown(Buttons.LeftThumbstickUp) &&
                        previousPadState[x].IsButtonUp(Buttons.LeftThumbstickUp) ||
                        currentPadState[x].IsButtonDown(Buttons.DPadUp) &&
                        previousPadState[x].IsButtonUp(Buttons.DPadUp))
                    {
                        return true;
                    }
                }
                if (sticks != null)
                {
                    for (int x = 0; x < sticks.Length; x++)
                        if (MyJoystickSPlayer[x].Buttons[1])
                            return true;
                }
                return false;
            }
            public bool IsButtonPressDirectionUp(PlayerIndex player)
            {
                return (currentPadState[(int)player].IsButtonDown(Buttons.LeftThumbstickUp) &&
                        previousPadState[(int)player].IsButtonUp(Buttons.LeftThumbstickUp) ||
                        currentPadState[(int)player].IsButtonDown(Buttons.DPadUp) &&
                        previousPadState[(int)player].IsButtonUp(Buttons.DPadUp));
            }

            public bool IsButtonPressDirectionLeft()
            {
                for (int x = 0; x < 4; x++)
                {
                    if ((currentPadState[x].IsButtonDown(Buttons.LeftThumbstickLeft) &&
                        previousPadState[x].IsButtonUp(Buttons.LeftThumbstickLeft)) ||
                        (currentPadState[x].IsButtonDown(Buttons.DPadLeft) &&
                        previousPadState[x].IsButtonUp(Buttons.DPadLeft)) ||
                        IsKeyPress(Keys.Q))
                    {
                        return true;
                    }
                }
                return false;
            }
            public bool IsButtonPressDirectionLeft(PlayerIndex player)
            {
                return (currentPadState[(int)player].IsButtonDown(Buttons.LeftThumbstickLeft) &&
                        previousPadState[(int)player].IsButtonUp(Buttons.LeftThumbstickLeft) ||
                        currentPadState[(int)player].IsButtonDown(Buttons.DPadLeft) &&
                        previousPadState[(int)player].IsButtonUp(Buttons.DPadLeft));
            }


            public bool IsButtonPressDirectionRight()
            {
                for (int x = 0; x < 4; x++)
                {
                    if (currentPadState[x].IsButtonDown(Buttons.LeftThumbstickRight) &&
                        previousPadState[x].IsButtonUp(Buttons.LeftThumbstickRight) ||
                        currentPadState[x].IsButtonDown(Buttons.DPadRight) &&
                        previousPadState[x].IsButtonUp(Buttons.DPadRight))
                    {
                        return true;
                    }
                }
                return false;
            }

            public bool IsButtonPressDirectionRight(PlayerIndex player)
            {
                return (currentPadState[(int)player].IsButtonDown(Buttons.LeftThumbstickRight) &&
                        previousPadState[(int)player].IsButtonUp(Buttons.LeftThumbstickRight) ||
                        currentPadState[(int)player].IsButtonDown(Buttons.DPadRight) &&
                        previousPadState[(int)player].IsButtonUp(Buttons.DPadRight));
            }

            public bool IsButtonRelease(Buttons button, PlayerIndex player)
            {
                return (currentPadState[(int)player].IsButtonUp(button) &&
                        previousPadState[(int)player].IsButtonDown(button));
            }

            public bool IsButtonRelease(Buttons button, int player)
            {
                return (currentPadState[player].IsButtonUp(button) &&
                        previousPadState[player].IsButtonDown(button));
            }

            public bool IsButtonRelease(Buttons button)
            {
                for (int x = 0; x < 4; x++)
                {
                    if (currentPadState[x].IsButtonUp(button) &&
                        previousPadState[x].IsButtonDown(button))
                    {
                        return true;
                    }
                }
                return false;
            }

            public bool IsButtonDown(Buttons button, PlayerIndex player)
            {
                return currentPadState[(int)player].IsButtonDown(button);
            }

            public bool IsButtonDown(Buttons button)
            {
                for (int x = 0; x < 4; x++)
                    if (currentPadState[x].IsButtonDown(button))
                        return true;
                return false;
            }

            public bool IsButtonUp(Buttons button, PlayerIndex player)
            {
                return currentPadState[(int)player].IsButtonUp(button);
            }


            public Vector2 LeftThumbStick(PlayerIndex player)
            {
                return currentPadState[(int)player].ThumbSticks.Left;
            }

            
            public Vector2 LeftThumbStick(int player)
            {
                return currentPadState[player].ThumbSticks.Left;
            }

            public Vector2 LeftThumbStick()
            {
                Vector2 ret = Vector2.Zero;

                for (int x = 0; x < 4; x++)
                {
                    if (currentPadState[x].ThumbSticks.Left.
                            LengthSquared() > 0.01f)
                    {
                        ret = currentPadState[x].ThumbSticks.Left;
                    }
                }
                return ret;
            }

            public Vector2 RightThumbStick(PlayerIndex player)
            {
                return currentPadState[(int)player].ThumbSticks.Right;
            }

            public Vector2 RightThumbStick(int player)
            {
                return currentPadState[player].ThumbSticks.Right;
            }

            public Vector2 RightThumbStick()
            {
                Vector2 ret = Vector2.Zero;

                for (int x = 0; x < 4; x++)
                {
                    if (currentPadState[x].ThumbSticks.Right.
                            LengthSquared() > 0.01f)
                    {
                        ret = currentPadState[x].ThumbSticks.Right;
                    }
                }
                return ret;
            }

            public float RightTrigger(PlayerIndex player)
            {
                return currentPadState[(int)player].Triggers.Right;
            }

            public float RightTrigger(int player)
            {
                return currentPadState[player].Triggers.Right;
            }

            public float RightTrigger()
            {
                float ret = 0;
                for (int x = 0; x < 4; x++)
                {
                    if (currentPadState[x].Triggers.Right > float.Epsilon)
                    {
                        ret = currentPadState[x].Triggers.Right;
                        break;
                    }
                }
                return ret;
            }

            public float LeftTrigger(PlayerIndex player)
            {
                return currentPadState[(int)player].Triggers.Left;
            }

            public float LeftTrigger(int player)
            {
                return currentPadState[player].Triggers.Left;
            }

            public float LeftTrigger()
            {
                float ret = 0;
                for (int x = 0; x < 4; x++)
                {
                    if (currentPadState[x].Triggers.Left > float.Epsilon)
                    {
                        ret = currentPadState[x].Triggers.Left;
                        break;
                    }
                }
                return ret;
            }
#if !XBOX360
            //##################################################

            public float DirectionRightPadPc(PlayerIndex player)
            {
                return DirectionRightPadPc((int)player);
            }

            public float DirectionRightPadPc(int player)
            {

                float ret = 0;
                if (sticks != null)
                {
                    ret = MyJoystickSPlayer[player].AxisC - 32767.0f;
                    if (ret < 2000)
                        ret = 0;
                } 
                return ret;
            }

            public float DirectionRightPadPc()
            {
                float ret = 0;
                if (sticks != null)
                {
                    for (int x = 0; x < sticks.Length; x++)
                    {
                        ret = MyJoystickSPlayer[x].AxisC - 32767.0f;
                        if (ret < 2000)
                            ret = 0;
                        break;
                    }
                }
                return ret;
            }

            //#################################################

            public float DirectionLeftPadPc(PlayerIndex player)
            {
                return DirectionLeftPadPc((int)player);
            }

            public float DirectionLeftPadPc(int player)
            {
                float ret = 0;
                if (sticks != null)
                {
                    ret = MyJoystickSPlayer[player].AxisC - 32767.0f;
                    if (ret > -2000)
                        ret = 0;
                }
                return ret;
            }

            public float DirectionLeftPadPc()
            {
                float ret = 0;
                if (sticks != null)
                {
                    for (int x = 0; x < sticks.Length; x++)
                    {
                        ret = MyJoystickSPlayer[x].AxisC - 32767.0f;
                        if (ret > -2000)
                            ret = 0;
                        break;
                    }
                }
                return ret;
            }

            //######################################################

            public float DirectionLeftRightPadPc(PlayerIndex player)
            {
                return DirectionLeftRightPadPc((int)player);
            }

            public float DirectionLeftRightPadPc(int player)
            {
                float ret = 0;
                if (sticks != null)
                {
                    ret = MyJoystickSPlayer[player].AxisC - 32767.0f;
                    if (ret < 2000 || ret < -2000)
                        ret = 0;
                }
                return ret;
            }

            public float DirectionLeftRightPadPc()
            {
                float ret = 0;
                if (sticks != null)
                {
                    for (int x = 0; x < sticks.Length; x++)
                    {
                        ret = MyJoystickSPlayer[x].AxisC - 32767.0f;
                        if (ret < 2000 || ret < -2000)
                            ret = 0;
                        break;
                    }
                }
                return ret;
            }


            //#################################################  

            public float DirectionUpPadPc()
            {
                float ret = 0;
                if (sticks != null)
                {
                    for (int x = 0; x < sticks.Length; x++)
                    {
                        ret = MyJoystickSPlayer[x].AxisD - 32767.0f;
                        if (ret > -2000)
                            ret = 0;
                        break;
                    }
                }
                return -ret;
            }

            public float DirectionUpPadPc(int player)
            {
                float ret = 0;
                if (sticks != null)
                {
                    ret = MyJoystickSPlayer[player].AxisD - 32767.0f;
                    if (ret > -2000)
                        ret = 0;
                }
                return -ret;
            }

            public float DirectionUpPadPc(PlayerIndex player)
            {
                return DirectionUpPadPc((int)player);
            }

            //##################################################

            public float DirectionDownPadPc()
            {
                float ret = 0;
                if (sticks != null)
                {
                    for (int x = 0; x < sticks.Length; x++)
                    {
                        ret = MyJoystickSPlayer[x].AxisD - 32767.0f;
                        if (ret < 2000)
                            ret = 0;
                        break;
                    }
                }
                return -ret;
            }

            public float DirectionDownPadPc(int player)
            {
                float ret = 0;
                if (sticks != null)
                {
                    ret = MyJoystickSPlayer[player].AxisD - 32767.0f;
                    if (ret < 2000)
                        ret = 0;
                }
                return -ret;
            }

            public float DirectionDownPadPc(PlayerIndex player)
            {
                return DirectionDownPadPc((int)player);
            }

            //#####################################################

            public float DirectionUpDownPadPc(PlayerIndex player)
            {
                return DirectionUpDownPadPc((int)player);
            }

            public float DirectionUpDownPadPc(int player)
            {
                float ret = 0;
                if (sticks != null)
                {
                    ret = MyJoystickSPlayer[player].AxisD - 32767.0f;
                    if (ret < 2000 || ret < -2000)
                        ret = 0;
                }
                return -ret;
            }

            public float DirectionUpDownPadPc()
            {
                float ret = 0;
                if (sticks != null)
                {
                    for (int x = 0; x < sticks.Length; x++)
                    {
                        ret = MyJoystickSPlayer[x].AxisD - 32767.0f;
                        if (ret < 2000 || ret < -2000)
                            ret = 0;
                        break;
                    }
                }
                return -ret;
            }

            //#####################################################
            
            public Vector2 DirectionPadPc()
            {
                Vector2 ret = new Vector2(DirectionLeftRightPadPc(), DirectionUpDownPadPc());
                ret.Normalize();
                return ret;
            }

            public Vector2 DirectionPadPc(int player)
            {
                Vector2 ret = new Vector2(DirectionLeftRightPadPc(player), DirectionUpDownPadPc(player));
                ret.Normalize();
                return ret;
            }

            public Vector2 DirectionPadPc(PlayerIndex player)
            {
                Vector2 ret = new Vector2(DirectionLeftRightPadPc((int)player), DirectionUpDownPadPc((int)player));
                ret.Normalize();
                return ret;
            }

            //#######################################################

            // Latest Mouse Position as Vector2.
            private Vector2 mousePosition;
            public Vector2 MousePosition
            {
                get { return mousePosition; }
                set 
                { 
                    mousePosition = value;
                    Mouse.SetPosition((int)mousePosition.X, (int)mousePosition.Y);
                    
                    Utility.SetPrivateMemberValueType<MouseState,int>(
                        ref currentMouseState, "x", (int)mousePosition.X);
                    Utility.SetPrivateMemberValueType<MouseState,int>(
                        ref currentMouseState, "y", (int)mousePosition.Y);
                    Utility.SetPrivateMemberValueType<MouseState, int>(
                        ref previousMouseState, "x", (int)mousePosition.X);
                    Utility.SetPrivateMemberValueType<MouseState, int>(
                        ref previousMouseState, "y", (int)mousePosition.Y);
                }
            }

            Vector2 mousePositionDelta = Vector2.Zero;
            public Vector2 MousePositionDelta
            {
                get
                {
                    mousePositionDelta.X = (float)currentMouseState.X - (float)previousMouseState.X;
                    mousePositionDelta.Y = (float)currentMouseState.Y - (float)previousMouseState.Y;

                    return mousePositionDelta;
                }
            }

            // Normalized Delta ScrollWheelValue.
            public float ScrollWheelDelta
            {
                get
                {
                    return MathHelper.Clamp(
                        (currentMouseState.ScrollWheelValue -
                         previousMouseState.ScrollWheelValue),
                         -1,
                         1);
                }
            }

            // Newly Pressed Button
            public bool IsMouseButtonPress(MouseButtons mb)
            {
                return (IsMouseButtonDown(mb) &&
                    IsMouseButtonState(mb, ButtonState.Released,
                        ref previousMouseState));
            }

            // Newly Released Button
            public bool IsMouseButtonRelease(MouseButtons mb)
            {
                return (IsMouseButtonUp(mb) &&
                    IsMouseButtonState(mb, ButtonState.Pressed,
                        ref previousMouseState));
            }

            // Continuously Pressed Button.
            public bool IsMouseButtonDown(MouseButtons mb)
            {
                return IsMouseButtonState(mb, ButtonState.Pressed,
                    ref currentMouseState);
            }

            // Continuously Released Button.
            public bool IsMouseButtonUp(MouseButtons mb)
            {
                return IsMouseButtonState(mb, ButtonState.Released,
                    ref currentMouseState);
            }

            // Base method
            private bool IsMouseButtonState(MouseButtons mb,
                ButtonState state, ref MouseState mouseState)
            {
                switch (mb)
                {
                    case MouseButtons.LeftButton:
                        return (mouseState.LeftButton == state);
                    case MouseButtons.MiddleButton:
                        return (mouseState.MiddleButton == state);
                    case MouseButtons.RightButton:
                        return (mouseState.RightButton == state);
                    case MouseButtons.XButton1:
                        return (mouseState.XButton1 == state);
                    case MouseButtons.XButton2:
                        return (mouseState.XButton2 == state);
                }
                return false;
            }

            public bool IsButtonJoYstickPCDown(int nButton)
            {
                if (sticks != null)
                {
                    for (int x = 0; x < sticks.Length; x++)
                    {
                        if (MyJoystickSPlayer[x].Buttons[nButton])
                            return true;
                    }
                }
                return false;
            }


            public bool IsButtonJoYstickPCDown(PlayerIndex player, int nButton)
            {
                return IsButtonJoYstickPCDown((int)player,nButton);
            }

            public bool IsButtonJoYstickPCDown(int player, int nButton)
            {
                if (sticks != null && (int)player < sticks.Length)
                    return MyJoystickSPlayer[player].Buttons[nButton];
                else
                    return false;
            }
#endif

            // Newly Pressed Key
            public bool IsKeyPress(Keys key)
            {
                return (currentKeyState.IsKeyDown(key) &&
                        previousKeyState.IsKeyUp(key));
            }

            // Newly Released Key
            public bool IsKeyRelease(Keys key)
            {
                return (currentKeyState.IsKeyUp(key) &&
                        previousKeyState.IsKeyDown(key));
            }

            // Continuously Pressed Key
            public bool IsKeyDown(Keys key)
            {
                return currentKeyState.IsKeyDown(key);
            }

            // Continuously Released Key
            public bool IsKeyUp(Keys key)
            {
                return currentKeyState.IsKeyUp(key);
            }

        }
 
}