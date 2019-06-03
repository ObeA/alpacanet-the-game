#include "material_manager.h"

MaterialManager::MaterialManager() : materials() {

}

std::shared_ptr<Material> MaterialManager::getMaterial(const std::string& key) {
    auto kvp = materials.find(key);
    if (kvp == materials.end()) {
        return nullptr; // Or throw an exception?
    }

    return materials.find(key)->second;
}

std::shared_ptr<Material> MaterialManager::registerMaterial(const std::string& key, std::shared_ptr<Material> material) {
    if (hasMaterial(key)) {
        throw std::runtime_error("Material is already registered");
    }

    material->initialize(); // Hmmm...
    materials.emplace(key, material);

    return material;
}

bool MaterialManager::hasMaterial(const std::string& key) const {
    return materials.find(key) != materials.end();
}

void MaterialManager::cleanup() {
    materials.clear();
}


