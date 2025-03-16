#pragma once

#include "../Module.hpp"
#include "Events/Render/RenderEvent.hpp"
#include "Events/Render/SetupAndRenderEvent.hpp"

class LowHealthIndicator : public Module {
    int health = 20.f;
    ClickGUI cgui;
public:
    LowHealthIndicator() : Module("Low Health", "Warns you when you are at low health.", IDR_HEART_PNG, "") {
        Module::setup();
    };

    void defaultConfig() override { Module::defaultConfig();

        if (settings.getSettingByName<float>("health") == nullptr) this->settings.addSetting("health", 6.0f);
        if (settings.getSettingByName<float>("maxopacity") == nullptr) this->settings.addSetting("maxopacity", 0.3f);
        if (settings.getSettingByName<std::string>("color") == nullptr) this->settings.addSetting("color", (std::string) "FF0000");
        if (settings.getSettingByName<bool>("rgb") == nullptr) this->settings.addSetting("rgb", false);
	}

    void settingsRender(float settingsOffset) override {

        float x = Constraints::PercentageConstraint(0.019, "left");
        float y = Constraints::PercentageConstraint(0.10, "top");

        const float scrollviewWidth = Constraints::RelativeConstraint(0.12, "height", true);
        FlarialGUI::ScrollBar(x, y, 140, Constraints::SpacingConstraint(5.5, scrollviewWidth), 2);
        FlarialGUI::SetScrollView(x - settingsOffset, Constraints::PercentageConstraint(0.00, "top"),
            Constraints::RelativeConstraint(1.0, "width"),
            Constraints::RelativeConstraint(0.88f, "height"));


        this->addHeader("Misc");
        this->addSlider("Maximum overlay opacity", "The maximum overlay opacity", this->settings.getSettingByName<float>("maxopacity")->value, 1.0f, 0.0f);
        this->addColorPicker("color", "", settings.getSettingByName<std::string>("color")->value, this->settings.getSettingByName<float>("maxopacity")->value, this->settings.getSettingByName<bool>("rgb")->value);
        this->addSlider("Health", "The health at which the overlay will be shown", this->settings.getSettingByName<float>("health")->value, 20.0f, 0.0f);
        FlarialGUI::UnsetScrollView();

        this->resetPadding();
    }

    void onEnable() override {
        Module::onEnable();
        Listen(this, SetupAndRenderEvent, &LowHealthIndicator::onSetupAndRender);
        Listen(this, RenderEvent, &LowHealthIndicator::onRender);
    }

    void onDisable() override {
        Module::onDisable();
        Deafen(this, SetupAndRenderEvent, &LowHealthIndicator::onSetupAndRender);
        Deafen(this, RenderEvent, &LowHealthIndicator::onRender);
    }

    void onSetupAndRender(SetupAndRenderEvent& event) {
        if (SDK::clientInstance) if (SDK::clientInstance->getLocalPlayer()) if (SDK::currentScreen == "hud_screen")
        health = SDK::clientInstance->getLocalPlayer()->getHealth();
    }

    void onRender(RenderEvent& event) {
        if (this->isEnabled() && this->health <= this->settings.getSettingByName<float>("health")->value && SDK::currentScreen == "hud_screen" && !cgui.isEnabled()) {
            D2D1_COLOR_F color2 = this->settings.getSettingByName<bool>("rgb")->value ? FlarialGUI::rgbColor : FlarialGUI::HexToColorF(this->settings.getSettingByName<std::string>("color")->value);
            color2.a = this->settings.getSettingByName<float>("maxopacity")->value;
            FlarialGUI::RoundedRect(0, 0, color2, MC::windowSize.x, MC::windowSize.y, 0, 0);
        }
    }
};
