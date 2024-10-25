#pragma once

#include "util/fractional_int.h"
#include "util/singleton.h"

namespace archimedes {

class gsml_data;
class gsml_property;

class defines_base : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QColor green_text_color MEMBER green_text_color READ get_green_text_color NOTIFY changed)
	Q_PROPERTY(QColor red_text_color MEMBER red_text_color READ get_red_text_color NOTIFY changed)

public:
	static const defines_base *get()
	{
		if (defines_base::instance == nullptr) {
			throw std::runtime_error("Failed to get defines_base instance, since no defines object has been instantiated yet.");
		}

		return defines_base::instance;
	}

protected:
	static inline const defines_base *instance = nullptr;

public:
	void load(const std::filesystem::path &base_path);

	virtual void process_gsml_property(const gsml_property &property);
	virtual void process_gsml_scope(const gsml_data &scope);

	virtual void initialize()
	{
	}

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
