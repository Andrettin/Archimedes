#pragma once

namespace archimedes {
	template <int N>
	class fractional_int;

	using centesimal_int = fractional_int<2>;
	using decimillesimal_int = fractional_int<4>;
}

namespace archimedes::astronomy {

constexpr int ly_per_100_pc = 326; //light-years per parsecs; 1 parsec = 3.26 light years
constexpr int gm_per_au = 150; //gigameters per astronomical units
constexpr uint64_t zg_per_jovian_mass = 1898130000; //Jovian mass in zettagrams

//light-years to parsecs
extern centesimal_int ly_to_pc(const centesimal_int &light_years);

//parsecs to light-years
extern centesimal_int pc_to_ly(const centesimal_int &parsecs);

//gigameters to astronomical units
extern centesimal_int gm_to_au(const int gm);

//astronomical units to gigameters
extern int au_to_gm(const centesimal_int &au);

//right ascension to longitude
extern decimillesimal_int ra_to_lon(const decimillesimal_int &ra);

//longitude to right ascension
extern decimillesimal_int lon_to_ra(const decimillesimal_int &lon);

//zettagrams to jovian masses
extern centesimal_int zg_to_jovian_mass(const uint64_t zg);

//jovian masses to zettagrams
extern uint64_t jovian_mass_to_zg(const centesimal_int &jm);

}
