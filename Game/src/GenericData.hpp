#pragma once

class EntityManager;
class RenderManager;
class SoundManager;
class InputManager;
class PhysicsManager;
class PowerUpManager;
class CheckpointManager;
class MapManager;
class EventManager;
class AIManager;
class CameraManager;
class GUIManager;
class DebugGUIManager;
class PositionManager;
class WaypointManager;
class LapManager;

struct GenericData{

   GenericData(EntityManager *&l_entityManager, RenderManager *&l_renderManager, SoundManager *&l_soundManager, InputManager *&l_inputManager,
               PhysicsManager *&l_physicsManager, PowerUpManager *&l_powerUpManager, CheckpointManager *&l_checkpointManager, MapManager *&l_mapManager,
               EventManager *&l_eventManager_ptr, AIManager *&l_aiManager_ptr, CameraManager *&l_cameraManager_ptr,GUIManager *&l_guiManager_ptr,
               DebugGUIManager *&l_debugGuiManager_ptr, PositionManager *&l_positionManager_ptr, WaypointManager *&l_waypointManager_ptr)
   {
      g_entityManager_ptr  =  l_entityManager;
      g_inputManager_ptr   =  l_inputManager;
      g_soundManager_ptr   =  l_soundManager;
      g_renderManager_ptr  =  l_renderManager;
      g_physicsManager_ptr =  l_physicsManager;
      g_powerUpManager_ptr =  l_powerUpManager;
      g_checkPointManager_ptr = l_checkpointManager;
      g_mapManager_ptr     = l_mapManager;
      g_eventManager_ptr   = l_eventManager_ptr;
      g_aiManager_ptr      = l_aiManager_ptr;
      g_cameraManager_ptr  = l_cameraManager_ptr;
      g_guiManager_ptr     = l_guiManager_ptr;
      g_debugGuiManager_ptr= l_debugGuiManager_ptr;
      g_positionManager_ptr= l_positionManager_ptr;
      g_waypointManager_ptr= l_waypointManager_ptr;
   }

   GenericData(EntityManager *&l_entityManager, EventManager *&l_eventManager, PhysicsManager *&l_physicsManager, 
    MapManager *&l_mapManager, PositionManager *&l_positionManager, CheckpointManager *&l_checkpointManager, PowerUpManager *&l_powerupmanager)
   {
      g_entityManager_ptr  =  l_entityManager;
      g_inputManager_ptr   =  nullptr;
      g_soundManager_ptr   =  nullptr;
      g_renderManager_ptr  =  nullptr;
      g_physicsManager_ptr =  l_physicsManager;
      g_powerUpManager_ptr =  l_powerupmanager;
      g_checkPointManager_ptr = l_checkpointManager;
      g_mapManager_ptr     = l_mapManager;
      g_eventManager_ptr   = l_eventManager;
      g_aiManager_ptr      = nullptr;
      g_cameraManager_ptr  = nullptr;
      g_guiManager_ptr     = nullptr;
      g_debugGuiManager_ptr= nullptr;
      g_positionManager_ptr= l_positionManager;
      g_waypointManager_ptr= nullptr;
   };

  ~GenericData(){ };
    
   EntityManager*     g_entityManager_ptr;
   RenderManager*     g_renderManager_ptr;
   SoundManager*      g_soundManager_ptr;
   InputManager*      g_inputManager_ptr;
   PhysicsManager*    g_physicsManager_ptr;
   PowerUpManager*    g_powerUpManager_ptr;
   CheckpointManager* g_checkPointManager_ptr;
   MapManager*        g_mapManager_ptr;
   EventManager*      g_eventManager_ptr;
   AIManager*         g_aiManager_ptr;
   CameraManager*     g_cameraManager_ptr;
   GUIManager*        g_guiManager_ptr;
   DebugGUIManager*   g_debugGuiManager_ptr;
   PositionManager*   g_positionManager_ptr;
   WaypointManager*   g_waypointManager_ptr;
};