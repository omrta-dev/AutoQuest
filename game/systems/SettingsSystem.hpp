//
// Created by omar on 5/20/20.
//
#pragma once
#include <entt/entt.hpp>
#include <components/Settings.hpp>

namespace aik
{
    class SettingsSystem
    {
    public:
        // explicit to only allow this class to be created with this constructor
        explicit SettingsSystem(entt::registry * registry);
        // on deconstruction we save the settings file
        ~SettingsSystem();

        aik::Component::Settings& getSettings();
    private:
        // returns true if the settings file is found at the settingsFileLocation directory
        bool hasSettingsFile(const std::string & settingsFileLocation);
        // reads the settings file and saves it to the only settings object in the ECS
        void loadSettingsFile();
        // reads and saves the file to the only settings object in the ECS
        void saveSettingsFile();
        entt::registry * registry_;
    };
}