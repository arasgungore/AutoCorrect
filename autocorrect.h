#ifndef AUTOCORRECT_H
#define AUTOCORRECT_H

#include <string>

class AutoCorrect;

class AutoCorrect {
private:
	std::string wrong_word;
	const std::string dictionary_filename;
public:
	AutoCorrect(const std::string &dictionary_filename) noexcept;
	void setWord(const std::string &wrong_word) noexcept;
	std::string getWord() const noexcept;
	std::string getDictionaryFilename() const noexcept;
	bool checkSpelling() const noexcept;
	bool checkLetterArrangement() const noexcept;
	bool checkExchangedLetters(const unsigned int &EXCHANGED = 1) const noexcept;
	bool checkMissingLetters(const unsigned int &MISSING = 1) const noexcept;
	bool checkExtraLetters(const unsigned int &EXTRA = 1) const noexcept;
	bool checkMissingAndExtraLetters(const unsigned int &MISSING = 1, const unsigned int &EXTRA = 1) const noexcept;
	bool checkAll(const unsigned int &MISSING = 1, const unsigned int &EXTRA = 1, const bool &is_disarranged = false) const noexcept;
	static void sortDictionary(const char *unsorted_dictionary_filename, const char *sorted_dictionary_filename) noexcept(false);
};

#endif		//AUTOCORRECT_H
