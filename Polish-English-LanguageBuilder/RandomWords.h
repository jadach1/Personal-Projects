#pragma once
#include <fstream>
#include <vector>
#include <deque>
#include <iomanip>
#include <time.h>

namespace RandomWordGenerator {
	auto RemoveWiteSpace = [](std::string& str) {
		if (str[0] == ' ') {
			size_t p = str.find_first_not_of(' ');
			str.erase(0, p);
		}
		if (str[str.length()] == ' ')
		{
			size_t p = str.find_last_not_of(' ');
			if (std::string::npos != p)
				str.erase(p + 1);
		}
	};

	// Checks to see if a flash card has already been played
	auto FlashCardNotUsed = [](int number, const std::vector<int> reference)->bool {
		bool NoCopy = true;
		for (int i = 0; i < reference.size(); i++)
		{
			if (reference[i] == number)
			{
				NoCopy = false;
				break;
			}
		}
		return NoCopy;
	};

	// Create a 'fresh page'
	auto FreshPage = []() {
		for (int i = 0; i < 80; i++)
			std::cout << "\n";
	};
	//template<typename T>
	class WordProcessor {
		std::vector<std::string> Words;
		std::vector<std::string> Definitions;
		int WordCount;
		std::vector<int> FlashCardUsed;
		std::deque<int> PracticeWords;
		std::deque<int> PracticeWordsUsed;
	public:
		WordProcessor() {
			srand(time(NULL)); // Randomize FlashCards
			std::fstream is("Words.txt", std::ios::in);
			if (is.is_open()) {
				WordCount = 0; // initialize count for size of file
				std::string line;
				while (!is.eof()) {
					getline(is, line);
					//std::cout << line << "\n";
					WordCount++;
				} // while
				std::cout << "Word Count is " << WordCount << "\n";
				Words.resize(WordCount);  // Resize Vectors
				Definitions.resize(WordCount);
				FlashCardUsed.resize(WordCount);
				LoadFlashCardsUsed();
				LoadPracticeCards();
				is.clear();
				is.seekg(0);
				// Push first word of each line to Words, and second word to Definitions
				for (int i = 0; i < WordCount; i++) {
					getline(is, line, ',');
					RemoveWiteSpace(line);
					Words[i] = line;
					getline(is, line, '\n');
					RemoveWiteSpace(line);
					Definitions[i] = line;
				}
				is.close();
			}
			else throw("Error Opening File words\n");
		}
		~WordProcessor() { SaveCards(FlashCardUsed, PracticeWords); std::cout << "Dtor\n"; }
		void AddSinglePracticeCard(const int UserNumber) // takes number user inputs, verifies it is not being readded, and adds it to the practice cards
		{
			if (UserNumber > 0 && UserNumber <= Words.size())
			{
				bool NoDuplicate = true;
				for (int i = 0; i < PracticeWords.size(); i++) {
					if (UserNumber == PracticeWords[i])
						NoDuplicate = false;
				} // for
				if (NoDuplicate == true)
				{
					UserNumber;
					PracticeWords.push_front(UserNumber);
					std::cout << "Word Added to List!  Press any key to continue or ESCAPE to EXIT\n";
				}
				else { std::cout << "Word Already In List. Press any key to continue or ESCAPE to EXIT\n"; } // if inside
			}
			else { // if usernumber is not valid
				std::cout << "Invalid Select Please try again. Press any key to continue or ESCAPE to EXIT\n";
			}
			SaveCards(FlashCardUsed, PracticeWords);  //save cards before exiting
		}
		void AddMultiplePracticeCards(const std::deque<int> TempDeque)
		{
			bool Duplicate = false;
			unsigned int MaxNumber = Words.size();

			// compare numbers in existing array to numbers in temp array
			for (int i = 0; i < TempDeque.size(); i++)
			{
				for (int k = 0; k < PracticeWords.size(); k++)
				{
					if (TempDeque[i] == PracticeWords[k])
					{
						Duplicate = true;
						break;
					}
				}

				if (Duplicate == false)
				{
					if (TempDeque[i] <= MaxNumber)
					{
						PracticeWords.push_back(TempDeque[i]);
						std::cout << "Successfully added # " << TempDeque[i] << " to list\n";
					}
					else
					{
						std::cout << "Error # exceeds limit cannot add " << TempDeque[i] << "\n";
					}				
				}
				else
				{
					std::cout << "Cannot Add # " << TempDeque[i] << " it already exists\n";
					Duplicate = false;
				}
			}
			SaveCards(FlashCardUsed, PracticeWords);
		}
		void DeleteSinglePracticeCard(int UserNumber) {
			if (UserNumber > 0 && UserNumber <= PracticeWords.size())
			{
				PracticeWords.erase(PracticeWords.begin() + (UserNumber - 1));
				std::cout << "Word Succesfully Deleted! Press any key to continue or ESCAPE to EXIT\n";
			}
			else { // if usernumber is not valid
				std::cout << "Invalid Select Please try again. Press any key to continue or ESCAPE to EXIT\n";
			}
		}  // DeleteSinglePRacticeCard
		void DisplayVecs() {
			std::cout << "Size of Words " << Words.size() << " --- Size of Def = " << Definitions.size() << "\n";
			for (int i = 0, k = 1; i < Words.size(); i++) {
				std::cout << k++ << " " << std::left << std::setw(50) << std::setfill('-') << Words[i] << ", " << std::right << std::setw(20) << std::setfill('-') << Definitions[i] << "\n";
			}
		}
		void DisplayPractice() // displays all practice cards
		{
			if (PracticeWords.size() > 0 && PracticeWords[0] > 0) {
				for (int i = 0, k = 1; i < PracticeWords.size(); i++)  // practicewords and i is the marker for words and definitinos
				{
					if (PracticeWords[i] > 0)
						std::cout << k++ << " " << std::left << std::setw(50) << std::setfill('-') << Words[PracticeWords[i] - 1] << std::left << Definitions[PracticeWords[i] - 1] << "\n";
				}
			}
			else {
				std::cout << "No Words Saved !\n";
			}

		}
		void FlashCards(const int NumOfCards = 1) { // shows non english words first
			FreshPage();
			int *StoredRandomNumbers = new int[NumOfCards];  // create arroy to store random numbers
															 // title
			int select;
			std::cout << "Welcome to Flash Cards !\n"
				<< "**************************\n";
			do {
				std::cout << "Press 1 to begin or continue\n or 0 to Quit\n Or 2 to Save a Card into the favourites: ";
				std::cin >> select;
				if (select == 1) {
					FreshPage();
					// loops random numbers to geenrate flash cards
					for (int i = 0; i < WordCount; i++) {
						if (FlashCardUsed[i] == 0) {
							int RandomNumber = rand() % Words.size() + 1;
							for (int k = 0; k < NumOfCards; k++) {
								// Check if the random number has already been selected in the flashcards vector
								while (!FlashCardNotUsed(RandomNumber, FlashCardUsed)) {
									RandomNumber = rand() % Words.size() + 1;
								}
								FlashCardUsed[i + k] = RandomNumber;
								StoredRandomNumbers[k] = RandomNumber;  //pushes random number to array
								std::cout << std::setw(50) << std::right << LowertoUpper(Words[RandomNumber - 1]) << "\n";
							} // for < numofcards			
							std::cout << "Flash Words Remaining -->" << RemainingFlashCards() << "\n" << "   ------ Show Definition?\n";
							system("pause");
							ShowDefinition(StoredRandomNumbers, NumOfCards);
							break;
						} // if
					} // oustside for loop
				} // if select == 1
				  // Save Favourites or words to practice for the future
				if (select == 2 && StoredRandomNumbers[0] > 0)
				{
					std::cout << "Save Favourites or words to practice for the future\nq for first word\nw second word\ne third word\nr fourth word\nf fifth word\nescape for no words\n";
					while (1) {
						char KeyPress = _getch();
						int ASCIIvalue = KeyPress;
						switch (ASCIIvalue) {
						case 113:
							SavePracticeCard(0, StoredRandomNumbers);
							break;
						case 119:
							if (NumOfCards >= 2)
								SavePracticeCard(1, StoredRandomNumbers);
							else std::cout << "No Word to Select\n";
							break;
						case 101:
							if (NumOfCards >= 3)
								SavePracticeCard(2, StoredRandomNumbers);
							else std::cout << "No Word to Select\n";
							break;
						case 114:
							if (NumOfCards >= 4)
								SavePracticeCard(3, StoredRandomNumbers);
							else std::cout << "No Word to Select\n";
							break;
						case 102:
							if (NumOfCards == 5)
								SavePracticeCard(4, StoredRandomNumbers);
							else std::cout << "No Word to Select\n";
							break;
						default: // this will clear the input buffer if none of the selected options are available
							std::cin.clear();
							std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
							break;
						}
						if (ASCIIvalue == 27)
							break;
					}
				} // if select == 2
			} while (select != 0);
			FreshPage();
			SaveCards(FlashCardUsed, PracticeWords);
			delete StoredRandomNumbers;
		}
		std::deque<int> GetPracticeCards()
		{
			// returns the Practice Word collection
			return PracticeWords;
		}
		void InvertedFlashCards(const int NumOfCards) {  // shows definitions first
			FreshPage();
			int *StoredRandomNumbers = new int[NumOfCards];  // create arroy to store random numbers
															 // title
			int select;
			std::cout << "Welcome to Flash Cards !\n"
				<< "**************************\n";
			do {
				std::cout << "Press 1 to begin or continue\n or 0 to Quit\n Or 2 to Save a Card into the favourites: ";
				std::cin >> select;
				if (select == 1) {
					FreshPage();
					// loops random numbers to geenrate flash cards
					for (int i = 0; i < WordCount; i++) {
						if (FlashCardUsed[i] == 0) {
							int RandomNumber = rand() % Words.size() + 1;
							for (int k = 0; k < NumOfCards; k++) {
								// Check if the random number has already been selected in the flashcards vector
								while (!FlashCardNotUsed(RandomNumber, FlashCardUsed)) {
									RandomNumber = rand() % Words.size() + 1;
								}
								FlashCardUsed[i + k] = RandomNumber;
								StoredRandomNumbers[k] = RandomNumber;  //pushes random number to array
								std::cout << std::setw(50) << std::right << LowertoUpper(Definitions[RandomNumber - 1]) << "\n";
							} // for < numofcards			
							std::cout << "Flash Words Remaining -->" << RemainingFlashCards() << "\n" << "   ------ Show Definition?\n";
							system("pause");
							ShowWord(StoredRandomNumbers, NumOfCards);
							break;
						} // if
					} // oustside for loop
				} // if select
				  // Save Favourites or words to practice for the future
				if (select == 2 && StoredRandomNumbers[0] > 0)
				{
					std::cout << "Save Favourites or words to practice for the future\nq for first word\nw second word\ne third word\nr fourth word\nf fifth word\nescape for no words\n";
					while (1) {
						char KeyPress = _getch();
						int ASCIIvalue = KeyPress;
						switch (ASCIIvalue) {
						case 113:
							SavePracticeCard(0, StoredRandomNumbers);
							break;
						case 119:
							if (NumOfCards >= 2)
								SavePracticeCard(1, StoredRandomNumbers);
							else std::cout << "No Word to Select\n";
							break;
						case 101:
							if (NumOfCards >= 3)
								SavePracticeCard(2, StoredRandomNumbers);
							else std::cout << "No Word to Select\n";
							break;
						case 114:
							if (NumOfCards >= 4)
								SavePracticeCard(3, StoredRandomNumbers);
							else std::cout << "No Word to Select\n";
							break;
						case 102:
							if (NumOfCards == 5)
								SavePracticeCard(4, StoredRandomNumbers);
							else std::cout << "No Word to Select\n";
							break;
						default: // this will clear the input buffer if none of the selected options are available
							std::cin.clear();
							std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
							break;
						}
						if (ASCIIvalue == 27)
							break;
					}
				} // if select == 2
			} while (select != 0);
			FreshPage();
			SaveCards(FlashCardUsed, PracticeWords);
			delete StoredRandomNumbers;
		}
		void LoadFlashCardsUsed()// Will load saved file to vector FlashCArdsUsed + Practice  Words + Practice Definitions
		{
			std::ifstream is("FlashCards.txt");
			if (is.is_open())
			{
				//take each number in the file and push them into usedflashcards array
				int Number;
				std::string line;
				int Count = 0;
				while (!is.eof()) { // count number of lines 
					getline(is, line);
					if (line != "")
						Count++;
				}
				std::vector<int>TempStorage;
				TempStorage.resize(Count);
				// reset file then copy
				is.clear();
				is.seekg(0);
				for (int i = 0; Count > 0 && i < TempStorage.size(); Count--, i++) {
					is >> Number;
					if (Number > 0)
						TempStorage[i] = Number;
				}
				// Load the TempStorage into the actual FlashCardsUsed with validation
				for (int i = 0; i < TempStorage.size(); i++)
				{
					if (TempStorage[i] > 0)
						FlashCardUsed[i] = TempStorage[i];
				}
				is.close();
			}
			else
				throw ("Failed Loading from File FlashCards.txt");
		}
		void LoadPracticeCards() // load the practice cards from text file practicewords to deque practicewords
		{
			std::ifstream is("PracticeWords.txt");
			if (is.is_open())
			{
				//take each number in the file and push them into usedflashcards array
				int Number;
				std::string line;
				int Count = 0;
				while (!is.eof()) { // count number of lines 
					getline(is, line);
					if (line != "")
						Count++;
				}
				std::deque<int>TempStorage;
				TempStorage.resize(Count);
				PracticeWords.resize(Count);
				// reset file then copy
				is.clear();
				is.seekg(0);
				for (int i = 0; Count > 0 && i < TempStorage.size(); Count--, i++) {
					is >> Number;
					if (Number > 0)
						TempStorage[i] = Number;
				}
				// Load the TempStorage into the actual FlashCardsUsed with validation
				for (int i = 0; i < TempStorage.size(); i++)
				{
					if (TempStorage[i] > 0)
						PracticeWords[i] = TempStorage[i];
				}
				is.close();
			}
			else
				throw ("Failed Loading from File PracticeWords.txt");
		} // loadpracticecards
		std::string LowertoUpper(std::string str) // converts showdefinition or showword to upper case
		{
			if (str[0] != '\0')
			{
				for (int i = 0; i < str.length(); i++)
				{
					str[i] = toupper(str[i]);
				} // for
			} // if
			return str;
		} // end of convert
		void PracticeCardOne() {  // Displays randomly the cards saved in the practice list
			int RemainingWords, RandomNumber;
			bool ResetActive = false;
			RemainingWords = PracticeWords.size() - PracticeWordsUsed.size();
			// Function to see if random number is duplicate or not
			auto RandomNumberCompare = [](const int num, const std::deque<int> WordsUsed)->bool {
				bool DuplicateFound = false;  // starts off flase, no duplicates, if one is found then it will become true
				for (int i = 0; i < WordsUsed.size(); i++)
					if (num == WordsUsed[i])
						DuplicateFound = true;
				return DuplicateFound;
			};
			/*-----------------------------------------------------------------------------*/
			if (PracticeWords.size() > 0)  // makes sure their is at least one practice words
			{
				FreshPage();
				std::cout << "Begin Practice Cards Hit Enter\nTo Reset the Deck hit r\nTo exit hit ESCAPE\n";
				std::cout << RemainingWords << " Words Remaining\n";
				while (1) {
					char keypress = _getch();
					int ASCIIvalue = keypress;
					// if ResetActive flag is set to true, it will redisplay the menu and cards remaining
					if (ResetActive == true)
					{
						FreshPage();
						RemainingWords = PracticeWords.size() - PracticeWordsUsed.size();
						std::cout << "Begin Practice Cards Hit Enter\nTo Reset the Deck hit r\nTo exit hit ESCAPE\n";
						std::cout << RemainingWords << " Words Remaining\n";
						ResetActive = false;
					}
					switch (ASCIIvalue) {
					case 13: // enter is presssed
						if (RemainingWords > 0) {
							RandomNumber = rand() % PracticeWords.size() + 1;
							while (RandomNumberCompare(RandomNumber, PracticeWordsUsed))  // this will keep looping until a random number returns FALSE
							{
								RandomNumber = rand() % PracticeWords.size() + 1;
							} // while
							  // RandomNumber Successfully Found, push to list
							PracticeWordsUsed.push_front(RandomNumber);
							RemainingWords--;
							// Display Word and Wait for User to Hit Enter to Show Definition
							std::cout << "-------------->" << std::left << LowertoUpper(Words[PracticeWords[RandomNumber - 1] - 1]) << "<---------------------" << "Words Remaining-->" << RemainingWords << "\n"; // subtract 1 from random number and one from practicewords to fit array
							system("pause");
							std::cout << "-------------->" << std::left << LowertoUpper(Definitions[PracticeWords[RandomNumber - 1] - 1]) << "<---------------------\n";
							std::cout << "Hit Enter to Continue to Next Word\n";
							break;
						} // if
						else { std::cout << "No Words Remaining.  Hit r to rest or ESCAPE to quit\n"; break; }
					case 114:  // r to reset is pressed
						std::cout << "Practice Cards Reset!\n";
						ResetActive = true;
						ResetPracticeCardsUsed();
						break;
					}; //switch 
					if (ASCIIvalue == 27)
						break;
				}
			}
			else {
				std::cout << "No Practice Words Saved!\n"; system("pause");
			}
		}
		void PracticeCardTwo() {  // Displays randomly the cards saved in the practice list
			int RemainingWords, RandomNumber;
			bool ResetActive = false;
			RemainingWords = PracticeWords.size() - PracticeWordsUsed.size();
			// Function to see if random number is duplicate or not
			auto RandomNumberCompare = [](const int num, const std::deque<int> WordsUsed)->bool {
				bool DuplicateFound = false;  // starts off flase, no duplicates, if one is found then it will become true
				for (int i = 0; i < WordsUsed.size(); i++)
					if (num == WordsUsed[i])
						DuplicateFound = true;
				return DuplicateFound;
			};
			/*-----------------------------------------------------------------------------*/
			if (PracticeWords.size() > 0)  // makes sure their is at least one practice words
			{
				FreshPage();
				std::cout << "Begin Practice Cards Hit Enter\nTo Reset the Deck hit r\nTo exit hit ESCAPE\n";
				std::cout << RemainingWords << " Words Remaining\n";
				while (1) {
					char keypress = _getch();
					int ASCIIvalue = keypress;
					// if ResetActive flag is set to true, it will redisplay the menu and cards remaining
					if (ResetActive == true)
					{
						FreshPage();
						RemainingWords = PracticeWords.size() - PracticeWordsUsed.size();
						std::cout << "Begin Practice Cards Hit Enter\nTo Reset the Deck hit r\nTo exit hit ESCAPE\n";
						std::cout << RemainingWords << " Words Remaining\n";
						ResetActive = false;
					}
					switch (ASCIIvalue) {
					case 13: // enter is presssed
						if (RemainingWords > 0) {
							RandomNumber = rand() % PracticeWords.size() + 1;
							while (RandomNumberCompare(RandomNumber, PracticeWordsUsed))  // this will keep looping until a random number returns FALSE
							{
								RandomNumber = rand() % PracticeWords.size() + 1;
							} // while
							  // RandomNumber Successfully Found, push to list
							PracticeWordsUsed.push_front(RandomNumber);
							RemainingWords--;
							// Display Word and Wait for User to Hit Enter to Show Definition
							std::cout << "-------------->" << std::left << LowertoUpper(Definitions[PracticeWords[RandomNumber - 1] - 1]) << "<---------------------" << "Words Remaining-->" << RemainingWords << "\n"; // subtract 1 from random number and one from practicewords to fit array
							system("pause");
							std::cout << "-------------->" << std::left << LowertoUpper(Words[PracticeWords[RandomNumber - 1] - 1]) << "<---------------------\n";
							std::cout << "Hit Enter to Continue to Next Word\n";
							break;
						} // if
						else { std::cout << "No Words Remaining.  Hit r to rest or ESCAPE to quit\n"; break; }
					case 114:  // r to reset is pressed
						std::cout << "Practice Cards Reset!\n";
						ResetActive = true;
						ResetPracticeCardsUsed();
						break;
					}; //switch 
					if (ASCIIvalue == 27)
						break;
				}
			}
			else {
				std::cout << "No Practice Words Saved!\n"; system("pause");
			}
		}
		int RemainingFlashCards() { // counts the number or cards that are remaining
			int count = 0;
			for (int i = 0; i < FlashCardUsed.size(); i++) {
				if (FlashCardUsed[i] == 0)
					count++;
			}
			return count;
		}
		void ResetFlashCards() {  //message box appears asking to ok or cancel
			int msgboxID = MessageBox(
				NULL,
				(LPCWSTR)L"ARE YOU SURE YOU WANT TO RESET FLASH CARDS?\n",
				(LPCWSTR)L"WARNING\n",
				MB_ICONWARNING | MB_OKCANCEL | MB_DEFBUTTON2
			);
			switch (msgboxID) {
			case 2: // cancel
				break;
			case 1: // ok
				std::cout << "Cards Reset !\n";
				for (int i = 0; i < FlashCardUsed.size(); i++)
					FlashCardUsed[i] = 0;
				SaveCards(FlashCardUsed, PracticeWords);
				break;
			};
		}
		void ResetPracticeCards()
		{
			int msgboxID = MessageBox(
				NULL,
				(LPCWSTR)L"ARE YOU SURE YOU WANT TO RESET PRACTICE CARDS?\n",
				(LPCWSTR)L"WARNING\n",
				MB_ICONWARNING | MB_OKCANCEL | MB_DEFBUTTON2
			);
			switch (msgboxID) {
			case IDCANCEL:
				break;
			case IDOK:
				std::ofstream is("PracticeWords.txt", std::ios::out | std::ios::trunc);
				std::cout << "Saved Cards Erased !\n";
				is.close();
				for (int i = 0; i < PracticeWords.size();) // erase all the practice words
					PracticeWords.pop_back();
				SaveCards(FlashCardUsed, PracticeWords);
				break;
			};
		}
		void ResetPracticeCardsUsed() {
			for (int i = PracticeWordsUsed.size(); i > 0; i--)
				PracticeWordsUsed.pop_back();
		}
		int SizeofList() // returns the size of the lise
		{
			return Words.size();
		}
		int SizeofPracticeList() // returns number of practice cards saved
		{
			return PracticeWords.size();
		}
		void ShowDefinition(int *number, const int NumOfCards = 1) {
			for (int i = 0; i < NumOfCards; i++)
				std::cout << std::setw(35) << std::right << "Definition = " << LowertoUpper(Definitions[number[i] - 1]) << "\n";
		}
		void ShowWord(int *number, const int NumOfCards = 1) {
			for (int i = 0; i < NumOfCards; i++)
				std::cout << std::setw(35) << std::right << "Definition = " << LowertoUpper(Words[number[i] - 1]) << "\n";
		}
		void SaveCards(const std::vector<int> FlashCard, const std::deque<int> Words) // will create a file and save the flash cards used
		{
			std::ofstream of("FlashCards.txt");
			std::ofstream of2("PracticeWords.txt");
			if (of.is_open() && of2.is_open())
			{
				// write anything that is not a 0 to the file
				for (int i = 0; i < FlashCard.size(); i++)
				{
					if (FlashCard[i] != 0)
						of << FlashCard[i] << "\n";
				}
				// write anything that is not a 0 to the file
				for (int i = 0; i < PracticeWords.size(); i++)
				{
					if (PracticeWords[i] != 0)
						of2 << PracticeWords[i] << "\n";
				}
				of.close();
				of2.close();
			} // if of is open
			else
				throw ("Error, trying to save file FlashCards");
		}
		void SavePracticeCard(int number, const int *numberarray)  // Gets passed an array of ints and an int stores values into practicearray
		{

			auto CheckDeck = [](int n, std::deque<int> PracticeWords)->bool {  // checks to see if tehir is a duplicate
				bool NoDuplicate = true; // true means no duplicate found
				for (int i = 0; i < PracticeWords.size(); i++) {
					if (n == PracticeWords[i])
						NoDuplicate = false;
				} // for
				return NoDuplicate;
			};
			// passes to lambda to check if duplicate or not, if not then save to PracticeWords
			if (CheckDeck(numberarray[number], PracticeWords)) {
				PracticeWords.push_front(numberarray[number]);
				std::cout << "Word Saved !\n";
			}
			else
				std::cout << "Word Already Saved !\n";


		}	
	};
}