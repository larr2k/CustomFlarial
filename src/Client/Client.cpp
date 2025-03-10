﻿#include "Client.hpp"

#include <filesystem>
#include <thread>
#include <wingdi.h>
#include <wininet.h>

#include <Utils/VersionUtils.hpp>
#include <Utils/WinrtUtils.hpp>

#include "Command/CommandManager.hpp"

#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Windows.Foundation.Collections.h>

#include "Utils/APIUtils.hpp"

using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::ApplicationModel::Activation;
using namespace winrt::Windows::ApplicationModel::Core;

Settings Client::settings = Settings();

bool notifiedOfConnectionIssue = false;

std::string Client::current_commit = COMMIT_HASH;

std::vector<std::string> Client::getPlayersVector(const nlohmann::json& data) {
    std::vector<std::string> allPlayers;

    try {
        // Check if the JSON is an array of players
        if (data.is_array()) {
            for (const auto& player : data) {
                if (player.is_string()) {
                    allPlayers.push_back(player.get<std::string>());
                }
            }
        } else {
            Logger::error("Invalid JSON format: expected an array of players.");
        }
    } catch (const nlohmann::json::exception& e) {
        Logger::error("Error parsing players: {}", e.what());
    }

    if (SDK::clientInstance && SDK::clientInstance->getLocalPlayer()) {
        try {
            std::string name = SDK::clientInstance->getLocalPlayer()->getPlayerName();
            std::string clearedName = String::removeNonAlphanumeric(String::removeColorCodes(name));

            if (clearedName.empty()) {
                clearedName = String::removeColorCodes(name);
            }
            allPlayers.push_back(clearedName);
        } catch (const std::exception& e) {
            Logger::error("Error processing local player name: {}", e.what());
        }
    }

    return allPlayers;
}


bool Client::disable = false;

winrt::event_token activationToken;

void RegisterActivationHandler()
{
    activationToken = CoreApplication::MainView().Activated(
        [](const auto &, const IActivatedEventArgs &context) {
            if (context.Kind() != ActivationKind::Protocol)
                return;

            auto uri = winrt::unbox_value<ProtocolActivatedEventArgs>(context).Uri();

            std::vector<std::pair<std::wstring, std::wstring>> dataList;

            for (const auto& dataContext : uri.QueryParsed()) {
                std::wstring name = dataContext.Name().c_str();
                std::wstring value = dataContext.Value().c_str();

                dataList.emplace_back(name, value);
            }

            auto event = nes::make_holder<ProtocolEvent>(uri.Host().c_str(), dataList);
            eventMgr.trigger(event);
        });
}

void Client::UnregisterActivationHandler()
{
    if (activationToken) // Check if the token is valid
    {
        CoreApplication::MainView().Activated(activationToken); // Unregister using the token
    }
}

void Client::initialize() {
    winrt::init_apartment();

#if defined(__TEST__)
    WinrtUtils::setWindowTitle(fmt::format("Flarial v{} {} {}", FLARIAL_VERSION, FLARIAL_BUILD_TYPE, FLARIAL_BUILD_DATE));
#else
    WinrtUtils::setWindowTitle(fmt::format("Flarial v{} {}", FLARIAL_VERSION, FLARIAL_BUILD_TYPE));
#endif

    VersionUtils::initialize();
    version = VersionUtils::getFormattedVersion();

    if (!VersionUtils::isSupported(Client::version)) {
        Logger::fatal("Minecraft version is not supported!");
        MessageBox(nullptr, "Flarial: this version is not supported!", "", MB_OK);
        ModuleManager::terminate();
        Client::disable = true;
        return;
    }
    Client::elapsed = (Utils::getCurrentMs() - Client::start) / 1000.0;

    Logger::success("Time: {:.2f}s", Client::elapsed);

    VersionUtils::addData();

    Client::elapsed = (Utils::getCurrentMs() - Client::start) / 1000.0;

    Logger::success("Time: {:.2f}s", Client::elapsed);
    std::vector<std::filesystem::path> directories = {
        Utils::getRoamingPath() + "\\Flarial",
        Utils::getRoamingPath() + "\\Flarial\\assets",
        Utils::getRoamingPath() + "\\Flarial\\logs",
        Utils::getRoamingPath() + "\\Flarial\\Config",
        Utils::getRoamingPath() + "\\Flarial\\scripts",
    };


    std::thread updateThread([]() {
        std::string playersList;
        std::string filePath = Utils::getRoamingPath() + "\\Flarial\\playerscache.txt";
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::ofstream createFile(filePath);
            if (!createFile.is_open()) {
               Logger::error("Could not create file: ");
            } else {
                createFile.close();
                file.open(filePath);
                if (!file.is_open()) {
                    Logger::error("Could not open file for reading after creation: ");
                }
            }
        }


        std::stringstream buffer;
        buffer << file.rdbuf();
        playersList = buffer.str();
        file.close();

        APIUtils::onlineUsers = APIUtils::ListToVector(playersList);
    });

    updateThread.detach();    Client::elapsed = (Utils::getCurrentMs() - Client::start) / 1000.0;

    Logger::success("Time: {:.2f}s", Client::elapsed);


    for (const auto& path : directories) {
        if (!std::filesystem::exists(path)) {
            std::filesystem::create_directory(path);
        }
    }    Client::elapsed = (Utils::getCurrentMs() - Client::start) / 1000.0;

    Logger::success("Time: {:.2f}s", Client::elapsed);

    Client::CheckSettingsFile();
    Client::LoadSettings();

    Logger::success("4");    Client::elapsed = (Utils::getCurrentMs() - Client::start) / 1000.0;

    Logger::success("Time: {:.2f}s", Client::elapsed);


    ADD_SETTING("fontname", std::string("Space Grotesk"));
    ADD_SETTING("mod_fontname", std::string("Space Grotesk"));
    ADD_SETTING("blurintensity", 2.0f);
    ADD_SETTING("killdx", false);
    ADD_SETTING("disable_alias", false);
    ADD_SETTING("vsync", false);
    ADD_SETTING("promotions", true);
    ADD_SETTING("donotwait", true);
    ADD_SETTING("bufferingmode", std::string("Double Buffering"));
    ADD_SETTING("swapeffect", std::string("FLIP_SEQUENTIAL"));
    ADD_SETTING("disableanims", false);
    ADD_SETTING("anonymousApi", false);
    ADD_SETTING("dlassets", true);
    ADD_SETTING("noicons", false);
    ADD_SETTING("noshadows", false);
    ADD_SETTING("watermark", true);
    ADD_SETTING("centreCursor", false);
    ADD_SETTING("aliasingMode", std::string("Default"));
    ADD_SETTING("ejectKeybind", std::string(""));
    ADD_SETTING("enabledModulesOnTop", false);
    ADD_SETTING("rgb_speed", 1.0f);
    ADD_SETTING("rgb_saturation", 1.0f);
    ADD_SETTING("rgb_value", 1.0f);
    ADD_SETTING("modules_font_scale", 1.0f);
    ADD_SETTING("gui_font_scale", 1.0f);
    ADD_SETTING("overrideFontWeight", false);
    ADD_SETTING("fontWeight", std::string("Normal"));

    Logger::success("5");


    FlarialGUI::ExtractImageResource(IDR_RED_LOGO_PNG, "red-logo.png","PNG");
    FlarialGUI::ExtractImageResource(IDR_CYAN_LOGO_PNG, "dev-logo.png", "PNG");
    FlarialGUI::ExtractImageResource(IDR_GAMER_LOGO_PNG, "gamer-logo.png", "PNG");
    FlarialGUI::ExtractImageResource(IDR_BOOSTER_LOGO_PNG, "booster-logo.png", "PNG");

    FlarialGUI::LoadFont(IDR_FONT_TTF);

    FlarialGUI::LoadFont(IDR_FONT_BOLD_TTF);

    FlarialGUI::LoadFont(IDR_MINECRAFTIA_TTF);

    Logger::success("6");


RegisterActivationHandler();
    HookManager::initialize();
    ModuleManager::initialize();
    CommandManager::initialize();
}

std::string window = "Minecraft";

HWND hWnd = FindWindow(nullptr, window.c_str());
RECT currentRect = {0};
RECT clientRect = {0};
RECT previousRect = {0};
bool InHudScreen = false;

bool toes = false;

void Client::centerCursor() {
    if(Client::disable) return;
    if (hWnd != nullptr && Client::settings.getSettingByName<bool>("centreCursor")->value) {
        if (!toes) {
            toes = true;
            std::thread wow([&]() {
                while (!Client::disable && Client::settings.getSettingByName<bool>("centreCursor")->value) {
                    GetWindowRect(hWnd, &currentRect);
                    GetClientRect(hWnd, &clientRect);

                    if (memcmp(&currentRect, &previousRect, sizeof(RECT)) != 0) {
                        previousRect = currentRect;
                    }
                    Sleep(1000);
                }
                if (Client::disable && !Client::settings.getSettingByName<bool>("centreCursor")->value) toes = false;
            });

            wow.detach();
        }


        if ((SDK::getCurrentScreen() != "hud_screen" && InHudScreen) ||
            (SDK::getCurrentScreen() == "hud_screen" && !InHudScreen)) {
            GetWindowRect(hWnd, &currentRect);
            GetClientRect(hWnd, &clientRect);

            int windowX = currentRect.left;
            int windowY = currentRect.top;

            int centerX = windowX + (clientRect.right) / 2;
            int centerY = windowY + (clientRect.bottom) / 2;

            SetCursorPos(centerX, centerY);

            InHudScreen = !InHudScreen;
        }
    }
}