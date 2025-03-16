#pragma once

#include "../Module.hpp"
#include "Events/Render/RenderEvent.hpp"
#include "Events/Render/SetupAndRenderEvent.hpp"

class LowHealthIndicator : public Module {
    int health = 20.f;
public:
    LowHealthIndicator() : Module("Low Health", "Warns you when you are at low health.", IDR_HEART_PNG, "") {
        Module::setup();
    };

    void onEnable() override {
        Module::onEnable();
        Listen(this, TickEvent, &LowHealthIndicator::onTick);
        Listen(this, RenderEvent, &LowHealthIndicator::onRender);
    }

    void onDisable() override {
        Module::onDisable();
        Deafen(this, TickEvent, &LowHealthIndicator::onTick);
        Deafen(this, RenderEvent, &LowHealthIndicator::onRender);
    }

    void onTick(TickEvent& event) {
        if (SDK::clientInstance) if (SDK::clientInstance->getLocalPlayer())
        health = SDK::clientInstance->getLocalPlayer()->getHealth();
    }

    void onRender(RenderEvent& event) {
        if (this->isEnabled() && this->health < 7) {

            D2D1_COLOR_F color2 = D2D1::ColorF(D2D1::ColorF::Red);
            color2.a = 0.2f;
            FlarialGUI::RoundedRect(0, 0, color2, MC::windowSize.x, MC::windowSize.y, 0, 0);
        }
    }
};
