#include "SimpleEngine2D/core/TagManager.hpp"

namespace simpleengine2d::core {

TagManager &TagManager::getInstance() {
    static TagManager instance;
    return instance;
}

/**
 * Tags creation
 */

void TagManager::createTag(std::string tagName) {
    auto it = std::find(tags.begin(), tags.end(), tagName);
    if (it == tags.end()) {
        tags.push_back(tagName);
    }
}

void TagManager::destroyTag(std::string tagName) {
    auto it = std::find(tags.begin(), tags.end(), tagName);
    if (it != tags.end()) {
        tags.erase(it);
    }
}

void TagManager::reset() {
    entityToTags.clear();
    tags.clear();
}

/**
 * Tag management with entities
 */

void TagManager::addTag(EntityId entity, std::string tagName) {
    auto it_e = entityToTags.find(entity);
    if (it_e != entityToTags.end()) {
        auto it_v = std::find(it_e->second.begin(), it_e->second.end(), tagName);
        if (it_v == it_e->second.end()) {
            it_e->second.push_back(tagName);
        }
    } else {
        entityToTags[entity].push_back(tagName);
    }
}

void TagManager::removeTag(EntityId entity, std::string tagName) {
    auto it_e = entityToTags.find(entity);
    if (it_e != entityToTags.end()) {
        auto it_v = std::find(it_e->second.begin(), it_e->second.end(), tagName);
        if (it_v != it_e->second.end()) {
            it_e->second.erase(it_v);
        }
    }
}

bool TagManager::hasTag(EntityId entity, std::string tagName) {
    auto it_e = entityToTags.find(entity);
    if (it_e != entityToTags.end()) {
        auto it_v = std::find(it_e->second.begin(), it_e->second.end(), tagName);
        if (it_v != it_e->second.end()) {
            return true;
        }
    }

    return false;
}

std::vector<std::string> TagManager::getAllTags() {
    return tags;
}

std::vector<std::string> TagManager::getTags(EntityId entity) {
    auto it = entityToTags.find(entity);
    if (it == entityToTags.end()) {
        return {};
    }

    return it->second;
}

}
