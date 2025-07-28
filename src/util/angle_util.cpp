#include "archimedes.h"

#include "util/angle_util.h"

#include "util/decimillesimal_int.h"
#include "util/number_util.h"

#include <boost/math/constants/constants.hpp>

namespace archimedes::angle {

const number_type pi("3.1416");
const number_type half_pi = pi / 2;

number_type degrees_to_radians(const number_type &degrees)
{
	return degrees * angle::pi / 180;
}

double degrees_to_radians(const double degrees)
{
	return degrees * boost::math::constants::pi<double>() / 180.;
}

number_type radians_to_degrees(const number_type &radians)
{
	return radians * 180 / angle::pi;
}

double radians_to_degrees(const double radians)
{
	return radians * 180. / boost::math::constants::pi<double>();
}

//n is in the -pi to +pi range
number_type gudermannian(const number_type &n)
{
	const double d = n.to_double();
	return angle::radians_to_degrees(number_type(atan(sinh(d))));
}

number_type gudermannian_inverse(const number_type &degrees)
{
	const number_type sign = number::sign(degrees);
	const double sin = std::sin((angle::degrees_to_radians(degrees) * sign).to_double());
	return sign * (number_type(log((1.0 + sin) / (1.0 - sin))) / 2);
}

}
