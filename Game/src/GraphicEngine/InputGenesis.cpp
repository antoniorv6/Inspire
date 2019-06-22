/**
 * @file InputGenesis.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-03-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <InputGenesis.hpp>
#include <GenesisEngine.hpp>
#include <GenericData.hpp>
#include <RenderManager.h>
#include <vector>

std::list<InputGenesis*> registeredObjects;
void registerKeyCallback(InputGenesis* objectToRegister);
/**
 * @brief Construct a new Input Genesis:: Input Genesis object
 * 
 */
InputGenesis::InputGenesis() : m_keyStatus_vb()
{
    InitGameKeyCodes();
    InitGamepadAxisCodes();
    InitGamepadButtonCodes();
    registerKeyCallback(this);
}

/**
 * @brief 
 * 
 * @param l_genericData 
 */
void InputGenesis::Init(GenericData* l_genericData_ptr)
{
    // GenesisEngine* l_engine_ptr = reinterpret_cast<GenesisEngine*>(l_genericData_ptr->g_renderManager_ptr->GetRenderFacade());
    // std::vector<int> aux = l_engine_ptr->GetKeys();
    // // m_keyStatus_n = 
}

/**
 * @brief Destroy the Input Genesis:: Input Genesis object
 * 
 */
InputGenesis::~InputGenesis()
{
    m_gameKeyCodes_u32v.clear();
    m_gamepadAxisCodes_u32v.clear();
    m_gamepadButtonCodes_u32v.clear();
}

/**
 * @brief 
 * 
 * @param l_window_ptr 
 * @param l_key_n 
 * @param l_scanCode_n 
 * @param l_action_n 
 * @param mods 
 */
void InputGenesis::OnEvent(GLFWwindow* l_window_ptr, int l_key_n, int l_scanCode_n, int l_action_n, int mods)
{
    // std::cout << "KeyCode: "    << l_key_n      << std::endl;
    // std::cout << "ScanCode: "   << l_scanCode_n << std::endl;
    // std::cout << "Action: "     << l_action_n   << std::endl;
    // //std::cout << "Mods: "       << mods         << std::endl;
    // std::cout << "============================" << std::endl;

    if(l_key_n >= 0 && l_key_n < 1024)
    {
        if(l_action_n == GLFW_PRESS || l_action_n == GLFW_REPEAT)
        {
            m_keyStatus_vb[l_key_n] = true;
        }
        else if(l_action_n == GLFW_RELEASE)
        {
            m_keyStatus_vb[l_key_n] = false;
        }
    }
}

/**
 * @brief 
 * 
 */
void InputGenesis::UpdateGamepadValues()
{
    int last_joystick = -1;

    for(unsigned int joystick = 1; joystick < GLFW_JOYSTICK_LAST; joystick++)
    {
        if(glfwJoystickPresent(joystick))
            last_joystick = joystick;
        else
            break;   
    }

    if(last_joystick != -1)
    {

        int c_axes;
        const float* axes = glfwGetJoystickAxes(last_joystick, &c_axes);

        for(unsigned int i=0; i<sizeof(axes); i++) { m_axisStatus_bv[i] = axes[i]; }

        int c_buttons;
        const unsigned char* buttons = glfwGetJoystickButtons(last_joystick, &c_buttons);

        for(unsigned int i=0; i<c_buttons; i++) { m_buttonsStatus_bv[i] = buttons[i]; }

        //int present = glfwJoystickPresent(last_joystick);
        //const char* name = glfwGetJoystickName(last_joystick);

        //ShowJoystickValues(present, c_axes, axes, c_buttons, buttons, name);  
    }
}


void InputGenesis::ShowJoystickValues(int present, int c_axes, const float* axes, int c_buttons, const unsigned char* buttons, const char* name)
{
    std::cout << "Present: " << present << std::endl;
    if(present == 1)
    {
        std::cout << "Num Axes: " << c_axes << std::endl;

        for(unsigned int i=0; i<sizeof(axes); i++)
        {
            std::cout << "Axes[" << i << "]: " << axes[i] << std::endl;
            m_axisStatus_bv[i] = axes[i];
        }

        std::cout << "Num Buttons: " << c_buttons << std::endl;

        for(unsigned int i=0; i<sizeof(buttons); i++)
        {
            std::cout << "Buttons[" << i << "]: " << (float)buttons[i] << std::endl;
            m_buttonsStatus_bv[i] = buttons[i];
        }
        std::cout << "Name: " << name << std::endl;
    }
    
    std::cout << "==================" << std::endl;
}

//Get when i click or release the mouse
void InputGenesis::On_MouseEvent(GLFWwindow* window, int button, int action, int mods)
{       
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        m_mousePressed_b = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        m_mousePressed_b = false;
    }
}

//Get where the mouse is at any time
void InputGenesis::GetCursorPosition(GLFWwindow* window, double xPos, double yPos)
{
    m_mousePosition_x = xPos;
    m_mousePosition_y = yPos;
}

/**
 * @brief 
 * 
 * @param l_keyCode_u32 
 * @return float 
 */
float InputGenesis::IsKeyPressed(unsigned int l_keyCode_u32) const
{
    return (float)m_keyStatus_vb[l_keyCode_u32];
}

/**
 * @brief 
 * 
 * @param l_buttonCode_u32 
 * @return float 
 */
float InputGenesis::IsButtonPressed(unsigned int l_buttonCode_u32) const
{
    return m_buttonsStatus_bv[l_buttonCode_u32];
}

/**
 * @brief 
 * 
 * @param l_AxisCode_u32 
 * @return float 
 */
float InputGenesis::IsAxisPressed(unsigned int l_AxisCode_u32) const
{
    return m_axisStatus_bv[l_AxisCode_u32];
}

/**
 * @brief 
 * 
 */
void InputGenesis::InitGameKeyCodes()
{
    m_gameKeyCodes_u32v.reserve(12);
    m_gameKeyCodes_u32v.emplace_back(GLFW_KEY_W);
    m_gameKeyCodes_u32v.emplace_back(GLFW_KEY_A);
    m_gameKeyCodes_u32v.emplace_back(GLFW_KEY_S);
    m_gameKeyCodes_u32v.emplace_back(GLFW_KEY_D);
    m_gameKeyCodes_u32v.emplace_back(GLFW_KEY_K);
    m_gameKeyCodes_u32v.emplace_back(GLFW_KEY_I);
    m_gameKeyCodes_u32v.emplace_back(GLFW_KEY_F8);
    m_gameKeyCodes_u32v.emplace_back(GLFW_KEY_SPACE);
    m_gameKeyCodes_u32v.emplace_back(GLFW_KEY_ESCAPE);
    m_gameKeyCodes_u32v.emplace_back(GLFW_KEY_J);
    m_gameKeyCodes_u32v.emplace_back(GLFW_KEY_L);
    m_gameKeyCodes_u32v.emplace_back(GLFW_KEY_H);
}

/**
 * @brief 
 * 
 * @param l_keyI_st 
 */
void InputGenesis::SetUpKeyIdentifiers(KeyIdentifiers& l_keyI_st)
{
    l_keyI_st.W      = 0;
    l_keyI_st.A      = 1;
    l_keyI_st.S      = 2;
    l_keyI_st.D      = 3;
    l_keyI_st.K      = 4;
    l_keyI_st.I      = 5;
    l_keyI_st.F8     = 6;
    l_keyI_st.SPACE  = 7;
    l_keyI_st.ESCAPE = 8;
    l_keyI_st.J      = 9;
    l_keyI_st.L      = 10;
    l_keyI_st.H      = 11;
}

/**
 * @brief 
 * 
 */
void InputGenesis::InitGamepadButtonCodes()
{
    m_gamepadButtonCodes_u32v.reserve(11);
    m_gamepadButtonCodes_u32v.emplace_back(0); // A
    m_gamepadButtonCodes_u32v.emplace_back(1); // B
    m_gamepadButtonCodes_u32v.emplace_back(2); // X
    m_gamepadButtonCodes_u32v.emplace_back(3); // Y
    m_gamepadButtonCodes_u32v.emplace_back(4); // L1
    m_gamepadButtonCodes_u32v.emplace_back(5); // R1
    m_gamepadButtonCodes_u32v.emplace_back(6); // Back
    m_gamepadButtonCodes_u32v.emplace_back(7); // Start
    m_gamepadButtonCodes_u32v.emplace_back(8); // On/Off
    m_gamepadButtonCodes_u32v.emplace_back(9); // L3
    m_gamepadButtonCodes_u32v.emplace_back(10);// R3
}

/**
 * @brief 
 * 
 * @param l_buttonI_st 
 */
void InputGenesis::SetUpButtonIdentifiers(ButtonIdentifiers& l_buttonI_st)
{
    l_buttonI_st.A     = 0;
    l_buttonI_st.B     = 1;
    l_buttonI_st.X     = 2;
    l_buttonI_st.Y     = 3;
    l_buttonI_st.L1    = 4;
    l_buttonI_st.R1    = 5;
    l_buttonI_st.Back  = 6; 
    l_buttonI_st.Start = 7;
    l_buttonI_st.On    = 8;
    l_buttonI_st.L3    = 9;
    l_buttonI_st.R3    = 10;
}

/**
 * @brief 
 * 
 */
void InputGenesis::InitGamepadAxisCodes()
{
    m_gamepadAxisCodes_u32v.reserve(6);
    m_gamepadAxisCodes_u32v.emplace_back(0); // Joystick Left (Left ... Right) (-1 ... 1)
    m_gamepadAxisCodes_u32v.emplace_back(1); // Joystick Left (Up ... Down) (-1 ... 1)
    m_gamepadAxisCodes_u32v.emplace_back(2); // L2 (-1 not pressed) (1 pressed)
    m_gamepadAxisCodes_u32v.emplace_back(3); // Joystick Right (Left ... Right) (-1 ... 1)
    m_gamepadAxisCodes_u32v.emplace_back(4); // Joystick Right (Up ... Down) (-1 ... 1)
    m_gamepadAxisCodes_u32v.emplace_back(5); // R2 (-1 not pressed) (1 pressed)
}

/**
 * @brief 
 * 
 * @param l_buttonI_st 
 */
void InputGenesis::SetUpAxisIdentifiers(AxisIdentifiers& l_axisI_st)
{
    l_axisI_st.LJLR = 0;
    l_axisI_st.LJUD = 1;
    l_axisI_st.L2   = 2;
    l_axisI_st.RJLR = 3;
    l_axisI_st.RJUD = 4;
    l_axisI_st.R2   = 5;
}

/**
 * @brief 
 * 
 * @return std::vector<unsigned int> 
 */
std::vector<unsigned int> InputGenesis::GetGameKeyCodes() const 
{
    return m_gameKeyCodes_u32v;
}

/**
 * @brief 
 * 
 * @return std::vector<unsigned int> 
 */
std::vector<unsigned int> InputGenesis::GetGamepadButtonCodes() const 
{
    return m_gamepadButtonCodes_u32v;
}

/**
 * @brief 
 * 
 * @return std::vector<unsigned int> 
 */
std::vector<unsigned int> InputGenesis::GetGamepadAxisCodes() const 
{
    return m_gamepadAxisCodes_u32v;
}

int InputGenesis::GetMouseXPos()
{
    return m_mousePosition_x;
}
int InputGenesis::GetMouseYPos()
{
    return m_mousePosition_y;
}
bool InputGenesis::IsMousePressed()
{
    return m_mousePressed_b;
}

/**
 * @brief 
 * 
 * @param objectToRegister 
 */
void registerKeyCallback(InputGenesis* objectToRegister)
{
    for (InputGenesis * obj : registeredObjects)
    {
        if (obj == objectToRegister) 
        {
            return; 
        }
    }

    registeredObjects.push_back(objectToRegister);
}


/**
 * @brief 
 * 
 * @param l_window_ptr 
 * @param l_key_n 
 * @param l_scanCode_n 
 * @param l_action_n 
 * @param mods 
 */
void On_Event(GLFWwindow* l_window_ptr, int l_key_n, int l_scanCode_n, int l_action_n, int mods)
{
    for (InputGenesis *registeredObject : registeredObjects)
        registeredObject->OnEvent(l_window_ptr, l_key_n, l_scanCode_n, l_action_n, mods);
}

void On_MouseEvent(GLFWwindow* window, int button, int action, int mods)
{
    for (InputGenesis *registeredObject : registeredObjects)
        registeredObject->On_MouseEvent(window, button, action, mods);
}

void GetCursorPosition(GLFWwindow* window, double xPos, double yPos)
{
    for (InputGenesis *registeredObject : registeredObjects)
        registeredObject->GetCursorPosition(window, xPos, yPos);
}