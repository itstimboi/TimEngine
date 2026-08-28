#include "something.h"

#include <string>

float lineWidth = 0.1f;
float vertexSelectionWidth = 0.1f;

float average(std::vector<float> values)
{
    if (values.empty())
        return 0.0f;

    float returnValue;

    for (int i = 0; i < sizeof(values); i++)
    {
        returnValue += values[i];
    }

    return returnValue / sizeof(values);
}