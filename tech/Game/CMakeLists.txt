# library name
set(TARGET Game)

# library source files
set(SOURCES
    Avatar.cpp
    Avatar.h
    BeamWeapon.cpp
    BeamWeapon.h
    CameraController.cpp
    CameraController.h
    ChaseCamController.cpp
    ChaseCamController.h
    Command.h
    CommandTarget.cpp
    CommandTarget.h
    Component.cpp
    Component.h
    Component_Animation.cpp
    Component_Animation.h
    Component_Audio.cpp
    Component_Audio.h
    Component_BasicPhysics.cpp
    Component_BasicPhysics.h
    Component_CameraInterface.cpp
    Component_CameraInterface.h
    Component_CharacterPhysics.cpp
    Component_CharacterPhysics.h
    Component_EquipmentSlot.cpp
    Component_EquipmentSlot.h
    Component_Interpolator.cpp
    Component_Interpolator.h
    Component_Physics.cpp
    Component_Physics.h
    Component_Render.cpp
    Component_Render.h
    ComponentHost.cpp
    ComponentHost.h
    CountdownTrigger.cpp
    CountdownTrigger.h
    DebugMeshRenderStep.cpp
    DebugMeshRenderStep.h
    DepthOnlyRenderStep.cpp
    DepthOnlyRenderStep.h
    DirectionalLight.cpp
    DirectionalLight.h
    DynamicLight.cpp
    DynamicLight.h
    FirstPersonCamController.cpp
    FirstPersonCamController.h
    FreeCamController.cpp
    FreeCamController.h
    GameState.cpp
    GameState.h
    GameStateManager.cpp
    GameStateManager.h
    IFF.h
    InputFilter.cpp
    InputFilter.h
    Kinematic.h
    LandscapeScene.cpp
    LandscapeScene.h
    LoadListener.h
    MACRO.cpp
    MACRO.h
    Manifest.cpp
    Manifest.h
    Mountable.cpp
    Mountable.h
    ObjectLibrary.cpp
    ObjectLibrary.h
    OrbitCamController.cpp
    OrbitCamController.h
    Overlay.cpp
    Overlay.h
    Player.cpp
    Player.h
    PlayerController.h
    PlayerInput.h
    PointLight.cpp
    PointLight.h
    Prop.cpp
    Prop.h
    RenderStep.cpp
    RenderStep.h
    Scene.cpp
    Scene.h
    SceneRenderer.cpp
    SceneRenderer.h
    SceneRenderStep.cpp
    SceneRenderStep.h
    ShadowRenderStep.cpp
    ShadowRenderStep.h
    SpatialPartitioning.h
    SpawnPoint.cpp
    SpawnPoint.h
    SpotLight.cpp
    SpotLight.h
    TerrainPatch.cpp
    TerrainPatch.h
    TerrainZone.cpp
    TerrainZone.h
    Trigger.cpp
    Trigger.h
    Variant.cpp
    Variant.h
    Weapon.cpp
    Weapon.h
    Zone.cpp
    Zone.h
    ZoneObject.cpp
    ZoneObject.h
    ZoneObjects.h
)

# create static library with standard build settings
td_add_library(${TARGET}
    LIBRARY_TYPE
        STATIC
    SOURCES
        ${SOURCES}
)
