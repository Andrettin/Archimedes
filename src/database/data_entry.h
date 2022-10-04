#pragma once

#include "database/basic_data_entry.h"

namespace archimedes {

class data_entry_history;
class data_module;
class gsml_data;
class gsml_property;
class timeline;

//a (de)serializable and identifiable entry to the database
class data_entry : public basic_data_entry
{
	Q_OBJECT

	Q_PROPERTY(QString identifier READ get_identifier_qstring CONSTANT)

public:
	explicit data_entry(const std::string &identifier);
	virtual ~data_entry();

	const std::string &get_identifier() const
	{
		return this->identifier;
	}

	QString get_identifier_qstring() const
	{
		return QString::fromStdString(this->get_identifier());
	}

	const std::set<std::string> &get_aliases() const
	{
		return this->aliases;
	}

	void add_alias(const std::string &alias)
	{
		this->aliases.insert(alias);
	}

	virtual void process_gsml_property(const gsml_property &property) override;
	virtual void process_gsml_scope(const gsml_data &scope) override;
	virtual void process_gsml_dated_property(const gsml_property &property, const QDateTime &date);
	virtual void process_gsml_dated_scope(const gsml_data &scope, const QDateTime &date);

	bool is_defined() const
	{
		return this->defined;
	}

	void set_defined(const bool defined)
	{
		this->defined = defined;
	}

	bool is_initialized() const
	{
		return this->initialized;
	}

	virtual void initialize();

	virtual void process_text()
	{
	}

	virtual void check() const
	{
	}

	const archimedes::data_module *get_module() const
	{
		return this->data_module;
	}

	void set_module(const archimedes::data_module *data_module)
	{
		if (data_module == this->get_module()) {
			return;
		}

		this->data_module = data_module;
	}

	virtual data_entry_history *get_history_base()
	{
		return nullptr;
	}

	void load_history(const QDateTime &start_date, const timeline *current_timeline);
	void load_date_scope(const gsml_data &date_scope, const QDateTime &date);

	virtual void reset_history()
	{
	}

	virtual bool has_encyclopedia_entry() const
	{
		return false;
	}

	virtual std::string get_link_name() const
	{
		return this->get_identifier();
	}

signals:
	void changed();

private:
	std::string identifier;
	std::set<std::string> aliases;
	bool defined = false; //whether the data entry's definition has been concluded (with its data having been processed)
	bool initialized = false;
	const archimedes::data_module *data_module = nullptr; //the module to which the data entry belongs, if any
	std::vector<gsml_data> history_data;
};

}
