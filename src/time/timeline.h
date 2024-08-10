#pragma once

#include "database/data_type.h"
#include "database/named_data_entry.h"

namespace archimedes {

class timeline final : public named_data_entry, public data_type<timeline>
{
	Q_OBJECT

	Q_PROPERTY(archimedes::timeline* parent_timeline MEMBER parent_timeline READ get_parent_timeline)
	Q_PROPERTY(QDate point_of_divergence MEMBER point_of_divergence READ get_point_of_divergence)

public:
	static constexpr const char class_identifier[] = "timeline";
	static constexpr const char property_class_identifier[] = "archimedes::timeline*";
	static constexpr const char database_folder[] = "timelines";

	explicit timeline(const std::string &identifier) : named_data_entry(identifier)
	{
	}

	virtual void check() const override;

	timeline *get_parent_timeline() const
	{
		return this->parent_timeline;
	}

	const QDate &get_point_of_divergence() const
	{
		return this->point_of_divergence;
	}

	bool derives_from_timeline(const timeline *timeline) const
	{
		if (this->get_parent_timeline() == nullptr) {
			return false;
		}

		if (timeline == this->get_parent_timeline()) {
			return true;
		}

		return this->get_parent_timeline()->derives_from_timeline(timeline);
	}

	bool contains_timeline_date(const timeline *timeline, const QDate &date) const
	{
		if (timeline == this) {
			return true;
		} else if (this->get_parent_timeline() == nullptr && timeline != nullptr) {
			return false;
		}

		if (timeline == this->get_parent_timeline()) {
			return date < this->point_of_divergence;
		}

		return this->get_parent_timeline()->contains_timeline_date(timeline, date);
	}

private:
	timeline *parent_timeline = nullptr; //the timeline from which this one derives (null means the default timeline)
	QDate point_of_divergence; //the point of divergence from the parent timeline
};

}
