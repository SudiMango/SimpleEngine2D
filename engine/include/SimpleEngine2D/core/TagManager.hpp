#pragma once

#include <vector>
#include <map>
#include <string>
#include "EntityManager.hpp"

namespace simpleengine2d::core {

class TagManager {

public:
    static TagManager &getInstance();

    void createTag(std::string tagName);
    void destroyTag(std::string tagName);

    void addTag(EntityId entity, std::string tagName);
    void removeTag(EntityId entity, std::string tagName);
    bool hasTag(EntityId entity, std::string tagName);

    void reset();

private:
    TagManager() = default;
    ~TagManager() = default;

    TagManager(const TagManager&) = delete;
    TagManager& operator=(const TagManager&) = delete;

    std::vector<std::string> tags;
    std::map<EntityId, std::vector<std::string>> entityToTags;

};

}