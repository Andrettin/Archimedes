#pragma once

namespace archimedes {
	class georectangle;
	class map_projection;
}

namespace archimedes::geopath {

extern void write_to_image(const QGeoPath &geopath, QImage &image, const QColor &color, const georectangle &georectangle, const map_projection *map_projection, const int x_offset);

extern void write_edges_to_image(const QGeoPath &geopath, QImage &image, const georectangle &georectangle, const map_projection *map_projection, const int x_offset);

}
