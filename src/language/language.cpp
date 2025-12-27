#include "archimedes.h"

#include "language/language.h"

#include "language/grammatical_gender.h"
#include "language/word.h"
#include "language/word_type.h"
#include "util/assert_util.h"
#include "util/path_util.h"
#include "util/string_util.h"

namespace archimedes {

word *language::GetWord(const std::string &word, const word_type word_type, const std::vector<std::string> &word_meanings) const
{
	for (archimedes::word *language_word : this->words) {
		if (
			language_word->get_name() == word
			&& (word_type == word_type::none || language_word->get_type() == word_type)
			&& (word_meanings.size() == 0 || language_word->get_meanings() == word_meanings)
		) {
			return language_word;
		}
	}

	return nullptr;
}

const std::string &language::GetArticle(const grammatical_gender gender, int grammatical_case, int article_type, int grammatical_number)
{
	for (const word *word : this->words) {
		if (word->get_type() != word_type::article || word->ArticleType != article_type) {
			continue;
		}

		if (grammatical_number != -1 && word->GrammaticalNumber != -1 && word->GrammaticalNumber != grammatical_number) {
			continue;
		}

		if (gender == grammatical_gender::none || word->get_gender() == grammatical_gender::none || gender == word->get_gender()) {
			if (grammatical_case == GrammaticalCaseNominative && !word->Nominative.empty()) {
				return word->Nominative;
			} else if (grammatical_case == GrammaticalCaseAccusative && !word->Accusative.empty()) {
				return word->Accusative;
			} else if (grammatical_case == GrammaticalCaseDative && !word->Dative.empty()) {
				return word->Dative;
			} else if (grammatical_case == GrammaticalCaseGenitive && !word->Genitive.empty()) {
				return word->Genitive;
			}
		}
	}
	return string::empty_str;
}

std::string language::GetNounEnding(int grammatical_number, int grammatical_case, int word_junction_type)
{
	if (word_junction_type == -1) {
		word_junction_type = WordJunctionTypeNoWordJunction;
	}

	if (!this->NounEndings[grammatical_number][grammatical_case][word_junction_type].empty()) {
		return this->NounEndings[grammatical_number][grammatical_case][word_junction_type];
	} else if (!this->NounEndings[grammatical_number][grammatical_case][WordJunctionTypeNoWordJunction].empty()) {
		return this->NounEndings[grammatical_number][grammatical_case][WordJunctionTypeNoWordJunction];
	}

	return "";
}

std::string language::GetAdjectiveEnding(int article_type, int grammatical_case, int grammatical_number, const grammatical_gender grammatical_gender)
{
	if (grammatical_number == -1) {
		grammatical_number = GrammaticalNumberNoNumber;
	}

	auto find_iterator = this->AdjectiveEndings[article_type][grammatical_case][grammatical_number].find(grammatical_gender);
	if (find_iterator != this->AdjectiveEndings[article_type][grammatical_case][grammatical_number].end()) {
		return find_iterator->second;
	}

	find_iterator = this->AdjectiveEndings[article_type][grammatical_case][grammatical_number].find(grammatical_gender::none);
	if (find_iterator != this->AdjectiveEndings[article_type][grammatical_case][grammatical_number].end()) {
		return find_iterator->second;
	}

	find_iterator = this->AdjectiveEndings[article_type][grammatical_case][GrammaticalNumberNoNumber].find(grammatical_gender::none);
	if (find_iterator != this->AdjectiveEndings[article_type][grammatical_case][GrammaticalNumberNoNumber].end()) {
		return find_iterator->second;
	}

	return std::string();
}

void language::print_df_words() const
{
	const std::string filename = std::format("language_{}.txt", this->get_identifier());
	std::ofstream ofstream(path::from_string(filename));

	if (!ofstream) {
		throw std::runtime_error(std::format("Failed to open file \"{}\" for printing DF words to.", filename));
	}

	ofstream << std::format("language_{}", this->get_identifier());
	ofstream << "\n\n" << "[OBJECT:LANGUAGE]";
	ofstream << "\n\n" << std::format("[TRANSLATION:{}]", string::uppered(this->get_identifier()));

	std::map<std::string, std::vector<const word *>> df_words_to_words;

	for (const word *word : this->words) {
		const std::vector<std::string> df_words = word->get_df_words();
		for (const std::string &df_word : df_words) {
			df_words_to_words[df_word].push_back(word);
		}
	}

	for (auto &[df_word, potential_words] : df_words_to_words) {
		const word *chosen_word = nullptr;
		for (const word *word : potential_words) {
			if (chosen_word == nullptr || word->get_name().length() < chosen_word->get_name().length()) {
				chosen_word = word;
			}
		}
		assert_throw(chosen_word != nullptr);

		std::string word_str = chosen_word->get_name();
		string::to_lower(word_str);
		word_str = string::to_code_page_437(word_str);

		ofstream << "\n\t" << std::format("[T_WORD:{}:{}]", df_word, word_str);
	}

	ofstream << "\n";
}

}

std::string GetArticleTypeNameById(int article_type)
{
	if (article_type == ArticleTypeNoArticle) {
		return "no-article";
	} else if (article_type == ArticleTypeDefinite) {
		return "definite";
	} else if (article_type == ArticleTypeIndefinite) {
		return "indefinite";
	}

	return "";
}

int GetArticleTypeIdByName(const std::string &article_type)
{
	if (article_type == "no-article") {
		return ArticleTypeNoArticle;
	} else if (article_type == "definite") {
		return ArticleTypeDefinite;
	} else if (article_type == "indefinite") {
		return ArticleTypeIndefinite;
	}

	return -1;
}

std::string GetGrammaticalCaseNameById(int grammatical_case)
{
	if (grammatical_case == GrammaticalCaseNoCase) {
		return "no-case";
	} else if (grammatical_case == GrammaticalCaseNominative) {
		return "nominative";
	} else if (grammatical_case == GrammaticalCaseAccusative) {
		return "accusative";
	} else if (grammatical_case == GrammaticalCaseDative) {
		return "dative";
	} else if (grammatical_case == GrammaticalCaseGenitive) {
		return "genitive";
	}

	return "";
}

int GetGrammaticalCaseIdByName(const std::string &grammatical_case)
{
	if (grammatical_case == "no-case") {
		return GrammaticalCaseNoCase;
	} else if (grammatical_case == "nominative") {
		return GrammaticalCaseNominative;
	} else if (grammatical_case == "accusative") {
		return GrammaticalCaseAccusative;
	} else if (grammatical_case == "dative") {
		return GrammaticalCaseDative;
	} else if (grammatical_case == "genitive") {
		return GrammaticalCaseGenitive;
	}

	return -1;
}

std::string GetGrammaticalNumberNameById(int grammatical_number)
{
	if (grammatical_number == GrammaticalNumberNoNumber) {
		return "no-number";
	} else if (grammatical_number == GrammaticalNumberSingular) {
		return "singular";
	} else if (grammatical_number == GrammaticalNumberPlural) {
		return "plural";
	}

	return "";
}

int GetGrammaticalNumberIdByName(const std::string &grammatical_number)
{
	if (grammatical_number == "no-number") {
		return GrammaticalNumberNoNumber;
	} else if (grammatical_number == "singular") {
		return GrammaticalNumberSingular;
	} else if (grammatical_number == "plural") {
		return GrammaticalNumberPlural;
	}

	return -1;
}

std::string GetGrammaticalPersonNameById(int grammatical_person)
{
	if (grammatical_person == GrammaticalPersonFirstPerson) {
		return "first-person";
	} else if (grammatical_person == GrammaticalPersonSecondPerson) {
		return "second-person";
	} else if (grammatical_person == GrammaticalPersonThirdPerson) {
		return "third-person";
	}

	return "";
}

int GetGrammaticalPersonIdByName(const std::string &grammatical_person)
{
	if (grammatical_person == "first-person") {
		return GrammaticalPersonFirstPerson;
	} else if (grammatical_person == "second-person") {
		return GrammaticalPersonSecondPerson;
	} else if (grammatical_person == "third-person") {
		return GrammaticalPersonThirdPerson;
	}

	return -1;
}

std::string GetGrammaticalTenseNameById(int grammatical_tense)
{
	if (grammatical_tense == GrammaticalTenseNoTense) {
		return "no-tense";
	} else if (grammatical_tense == GrammaticalTensePresent) {
		return "present";
	} else if (grammatical_tense == GrammaticalTensePast) {
		return "past";
	} else if (grammatical_tense == GrammaticalTenseFuture) {
		return "future";
	}

	return "";
}

int GetGrammaticalTenseIdByName(const std::string &grammatical_tense)
{
	if (grammatical_tense == "no-tense") {
		return GrammaticalTenseNoTense;
	} else if (grammatical_tense == "present") {
		return GrammaticalTensePresent;
	} else if (grammatical_tense == "past") {
		return GrammaticalTensePast;
	} else if (grammatical_tense == "future") {
		return GrammaticalTenseFuture;
	}

	return -1;
}

std::string GetGrammaticalMoodNameById(int grammatical_mood)
{
	if (grammatical_mood == GrammaticalMoodIndicative) {
		return "indicative";
	} else if (grammatical_mood == GrammaticalMoodSubjunctive) {
		return "subjunctive";
	}

	return "";
}

int GetGrammaticalMoodIdByName(const std::string &grammatical_mood)
{
	if (grammatical_mood == "indicative") {
		return GrammaticalMoodIndicative;
	} else if (grammatical_mood == "subjunctive") {
		return GrammaticalMoodSubjunctive;
	}

	return -1;
}

std::string GetComparisonDegreeNameById(int comparison_degree)
{
	if (comparison_degree == ComparisonDegreePositive) {
		return "positive";
	} else if (comparison_degree == ComparisonDegreeComparative) {
		return "comparative";
	} else if (comparison_degree == ComparisonDegreeSuperlative) {
		return "superlative";
	}

	return "";
}

int GetComparisonDegreeIdByName(const std::string &comparison_degree)
{
	if (comparison_degree == "positive") {
		return ComparisonDegreePositive;
	} else if (comparison_degree == "comparative") {
		return ComparisonDegreeComparative;
	} else if (comparison_degree == "superlative") {
		return ComparisonDegreeSuperlative;
	}

	return -1;
}

std::string GetAffixTypeNameById(int affix_type)
{
	if (affix_type == AffixTypePrefix) {
		return "prefix";
	} else if (affix_type == AffixTypeSuffix) {
		return "suffix";
	} else if (affix_type == AffixTypeInfix) {
		return "infix";
	}

	return "";
}

int GetAffixTypeIdByName(const std::string &affix_type)
{
	if (affix_type == "prefix") {
		return AffixTypePrefix;
	} else if (affix_type == "suffix") {
		return AffixTypeSuffix;
	} else if (affix_type == "infix") {
		return AffixTypeInfix;
	}

	return -1;
}

std::string GetWordJunctionTypeNameById(int word_junction_type)
{
	if (word_junction_type == WordJunctionTypeNoWordJunction) {
		return "no-word-junction";
	} else if (word_junction_type == WordJunctionTypeCompound) {
		return "compound";
	} else if (word_junction_type == WordJunctionTypeSeparate) {
		return "separate";
	}

	return "";
}

int GetWordJunctionTypeIdByName(const std::string &word_junction_type)
{
	if (word_junction_type == "no-word-junction") {
		return WordJunctionTypeNoWordJunction;
	} else if (word_junction_type == "compound") {
		return WordJunctionTypeCompound;
	} else if (word_junction_type == "separate") {
		return WordJunctionTypeSeparate;
	}

	return -1;
}
