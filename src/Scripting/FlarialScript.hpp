#pragma once

#include <lua.hpp>
#include <Utils/Logger/Logger.hpp>
#include <SDK/SDK.hpp>

#define ADD_ERROR_MESSAGE(msg) \
do { \
if (SDK::clientInstance && SDK::clientInstance->getGuiData()) { \
SDK::clientInstance->getGuiData()->displayClientMessage(std::string("§3[§1Lua§3] §c") + (msg)); \
} \
} while(0)


class FlarialScript {
public:
    FlarialScript(std::string filePath, std::string code);

    bool compile();
    void setEnabled(bool enabled);

    [[nodiscard]] const std::string& getName() const { return mName; }
    [[nodiscard]] const std::string& getDescription() const { return mDescription; }
    [[nodiscard]] const std::string& getAuthor() const { return mAuthor; }
    [[nodiscard]] lua_State* getState() const { return mState; }
    [[nodiscard]] bool isEnabled() const { return mEnabled; }

    ~FlarialScript() {
        if (mState) {
            lua_close(mState);
            mState = nullptr;
        }
    }

    void registerModuleFunction(const std::string& functionName, lua_CFunction function) {
        lua_pushcfunction(mState, function, functionName.c_str());
        lua_setglobal(mState, functionName.c_str());
    }

    std::mutex eventMutex;
    void registerEvent(const std::string& eventName);
    template <typename... Args>
    bool registerCancellableEvent(const std::string& eventName, Args&&... args) {
        std::lock_guard lock(eventMutex);

        bool cancelled = false;
        lua_getglobal(mState, "eventHandlers");
        if (lua_istable(mState, -1)) {
            lua_pushstring(mState, eventName.c_str());
            lua_gettable(mState, -2);

            if (lua_isfunction(mState, -1)) {
                auto pushArg = [&]<typename T0>(T0&& arg) {
                    using T = std::decay_t<T0>;
                    if constexpr (std::is_same_v<T, std::string>)
                        lua_pushstring(mState, arg.c_str());
                    else if constexpr (std::is_integral_v<T>)
                        lua_pushinteger(mState, static_cast<lua_Integer>(arg));
                    else if constexpr (std::is_floating_point_v<T>)
                        lua_pushnumber(mState, static_cast<lua_Number>(arg));
                    else if constexpr (std::is_same_v<T, bool>)
                        lua_pushboolean(mState, arg);
                    else
                        lua_pushnil(mState);
                };

                (pushArg(std::forward<Args>(args)), ...);

                if (lua_pcall(mState, sizeof...(Args), 1, 0) != LUA_OK) {
                    Logger::error("Error in event {}: {}", eventName, lua_tostring(mState, -1));
                    ADD_ERROR_MESSAGE("Error in event " + eventName + ": " + std::string(lua_tostring(mState, -1)));
                    lua_pop(mState, 1);
                } else {
                    if (lua_isboolean(mState, -1))
                        cancelled = lua_toboolean(mState, -1);
                    lua_pop(mState, 1);
                }
            } else {
                lua_pop(mState, 1);
            }
        }
        lua_pop(mState, 1);
        return cancelled;
    }

private:
    std::string mFilePath;
    std::string mCode;
    lua_State* mState = nullptr;
    bool mEnabled = false;

    // Values extracted from script
    std::string mName;
    std::string mDescription;
    std::string mAuthor = "Unknown";
};