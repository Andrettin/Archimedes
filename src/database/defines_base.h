#pragma once

#include "util/fractional_int.h"
#include "util/singleton.h"

namespace archimedes {

class gsml_data;
class gsml_property;

class defines_base : public QObject
{
	Q_OBJECT

public:
	void load(const std::filesystem::path &base_path);

	virtual void process_gsml_property(const gsml_property &property);
	virtual void process_gsml_scope(const gsml_data &scope);

	virtual void initialize()
	{
	}
};

}
