#pragma once

#include "database/data_type.h"
#include "database/named_data_entry.h"

Q_MOC_INCLUDE("time/calendar.h")

namespace archimedes {

enum class day_of_the_week;
enum class month;

class calendar final : public named_data_entry, public data_type<calendar>
{
	Q_OBJECT

	Q_PROPERTY(archimedes::calendar* base_calendar MEMBER base_calendar)
	Q_PROPERTY(int year_offset MEMBER year_offset READ get_year_offset)
	Q_PROPERTY(QString year_label READ get_year_label_qstring)
	Q_PROPERTY(QString negative_year_label READ get_negative_year_label_qstring)

public:
	static constexpr const char class_identifier[] = "calendar";
	static constexpr const char property_class_identifier[] = "archimedes::calendar*";
	static constexpr const char database_folder[] = "calendars";

	explicit calendar(const std::string &identifier);
	~calendar();

	virtual void process_gsml_scope(const gsml_data &scope) override;
	virtual void initialize() override;

	bool is_any_base_calendar(calendar *calendar) const
	{
		if (this->base_calendar == nullptr) {
			return false;
		}

		if (this->base_calendar == calendar) {
			return true;
		}

		return this->base_calendar->is_any_base_calendar(calendar);
	}

	int get_year_offset() const
	{
		return this->year_offset;
	}

	const std::string &get_year_label() const
	{
		return this->year_label;
	}
	
	QString get_year_label_qstring() const
	{
		return QString::fromStdString(this->get_year_label());
	}

	Q_INVOKABLE void set_year_label(const std::string &year_label)
	{
		this->year_label = year_label;
	}
	
	const std::string &get_negative_year_label() const
	{
		return this->negative_year_label;
	}
	
	QString get_negative_year_label_qstring() const
	{
		return QString::fromStdString(this->get_negative_year_label());
	}

	Q_INVOKABLE void set_negative_year_label(const std::string &year_label)
	{
		this->negative_year_label = year_label;
	}
	
private:
	calendar *base_calendar = nullptr; //the base calendar, used to ultimately calculate the year offset to the Gregorian calendar from
	int year_offset = 0; //the offset from the Gregorian calendar, in years
	std::string year_label;									/// label used for years (e.g. AD)
	std::string negative_year_label;						/// label used for "negative" years (e.g. BC)
	std::map<day_of_the_week, std::string> day_of_the_week_names; //the names of the days of the week in the calendar
	std::map<month, std::string> month_names; //the months in the calendar
};

}
