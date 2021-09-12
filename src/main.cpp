#include "main.hpp"
#include "GlobalNamespace/BeatmapObjectSpawnMovementData.hpp"
#include "GlobalNamespace/MainMenuViewController.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "UnityEngine/GameObject.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "HMUI/Touchable.hpp"
#include "questui/shared/QuestUI.hpp"
#include "config-utils/shared/config-utils.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "ModConfig.hpp"
#include "GlobalNamespace/NoteJump.hpp"
#include "GlobalNamespace/NoteController.hpp"
#include "GlobalNamespace/ObstacleController.hpp"
#include "GlobalNamespace/ObstacleData.hpp"
#include "GlobalNamespace/NoteData.hpp"
#include "GlobalNamespace/ColorType.hpp"
#include "System/Action.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <ctime>

using namespace QuestUI;
using namespace UnityEngine;
using namespace GlobalNamespace;

float i = 0.0f;

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup
DEFINE_CONFIG(ModConfig);

// Loads the config from disk using our modInfo, then returns it for use
Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

MAKE_HOOK_MATCH(ObstacleController_Init, &ObstacleController::Init, void, 
    ObstacleController* self,
    ObstacleData* ObstacleData,
    float worldRotation,
    UnityEngine::Vector3 startPos,
    UnityEngine::Vector3 midPos,
    UnityEngine::Vector3 endPos,
    float move1Duration,
    float move2Duration,
    float singleLineWidth,
    float height
) {
    if(getModConfig().Active.GetValue()){
        srand((unsigned) time(0));

        if(getModConfig().ThreeSixtyMode.GetValue()){
            i = (rand() % 20) - 10.0f;

            worldRotation = i;
        }

        if(getModConfig().RandomHeight.GetValue()){
            height = (rand() % 10);
        }

        if(getModConfig().RanWallStartPos.GetValue()){
            startPos = Vector3((rand() % 20) - 10, (rand() % 20) - 10, (rand() % 20) - 10);
        }

        if(getModConfig().RanWallMidPos.GetValue()){
            midPos = Vector3((rand() % 20) - 10, (rand() % 20) - 10, (rand() % 20) - 10);
        }

        if(getModConfig().RanWallEndPos.GetValue()){
            endPos = Vector3((rand() % 20) - 10, (rand() % 20) - 10, (rand() % 20) - 10);
        }
    }

    ObstacleController_Init(self, ObstacleData, worldRotation, startPos, midPos, endPos, move1Duration, move2Duration, singleLineWidth, height);
};

MAKE_HOOK_MATCH(NoteController_Init, &NoteController::Init, void, 
    NoteController* self,
    NoteData* noteData,
    float worldRotation,
    UnityEngine::Vector3 moveStartPos,
    UnityEngine::Vector3 moveEndPos,
    UnityEngine::Vector3 jumpEndPos,
    float moveDuration,
    float jumpDuration,
    float jumpGravity,
    float endRotation,
    float uniformScale
) {
    if(getModConfig().Active.GetValue()){
        srand((unsigned) time(0));

        if(getModConfig().ColourSwap.GetValue()){
            if(noteData->get_colorType() == ColorType::_get_ColorA()){
                noteData->set_colorType(ColorType::_get_ColorB());
            } else{
                noteData->set_colorType(ColorType::_get_ColorA());
            }
        }

        if(getModConfig().ThreeSixtyMode.GetValue()){
            i = (rand() % 20) - 10.0f;

            worldRotation = i;
        }
        
        if(getModConfig().RanStartPos.GetValue()){
            moveStartPos = Vector3((rand() % 20) - 10, (rand() % 20) - 10, (rand() % 20) - 10);
        }

        if(getModConfig().RanGravity.GetValue()){
            jumpGravity = (rand() % 4);
        }
    }

    NoteController_Init(self, noteData, worldRotation, moveStartPos, moveEndPos, jumpEndPos, moveDuration, jumpDuration, jumpGravity, endRotation, uniformScale);
};

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load(); // Load the config file
    getLogger().info("Completed setup!");
}

void DidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling){
    if(firstActivation){
        GameObject* container = BeatSaberUI::CreateScrollableSettingsContainer(self->get_transform());

        BeatSaberUI::CreateToggle(container->get_transform(), "Active", getModConfig().Active.GetValue(),
            [](bool value) { 
                getModConfig().Active.SetValue(value);
            });

        BeatSaberUI::CreateToggle(container->get_transform(), "360 Mode", getModConfig().ThreeSixtyMode.GetValue(),
            [](bool value) { 
                getModConfig().ThreeSixtyMode.SetValue(value);
            });

        BeatSaberUI::CreateToggle(container->get_transform(), "Random Jump Start Position", getModConfig().RanStartPos.GetValue(),
            [](bool value) { 
                getModConfig().RanStartPos.SetValue(value);
            });

        BeatSaberUI::CreateToggle(container->get_transform(), "Random Jump Gravity", getModConfig().RanGravity.GetValue(),
            [](bool value) { 
                getModConfig().RanGravity.SetValue(value);
            });

        BeatSaberUI::CreateToggle(container->get_transform(), "Random Wall height", getModConfig().RandomHeight.GetValue(),
            [](bool value) { 
                getModConfig().RandomHeight.SetValue(value);
            });

        BeatSaberUI::CreateToggle(container->get_transform(), "Random Wall start position", getModConfig().RanWallStartPos.GetValue(),
            [](bool value) { 
                getModConfig().RanWallStartPos.SetValue(value);
            });

        BeatSaberUI::CreateToggle(container->get_transform(), "Random Wall mid position", getModConfig().RanWallMidPos.GetValue(),
            [](bool value) { 
                getModConfig().RanWallMidPos.SetValue(value);
            });

        BeatSaberUI::CreateToggle(container->get_transform(), "Random Wall end position", getModConfig().RanWallEndPos.GetValue(),
            [](bool value) { 
                getModConfig().RanWallEndPos.SetValue(value);
            });

        BeatSaberUI::CreateToggle(container->get_transform(), "Note Colour Swap", getModConfig().ColourSwap.GetValue(),
            [](bool value) { 
                getModConfig().ColourSwap.SetValue(value);
            });
    }
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();
    getModConfig().Init(modInfo);

    LoggerContextObject logger = getLogger().WithContext("load");

    QuestUI::Init();
    QuestUI::Register::RegisterModSettingsViewController(modInfo, DidActivate);
    QuestUI::Register::RegisterMainMenuModSettingsViewController(modInfo, DidActivate);
    getLogger().info("Successfully installed Settings UI!");

    getLogger().info("Installing hooks...");
    //INSTALL_HOOK(logger, NoteJump_Init);
    INSTALL_HOOK(logger, NoteController_Init);
    INSTALL_HOOK(logger, ObstacleController_Init);
    getLogger().info("Installed all hooks!");
}