#include <iostream>
#include <IrrlichtRender.h>
#include <DebugGUIManager.hpp>

/**
 * @brief Construct a new Irrlicht Render:: Irrlicht Render object
 * 	The procces is to create a irrlichtDevice and a videoDriver pointers, because they are
 * 	essential for the irrlicht graphic engine to work. 
 * 	It also creates the scene manager to pass the reference to the SceneManager object, who is in charge of directing
 * 	and storaging  all the operations realted to the scene nodes (translations, rotations, scaling...)
 */
IrrlichtRender::IrrlichtRender(int width, int height, bool fullscreen, InputIrrlicht* inputIrrlicht)
{
	m_irrDevice_p = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(width, height), 16, fullscreen, false, false, inputIrrlicht);
	m_screenHeight = height;
	m_screenWidth = width;
	if(!m_irrDevice_p)
	{
		std::cout<<"[ERROR] - Could not create the Irrlicht device"<<std::endl;
	}
	m_irrDevice_p->setWindowCaption(L"Inspire - Pre-alpha footage");
	m_irrVideoDriver_p = m_irrDevice_p->getVideoDriver();

	scene::ISceneManager * l_sceneManager = m_irrDevice_p->getSceneManager();
	std::cout<<"Direcccion de memoria de ISceneManager: "<<l_sceneManager<<std::endl;
	m_SceneManager_p = std::make_unique<SceneManager>();
	m_SceneManager_p->initPointerValue(l_sceneManager);

	// debugGUI = IrrIMGUI::createIMGUI(m_irrDevice_p, inputIrrlicht);

	core::array<SJoystickInfo> joystickInfo;
	m_irrDevice_p->activateJoysticks(joystickInfo);
}

/**
 * @brief Destroy the Irrlicht Render:: Irrlicht Render object
 * 	It is essential to run this destructor, because it will delete all pointers related to the Graphic Engine
 * 	ONE WAY OF VERIFYING IT IS BY LOOKING AT THE MESSAGE IN THE COMMAND PROMPT
 */
IrrlichtRender::~IrrlichtRender()
{
	
}

void IrrlichtRender::UpdateCameraTarget(unsigned int c_nodeName_s, const glm::vec3& pos, const glm::vec3& newPos)
{
	m_SceneManager_p->UpdateCameraTarget(c_nodeName_s, glmVec2irrVec(pos), glmVec2irrVec(newPos));
}

/**
 * @brief It acts as a class destroyer that not only closes the window, it also frees memory that the engine is handling
 * 
 */
void IrrlichtRender::CloseWindow()
{
	//This three steps have to be followed in order to close the device
	m_irrDevice_p->closeDevice();
	m_irrDevice_p->run();
	m_irrDevice_p->drop();
	// debugGUI->drop();
	std::cout<<"[SUCCESS] - Irrlicht device has been drop correctly"<<std::endl;
}


void IrrlichtRender::CleanScene()
{
	m_SceneManager_p->CleanSceneTrees();
}

/**
 * @brief Begins the scene rendering proccess
 * 
 */
void IrrlichtRender::BeginScene()
{
	/*
	 * It starts a new scene with:
	 * 		Back Buffer enabled.
	 * 		Z-Buffer enabled
	 * 		A blue background color (from the irrlicht example)
	 */
	m_irrVideoDriver_p->beginScene(true, true, video::SColor(255,113,113,133));
	// debugGUI->startGUI();
}

/*
 * @brief 
 * 		Ends the scene rendering proccess
 */
void IrrlichtRender::EndScene()
{
	// debugGUI->drawAll();
	m_irrVideoDriver_p->endScene();
}

/**
 * @brief 
 * 		Calls the SceneManager to render all 3D objects (Not debug ones)
 */

void IrrlichtRender::Render()
{
	m_SceneManager_p->RenderObjects();
}


/**
 * @brief 
 * 
 * @return true if the window is openeed
 * @return false if the window is closed
 */
bool IrrlichtRender::IsWindowOpened()
{
	return m_irrDevice_p->run();
}

/**
 * @brief Creates a new CUBE node in our nodes tree
 * 
 * @param c_nodeName_s - Identity of the node in the node tree (Entity ID)
 * @param c_initial_position_vec3 - First position of the node in the 3D scene
 * 
 * @return true  - If there was no errors
 * @return false - If there were errors
 * 
 */
bool IrrlichtRender::AddNode(unsigned int c_nodeName_s, const glm::vec3& c_initial_position_vec3)
{
	return m_SceneManager_p->AddNodeObject(c_nodeName_s,glmVec2irrVec(c_initial_position_vec3));
}

/**
 * @brief Creates a MESH node in our nodes tree
 * 
 * @param c_nodeName_s 				- Identity of the node in the node tree (Entity ID)
 * @param c_initial_position_vec3 	- First position of the node in the 3D scene
 * @param route 					- Route of the 3D object file to be read
 * 
 * @return true   - If there was no errors
 * @return false  - If there were no errors
 */
bool IrrlichtRender::AddMesh(unsigned int c_nodeName_s, const glm::vec3& c_initial_position_vec3, const std::string& route, int c_detailed)
{
	return m_SceneManager_p->AddMeshObject(c_nodeName_s,glmVec2irrVec(c_initial_position_vec3),route);
}

/**
 * @brief Adds a new camera node to the scene tree
 * 
 * @param c_cameraName_s 			- Identity of the camera in the node tree (Entity ID, because it's a camera component)
 * @param c_cameraPosition_vec3 	- First position of the camera in the node tree
 * @param c_cameraLookAt_vec3 		- Where does the camera look in the creation 
 * @return true 	- If there was no errors
 * @return false 	- If there were errors
 */
bool IrrlichtRender::AddCamera(unsigned int c_cameraName_s, const glm::vec3& c_cameraPosition_vec3, const glm::vec3& c_cameraLookAt_vec3)
{
	return m_SceneManager_p->AddCamera(c_cameraName_s, glmVec2irrVec(c_cameraPosition_vec3), glmVec2irrVec(c_cameraLookAt_vec3));
}

/**
 * @brief Adds a new DEBUG node to our scene tree
 * 
 * @param c_nodeName_s 				- Name of the node
 * @param id 						- Identification of the node, just to know if it is from the Octree or not
 * @param c_initial_postiion_vec3 	- Initial position of the node
 */
void IrrlichtRender::AddDebugNode(const std::string& c_nodeName_s, unsigned int id, const glm::vec3& c_initial_postiion_vec3)
{
	// m_SceneManager_p->AddDebugObject(c_nodeName_s, id, glmVec2irrVec(c_initial_postiion_vec3));
}

/**
 * @brief Scales a DEBUG node given a scale factor
 * 
 * @param c_nodeName_s   	- Identifier of the debug node
 * @param c_scaleFactor_f	- Scale factor to be resized 
 */
void IrrlichtRender::ScaleDebugNode(unsigned int c_nodeName_s, float c_scaleFactor_f)
{
	// m_SceneManager_p->ScaleDebugNode(c_nodeName_s,c_scaleFactor_f);
}

/**
 * @brief Creates a DEBUG sphere, it is handled apart due it's parameters
 * 
 * @param id  	- ID of the node in the scene tree
 * @param pos 	- Position of the node	
 * @param rad   - Radius of the sfere of the node
 */
void IrrlichtRender::AddDebugSphere(unsigned int c_id_ui, const glm::vec3& c_pos_v3, float c_rad_f)
{
	// m_SceneManager_p->AddDebugSphere(c_id_ui, glmVec2irrVec(c_pos_v3), c_rad_f);
}

/**
 * @brief Translates a DEBUG node from the scene tree
 * 
 * @param id 	- Identificator of the scene node
 * @param pos 	- New position of the scene node 
 */
void IrrlichtRender::TranslateDebugSceneNode(unsigned int c_id_ui, const glm::vec3& c_pos_vec3)
{
    // m_SceneManager_p->TranslateDebugSceneNode(c_id_ui, glmVec2irrVec(c_pos_vec3));
}

/**
 * @brief Translates a scene node 
 * 
 * @param c_nodeName_s  			- Identificator of the scene node
 * @param c_nodeNewPosition_vec3 	- New position of the scene node
 */
void IrrlichtRender::TranslateSceneNode(unsigned int c_nodeName_s, const glm::vec3& c_nodeNewPosition_vec3)
{
	m_SceneManager_p->MoveNode(c_nodeName_s, glmVec2irrVec(c_nodeNewPosition_vec3));
}

/**
 * @brief Rotates a scene node 
 * 
 * @param c_nodeName_s 				- Indentificator of the scene node
 * @param c_nodeNewRotation_vec3    - New rotation of the scene node
 */
void IrrlichtRender::RotateSceneNode(unsigned int c_nodeName_s, const glm::vec3& c_nodeNewRotation_vec3)
{
	m_SceneManager_p->RotateNode(c_nodeName_s, glmVec2irrVec(c_nodeNewRotation_vec3));
}

/**
 * @brief Scales a scene node due a scale factor
 * 
 * @param c_nodeName_s 				- Identificator of the scene node
 * @param scaleFactor 				- Scale factor of the scene node
 */
void IrrlichtRender::ScaleSceneNode(unsigned int c_nodeName_s, float scaleFactor)
{
	m_SceneManager_p->ScaleNode(c_nodeName_s, scaleFactor);
}

/**
 * @brief Adds a debug cube to the scene tree
 * 
 * @param c_name_s 
 * @param c_initialPosition_v3dirr 
 * @param c_initialRotation_v3dirr 
 * @param c_initialScale_v3dirr 
 */
void IrrlichtRender::AddDebugCube(unsigned int c_name_s, const glm::vec3& c_initialPosition_v3dirr, const glm::vec3& c_initialRotation_v3dirr, const glm::vec3& c_initialScale_v3dirr)
{
	// m_SceneManager_p->AddDebugCube(c_name_s, glmVec2irrVec(c_initialPosition_v3dirr), glmVec2irrVec(c_initialRotation_v3dirr), glmVec2irrVec(c_initialScale_v3dirr));
}

/**
 * @brief Draws a debug bounding box in the screeen
 * 
 * @param c_nodeName_s 
 */
void IrrlichtRender::DrawBBOX(unsigned int c_nodeName_s)
{
	// m_SceneManager_p->DrawBBOX(c_nodeName_s);
}

/**
 * @brief Draws a 2D line on the screen
 * 
 * @param c_origin_v3 
 * @param c_destiny_v3 
 * @param color 
 */
void IrrlichtRender::DebugDraw2DLine(const glm::vec3& c_origin_v3, const glm::vec3& c_destiny_v3, unsigned int color)
{
	// video::SColor l_color;

	// if(color == 0)
	// 	l_color = video::SColor(255,255,0,0);
	// if(color == 1)
	// 	l_color = video::SColor(255,0,255, 0);
	// if(color == 2)
	// 	l_color = video::SColor(255,0,0, 255);

	// m_irrVideoDriver_p->setTransform(video::ETS_WORLD, core::IdentityMatrix);
	// m_irrVideoDriver_p->draw3DLine(glmVec2irrVec(c_origin_v3), glmVec2irrVec(c_destiny_v3), l_color);
}

/**
 * @brief Draws an X of a certain point on the screen
 * 
 * @param c_point_v3 
 * @param color 
 */
void IrrlichtRender::DebugDraw2DPoint(const glm::vec3& c_point_v3, unsigned int color)
{
	// video::SColor l_color;

	// if(color == 0)
	// 	l_color = video::SColor(255,0,0, 255);
	// if(color == 1)
	// 	l_color = video::SColor(0,255,0, 255);
	// if(color == 2)
	// 	l_color = video::SColor(0,0,255, 255);


	// glm::vec3 Point1 = glm::vec3(c_point_v3.x, c_point_v3.y+5, c_point_v3.z);
	// glm::vec3 Point2 = glm::vec3(c_point_v3.x, c_point_v3.y-5, c_point_v3.z);

	// m_irrVideoDriver_p->setTransform(video::ETS_WORLD, core::IdentityMatrix);
	// m_irrVideoDriver_p->draw3DLine(glmVec2irrVec(Point1), glmVec2irrVec(Point2), l_color);

	// glm::vec3 Point3 = glm::vec3(c_point_v3.x+5, c_point_v3.y, c_point_v3.z);
	// glm::vec3 Point4 = glm::vec3(c_point_v3.x-5, c_point_v3.y, c_point_v3.z);

	// m_irrVideoDriver_p->setTransform(video::ETS_WORLD, core::IdentityMatrix);
	// m_irrVideoDriver_p->draw3DLine(glmVec2irrVec(Point3), glmVec2irrVec(Point4), l_color);
}

/**
 * @brief Converts a GLM vector into an Irrlicht vector
 * 
 * @param c_glmVector_vec3 
 * @return core::vector3df 
 */
core::vector3df IrrlichtRender::glmVec2irrVec(const glm::vec3& c_glmVector_vec3)
{
	return core::vector3df(c_glmVector_vec3.x, c_glmVector_vec3.y, c_glmVector_vec3.z);
	//return core::vector3df(c_glmVector_vec3.x, c_glmVector_vec3.z, c_glmVector_vec3.y);
}

/**
 * @brief Converts a Irrlicht vector into a GLM vector
 * 
 * @param c_irrVector_vec3 
 * @return glm::vec3 
 */
glm::vec3 IrrlichtRender::irrVec2glmVec(const core::vector3df& c_irrVector_vec3)
{
	return glm::vec3(c_irrVector_vec3.X, c_irrVector_vec3.Y, c_irrVector_vec3.Z);
}

/**
 * @brief Adds a texture to a specific scene node (It doesn't depend if it is a mesh or not)
 * 
 * @param c_nodeName_s 
 * @param c_texturePath_s 
 * @return true 
 * @return false 
 */
bool IrrlichtRender::AddTexture(unsigned int c_nodeName_s, const std::string& c_texturePath_s)
{
	io::IReadFile* file = io::createReadFile(c_texturePath_s.c_str());
	video::ITexture* texture = m_irrVideoDriver_p->getTexture(file);
	m_SceneManager_p->AddTexture(c_nodeName_s, texture);
	delete file;
	file = nullptr;
	return true;
}

/**
 * @brief Specific function for the GUI. It will draw a box on the screen. In our case it is for drawing
 * the Nitro bar under the player, it may be disabled.
 * 
 * @param c_xPos 
 * @param c_yPos 
 * @param c_width 
 * @param c_height 
 * @param c_Red 
 * @param c_Blue 
 * @param c_Green 
 */
void IrrlichtRender::GUIDrawBox(int c_xPos, int c_yPos, int c_width, int c_height, int c_Red, int c_Blue, int c_Green)
{
	// core::rect<s32> rectangle = core::rect<s32>(c_xPos, c_yPos, c_xPos+c_width, c_yPos+c_height);
	// video::SColor rectangleColor = video::SColor(255,c_Red, c_Green, c_Blue);

	// m_irrVideoDriver_p->draw2DRectangle(rectangleColor, rectangle);
}

/**
 * @brief Draws a 2D sprite on the screen 
 * 
 * @param c_SpriteRoute 
 * @param c_xPos 
 * @param c_yPos 
 */
void IrrlichtRender::GUIDrawSprite(const std::string& c_SpriteRoute, int c_xPos, int c_yPos)
{
	video::ITexture* l_spriteToDraw = m_irrVideoDriver_p->getTexture(c_SpriteRoute.c_str());
	m_irrVideoDriver_p->draw2DImage(l_spriteToDraw, core::vector2di(c_xPos,c_yPos));
	//delete l_spriteToDraw;
	//l_spriteToDraw = nullptr;
}

/**
 * @brief  Draws a 2D point on the screen
 * 
 * @param c_xPos 
 * @param c_YPos 
 */
void IrrlichtRender::GUIDraw2DPoint(int c_xPos, int c_YPos)
{
	// m_irrVideoDriver_p->draw2DPolygon(core::vector2d<s32>(c_xPos, c_YPos),10, video::SColor(255,0,0,255));
}
