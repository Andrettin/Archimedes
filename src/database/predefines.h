#pragma once

#include "util/singleton.h"

namespace archimedes {

class gsml_data;
class gsml_property;

class predefines final : public QObject, public singleton<predefines>
{
	Q_OBJECT

	Q_PROPERTY(bool documents_modules_loading_enabled MEMBER documents_modules_loading_enabled READ is_documents_modules_loading_enabled)

public:
	void load(const std::filesystem::path &base_path);
	void process_gsml_property(const gsml_property &property);
	void process_gsml_scope(const gsml_data &scope);

	bool is_documents_modules_loading_enabled() const
	{
		return this->documents_modules_loading_enabled;
	}

private:
	bool documents_modules_loading_enabled = true;
};

}
