#pragma once

#include "map/map_projection.h"
#include "util/singleton.h"

namespace archimedes {

class mercator_map_projection final : public map_projection, public singleton<mercator_map_projection>
{
	virtual number_type latitude_to_scaled_latitude(const number_type &lat) const override;
	virtual number_type scaled_latitude_to_latitude(const number_type &scaled_lat) const override;

	virtual void validate_area(const georectangle &georectangle, const QSize &area_size) const override;
};

}
