#include "archimedes.h"

#include "language/word.h"

#include "language/grammatical_gender.h"
#include "language/language.h"
#include "language/word_type.h"
#include "util/container_util.h"
#include "util/vector_util.h"

namespace archimedes {

bool word::compare(const word *lhs, const word *rhs)
{
	if (lhs->get_language() != rhs->get_language()) {
		if ((lhs->get_language() == nullptr) != (rhs->get_language() == nullptr)) {
			return lhs->get_language() != nullptr;
		}

		return lhs->get_language()->get_identifier() < rhs->get_language()->get_identifier();
	}

	return lhs->get_identifier() < rhs->get_identifier();
}

word::word(const std::string &identifier)
	: named_data_entry(identifier), type(word_type::none), gender(grammatical_gender::none)
{
}

void word::process_gsml_scope(const gsml_data &scope)
{
	const std::string &tag = scope.get_tag();
	const std::vector<std::string> &values = scope.get_values();

	if (tag == "compound_elements") {
		for (const std::string &value : values) {
			word *other_word = word::get(value);
			this->add_compound_element(other_word);
		}
	} else {
		data_entry::process_gsml_scope(scope);
	}
}

void word::initialize()
{
	std::sort(this->reflexes.begin(), this->reflexes.end(), word::compare);
	std::sort(this->compound_element_of.begin(), this->compound_element_of.end(), word::compare);

	data_entry::initialize();
}

std::string word::get_encyclopedia_text() const
{
	std::string text;

	if (this->get_language() != nullptr) {
		named_data_entry::concatenate_encyclopedia_text(text, "Language: " + this->get_language()->get_name());
	}

	if (!this->get_meanings().empty()) {
		std::string meanings_text;
		for (const std::string &meaning : this->get_meanings()) {
			if (!meanings_text.empty()) {
				meanings_text += ", ";
			}

			meanings_text += "\"" + meaning + "\"";
		}

		named_data_entry::concatenate_encyclopedia_text(text, "Meaning: " + meanings_text);
	}

	if (this->get_type() != word_type::none) {
		named_data_entry::concatenate_encyclopedia_text(text, "Type: " + word_type_to_name(this->get_type()));
	}

	if (this->get_gender() != grammatical_gender::none) {
		named_data_entry::concatenate_encyclopedia_text(text, "Gender: " + grammatical_gender_to_name(this->get_gender()));
	}

	if (this->get_etymon() != nullptr) {
		named_data_entry::concatenate_encyclopedia_text(text, "Derives From: " + this->get_etymon()->get_link_string());
	}

	if (!this->compound_elements.empty()) {
		std::string compound_elements_text;
		for (const word *compound_element : this->compound_elements) {
			if (!compound_elements_text.empty()) {
				compound_elements_text += ", ";
			}

			compound_elements_text += compound_element->get_link_string();
		}

		named_data_entry::concatenate_encyclopedia_text(text, "Compound Elements: " + compound_elements_text);
	}

	if (!this->get_reflexes().empty()) {
		std::string reflexes_text;
		for (const word *reflex : this->get_reflexes()) {
			if (!reflexes_text.empty()) {
				reflexes_text += ", ";
			}

			reflexes_text += reflex->get_link_string();
		}

		named_data_entry::concatenate_encyclopedia_text(text, "Derives To: " + reflexes_text);
	}

	if (!this->compound_element_of.empty()) {
		std::string compounds_text;
		for (const word *compound : this->compound_element_of) {
			if (!compounds_text.empty()) {
				compounds_text += ", ";
			}

			compounds_text += compound->get_link_string();
		}

		named_data_entry::concatenate_encyclopedia_text(text, "Compound Element of: " + compounds_text);
	}

	return text;
}

void word::set_language(archimedes::language *language)
{
	if (language == this->get_language()) {
		return;
	}

	this->language = language;
	language->add_word(this);
}

QStringList word::get_meanings_qstring_list() const
{
	return container::to_qstring_list(this->get_meanings());
}

void word::remove_meaning(const std::string &meaning)
{
	vector::remove_one(this->meanings, meaning);
}

std::string word::GetNounInflection(int grammatical_number, int grammatical_case, int word_junction_type)
{
	auto find_iterator = this->NumberCaseInflections.find(std::make_tuple(grammatical_number, grammatical_case));
	if (find_iterator != this->NumberCaseInflections.end()) {
		return find_iterator->second;
	}

	return this->get_name() + this->language->GetNounEnding(grammatical_number, grammatical_case, word_junction_type);
}

const std::string &word::GetVerbInflection(int grammatical_number, int grammatical_person, int grammatical_tense, int grammatical_mood)
{
	auto find_iterator = this->NumberPersonTenseMoodInflections.find(std::make_tuple(grammatical_number, grammatical_person, grammatical_tense, grammatical_mood));
	if (find_iterator != this->NumberPersonTenseMoodInflections.end()) {
		return find_iterator->second;
	}

	return this->get_name();
}

std::string word::GetAdjectiveInflection(int comparison_degree, int article_type, int grammatical_case, int grammatical_number, const grammatical_gender grammatical_gender)
{
	std::string inflected_word;

	if (grammatical_case == -1) {
		grammatical_case = GrammaticalCaseNoCase;
	}

	if (!this->ComparisonDegreeCaseInflections[comparison_degree][grammatical_case].empty()) {
		inflected_word = this->ComparisonDegreeCaseInflections[comparison_degree][grammatical_case];
	} else if (!this->ComparisonDegreeCaseInflections[comparison_degree][GrammaticalCaseNoCase].empty()) {
		inflected_word = this->ComparisonDegreeCaseInflections[comparison_degree][GrammaticalCaseNoCase];
	} else {
		inflected_word = this->get_name();
	}

	if (article_type != -1 && grammatical_case != GrammaticalCaseNoCase && this->ComparisonDegreeCaseInflections[comparison_degree][grammatical_case].empty()) {
		inflected_word += this->language->GetAdjectiveEnding(article_type, grammatical_case, grammatical_number, grammatical_gender);
	}

	return inflected_word;
}

const std::string &word::GetParticiple(int grammatical_tense)
{
	if (!this->Participles[grammatical_tense].empty()) {
		return this->Participles[grammatical_tense];
	}

	return this->get_name();
}

}
