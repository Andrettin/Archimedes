#include "archimedes.h"

#include "character/character_base.h"

#include "time/calendar.h"
#include "util/assert_util.h"
#include "util/dice.h"
#include "util/gender.h"
#include "util/random.h"

namespace archimedes {

character_base::character_base(const std::string &identifier)
	: named_data_entry(identifier), gender(gender::none)
{
}

character_base::~character_base()
{
}

void character_base::initialize()
{
	if (this->vital_date_calendar != nullptr) {
		if (!this->vital_date_calendar->is_initialized()) {
			this->vital_date_calendar->initialize();
		}

		if (this->birth_date.isValid()) {
			this->birth_date = this->birth_date.addYears(this->vital_date_calendar->get_year_offset());
		}

		if (this->death_date.isValid()) {
			this->death_date = this->death_date.addYears(this->vital_date_calendar->get_year_offset());
		}

		if (this->start_date.isValid()) {
			this->start_date = this->start_date.addYears(this->vital_date_calendar->get_year_offset());
		}

		this->vital_date_calendar = nullptr;
	}

	this->initialize_dates();

	named_data_entry::initialize();
}

void character_base::check() const
{
	if (this->get_gender() == gender::none) {
		throw std::runtime_error(std::format("Character \"{}\" has no gender.", this->get_identifier()));
	}

	if (this->get_father() != nullptr && this->get_father()->get_gender() != gender::male) {
		throw std::runtime_error("The father of character \"" + this->get_identifier() + "\" is not male.");
	}

	if (this->get_mother() != nullptr && this->get_mother()->get_gender() != gender::female) {
		throw std::runtime_error("The mother of character \"" + this->get_identifier() + "\" is not female.");
	}

	if (!this->get_birth_date().isValid()) {
		throw std::runtime_error(std::format("Character \"{}\" has no birth date.", this->get_identifier()));
	}

	if (!this->get_death_date().isValid() && !this->is_immortal()) {
		throw std::runtime_error(std::format("Non-immortal character \"{}\" has no death date.", this->get_identifier()));
	}

	if (!this->get_start_date().isValid()) {
		throw std::runtime_error(std::format("Character \"{}\" has no start date.", this->get_identifier()));
	}

	assert_throw(this->get_start_date() >= this->get_birth_date());

	if (this->get_death_date().isValid()) {
		assert_throw(this->get_start_date() <= this->get_death_date());
		assert_throw(this->get_birth_date() <= this->get_death_date());
	}
}

void character_base::initialize_dates()
{
	const int adulthood_age = this->get_adulthood_age();
	assert_throw(adulthood_age != 0);
	const int venerable_age = this->get_venerable_age();
	assert_throw(venerable_age != 0);
	const dice &maximum_age_modifier = this->get_maximum_age_modifier();
	assert_throw(!maximum_age_modifier.is_null());

	const dice &starting_age_modifier = this->get_starting_age_modifier();

	bool date_changed = true;
	while (date_changed) {
		date_changed = false;

		if (!this->get_start_date().isValid()) {
			if (this->get_birth_date().isValid()) {
				QDate start_date = this->get_birth_date();
				start_date = start_date.addYears(adulthood_age);
				start_date = start_date.addYears(random::get()->roll_dice(starting_age_modifier));
				if (this->get_death_date().isValid() && start_date > this->get_death_date()) {
					//a character cannot have a start date beyond their death date
					start_date = this->get_death_date();
				}
				this->set_start_date(start_date);
				date_changed = true;
			} else if (this->get_contemporary_character() != nullptr) {
				if (!this->get_contemporary_character()->is_initialized()) {
					this->get_contemporary_character()->initialize();
				}

				if (this->get_contemporary_character()->get_start_date().isValid()) {
					this->set_start_date(this->get_contemporary_character()->get_start_date());
					date_changed = true;
				}
			}
		}

		if (!this->get_birth_date().isValid()) {
			if (this->get_start_date().isValid()) {
				const QDate birth_date = this->generate_birth_date_from_start_date(this->get_start_date());
				this->set_birth_date(birth_date);
				date_changed = true;
			} else if (this->get_death_date().isValid()) {
				QDate birth_date = this->get_death_date();
				birth_date = birth_date.addYears(-venerable_age);
				birth_date = birth_date.addYears(-random::get()->roll_dice(maximum_age_modifier));
				this->set_birth_date(birth_date);
				date_changed = true;
			}
		}

		if (!this->get_death_date().isValid() && !this->is_immortal()) {
			if (this->get_birth_date().isValid()) {
				const QDate death_date = this->generate_death_date_from_birth_date(this->get_birth_date());
				this->set_death_date(death_date);
				date_changed = true;
			}
		}
	}
}

QDate character_base::generate_birth_date_from_start_date(const QDate &start_date) const
{
	assert_throw(start_date.isValid());

	const int adulthood_age = this->get_adulthood_age();
	assert_throw(adulthood_age != 0);

	const dice &starting_age_modifier = this->get_starting_age_modifier();

	QDate birth_date = start_date;
	birth_date = birth_date.addYears(-adulthood_age);
	birth_date = birth_date.addYears(-random::get()->roll_dice(starting_age_modifier));
	return birth_date;
}

QDate character_base::generate_death_date_from_birth_date(const QDate &birth_date) const
{
	assert_throw(birth_date.isValid());

	const int venerable_age = this->get_venerable_age();
	assert_throw(venerable_age != 0);
	const dice &maximum_age_modifier = this->get_maximum_age_modifier();
	assert_throw(!maximum_age_modifier.is_null());

	QDate death_date = birth_date;
	death_date = death_date.addYears(venerable_age);
	death_date = death_date.addYears(random::get()->roll_dice(maximum_age_modifier));
	return death_date;
}

}
