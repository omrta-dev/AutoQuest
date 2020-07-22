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
    // on construct we try to read or create the settings file
    registry_ = registry;
    registry_->emplace<aik::Component::Settings>(registry_->create());
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

// loading a file requires creating a jsonInput, reading the file, parsing the json with entt(ECS), and storing the parsed data
void aik::SettingsSystem::loadSettingsFile()
{
    entt::registry dest;
    std::fstream storage(std::filesystem::current_path().string() + configFilePath, std::fstream::in | std::fstream::out);
    cereal::JSONInputArchive input{storage};
    entt::snapshot_loader{dest}.entities(input).component<aik::Component::Settings>(input);
    auto settingsView = dest.view<aik::Component::Settings>();
    auto loadedSettings = settingsView.get(settingsView.front());

    auto destinationSettings = registry_->view<aik::Component::Settings>().front();
    registry_->patch<aik::Component::Settings>(destinationSettings, [loadedSettings](auto & newSetting) {newSetting = loadedSettings;});
}

// saving a file requires serializing the settings file and writing it with a json output object
void aik::SettingsSystem::saveSettingsFile()
{
    std::ofstream storage(std::filesystem::current_path().string() + configFilePath, std::ofstream::trunc | std::ofstream::out);
    if(storage.is_open())
    {
        cereal::JSONOutputArchive output(storage);
        entt::snapshot{*registry_}.entities(output).component<aik::Component::Settings>(output);
    }
}

// there should always only be 1 settings entity so we can just return that
aik::Component::Settings &aik::SettingsSystem::getSettings()
{
    return registry_->view<aik::Component::Settings>().get(registry_->view<aik::Component::Settings>().front());
}
