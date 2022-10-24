#pragma once

namespace archimedes {
	class georectangle;
	class map_projection;
}

namespace archimedes::geocircle {

extern void write_to_image(const QGeoCircle &geocircle, QImage &image, const QColor &color, const georectangle &georectangle, const map_projection *map_projection, const int x_offset);

}
