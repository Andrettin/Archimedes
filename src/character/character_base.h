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
	Q_PROPERTY(const archimedes::character_base* father MEMBER father READ get_father NOTIFY changed)
	Q_PROPERTY(const archimedes::character_base* mother MEMBER mother READ get_mother NOTIFY changed)
	Q_PROPERTY(QDate birth_date MEMBER birth_date READ get_birth_date NOTIFY changed)
	Q_PROPERTY(QDate death_date MEMBER death_date READ get_death_date NOTIFY changed)
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

	const character_base *get_father() const
	{
		return this->father;
	}

	const character_base *get_mother() const
	{
		return this->mother;
	}

	const QDate &get_birth_date() const
	{
		return this->birth_date;
	}

	const QDate &get_death_date() const
	{
		return this->death_date;
	}

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
	calendar *vital_date_calendar = nullptr; //the calendar for the birth and death dates
};

}
