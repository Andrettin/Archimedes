#pragma once

#include "database/named_data_entry.h"

Q_MOC_INCLUDE("time/calendar.h")

namespace archimedes {

class calendar;
class dice;
enum class gender;

class character_base : public named_data_entry
{
	Q_OBJECT

	Q_PROPERTY(QString surname READ get_surname_qstring NOTIFY changed)
	Q_PROPERTY(std::string nickname MEMBER nickname NOTIFY changed)
	Q_PROPERTY(std::string epithet MEMBER epithet NOTIFY changed)
	Q_PROPERTY(QString full_name READ get_full_name_qstring NOTIFY changed)
	Q_PROPERTY(QString description READ get_description_qstring NOTIFY changed)
	Q_PROPERTY(archimedes::gender gender MEMBER gender NOTIFY changed)
	Q_PROPERTY(QDate birth_date MEMBER birth_date READ get_birth_date NOTIFY changed)
	Q_PROPERTY(QDate death_date MEMBER death_date READ get_death_date NOTIFY changed)
	Q_PROPERTY(QDate start_date MEMBER start_date READ get_start_date NOTIFY changed)
	Q_PROPERTY(archimedes::calendar* vital_date_calendar MEMBER vital_date_calendar)

public:
	static constexpr const char class_identifier[] = "character";
	static constexpr const char database_folder[] = "characters";

	explicit character_base(const std::string &identifier);
	~character_base();

	virtual void initialize() override;
	virtual void check() const override;

	virtual std::string get_scope_name() const override
	{
		return this->get_full_name();
	}

	const std::string &get_surname() const
	{
		return this->surname;
	}

	Q_INVOKABLE void set_surname(const std::string &surname)
	{
		this->surname = surname;
	}

	QString get_surname_qstring() const
	{
		return QString::fromStdString(this->get_surname());
	}

	const std::string &get_nickname() const
	{
		return this->nickname;
	}

	const std::string &get_epithet() const
	{
		return this->epithet;
	}

	virtual std::string get_full_name() const = 0;

	QString get_full_name_qstring() const
	{
		return QString::fromStdString(this->get_full_name());
	}

	virtual bool is_surname_first() const
	{
		return false;
	}

	const std::string &get_description() const
	{
		return this->description;
	}

	Q_INVOKABLE void set_description(const std::string &description)
	{
		this->description = description;
	}

	QString get_description_qstring() const
	{
		return QString::fromStdString(this->get_description());
	}

	gender get_gender() const
	{
		return this->gender;
	}

	void set_gender(const gender gender)
	{
		this->gender = gender;
	}

	character_base *get_father() const
	{
		return this->father;
	}

	void set_father(character_base *father)
	{
		this->father = father;
		father->add_child(this);
	}

	character_base *get_mother() const
	{
		return this->mother;
	}

	void set_mother(character_base *mother)
	{
		this->mother = mother;
		mother->add_child(this);
	}

	const std::vector<character_base *> &get_children() const
	{
		return this->children;
	}

	void add_child(character_base *child)
	{
		this->children.push_back(child);
	}

	void sort_children();

	character_base *get_contemporary_character() const
	{
		return this->contemporary_character;
	}

	void set_contemporary_character(character_base *contemporary_character)
	{
		this->contemporary_character = contemporary_character;
	}

	bool has_vital_dates() const
	{
		return this->get_birth_date().isValid() && (this->get_death_date().isValid() || this->is_immortal()) && this->get_start_date().isValid();
	}

	const QDate &get_birth_date() const
	{
		return this->birth_date;
	}

	void set_birth_date(const QDate &date)
	{
		this->birth_date = date;
	}

	const QDate &get_death_date() const
	{
		return this->death_date;
	}

	void set_death_date(const QDate &date)
	{
		this->death_date = date;
	}

	const QDate &get_start_date() const
	{
		return this->start_date;
	}

	void set_start_date(const QDate &date)
	{
		this->start_date = date;
	}

	void initialize_dates();
	QDate generate_birth_date_from_start_date(const QDate &start_date) const;
	QDate generate_death_date_from_birth_date(const QDate &birth_date) const;

	virtual int get_adulthood_age() const = 0;
	virtual int get_venerable_age() const = 0;
	virtual const dice &get_maximum_age_modifier() const = 0;
	virtual const dice &get_starting_age_modifier() const = 0;
	virtual bool is_immortal() const = 0;

signals:
	void changed();

private:
	std::string surname;
	std::string nickname;
	std::string epithet;
	std::string description;
	archimedes::gender gender;
	character_base *father = nullptr;
	character_base *mother = nullptr;
	std::vector<character_base *> children;
	character_base *contemporary_character = nullptr;
	QDate birth_date;
	QDate death_date;
	QDate start_date;
	calendar *vital_date_calendar = nullptr; //the calendar for the birth and death dates
};

}
