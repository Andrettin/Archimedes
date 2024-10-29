#include "archimedes.h"

#include "character/character_base.h"

#include "time/calendar.h"
#include "util/assert_util.h"
#include "util/gender.h"

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

	if (!this->get_death_date().isValid()) {
		throw std::runtime_error(std::format("Character \"{}\" has no birth date.", this->get_identifier()));
	}

	assert_throw(this->get_start_date() >= this->get_birth_date());
	assert_throw(this->get_start_date() <= this->get_death_date());
	assert_throw(this->get_birth_date() <= this->get_death_date());
}

std::string character_base::get_full_name() const
{
	if (!this->get_nickname().empty()) {
		return this->get_nickname();
	}

	const std::string &name = this->get_name();
	std::string full_name = name;

	if (!this->get_epithet().empty()) {
		if (!full_name.empty()) {
			full_name += " ";
		}

		full_name += this->get_epithet();
	} else if (!this->get_surname().empty()) {
		if (!full_name.empty()) {
			full_name += " ";
		}

		full_name += this->get_surname();
	}

	return full_name;
}

void character_base::initialize_dates()
{
	bool date_changed = true;
	while (date_changed) {
		date_changed = false;

		if (!this->get_start_date().isValid()) {
			if (this->get_birth_date().isValid()) {
				//if we have the birth date but not the start date, set the start date to when the character would become 30 years old
				this->start_date = this->get_birth_date().addYears(30);
				date_changed = true;
			}
		}

		if (!this->get_birth_date().isValid()) {
			if (this->get_start_date().isValid()) {
				this->birth_date = this->get_start_date().addYears(-30);
				date_changed = true;
			} else if (this->get_death_date().isValid()) {
				this->birth_date = this->get_death_date().addYears(-60);
				date_changed = true;
			}
		}

		if (!this->get_death_date().isValid()) {
			if (this->get_birth_date().isValid()) {
				this->death_date = this->get_birth_date().addYears(60);
				date_changed = true;
			}
		}
	}
}

}
