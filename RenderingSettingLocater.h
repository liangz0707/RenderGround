#pragma once
#include "Common.h"

class RenderingSettingLocater
{
public:
    static VkExtent2D GetExtent() { return extent; }

    static void provide(VkExtent2D service)
    {
        extent = service;
    }

private:

    static VkExtent2D extent;
};

