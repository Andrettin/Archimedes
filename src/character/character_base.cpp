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
		throw std::runtime_error(std::format("The father of character \"{}\" is not male.", this->get_identifier()));
	}

	if (this->get_biological_father() != nullptr && this->get_biological_father()->get_gender() != gender::male) {
		throw std::runtime_error(std::format("The biological father of character \"{}\" is not male.", this->get_identifier()));
	}

	if (this->get_mother() != nullptr && this->get_mother()->get_gender() != gender::female) {
		throw std::runtime_error(std::format("The mother of character \"{}\" is not female.", this->get_identifier()));
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

	if (this->get_start_date() < this->get_birth_date()) {
		throw std::runtime_error(std::format("Character \"{}\" has a start date that is earlier than their birth date.", this->get_identifier()));
	}

	//characters who die before their generated start date get their death date set as their start date instead
	if (this->get_start_date() < this->get_adulthood_date() && this->get_start_date() < this->get_death_date()) {
		throw std::runtime_error(std::format("Character \"{}\" has a start date that is earlier than their date of adulthood.", this->get_identifier()));
	}

	if (this->get_death_date().isValid()) {
		assert_throw(this->get_start_date() <= this->get_death_date());
		assert_throw(this->get_birth_date() <= this->get_death_date());
	}
}

void character_base::sort_children()
{
	std::sort(this->children.begin(), this->children.end(), [](const character_base *lhs, const character_base *rhs) {
		if (lhs->get_birth_date() != rhs->get_birth_date()) {
			if (!lhs->get_birth_date().isValid() || !rhs->get_birth_date().isValid()) {
				return lhs->get_birth_date().isValid();
			}

			return lhs->get_birth_date() < rhs->get_birth_date();
		}

		return lhs->get_identifier() < rhs->get_identifier();
	});

	//ensure that the tree children are in the correct order
	this->clear_tree_children();

	for (const character_base *child : this->get_children()) {
		if (child->get_tree_parent() == this) {
			this->add_tree_child(child);
		}
	}
}

QDate character_base::get_adulthood_date() const
{
	QDate adulthood_date = this->get_birth_date();
	adulthood_date = adulthood_date.addYears(this->get_adulthood_age());
	return adulthood_date;
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
