#include "autocorrect.h"
#include <iostream>
#include <algorithm>
using namespace std;

string toLowerString(string str) noexcept;

int main() {
	AutoCorrect::sortDictionary("dictionary/words_alpha.txt", "dictionary/words_alpha_sorted.txt");
	AutoCorrect AC("dictionary/words_alpha_sorted.txt");
	for(string word;true;cout << endl << endl) {
		cout << "Enter a word: ";
		cin >> word;
		AC.setWord(toLowerString(word));
		if(AC.checkSpelling())
			cout << endl << "Spelling of the word is correct." << endl;
		else {
			cout << endl << "Spelling of the word is wrong. Possible right spellings are given below:" << endl << endl;
			const bool &arranged = AC.checkLetterArrangement();
			const bool &exchanged = AC.checkExchangedLetters();
			const bool &extra = AC.checkExtraLetters(), &extra2 = AC.checkExtraLetters(2), &extra3 = AC.checkExtraLetters(3);
			const bool &missing = AC.checkMissingLetters(), &missing2 = AC.checkMissingLetters(2), &missing3 = AC.checkMissingLetters(3);
			const bool &MandE = AC.checkMissingAndExtraLetters(), &MandE2 = AC.checkMissingAndExtraLetters(2), &MandE3 = AC.checkMissingAndExtraLetters(1, 2), &MandE4 = AC.checkMissingAndExtraLetters(2, 2);
//			const bool &all = AC.checkAll(2, 2), &all2 = AC.checkAll(2, 2, true);
			if(!(
				   arranged
				|| exchanged
				|| extra || extra2 || extra3
				|| missing || missing2 || missing3
				|| MandE || MandE2 || MandE3 || MandE4
//				|| all || all2
				))		cout << endl << "No such word exists." << endl;
		}
	}
}

string toLowerString(string str) noexcept {
	transform(str.begin(), str.end(), str.begin(), [](const unsigned char &ch){ return tolower(ch); });
	return str;
}
