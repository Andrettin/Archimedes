#pragma once

#include "database/data_entry.h"
#include "language/name_variant.h"

Q_MOC_INCLUDE("language/word.h")

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
	static bool compare_encyclopedia_entries(const named_data_entry *lhs, const named_data_entry *rhs);
	static void concatenate_encyclopedia_text(std::string &text, const std::string &additional_text);

	explicit named_data_entry(const std::string &identifier);
	virtual ~named_data_entry();

	virtual void initialize() override;
	virtual void process_text() override;

	virtual std::string get_encyclopedia_text() const;
	QString get_encyclopedia_text_qstring() const;

	const std::string &get_name() const;
	Q_INVOKABLE void set_name(const std::string &name);
	QString get_name_qstring() const;
	word *get_name_word() const;
	void set_name_word(word *word);
	bool has_name_variant() const;
	name_variant get_name_variant() const;

	virtual std::unique_ptr<text_processor_base> create_text_processor() const;

	virtual std::string get_scope_name() const;
	virtual std::string get_link_name() const override;

	virtual named_data_entry *get_tree_parent() const;
	void add_tree_child(const named_data_entry *data_entry);
	int get_tree_x() const;
	int get_tree_relative_x(const std::vector<const named_data_entry *> &siblings) const;
	virtual int get_tree_y() const;
	int get_tree_width() const;
	virtual std::vector<const named_data_entry *> get_top_tree_elements() const;
	virtual bool is_hidden_in_tree() const;

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
