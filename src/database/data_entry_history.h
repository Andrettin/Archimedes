#pragma once

namespace archimedes {

class gsml_data;
class gsml_property;

class data_entry_history : public QObject
{
	Q_OBJECT

protected:
	data_entry_history()
	{
	}

public:
	virtual ~data_entry_history()
	{
	}

	virtual void process_gsml_property(const gsml_property &property, const QDate &date);
	virtual void process_gsml_scope(const gsml_data &scope, const QDate &date);
};

}
