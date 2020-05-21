//
// Created by omar on 5/20/20.
//
#include "SettingsSystem.hpp"
#include <filesystem>
#include <iostream>
#include <sstream>
#include <cereal/archives/json.hpp>
#include <entt/entity/registry.hpp>
#include <entt/entity/snapshot.hpp>
#include <entt/entity/helper.hpp>

aik::SettingsSystem::SettingsSystem(entt::registry *registry)
{
    registry_ = registry;
    registry_->emplace<aik::Settings>(registry_->create());
    if(hasSettingsFile("config.json"))
    {
        loadSettingsFile();
    }
    else
    {
        saveSettingsFile();
    }
}

aik::SettingsSystem::~SettingsSystem()
{
    saveSettingsFile();
}

bool aik::SettingsSystem::hasSettingsFile(const std::string & settingsFileLocation)
{
    return std::filesystem::exists(std::filesystem::current_path().string() + '/' +  settingsFileLocation);
}

void aik::SettingsSystem::loadSettingsFile()
{

}

void aik::SettingsSystem::saveSettingsFile()
{
    entt::snapshot{registry_}.
}
