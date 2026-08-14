#pragma once

#include "database/defines_base.h"

namespace archimedes {

class ui_defines_base : public defines_base
{
	Q_OBJECT

	Q_PROPERTY(QColor green_text_color MEMBER green_text_color READ get_green_text_color NOTIFY changed)
	Q_PROPERTY(QColor red_text_color MEMBER red_text_color READ get_red_text_color NOTIFY changed)

public:
	static const ui_defines_base *get()
	{
		if (ui_defines_base::instance == nullptr) {
			throw std::runtime_error("Failed to get ui_defines_base instance, since no UI defines object has been instantiated yet.");
		}

		return ui_defines_base::instance;
	}

	virtual std::string_view get_file_name() const override
	{
		return "ui_defines.txt";
	}

protected:
	static inline const ui_defines_base *instance = nullptr;

public:
	const QColor &get_green_text_color() const
	{
		return this->green_text_color;
	}

	const QColor &get_red_text_color() const
	{
		return this->red_text_color;
	}

signals:
	void changed();

private:
	QColor green_text_color;
	QColor red_text_color;
};

}
