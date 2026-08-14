#pragma once

namespace archimedes {

class gsml_data;
class gsml_property;

class defines_base : public QObject
{
	Q_OBJECT

public:
	virtual std::string_view get_file_name() const = 0;

	void load(const std::filesystem::path &base_path);

	virtual void process_gsml_property(const gsml_property &property);
	virtual void process_gsml_scope(const gsml_data &scope);

	virtual void initialize()
	{
	}

	virtual void check() const
	{
	}
};

}
