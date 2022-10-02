#pragma once

namespace archimedes {

class gsml_data;
class gsml_property;

class data_module final : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString name READ get_name_qstring)
	Q_PROPERTY(std::string license MEMBER license)
	Q_PROPERTY(std::string version MEMBER version)

public:
	explicit data_module(const std::string &identifier, const std::filesystem::path &path, const data_module *parent_module)
		: identifier(identifier), path(path), parent_module(parent_module)
	{
	}

	void process_gsml_property(const gsml_property &property);
	void process_gsml_scope(const gsml_data &scope);

	const std::string &get_identifier() const
	{
		return this->identifier;
	}

	const std::string &get_name() const
	{
		return this->name;
	}

	Q_INVOKABLE void set_name(const std::string &name)
	{
		this->name = name;
	}

	QString get_name_qstring() const
	{
		return QString::fromStdString(this->get_name());
	}

	const std::filesystem::path &get_path() const
	{
		return this->path;
	}

	void add_dependency(const data_module *data_module)
	{
		if (data_module->depends_on(this)) {
			throw std::runtime_error("Cannot make module \"" + this->identifier + "\" depend on module \"" + data_module->identifier + "\", as that would create a circular dependency.");
		}

		this->dependencies.insert(data_module);
	}

	bool depends_on(const data_module *data_module) const
	{
		if (data_module == this->parent_module) {
			return true;
		}

		if (this->dependencies.contains(data_module)) {
			return true;
		}

		for (const archimedes::data_module *dependency : this->dependencies) {
			if (dependency->depends_on(data_module)) {
				return true;
			}
		}

		if (this->parent_module != nullptr) {
			return this->parent_module->depends_on(data_module);
		}

		return false;
	}

	size_t get_dependency_count() const
	{
		size_t count = this->dependencies.size();

		for (const archimedes::data_module *dependency : this->dependencies) {
			count += dependency->get_dependency_count();
		}

		return count;
	}

private:
	std::string identifier;
	std::string name;
	std::string license;
	std::string version;
	std::filesystem::path path; //the module's path
	const data_module *parent_module = nullptr;
	std::set<const data_module *> dependencies; //modules on which this one is dependent
};

}
