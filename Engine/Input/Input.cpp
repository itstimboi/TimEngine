#include "Input.h"

#include <cstring>

namespace Input
{
    static GLFWwindow* g_Window = nullptr;

    static GLFWgamepadstate g_CurrentGamepadState[GLFW_JOYSTICK_LAST + 1]{};
    static GLFWgamepadstate g_PreviousGamepadState[GLFW_JOYSTICK_LAST + 1]{};

    static bool g_GamepadValid[GLFW_JOYSTICK_LAST + 1]{};
    static bool g_PreviousGamepadValid[GLFW_JOYSTICK_LAST + 1]{};


    // ============================================================
    // Initialization
    // ============================================================

    void Init(GLFWwindow* window)
    {
        g_Window = window;

        std::memset(
            g_CurrentGamepadState,
            0,
            sizeof(g_CurrentGamepadState)
        );

        std::memset(
            g_PreviousGamepadState,
            0,
            sizeof(g_PreviousGamepadState)
        );

        std::memset(
            g_GamepadValid,
            0,
            sizeof(g_GamepadValid)
        );

        std::memset(
            g_PreviousGamepadValid,
            0,
            sizeof(g_PreviousGamepadValid)
        );
    }


    // ============================================================
    // Update
    // ============================================================

    void Update()
    {
        if (!g_Window)
            return;


        // --------------------------------------------------------
        // Save previous gamepad state
        // --------------------------------------------------------

        for (int i = 0; i <= GLFW_JOYSTICK_LAST; ++i)
        {
            g_PreviousGamepadState[i] =
                g_CurrentGamepadState[i];

            g_PreviousGamepadValid[i] =
                g_GamepadValid[i];
        }


        // --------------------------------------------------------
        // Get current gamepad states
        // --------------------------------------------------------

        for (int i = 0; i <= GLFW_JOYSTICK_LAST; ++i)
        {
            g_GamepadValid[i] =
                glfwGetGamepadState(
                    i,
                    &g_CurrentGamepadState[i]
                );
        }
    }


    // ============================================================
    // Keyboard
    // ============================================================

    bool IsKeyDown(Key key)
    {
        if (!g_Window)
            return false;

        return glfwGetKey(
            g_Window,
            static_cast<int>(key)
        ) == GLFW_PRESS;
    }


    bool IsKeyPressed(Key key)
    {
        if (!g_Window)
            return false;

        static bool previous[GLFW_KEY_LAST + 1]{};

        int keyCode = static_cast<int>(key);

        if (keyCode < 0 || keyCode > GLFW_KEY_LAST)
            return false;

        bool current =
            glfwGetKey(
                g_Window,
                keyCode
            ) == GLFW_PRESS;

        bool pressed =
            current && !previous[keyCode];

        previous[keyCode] = current;

        return pressed;
    }


    bool IsKeyReleased(Key key)
    {
        if (!g_Window)
            return false;

        static bool previous[GLFW_KEY_LAST + 1]{};

        int keyCode = static_cast<int>(key);

        if (keyCode < 0 || keyCode > GLFW_KEY_LAST)
            return false;

        bool current =
            glfwGetKey(
                g_Window,
                keyCode
            ) == GLFW_PRESS;

        bool released =
            !current && previous[keyCode];

        previous[keyCode] = current;

        return released;
    }


    // ============================================================
    // Mouse
    // ============================================================

    bool IsMouseButtonDown(MouseButton button)
    {
        if (!g_Window)
            return false;

        return glfwGetMouseButton(
            g_Window,
            static_cast<int>(button)
        ) == GLFW_PRESS;
    }


    bool IsMouseButtonPressed(MouseButton button)
    {
        static bool previous[GLFW_MOUSE_BUTTON_LAST + 1]{};

        int buttonCode = static_cast<int>(button);

        if (buttonCode < 0 ||
            buttonCode > GLFW_MOUSE_BUTTON_LAST)
        {
            return false;
        }

        bool current =
            IsMouseButtonDown(button);

        bool pressed =
            current && !previous[buttonCode];

        previous[buttonCode] = current;

        return pressed;
    }


    bool IsMouseButtonReleased(MouseButton button)
    {
        static bool previous[GLFW_MOUSE_BUTTON_LAST + 1]{};

        int buttonCode = static_cast<int>(button);

        if (buttonCode < 0 ||
            buttonCode > GLFW_MOUSE_BUTTON_LAST)
        {
            return false;
        }

        bool current =
            IsMouseButtonDown(button);

        bool released =
            !current && previous[buttonCode];

        previous[buttonCode] = current;

        return released;
    }


    double GetMouseX()
    {
        if (!g_Window)
            return 0.0;

        double x;
        double y;

        glfwGetCursorPos(
            g_Window,
            &x,
            &y
        );

        return x;
    }


    double GetMouseY()
    {
        if (!g_Window)
            return 0.0;

        double x;
        double y;

        glfwGetCursorPos(
            g_Window,
            &x,
            &y
        );

        return y;
    }


    void GetMousePosition(double& x, double& y)
    {
        if (!g_Window)
        {
            x = 0.0;
            y = 0.0;
            return;
        }

        glfwGetCursorPos(
            g_Window,
            &x,
            &y
        );
    }


    // ============================================================
    // Gamepad
    // ============================================================

    bool IsGamepadConnected(int gamepad)
    {
        if (gamepad < 0 ||
            gamepad > GLFW_JOYSTICK_LAST)
        {
            return false;
        }

        return glfwJoystickPresent(gamepad) &&
               glfwJoystickIsGamepad(gamepad);
    }


    bool IsGamepadDown(
        GamepadButton button,
        int gamepad
    )
    {
        if (!IsGamepadConnected(gamepad))
            return false;

        int buttonCode =
            static_cast<int>(button);

        if (buttonCode < 0 ||
            buttonCode > GLFW_GAMEPAD_BUTTON_LAST)
        {
            return false;
        }

        return
            g_CurrentGamepadState[gamepad]
                .buttons[buttonCode]
            == GLFW_PRESS;
    }


    bool IsGamepadPressed(
        GamepadButton button,
        int gamepad
    )
    {
        if (!IsGamepadConnected(gamepad))
            return false;

        int buttonCode =
            static_cast<int>(button);

        if (buttonCode < 0 ||
            buttonCode > GLFW_GAMEPAD_BUTTON_LAST)
        {
            return false;
        }

        bool current =
            g_CurrentGamepadState[gamepad]
                .buttons[buttonCode]
            == GLFW_PRESS;

        bool previous =
            g_PreviousGamepadState[gamepad]
                .buttons[buttonCode]
            == GLFW_PRESS;

        return current && !previous;
    }


    bool IsGamepadReleased(
        GamepadButton button,
        int gamepad
    )
    {
        if (!IsGamepadConnected(gamepad))
            return false;

        int buttonCode =
            static_cast<int>(button);

        if (buttonCode < 0 ||
            buttonCode > GLFW_GAMEPAD_BUTTON_LAST)
        {
            return false;
        }

        bool current =
            g_CurrentGamepadState[gamepad]
                .buttons[buttonCode]
            == GLFW_PRESS;

        bool previous =
            g_PreviousGamepadState[gamepad]
                .buttons[buttonCode]
            == GLFW_PRESS;

        return !current && previous;
    }


    float GetGamepadAxis(
        GamepadAxis axis,
        int gamepad
    )
    {
        if (!IsGamepadConnected(gamepad))
            return 0.0f;

        int axisCode =
            static_cast<int>(axis);

        if (axisCode < 0 ||
            axisCode > GLFW_GAMEPAD_AXIS_LAST)
        {
            return 0.0f;
        }

        return
            g_CurrentGamepadState[gamepad]
                .axes[axisCode];
    }


    const char* GetGamepadName(int gamepad)
    {
        if (!IsGamepadConnected(gamepad))
            return nullptr;

        return glfwGetGamepadName(gamepad);
    }
}