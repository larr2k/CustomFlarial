#pragma once

enum class ScriptSettingType {
    Bool,
    String,
    Int,
    Float,
    TextBox,
    KeyBind,
};

struct BaseSetting {
    std::string name;
    std::string description;
    ScriptSettingType type;
    virtual ~BaseSetting() = default;
};

// Going to separate all the settings into different files later

struct BoolSetting : BaseSetting {
    bool value;
    bool defaultValue;
    BoolSetting(const std::string& name, const std::string& description, bool def)
        : value(def), defaultValue(def) {
        this->name = name;
        this->description = description;
        this->type = ScriptSettingType::Bool;
    }
};