#include "archimedes.h"

#include "database/named_data_entry.h"

#include "language/word.h"
#include "util/text_processor_base.h"

namespace archimedes {

bool named_data_entry::compare_encyclopedia_entries(const named_data_entry *lhs, const named_data_entry *rhs)
{
	return lhs->get_name() < rhs->get_name();
}

void named_data_entry::concatenate_encyclopedia_text(std::string &text, const std::string &additional_text)
{
	if (additional_text.empty()) {
		return;
	}

	if (!text.empty()) {
		text += "\n\n";
	}

	text += additional_text;
}

named_data_entry::named_data_entry(const std::string &identifier) : data_entry(identifier)
{
}

named_data_entry::~named_data_entry()
{
}

void named_data_entry::initialize()
{
	if (this->get_name_word() != nullptr && this->name.empty()) {
		this->name = this->get_name_word()->get_anglicized_name();
	}

	if (this->get_tree_parent() != nullptr) {
		this->get_tree_parent()->add_tree_child(this);
	}

	data_entry::initialize();
}

void named_data_entry::process_text()
{
	//process the name for the named data entry
	if (!this->name.empty()) {
		const std::unique_ptr<text_processor_base> text_processor = this->create_text_processor();
		if (text_processor != nullptr) {
			this->name = text_processor->process_text(std::move(this->name), false);
		}
	}

	data_entry::process_text();
}

std::string named_data_entry::get_encyclopedia_text() const
{
	return std::string();
}

QString named_data_entry::get_encyclopedia_text_qstring() const
{
	return QString::fromStdString(this->get_encyclopedia_text());
}

const std::string &named_data_entry::get_name() const
{
	return this->name;
}

void named_data_entry::set_name(const std::string &name)
{
	this->name = name;
}

QString named_data_entry::get_name_qstring() const
{
	return QString::fromStdString(this->get_name());
}

word *named_data_entry::get_name_word() const
{
	return this->name_word;
}

void named_data_entry::set_name_word(word *word)
{
	if (word == this->name_word) {
		return;
	}

	this->name_word = word;
}

bool named_data_entry::has_name_variant() const
{
	return this->get_name_word() != nullptr || !this->get_name().empty();
}

name_variant named_data_entry::get_name_variant() const
{
	if (this->get_name_word() != nullptr) {
		return this->get_name_word();
	}

	return this->get_name();
}

std::unique_ptr<text_processor_base> named_data_entry::create_text_processor() const
{
	return nullptr;
}

std::string named_data_entry::get_scope_name() const
{
	return this->get_name();
}

std::string named_data_entry::get_link_name() const
{
	return this->get_name();
}

named_data_entry *named_data_entry::get_tree_parent() const
{
	return nullptr;
}

void named_data_entry::add_tree_child(const named_data_entry *data_entry)
{
	this->tree_children.push_back(data_entry);
}

int named_data_entry::get_tree_x() const
{
	if (this->get_tree_parent() != nullptr) {
		return this->get_tree_parent()->get_tree_x() + this->get_tree_relative_x(this->get_tree_parent()->tree_children);
	}

	std::vector<const named_data_entry *> siblings;

	for (const named_data_entry *data_entry : this->get_top_tree_elements()) {
		if (data_entry->get_tree_parent() != nullptr) {
			continue;
		}

		siblings.push_back(data_entry);
	}

	return this->get_tree_relative_x(siblings);
}

int named_data_entry::get_tree_relative_x(const std::vector<const named_data_entry *> &siblings) const
{
	int relative_x = 0;

	for (const named_data_entry *data_entry : siblings) {
		if (data_entry == this) {
			break;
		}

		if (data_entry->is_hidden_in_tree()) {
			continue;
		}

		relative_x += data_entry->get_tree_width();
	}

	return relative_x;
}

int named_data_entry::get_tree_y() const
{
	if (this->get_tree_parent() != nullptr) {
		return this->get_tree_parent()->get_tree_y() + 1;
	}

	return 0;
}

int named_data_entry::get_tree_width() const
{
	int children_width = 0;

	for (const named_data_entry *data_entry : this->tree_children) {
		if (data_entry->is_hidden_in_tree()) {
			continue;
		}

		children_width += data_entry->get_tree_width();
	}

	return std::max(children_width, 1);
}

std::vector<const named_data_entry *> named_data_entry::get_top_tree_elements() const
{
	return {};
}

bool named_data_entry::is_hidden_in_tree() const
{
	return false;
}

}
