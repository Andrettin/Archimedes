#pragma once

#include "database/data_type.h"
#include "database/named_data_entry.h"

Q_MOC_INCLUDE("time/calendar.h")

namespace archimedes {

class calendar;
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

	std::string get_full_name() const;

	QString get_full_name_qstring() const
	{
		return QString::fromStdString(this->get_full_name());
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

	const character_base *get_father() const
	{
		return this->father;
	}

	void set_father(const character_base *father)
	{
		this->father = father;
	}

	const character_base *get_mother() const
	{
		return this->mother;
	}

	void set_mother(const character_base *mother)
	{
		this->mother = mother;
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

	virtual void initialize_dates();

signals:
	void changed();

private:
	std::string surname;
	std::string nickname;
	std::string epithet;
	std::string description;
	archimedes::gender gender;
	const character_base *father = nullptr;
	const character_base *mother = nullptr;
	QDate birth_date;
	QDate death_date;
	QDate start_date;
	calendar *vital_date_calendar = nullptr; //the calendar for the birth and death dates
};

}
