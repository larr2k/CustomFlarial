#pragma once

#include "../../../../SDK/SDK.hpp"
#include <format>
#include "../../../Events/Listener.hpp"
#include "../../../Events/Input/KeyEvent.hpp"
#include "../Module.hpp"
#include "../../../GUI/Engine/Engine.hpp"
#include <Windows.h>

class TabListListener : public Listener {

    Module* module;

    void onRender(RenderEvent& event) override {

        if (SDK::CurrentScreen == "hud_screen")
            if (module->settings.getSettingByName<bool>("enabled")->value) {

                this->module->NormalRender(20, "", "");

            }
    }

public:
    explicit TabListListener(const char string[5], Module* module) {
        this->name = string;
        this->module = module;
    }

};