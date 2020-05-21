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
        SettingsSystem(entt::registry * registry);
        ~SettingsSystem();
    private:
        bool hasSettingsFile(const std::string & settingsFileLocation);
        void loadSettingsFile();
        void saveSettingsFile();
        entt::registry * registry_;
    };
}
