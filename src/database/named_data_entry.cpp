#include "archimedes.h"

#include "database/named_data_entry.h"

#include "language/word.h"
#include "util/text_processor_base.h"

namespace archimedes {

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

std::unique_ptr<text_processor_base> named_data_entry::create_text_processor() const
{
	return nullptr;
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

}
