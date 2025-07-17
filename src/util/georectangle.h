#pragma once

#include "util/geocoordinate.h"

namespace archimedes {

class georectangle final
{
public:
	using number_type = geocoordinate::number_type;

	static const georectangle get_global_georectangle()
	{
		return georectangle(geocoordinate(geocoordinate::min_longitude, geocoordinate::min_latitude), geocoordinate(geocoordinate::max_longitude, geocoordinate::max_latitude));
	}

	static georectangle from_qgeorectangle(const QGeoRectangle &qgeorectangle)
	{
		const QGeoCoordinate bottom_left = qgeorectangle.bottomLeft();
		const QGeoCoordinate top_right = qgeorectangle.topRight();

		return georectangle(geocoordinate(bottom_left), geocoordinate(top_right));
	}

	explicit constexpr georectangle(const geocoordinate &min_geocoordinate, const geocoordinate &max_geocoordinate)
		: min_geocoordinate(min_geocoordinate), max_geocoordinate(max_geocoordinate)
	{
	}

	constexpr const geocoordinate &get_min_geocoordinate() const
	{
		return this->min_geocoordinate;
	}

	constexpr const geocoordinate &get_max_geocoordinate() const
	{
		return this->max_geocoordinate;
	}

	constexpr const decimillesimal_int &get_min_longitude() const
	{
		return this->get_min_geocoordinate().get_longitude();
	}

	constexpr void set_min_longitude(const decimillesimal_int &lon)
	{
		this->min_geocoordinate.set_longitude(lon);
	}

	constexpr const decimillesimal_int &get_max_longitude() const
	{
		return this->get_max_geocoordinate().get_longitude();
	}

	constexpr void set_max_longitude(const decimillesimal_int &lon)
	{
		this->max_geocoordinate.set_longitude(lon);
	}

	constexpr const decimillesimal_int &get_min_latitude() const
	{
		return this->get_min_geocoordinate().get_latitude();
	}

	constexpr void set_min_latitude(const decimillesimal_int &lat)
	{
		this->min_geocoordinate.set_latitude(lat);
	}

	constexpr const decimillesimal_int &get_max_latitude() const
	{
		return this->get_max_geocoordinate().get_latitude();
	}

	constexpr void set_max_latitude(const decimillesimal_int &lat)
	{
		this->max_geocoordinate.set_latitude(lat);
	}

	constexpr number_type get_width() const
	{
		return this->get_max_geocoordinate().get_longitude() - this->get_min_geocoordinate().get_longitude();
	}

	bool contains(const geocoordinate &geocoordinate) const
	{
		return (geocoordinate.get_longitude() >= this->get_min_geocoordinate().get_longitude()
			&& geocoordinate.get_latitude() >= this->get_min_geocoordinate().get_latitude()
			&& geocoordinate.get_longitude() <= this->get_max_geocoordinate().get_longitude()
			&& geocoordinate.get_latitude() <= this->get_max_geocoordinate().get_latitude());
	}

	QGeoRectangle to_qgeorectangle() const
	{
		return QGeoRectangle(QGeoCoordinate(this->get_max_latitude().to_double(), this->get_min_longitude().to_double()), QGeoCoordinate(this->get_min_latitude().to_double(), this->get_max_longitude().to_double()));
	}

	constexpr bool operator ==(const georectangle &other) const
	{
		return this->get_min_geocoordinate() == other.get_min_geocoordinate() && this->get_max_geocoordinate() == other.get_max_geocoordinate();
	}

private:
	geocoordinate min_geocoordinate;
	geocoordinate max_geocoordinate;
};

}
