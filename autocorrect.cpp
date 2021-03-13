#include "autocorrect.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <set>

// constructor
AutoCorrect::AutoCorrect(const std::string &dictionary_filename) noexcept : dictionary_filename(dictionary_filename) {	}

// setter method for the word to check
void AutoCorrect::setWord(const std::string &wrong_word) noexcept {
	this->wrong_word = wrong_word;
}

// getter method for the word to check
std::string AutoCorrect::getWord() const noexcept {
	return wrong_word;
}

// getter method for the name of the dictionary file we are using
std::string AutoCorrect::getDictionaryFilename() const noexcept {
	return dictionary_filename;
}

// checks the spelling of the given word, returns true if the spelling is correct, false otherwise
bool AutoCorrect::checkSpelling() const noexcept {
	std::ifstream dictionary(dictionary_filename);
	for(std::string dict_word;dictionary >> dict_word;)
		if(wrong_word.front() > dict_word.front())
			continue;
		else if(wrong_word.front() < dict_word.front()) {
			dictionary.close();
			return false;
		}
		else if(wrong_word == dict_word) {
			dictionary.close();
			return true;
		}
	dictionary.close();
	return false;
}

// gives suggestions with different letter arrangements, returns true if any suggestions are found, false otherwise
// e.g., "cwro" is misspelled, so "crow" is a possible correct suggestion because the letters of "cwro" are disarranged
bool AutoCorrect::checkLetterArrangement() const noexcept {
	bool found = false;
	unsigned int word_count = 0;
	std::ifstream dictionary(dictionary_filename);
	std::cout << "*** Incorrect Arrangement ***\t:\t";
	std::string wrong_word_copy = wrong_word;
	sort(wrong_word_copy.begin() + 1, wrong_word_copy.end());
	for(std::string dict_word;dictionary >> dict_word;)
		if(wrong_word.front() > dict_word.front())
			continue;
		else if(wrong_word.front() < dict_word.front())
			break;
		else if(wrong_word.length() == dict_word.length()) {
			std::string dict_word_copy = dict_word;
			sort(dict_word_copy.begin() + 1, dict_word_copy.end());
			if(wrong_word_copy == dict_word_copy) {
				std::cout << dict_word << ", ";
				found = true;
				++word_count;
			}
		}
	std::cout << "\b\b\t(" << word_count << ")" << std::endl;
	dictionary.close();
	return found;
}

// gives suggestions assuming the given word is including wrong letters in certain positions, returns true if any suggestions are found, false otherwise
// e.g., "cfotn" is misspelled, so "crown" or "clown" are 2 possible correct suggestions because "cfotn" needs 2 letters to be exchanged
bool AutoCorrect::checkExchangedLetters(const unsigned int &EXCHANGED) const noexcept {
	switch(EXCHANGED) {
		case 0:
			return false;
		case 1:
			std::cout << "*** Exchanged Character ***\t:\t";
			break;
		default:
			std::cout << "*** Exchanged " << EXCHANGED << " Characters ***\t:\t";
	}
	bool found = false;
	unsigned int word_count = 0;
	std::ifstream dictionary(dictionary_filename);
	for(std::string dict_word;dictionary >> dict_word;)
		if(wrong_word.front() > dict_word.front())
			continue;
		else if(wrong_word.front() < dict_word.front())
			break;
		else if(wrong_word.length() == dict_word.length()) {
			unsigned int uncommon_letter_count = 0;
			for(unsigned int i=1;i<wrong_word.length();++i)
				if(wrong_word[i] != dict_word[i])	++uncommon_letter_count;
			if(uncommon_letter_count == EXCHANGED) {
				std::cout << dict_word << ", ";
				found = true;
				++word_count;
			}
		}
	std::cout << "\b\b\t(" << word_count << ")" << std::endl;
	dictionary.close();
	return found;
}

// gives suggestions assuming the given word is missing some letters, returns true if any suggestions are found, false otherwise
// e.g. "acomodation" is misspelled, so "accommodation" is a possible correct suggestion because "acomodation" is missing 2 letters (ac(c)om(m)odation)
bool AutoCorrect::checkMissingLetters(const unsigned int &MISSING) const noexcept {
	switch(MISSING) {
		case 0:
			return false;
		case 1:
			std::cout << "*** Missing Character ***\t:\t";
			break;
		default:
			std::cout << "*** Missing " << MISSING << " Characters ***\t:\t";
	}
	bool found = false;
	unsigned int word_count = 0;
	std::ifstream dictionary(dictionary_filename);
	for(std::string dict_word;dictionary >> dict_word;)
		if(wrong_word.front() > dict_word.front())		continue;
		else if(wrong_word.front() < dict_word.front())	break;
		else if(wrong_word.length() + MISSING == dict_word.length()) {
			unsigned int missing_letter_count = 0;
			auto wrong_letter = wrong_word.cbegin() + 1;
			for(const auto &dict_letter : dict_word.substr(1))
				if(*wrong_letter == dict_letter)
					++wrong_letter;
				else
					++missing_letter_count;
			if(wrong_letter == wrong_word.cend() && missing_letter_count == MISSING) {
				std::cout << dict_word << ", ";
				found = true;
				++word_count;
			}
		}
	std::cout << "\b\b\t(" << word_count << ")" << std::endl;
	dictionary.close();
	return found;
}

// gives suggestions assuming the given word has some extra letters, returns true if any suggestions are found, false otherwise
// e.g. "acccommodddation" is misspelled, so "accommodation" is a possible correct suggestion because "acccommodddation" has 3 extra letters (acc/c/ommod/dd/ation)
bool AutoCorrect::checkExtraLetters(const unsigned int &EXTRA) const noexcept {
	switch(EXTRA) {
		case 0:
			return false;
		case 1:
			std::cout << "*** Extra Character ***\t\t:\t";
			break;
		default:
			std::cout << "*** Extra " << EXTRA << " Characters ***\t:\t";
	}
	bool found = false;
	unsigned int word_count = 0;
	std::ifstream dictionary(dictionary_filename);
	for(std::string dict_word;dictionary >> dict_word;)
		if(wrong_word.front() > dict_word.front())
			continue;
		else if(wrong_word.front() < dict_word.front())
			break;
		else if(wrong_word.length() == dict_word.length() + EXTRA) {
			unsigned int extra_letter_count = 0;
			auto dict_letter = dict_word.cbegin() + 1;
			for(const auto &wrong_letter : wrong_word.substr(1))
				if(wrong_letter == *dict_letter)
					++dict_letter;
				else
					++extra_letter_count;
			if(dict_letter == dict_word.cend() && extra_letter_count == EXTRA) {
				std::cout << dict_word << ", ";
				found = true;
				++word_count;
			}
		}
	std::cout << "\b\b\t(" << word_count << ")" << std::endl;
	dictionary.close();
	return found;
}

// gives suggestions assuming the given word is missing some letters and has some extra letters, returns true if any suggestions are found, false otherwise
// e.g. "acomoddation" is misspelled, so "accommodation" is a possible correct suggestion because "acomoddation" is missing 2 letters (ac(c)om(m)oddation) and has 1 extra letter (accommod/d/ation)
// "acomoddation" => "ac(c)om(m)oddation" => "accommoddation" => "accommod/d/ation" => "accommodation"
bool AutoCorrect::checkMissingAndExtraLetters(const unsigned int &MISSING, const unsigned int &EXTRA) const noexcept {
	if(!(MISSING && EXTRA))
		return false;
	std::cout << "*** " << MISSING << " Missing, " << EXTRA << " Extra Characters ***\t:\t";
	bool found = false;
	unsigned int word_count = 0;
	std::ifstream dictionary(dictionary_filename);
	for(std::string dict_word;dictionary >> dict_word;)
		if(wrong_word.front() > dict_word.front())
			continue;
		else if(wrong_word.front() < dict_word.front())
			break;
		else if(wrong_word.length() + MISSING == dict_word.length() + EXTRA) {
			unsigned int missing_letter_count = 0, extra_letter_count = 0;
			auto dict_letter = dict_word.cbegin() + 1, wrong_letter = wrong_word.cbegin() + 1;
			for(;wrong_letter != wrong_word.cend();++wrong_letter)
				if(*wrong_letter == *dict_letter)
					++dict_letter;
				else if(dict_letter != dict_word.cend()) {
					auto dict_letter_2 = dict_letter + 1;
					for(;dict_letter_2 != dict_word.cend();++dict_letter_2)
						if(*wrong_letter == *dict_letter_2) {
							missing_letter_count += distance(dict_letter, dict_letter_2);
							dict_letter = dict_letter_2 + 1;
							break;
						}
					if(dict_letter_2 == dict_word.cend())
						++extra_letter_count;
				}
				else
					break;
			extra_letter_count += distance(wrong_letter, wrong_word.cend());
			missing_letter_count += distance(dict_letter, dict_word.cend());
			if(missing_letter_count == MISSING && extra_letter_count == EXTRA) {
				std::cout << dict_word << ", ";
				found = true;
				++word_count;
			}
		}
	std::cout << "\b\b\t(" << word_count << ")" << std::endl;
	dictionary.close();
	return found;
}

// gives suggestions assuming the given word is missing some letters, has some extra letters, and also might be disarranged
// returns true if any suggestions are found, false otherwise
bool AutoCorrect::checkAll(const unsigned int &MISSING, const unsigned int &EXTRA, const bool &is_disarranged) const noexcept {
	if(!(MISSING && EXTRA))
		return false;
	bool found = false;
	unsigned int word_count = 0;
	std::ifstream dictionary(dictionary_filename);
	if(is_disarranged) {
		std::cout << "*** Max " << MISSING << " Missing, " << EXTRA << " Extra, Disarranged Characters ***\t:\t";
		std::string wrong_word_copy = wrong_word;
		sort(wrong_word_copy.begin() + 1, wrong_word_copy.end());
		for(std::string dict_word;dictionary >> dict_word;)
			if(wrong_word.front() > dict_word.front())
				continue;
			else if(wrong_word.front() < dict_word.front())
				break;
			else if((wrong_word.length() <= dict_word.length() + EXTRA) && (wrong_word.length() + MISSING >= dict_word.length())) {
				std::string dict_word_copy = dict_word;
				sort(dict_word_copy.begin() + 1, dict_word_copy.end());
				unsigned int missing_letter_count = 0, extra_letter_count = 0;
				auto dict_letter = dict_word_copy.cbegin() + 1, wrong_letter = wrong_word_copy.cbegin() + 1;
				for(;wrong_letter != wrong_word_copy.cend();++wrong_letter)
					if(*wrong_letter == *dict_letter)
						++dict_letter;
					else if(dict_letter != dict_word_copy.cend()) {
						auto dict_letter_2 = dict_letter + 1;
						for(;dict_letter_2 != dict_word_copy.cend();++dict_letter_2)
							if(*wrong_letter == *dict_letter_2) {
								missing_letter_count += distance(dict_letter, dict_letter_2);
								dict_letter = dict_letter_2 + 1;
								break;
							}
						if(dict_letter_2 == dict_word_copy.cend())
							++extra_letter_count;
					}
					else
						break;
				extra_letter_count += distance(wrong_letter, wrong_word_copy.cend());
				missing_letter_count += distance(dict_letter, dict_word_copy.cend());
				if(missing_letter_count <= MISSING && extra_letter_count <= EXTRA) {
					std::cout << dict_word << ", ";
					found = true;
					++word_count;
				}
			}
	}
	else {
		std::cout << "*** Max " << MISSING << " Missing, " << EXTRA << " Extra Characters ***\t:\t";
		for(std::string dict_word;dictionary >> dict_word;)
			if(wrong_word.front() > dict_word.front())
				continue;
			else if(wrong_word.front() < dict_word.front())
				break;
			else if((wrong_word.length() <= dict_word.length() + EXTRA) && (wrong_word.length() + MISSING >= dict_word.length())) {
				unsigned int missing_letter_count = 0, extra_letter_count = 0;
				auto dict_letter = dict_word.cbegin() + 1, wrong_letter = wrong_word.cbegin() + 1;
				for(;wrong_letter != wrong_word.cend();++wrong_letter)
					if(*wrong_letter == *dict_letter)
						++dict_letter;
					else if(dict_letter != dict_word.cend()) {
						auto dict_letter_2 = dict_letter + 1;
						for(;dict_letter_2 != dict_word.cend();++dict_letter_2)
							if(*wrong_letter == *dict_letter_2) {
								missing_letter_count += distance(dict_letter, dict_letter_2);
								dict_letter = dict_letter_2 + 1;
								break;
							}
						if(dict_letter_2 == dict_word.cend())
							++extra_letter_count;
					}
					else
						break;
				extra_letter_count += distance(wrong_letter, wrong_word.cend());
				missing_letter_count += distance(dict_letter, dict_word.cend());
				if(missing_letter_count <= MISSING && extra_letter_count <= EXTRA) {
					std::cout << dict_word << ", ";
					found = true;
					++word_count;
				}
			}
	}
	std::cout << "\b\b\t(" << word_count << ")" << std::endl;
	dictionary.close();
	return found;
}

// sorts the dictionary file according to alphabetical order, then writes it in a different file
void AutoCorrect::sortDictionary(const char *unsorted_dictionary_filename, const char *sorted_dictionary_filename) noexcept(false) {
	try {
		std::ifstream unsorted_dictionary(unsorted_dictionary_filename);
		unsorted_dictionary.exceptions(std::ifstream::badbit);
		std::set<std::string> sorted_words;
		for(std::string dict_word; unsorted_dictionary >> dict_word; sorted_words.emplace(dict_word));
		unsorted_dictionary.close();
		std::ofstream sorted_dictionary(sorted_dictionary_filename);
		sorted_dictionary.exceptions(std::ofstream::failbit | std::ofstream::badbit);
		for(const auto &word : sorted_words)
			sorted_dictionary << word << std::endl;
		sorted_dictionary.close();
	}
	catch(const std::exception &e) {
		std::cerr << "[!ERROR!]: " << e.what() << std::endl;
	}
}
