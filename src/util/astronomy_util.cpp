#include "archimedes.h"

#include "util/astronomy_util.h"

#include "util/fractional_int.h"

namespace archimedes::astronomy {

centesimal_int ly_to_pc(const centesimal_int &light_years)
{
	centesimal_int parsecs = light_years;
	parsecs *= 100;
	parsecs /= astronomy::ly_per_100_pc;
	return parsecs;
}

centesimal_int pc_to_ly(const centesimal_int &parsecs)
{
	centesimal_int light_years = parsecs;
	light_years *= astronomy::ly_per_100_pc;
	light_years /= 100;
	return light_years;
}

centesimal_int gm_to_au(const int gm)
{
	return centesimal_int(gm) / astronomy::gm_per_au;
}

int au_to_gm(const centesimal_int &au)
{
	return (au * astronomy::gm_per_au).to_int();
}

decimillesimal_int ra_to_lon(const decimillesimal_int &ra)
{
	decimillesimal_int res(ra);

	if (ra > 12) {
		res -= 24;
	}

	return res * 15;
}

decimillesimal_int lon_to_ra(const decimillesimal_int &lon)
{
	decimillesimal_int res(lon);

	res /= 15;

	if (lon < 0) {
		res += 24;
	}

	return res;
}

centesimal_int zg_to_jovian_mass(const uint64_t zg)
{
	return centesimal_int(zg) / astronomy::zg_per_jovian_mass;
}

uint64_t jovian_mass_to_zg(const centesimal_int &jm)
{
	const centesimal_int zg_dec = jm * astronomy::zg_per_jovian_mass;
	const uint64_t zg = zg_dec.to_uint64();
	return zg;
}

}
