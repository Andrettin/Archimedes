#pragma once

#include "map/map_projection.h"
#include "util/singleton.h"

namespace archimedes {

class equirectangular_map_projection final : public map_projection, public singleton<equirectangular_map_projection>
{
};

}
