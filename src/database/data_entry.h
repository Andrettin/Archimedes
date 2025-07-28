#pragma once

#include "database/basic_data_entry.h"

namespace archimedes {

class data_entry_history;
class data_module;
class game_rules_base;
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

	const std::string &get_identifier() const;
	QString get_identifier_qstring() const;
	const std::set<std::string> &get_aliases() const;
	void add_alias(const std::string &alias);

	virtual void process_gsml_property(const gsml_property &property) override;
	virtual void process_gsml_scope(const gsml_data &scope) override;
	virtual void process_gsml_dated_property(const gsml_property &property, const QDate &date);
	virtual void process_gsml_dated_scope(const gsml_data &scope, const QDate &date);

	bool is_defined() const;
	void set_defined(const bool defined);

	bool is_initialized() const;
	virtual void initialize();
	virtual void process_text();
	virtual void check() const;

	const archimedes::data_module *get_module() const;
	void set_module(const archimedes::data_module *data_module);

	virtual data_entry_history *get_history_base();
	void load_history(const QDate &start_date, const timeline *current_timeline, const game_rules_base *game_rules);
	void load_history_scope(const gsml_data &history_scope, const QDate &start_date, const timeline *current_timeline, const game_rules_base *game_rules, std::map<QDate, std::vector<const gsml_data *>> &history_entries);
	void load_date_scope(const gsml_data &date_scope, const QDate &date);
	virtual void reset_history();

	virtual bool has_encyclopedia_entry() const;
	virtual std::string get_link_name() const;

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
