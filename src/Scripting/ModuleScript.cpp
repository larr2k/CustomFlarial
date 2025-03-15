#include "ModuleScript.hpp"

#include "ScriptManager.hpp"
#include <SDK/Client/Network/Packet/TextPacket.hpp>

void ModuleScript::onEnable() {
    if (const auto& script = linkedScript.lock()) {
        if (script->getState()) {
            ScriptManager::executeFunction(script->getState(), "onEnable");
            Module::onEnable();
        }
    }
}

void ModuleScript::onDisable() {
    if (const auto& script = linkedScript.lock()) {
        if (script->getState()) {
            ScriptManager::executeFunction(script->getState(), "onDisable");
            Module::onDisable();
        }
    }
}

void ModuleScript::onKey(KeyEvent& event) {
    if (!isEnabled() || !ScriptManager::initialized) return;

    if (const auto& script = linkedScript.lock()) {
        bool cancelled = script->registerCancellableEvent("KeyEvent", event.getKey(), static_cast<int>(event.getAction()));
        if (cancelled) event.cancel();
    }
}

void ModuleScript::onMouse(MouseEvent& event) {
    if (!isEnabled() || !ScriptManager::initialized || event.getButton() == 0 || event.getAction() == 0) return;

    if (const auto& script = linkedScript.lock()) {
        bool cancelled = script->registerCancellableEvent("MouseEvent", static_cast<int>(event.getButton()), static_cast<int>(event.getAction()));
        if (cancelled) event.cancel();
    }

}

void ModuleScript::onPacketReceive(PacketEvent &event) {
    if (!isEnabled() || !ScriptManager::initialized) return;

    if (const auto& script = linkedScript.lock()) {
        bool cancel = false;

        bool onPacketReceive = script->registerCancellableEvent("PacketReceiveEvent", event.getPacket(), static_cast<int>(event.getPacket()->getId()));
        if (onPacketReceive) cancel = true;

        if (event.getPacket()->getId() == MinecraftPacketIds::Text) {
            const auto *pkt = reinterpret_cast<TextPacket*>(event.getPacket());
            if (pkt) {
                std::string msg = pkt->message;
                std::string name = pkt->name;
                auto type = pkt->type;
                std::string xuid = pkt->xuid;

                bool ChatReceive = script->registerCancellableEvent("ChatReceiveEvent", msg, name, static_cast<int>(type), xuid);
                if (ChatReceive) cancel = true;
            }
        }
        if (cancel) event.cancel();
    }
}

void ModuleScript::onPacketSent(PacketSendEvent& event) {
    if (!isEnabled() || !ScriptManager::initialized) return;
    /*

    if (const auto& script = linkedScript.lock()) {
        bool cancel = false;

        bool onPacketSent = script->registerCancellableEvent("PacketSendEvent", event.getPacket(), static_cast<int>(event.getPacket()->getId()));
        if (onPacketSent) cancel = true;

        if (event.getPacket()->getId() == MinecraftPacketIds::Text) {
            const auto *pkt = reinterpret_cast<TextPacket*>(event.getPacket());
            if (pkt) {
                std::string msg = pkt->message;
                std::string name = pkt->name;
                auto type = pkt->type;
                std::string xuid = pkt->xuid;

                bool ChatSent = script->registerCancellableEvent("ChatSendEvent", msg, name, static_cast<int>(type), xuid);
                if (ChatSent) cancel = true;
            }
        }
        if (cancel) event.cancel();
    }
    */
}

void ModuleScript::onTick(TickEvent& event) {
    if (!isEnabled() || !ScriptManager::initialized) return;

    if (const auto& script = linkedScript.lock()) {
        script->registerEvent("TickEvent");
    }
}

void ModuleScript::onRender(RenderEvent& event) {
    if (!isEnabled() || !ScriptManager::initialized) return;

    if (const auto& script = linkedScript.lock()) {
        script->registerEvent("RenderEvent");
    }
}

void ModuleScript::onSetupAndRender(SetupAndRenderEvent& event) {
    if (!isEnabled() || !ScriptManager::initialized) return;

    if (const auto& script = linkedScript.lock()) {
        script->registerEvent("SetupAndRenderEvent");
    }
}
