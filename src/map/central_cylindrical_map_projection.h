#pragma once

#include "map/map_projection.h"
#include "util/singleton.h"

namespace archimedes {

class central_cylindrical_map_projection final : public map_projection, public singleton<central_cylindrical_map_projection>
{
	virtual number_type latitude_to_scaled_latitude(const number_type &lat) const override;
	virtual number_type scaled_latitude_to_latitude(const number_type &scaled_lat) const override;
};

}
