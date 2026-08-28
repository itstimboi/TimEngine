#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <json/json.hpp>

extern float lineWidth;
extern float vertexSelectionWidth;

float average(std::vector<float> values);

namespace glm {
    inline void to_json(nlohmann::json& j, const glm::vec3& v)
    {
        j = {
            v.x,
            v.y,
            v.z
        };
    }

    inline void from_json(const nlohmann::json& j, glm::vec3& v)
    {
        if (!j.is_array() || j.size() != 3)
        {
            throw std::runtime_error(
                "Expected glm::vec3 JSON array with 3 elements"
            );
        }

        v.x = j.at(0).get<float>();
        v.y = j.at(1).get<float>();
        v.z = j.at(2).get<float>();
    }
}