#pragma once

namespace archimedes {

class gsml_data;
class gsml_property;

class basic_data_entry : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString class_name READ get_class_name CONSTANT)

public:
	virtual void process_gsml_property(const gsml_property &property);
	virtual void process_gsml_scope(const gsml_data &scope);
	void process_gsml_data(const gsml_data &data);

	QString get_class_name() const
	{
		return this->metaObject()->className();
	}
};

}
