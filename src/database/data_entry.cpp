#include "archimedes.h"

#include "database/data_entry.h"

#include "database/data_entry_history.h"
#include "database/database.h"
#include "time/calendar.h"
#include "time/timeline.h"
#include "util/date_util.h"
#include "util/string_conversion_util.h"

namespace archimedes {

data_entry::data_entry(const std::string &identifier) : identifier(identifier)
{
}

data_entry::~data_entry()
{
}

void data_entry::process_gsml_property(const gsml_property &property)
{
	if (property.get_key() == "aliases") {
		return; //alias addition is already handled in the data type class
	}

	basic_data_entry::process_gsml_property(property);
}

void data_entry::process_gsml_scope(const gsml_data &scope)
{
	if (scope.get_tag() == "history") {
		this->history_data.push_back(scope);
	} else {
		basic_data_entry::process_gsml_scope(scope);
	}
}

void data_entry::process_gsml_dated_property(const gsml_property &property, const QDateTime &date)
{
	Q_UNUSED(date)

	try {
		this->get_history_base()->process_gsml_property(property);
	} catch (...) {
		std::throw_with_nested(std::runtime_error("Error processing history property \"" + property.get_key() + "\"."));
	}
}

void data_entry::process_gsml_dated_scope(const gsml_data &scope, const QDateTime &date)
{
	Q_UNUSED(date)

	try {
		this->get_history_base()->process_gsml_scope(scope);
	} catch (...) {
		std::throw_with_nested(std::runtime_error("Error processing history scope \"" + scope.get_tag() + "\"."));
	}
}

void data_entry::initialize()
{
	if (this->is_initialized()) {
		throw std::runtime_error("Tried to initialize data entry \"" + this->get_identifier() + "\", even though it has already been initialized.");
	}

	if (this->parent() == nullptr) {
		QMetaObject::invokeMethod(this, [this]() {
			this->setParent(QApplication::instance()); //to ensure that the object isn't deleted by QML
		}, Qt::QueuedConnection);
	}

	this->initialized = true;
}

void data_entry::load_history(const QDateTime &start_date, const timeline *current_timeline)
{
	this->reset_history();

	std::map<QDateTime, std::vector<const gsml_data *>> history_entries;

	for (const gsml_data &data : this->history_data) {
		data.for_each_property([&](const gsml_property &property) {
			this->process_gsml_dated_property(property, QDateTime()); //properties outside of a date scope, to be applied regardless of start date
		});

		data.for_each_child([&](const gsml_data &history_entry) {
			const timeline *timeline = nullptr;
			const calendar *calendar = nullptr;

			if (!std::isdigit(history_entry.get_tag().front()) && history_entry.get_tag().front() != '-') {
				timeline = timeline::try_get(history_entry.get_tag());

				if (timeline == nullptr) {
					calendar = calendar::try_get(history_entry.get_tag());

					if (calendar == nullptr) {
						//treat the scope as a property to be applied immediately
						this->process_gsml_dated_scope(history_entry, QDateTime());
						return;
					}
				}
			}

			if (timeline != nullptr) {
				if (current_timeline == nullptr || (current_timeline != timeline && !current_timeline->derives_from_timeline(timeline))) {
					return;
				}

				history_entry.for_each_child([&](const gsml_data &timeline_entry) {
					QDateTime date = string::to_date(timeline_entry.get_tag());
					if (date::contains_date(start_date, current_timeline, date, timeline)) {
						history_entries[date].push_back(&timeline_entry);
					}
				});
			} else if (calendar != nullptr) {
				history_entry.for_each_child([&](const gsml_data &calendar_entry) {
					QDateTime date = string::to_date(calendar_entry.get_tag());
					date = date.addYears(calendar->get_year_offset());

					if (date::contains_date(start_date, current_timeline, date)) {
						history_entries[date].push_back(&calendar_entry);
					}
				});
			} else {
				QDateTime date = string::to_date(history_entry.get_tag());

				if (date::contains_date(start_date, current_timeline, date)) {
					history_entries[date].push_back(&history_entry);
				}
			}
		});
	}

	for (const auto &kv_pair : history_entries) {
		const QDateTime &date = kv_pair.first;
		const std::vector<const gsml_data *> &date_history_entries = kv_pair.second;

		for (const gsml_data *history_entry : date_history_entries) {
			this->load_date_scope(*history_entry, date);
		}
	}
}

void data_entry::load_date_scope(const gsml_data &date_scope, const QDateTime &date)
{
	try {
		date_scope.for_each_element([&](const gsml_property &property) {
			this->process_gsml_dated_property(property, date);
		}, [&](const gsml_data &scope) {
			this->process_gsml_dated_scope(scope, date);
		});
	} catch (...) {
		std::throw_with_nested(std::runtime_error("Error loading history for data entry instance \"" + this->get_identifier() + "\", for the " + date::to_string(date) + " date."));
	}
}

}
