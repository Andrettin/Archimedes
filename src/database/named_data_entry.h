#pragma once

#include "database/data_entry.h"
#include "language/name_variant.h"

namespace archimedes {

class text_processor_base;
class word;

class named_data_entry : public data_entry
{
	Q_OBJECT

	Q_PROPERTY(QString name READ get_name_qstring NOTIFY changed)
	Q_PROPERTY(archimedes::word* name_word READ get_name_word WRITE set_name_word NOTIFY changed)
	Q_PROPERTY(QString encyclopedia_text READ get_encyclopedia_text_qstring NOTIFY encyclopedia_text_changed)
	Q_PROPERTY(int tree_x READ get_tree_x CONSTANT)
	Q_PROPERTY(int tree_y READ get_tree_y CONSTANT)
	Q_PROPERTY(int tree_width READ get_tree_width CONSTANT)
	Q_PROPERTY(bool tree_line_visible MEMBER tree_line_visible NOTIFY changed)

public:
	static bool compare_encyclopedia_entries(const named_data_entry *lhs, const named_data_entry *rhs)
	{
		return lhs->get_name() < rhs->get_name();
	}

	static void concatenate_encyclopedia_text(std::string &text, const std::string &additional_text)
	{
		if (additional_text.empty()) {
			return;
		}

		if (!text.empty()) {
			text += "\n\n";
		}

		text += additional_text;
	}

	explicit named_data_entry(const std::string &identifier) : data_entry(identifier)
	{
	}

	virtual ~named_data_entry() {}

	virtual void initialize() override;
	virtual void process_text() override;

	virtual std::string get_encyclopedia_text() const
	{
		return std::string();
	}

	QString get_encyclopedia_text_qstring() const
	{
		return QString::fromStdString(this->get_encyclopedia_text());
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

	word *get_name_word() const
	{
		return this->name_word;
	}

	void set_name_word(word *word)
	{
		if (word == this->name_word) {
			return;
		}

		this->name_word = word;
	}

	bool has_name_variant() const
	{
		return this->get_name_word() != nullptr || !this->get_name().empty();
	}

	name_variant get_name_variant() const
	{
		if (this->get_name_word() != nullptr) {
			return this->get_name_word();
		}

		return this->get_name();
	}

	virtual std::unique_ptr<text_processor_base> create_text_processor() const;

	virtual std::string get_scope_name() const
	{
		return this->get_name();
	}

	virtual std::string get_link_name() const override
	{
		return this->get_name();
	}

	virtual named_data_entry *get_tree_parent() const
	{
		return nullptr;
	}

	void add_tree_child(const named_data_entry *data_entry)
	{
		this->tree_children.push_back(data_entry);
	}

	int get_tree_x() const;
	int get_tree_relative_x(const std::vector<const named_data_entry *> &siblings) const;
	int get_tree_y() const;
	int get_tree_width() const;

	virtual std::vector<const named_data_entry *> get_top_tree_elements() const
	{
		return {};
	}

	virtual bool is_hidden_in_tree() const
	{
		return false;
	}

signals:
	void encyclopedia_text_changed();
	void changed();

private:
	std::string name;
	word *name_word = nullptr;
	std::vector<const named_data_entry *> tree_children;
	bool tree_line_visible = true;
};

}
