//
// Created by omar on 5/20/20.
//
#include "SettingsSystem.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <cereal/archives/json.hpp>
#include <entt/entity/snapshot.hpp>

const std::string configFilePath = "/config.json";

aik::SettingsSystem::SettingsSystem(entt::registry *registry)
{
    registry_ = registry;
    registry_->emplace<aik::Settings>(registry_->create());
    if(hasSettingsFile(configFilePath))
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
    return std::filesystem::exists(std::filesystem::current_path().string() + settingsFileLocation);
}

void aik::SettingsSystem::loadSettingsFile()
{
    entt::registry dest;
    std::fstream storage(std::filesystem::current_path().string() + configFilePath, std::fstream::in | std::fstream::out);
    cereal::JSONInputArchive input{storage};
    entt::snapshot_loader{dest}.entities(input).component<aik::Settings>(input);
    auto settingsView = dest.view<aik::Settings>();
    auto loadedSettings = settingsView.get(settingsView.front());

    auto destinationSettings = registry_->view<aik::Settings>().front();
    registry_->patch<aik::Settings>(destinationSettings, [loadedSettings](auto & newSetting) {newSetting = loadedSettings;});
}

void aik::SettingsSystem::saveSettingsFile()
{
    std::ofstream storage(std::filesystem::current_path().string() + configFilePath, std::ofstream::trunc | std::ofstream::out);
    if(storage.is_open())
    {
        cereal::JSONOutputArchive output(storage);
        entt::snapshot{*registry_}.entities(output).component<aik::Settings>(output);
    }
}

aik::Settings &aik::SettingsSystem::getSettings()
{
    // there should always only be 1 settings entity so we can just return that
    return registry_->view<aik::Settings>().get(registry_->view<aik::Settings>().front());
}
