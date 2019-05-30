#pragma once

#include <memory>
#include "../materials/material.h"

class MaterialManager {
public:
    MaterialManager(MaterialManager const&) = delete;
    void operator=(MaterialManager const&) = delete;

    void setup () { }
    void cleanup();

    static MaterialManager& getInstance() {
        static MaterialManager instance;

        return instance;
    }

    std::shared_ptr<Material> getMaterial(const std::string& key);
    bool hasMaterial(const std::string& key) const;
    std::shared_ptr<Material> registerMaterial(const std::string& key, std::shared_ptr<Material> material);
private:
    MaterialManager();

    std::map<std::string, std::shared_ptr<Material>> materials;
};

