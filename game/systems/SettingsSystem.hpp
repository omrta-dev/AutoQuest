//
// Created by omar on 5/20/20.
//
#pragma once
#include <entt/entt.hpp>
#include "components/Settings.hpp"

namespace aik
{
    class SettingsSystem
    {
    public:
        explicit SettingsSystem(entt::registry * registry);
        ~SettingsSystem();

        aik::Component::Settings& getSettings();
    private:
        bool hasSettingsFile(const std::string & settingsFileLocation);
        void loadSettingsFile();
        void saveSettingsFile();
        entt::registry * registry_;
    };
}