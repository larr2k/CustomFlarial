#pragma once


#include "../Client/Module/Modules/Module.hpp"
#include "lua.h"
#include "Scripting.hpp"
#include "../Client/Events/EventManager.hpp"
#include "EventManager/ScriptingEventManager.hpp"

class ScriptModuleBase : public Module {


public:
    lua_State* module_lua_state;
    ScriptModuleBase(std::string name, std::string description, lua_State* lua_state) : Module(name, description, IDR_TIME_PNG, "", true) {
        this->module_lua_state = lua_state;
        Module::setup();
        Listen(this, KeyEvent, &ScriptModuleBase::onKey)
        Listen(this, MouseEvent, &ScriptModuleBase::onMouse)
        Listen(this, PacketEvent, &ScriptModuleBase::onPacketReceive)
        Listen(this, TickEvent, &ScriptModuleBase::onTickEvent)
        Listen(this, RenderEvent, &ScriptModuleBase::onRenderEvent)

    };

    void terminate() override {
        Deafen(this, KeyEvent, &ScriptModuleBase::onKey)
        Deafen(this, MouseEvent, &ScriptModuleBase::onMouse)
        Deafen(this, PacketEvent, &ScriptModuleBase::onPacketReceive)
        Deafen(this, TickEvent, &ScriptModuleBase::onTickEvent)
        Module::terminate();
    }

    void onEnable() override {
        Scripting::executeFunction(module_lua_state, "onEnable", true);
        Module::onEnable();

    }

    void onDisable() override {
        Scripting::executeFunction(module_lua_state, "onDisable", true);
        Module::onDisable();
    }

    void defaultConfig() override {
        Scripting::executeFunction(module_lua_state, "defaultConfig", false);
    };
    void settingsRender(float settingsOffset) override {
        float x = Constraints::PercentageConstraint(0.019, "left");
        float y = Constraints::PercentageConstraint(0.10, "top");

        const float scrollviewWidth = Constraints::RelativeConstraint(0.5, "height", true);


        FlarialGUI::ScrollBar(x, y, 140, Constraints::SpacingConstraint(5.5, scrollviewWidth), 2);
        FlarialGUI::SetScrollView(x - settingsOffset, Constraints::PercentageConstraint(0.00, "top"),
                                  Constraints::RelativeConstraint(1.0, "width"),
                                  Constraints::RelativeConstraint(0.88f, "height"));
        Scripting::executeFunction(module_lua_state, "settingsRender", false);

        FlarialGUI::UnsetScrollView();
        this->resetPadding();
    };

    void onKey(KeyEvent &event) {
        if(!Scripting::instalized) return;
        if(!this->isEnabled()) return;
        std::vector<std::any> args = {
                event.getKey(),
                event.getAction()
        };

        ScriptingEventManager::triggerEvent(module_lua_state, "onKeyEvent", args);
    };
    void onMouse(MouseEvent &event) {
        if(!Scripting::instalized) return;
        if(!this->isEnabled()) return;
        if(event.getButton() == 0) return;
        if(event.getAction() == 0) return;

        std::vector<std::any> args = {
                (int)event.getButton(),
                (int)event.getAction()

        };

        ScriptingEventManager::triggerEvent(module_lua_state, "onMouseEvent", args);
    };

    void onPacketReceive(PacketEvent &event) {
        if(!Scripting::instalized) return;
        if(!this->isEnabled()) return;
        std::vector<std::any> args = {
                (int)event.getPacket()->getId()
        };

        ScriptingEventManager::triggerEvent(module_lua_state, "onPacketReceiveEvent", args);
    };

    void onTickEvent(TickEvent &event) {
        if(!Scripting::instalized) return;
        if(!this->isEnabled()) return;
        std::vector<std::any> args = {

        };

        ScriptingEventManager::triggerEvent(module_lua_state, "onTickEvent", args);
    };

    void onRenderEvent(RenderEvent &event) {
        if(!Scripting::instalized) return;
        if(!this->isEnabled()) return;
        std::vector<std::any> args = {

        };

        ScriptingEventManager::triggerEvent(module_lua_state, "onRenderEvent", args);
    };


};

