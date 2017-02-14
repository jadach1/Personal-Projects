#include <string>
#include <sstream>
//#include <graphics.h>  //change file in video descript
#include <iostream>
#include <memory>
#include <regex>
#include <conio.h>
#define NOMINMAX
#include <Windows.h>
#include "RandomWords.h"


std::deque<int> AddPracticeCardsSeperation(std::string& UserInput) // takes a string and seperates it into a vector 
{

	// vector to be returned
	std::deque<int> TempStorage;

	// Lambda 's

	// Check to see if parsed number already exists in array
	auto CheckExistingArray = [&](int number)->bool {

		for (int i = 0; i < TempStorage.size(); i++)
		{
			// check to see if the number variable already exists
			if (number == TempStorage[i])
			{
				return false;
			}
		}

		return true;
	};

	// count commas in case user enters a single number
	int commas = 0;

	// for loop to search 
	for (int i = 0; i < UserInput.length(); i++)
	{
		if (UserInput[i] == ',' || commas == 0 && i == UserInput.length() - 1)
		{
			// fetch number, parse it, make sure it is not a duplicate and store it in array
			// k and j will be the iterators to isolate the number ie 12, 
			for (signed int k = i, j = i; k >= 0; k--)
			{
				// found a single number ie 12,13,14
				if (k == 0 || k - 1 == ',')
				{
					// unsigned is positives only and signed is postiive and negatives
					//the signed version uses the left-most-bit to identify if the number is positive or negative
					unsigned int difference = j - k;
					// add 1 more to the difference if there is only one # entered ie 13
					if (k == 0) difference++; 
					std::string ParsedNumber = UserInput.substr(k, difference);
					unsigned int NumberToAdd = stoi(ParsedNumber);

					// check to see if number is already added to existing deque
					if (CheckExistingArray(NumberToAdd))
					{
						// the number is not in the array because the lambda returned true
						// lets add the number
						TempStorage.push_front(NumberToAdd);
						break;
					}
				}
			}
			commas++;
		}  // if comma is found

		if (UserInput[i] == '-')
		{
			unsigned int RightSideNumber;
			unsigned int LeftSideNumber;

			// Parses the right side number of the dash '-'
			// k represents the start point and j represents the end point
			for (int k = i + 1, j = i + 1; j <= UserInput.length(); j++)
			{
				// comma is found, or end of string so we know number is finished
				if (UserInput[j] == ',' || j == UserInput.length())
				{
					// subtracts the end point j to the start point k
					int Difference = j - k;
					std::string NumberToBeConverted = UserInput.substr(k, Difference);
					RightSideNumber = stoi(NumberToBeConverted);
					break;
				}
			} // for RightSideNumber

			  // parses left side of dash '-' 
			  // k is represented as the end of the parse and j as the beginning
			for (int k = i, j = i; j >= 0; j--)
			{
				// comma is found, or end of string so we know number is finished
				if (j == 0 || UserInput[j - 1] == ',')
				{
					// subtracts the end point j to the start point k
					int Difference = (k - j) ;
					std::string NumberToBeConverted = UserInput.substr(j, Difference);
					LeftSideNumber = stoi(NumberToBeConverted);
					break;
				}
			} // for LeftSideNumber

			// check to make sure left side is less then right side
			if (LeftSideNumber < RightSideNumber || LeftSideNumber == RightSideNumber)
			{
				// create a new temp array for the list of numbers between the dash
				// 1-5 difference = 4; 0,1,2,3,4; 5#s   10-20 , difference = 10; 10,11,12,13,14,15,16,17,18,19,20; 11#s
				unsigned int DifferenceLeftAndRight = RightSideNumber - LeftSideNumber;
				// int* TempTempStoragre = new int[DifferenceLeftAndRight + 1]; // the + 1 is explained above

				 for (int m= 0; m <= DifferenceLeftAndRight; m++)
				 {
					 if (CheckExistingArray(LeftSideNumber + m))
					 {
						 TempStorage.push_back(LeftSideNumber + m);
					 }
					 else
						 std::cout << "Cannot add # " << LeftSideNumber + m << " it already exists\n";
				 }
				 /*for (int i = 0; i <= DifferenceLeftAndRight; i++)
				 {
				 std::cout << "Number in array" << i << " - " << TempStorage[i] << "\n";
				}*/
			}
			else
			{
				// error, left side is not greater then right side
				std::cout << "Sorry but cannot add " << LeftSideNumber << " - " << RightSideNumber << " Because the left number needs to be lower then the right number\n";
			}
			

		} // if dash found
	}

	return TempStorage;
}
void ResetBuffer() {
	std::cout << "press enter\n";
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	RandomWordGenerator::FreshPage();
}
void ResetBuffer2() {
	std::cin.clear();
	std::cin.ignore(2000, '\n');
}
bool ValidateNumber(std::string string) // checks to see if the user has entered a number or not
{
	// flag
	bool IsNumber = true;

	// Look for any input that is NOT A DIGIT
	if (string.length() < 10) {
		for (int i = 0; i < string.length(); i++) {
			// found false digit
			if (!isdigit(string[i]))
				IsNumber = false;
		}
	}

	// after calculating check for VALIDATION
	if (IsNumber == false)
		std::cout << "Invalid Entry.  Please Try Again\n"; // print error msg

	if (string.length() > 9) {
		std::cout << "Input too long.  Please Try Again\n"; //print error msg if string > 9
		IsNumber = false;
	}

	// reset buffer and Return Value
	ResetBuffer2();
	return IsNumber;
}
bool CheckForDuplicates(std::string& InputUser)
{
	// make sure Numbers exist in list to be added
	return true;
}
void FlashCardsOption(std::shared_ptr<RandomWordGenerator::WordProcessor> processor)
{
	std::string select;
	int choice = 0, NumOfCards = 0;
	while (choice != 1 && choice != 2) { // will loop until user enters a 1 or 2
		do {
			std::cout << "1. English to Polish Cards\n2. Polish to English\nSelect: ";
			std::cin >> select;
		} while (!ValidateNumber(select));// while not a number is found in user input
		choice = stoi(select);  // copies the user number if it passes validation
		if (choice == 1) { // option 1
			while (NumOfCards < 1 || NumOfCards > 5) {
				do {
					std::cout << "How Many Flash Cards to be Displayed ? (1...2...3...4...5)\n";
					std::cin >> select;
				} while (!ValidateNumber(select)); // do while. Wil keep looping if bad input from user 
				NumOfCards = stoi(select);
			}  // while user input isnt in range of 1 to 5
			processor->FlashCards(NumOfCards);
		}
		if (choice == 2) { // option 1
			while (NumOfCards < 1 || NumOfCards > 5) {
				do {
					std::cout << "How Many Flash Cards to be Displayed ? (1...2...3...4...5)\n";
					std::cin >> select;
				} while (!ValidateNumber(select)); // do while. Wil keep looping if bad input from user 
				NumOfCards = stoi(select);
			}  // while user input isnt in range of 1 to 5
			processor->InvertedFlashCards(NumOfCards);
		}
	} // first while loop
}
bool StringUserInputIsNotZero(std::string InputUser, int& Zero)
{
	//flag
	bool IsNotZero = true;

	// eliminate white space from front
	while (isspace(InputUser[0]))
		InputUser.erase(0, 1);

	// Remove last white space from string
	while (InputUser.length() > 0 && isspace(InputUser[InputUser.length() - 1]))
		InputUser.erase(InputUser.length() - 1, 1);

	// checks to see if user has entered 0
	if (InputUser.length() == 1 && InputUser[0] == '0')
	{
		IsNotZero = false;
		Zero = 0;
	}


	return IsNotZero;
}
bool StringUserInputValidation(std::string& InputUser) // validates that their is no bad data, excessive commas or dashes or letters
{
	//flag
	bool StringIsOk = true;

	if (InputUser.length()) {

		// Remove last white space from string
		while (isspace(InputUser[InputUser.length() - 1]))
			InputUser.erase(InputUser.length() - 1, 1);

		for (int i = 0; i < InputUser.length(); i++)
		{
			// Remove white space 
			while (isspace(InputUser[i]))
				InputUser.erase(i, 1);

			// Check for white space in between #'s. I.e: 1   2,3,4..Change to 1,2,3,4
			if (isdigit(InputUser[i]))
			{
				// peek to see when the white space ends and place a comma before it
				// Even if what is next is not valid the remaining code will catch it
				// First check if the next reference is a white space
				if (isspace(InputUser[i + 1]))
				{
					// copy value of i to k to use in a while loop to scan for white
					int k = i + 1;
					// Check white spaces up to next non white space
					while (isspace(InputUser[k]))
						k++;
					// Check to make sure next reference found is not a comma or dash
					if (InputUser[k] != ',' && InputUser[k] != '-')
					{
						// assign comma to reference before next non white space.
						// subtract 1 from k , because value of k is no at the next reference non white space
						InputUser[k - 1] = ',';
					} // inside if					
				} // middle if
			} // 1st if

			  // find each comma and eliminate white space
			if (InputUser[i] == ',')
			{
				std::cout << "Comma Found\n";
				// checks to see if dash is first or last object
				if (i == 0 || i + 1 == InputUser.length())
				{
					StringIsOk = false;
					std::cout << "Input Error, Comma (-) cannot be the first or last Item\n";
					return StringIsOk;
				}

				// check to see if their are multiple dashes or no numbers between dashes
				if (InputUser[i + 1] == ',' || InputUser[i - 1] == ',' || InputUser[i + 1] == '-' || InputUser[i - 1] == '-')
				{
					StringIsOk = false;
					std::cout << "Input Error,multiple dashes(-) or commas (,) found beside eachother\n";
					return StringIsOk;
				}

			} // if

			  // if user entered a dash
			if (InputUser[i] == '-')
			{
				std::cout << "Dash Found\n";

				// checks to see if dash is first or last object
				if (i == 0 || i + 1 == InputUser.length())
				{
					StringIsOk = false;
					std::cout << "Input Error, dash(-) cannot be the first or last object\n";
					return StringIsOk;
				}

				// check to see if their are multiple dashes or no numbers between dashes
				if (InputUser[i + 1] == '-' || InputUser[i - 1] == '-' || InputUser[i + 1] == ',' || InputUser[i - 1] == ',')
				{
					StringIsOk = false;
					std::cout << "Input Error,multiple dashes(-) or commas (,) found beside eachother\n";
					return StringIsOk;
				}

				// Checks to make sure user has not entered 5-5-5 for example
				for (int k = i + 1; k < InputUser.length(); k++)
				{
					// comma is found so we know user did not input 5-5-5
					if (InputUser[k] == ',')
						break;

					// another dash found, error and return
					if (InputUser[k] == '-')
					{
						StringIsOk = false;
						std::cout << "Input Error, Dash needs to be ended with a comma. Like this -> 3-5,6,7\n";
						return StringIsOk;
					}
				} // for					 
			} // if dash is found

			  // if it is not a digit, or a comma or dash it will produce an error
			if (!isdigit(InputUser[i]) && InputUser[i] != ',' && InputUser[i] != '-')
			{
				std::cout << "Invalid Character Found --> " << i << "\n";
				StringIsOk = false;
			}

			// Checks to see if their is only one value which is a Zero
			if (InputUser.length() == 1 && InputUser[0] == '0')
			{
				StringIsOk = false;
				std::cout << "Returning to Menu!\n";
			}
		} // for loop
	}
	else // first if() statement
	{
		std::cout << "Error.  Please Try Again\n";
		StringIsOk = false;
	}
	return StringIsOk;
}
void PracticeCardsManagement(std::shared_ptr<RandomWordGenerator::WordProcessor> processor)
{
	// hold the values of ints, of words that will be selected
	std::deque<int> UserSelectedWordsToAdd;


	// Grabs a collection of ints from the PRactice Cards to validate no duplicates
	std::deque<int> CopyPracticeCards = processor->GetPracticeCards();

	std::string select;
	int choice = 0;

	RandomWordGenerator::FreshPage();  // new screen

									   // print main menu
	std::cout << "Practice Card Management\n"
		<< "------------------------\n\n"
		<< "1.  View All Saved Cards\n"
		<< "2.  Erase All Cards\n"
		<< "3.  Add Specific Card\n"
		<< "4.  Delete Specific Card\n"
		<< "0.  Quit\nUser: ";

	do  // enter loop for main main, user needs to hit 0 to escape
	{

		// reshow options if necessary
		auto ShowOptions = []() {
			RandomWordGenerator::FreshPage();
			std::cout << "Practice Card Management\n"
				<< "------------------------\n\n"
				<< "1.  View All Saved Cards\n"
				<< "2.  Erase All Cards\n"
				<< "3.  Add Specific Card\n"
				<< "4.  Delete Specific Card\n"
				<< "0.  Quit\n";
		};

		// ask for user input, will loop until valid input
		do {
			std::cout << "Select:  ";
			std::cin >> select;
		} while (!ValidateNumber(select)); // keep looping if bad user input

										   // Convert user string to int and assign to choice variable
		choice = stoi(select);

		// Choose proper selection
		if (choice == 1)  // show all practice cards
		{
			processor->DisplayPractice();
			system("pause");
		}
		if (choice == 2) // reset practice cards
		{
			processor->ResetPracticeCards();
			system("pause");
		}
		if (choice == 3) // add practice cards
		{
			std::cout << "\nA list of Cards with a number to left will appear, enter which number(s) you want to add at the bottom\n";
			system("pause");

			// for keypress user input detection
			char KeyPress;
			int ASCIIvalue;
			// flag
			int ZeroEntered = 1;

			// display list for user
			processor->DisplayVecs();

			while (ZeroEntered != 0) {
				KeyPress = _getch();
				ASCIIvalue = KeyPress;

				// if user hits escape will exit loop
				if (ASCIIvalue == 27)
					break;

				// keep looping until user enters a number
				do {
					std::cout << "\nPress 0 to cancel\n"
						<< "Enter each number seperated by a comma, can use a dash to enter a range of numbers i.e-->1,2,3,4-10,11,12\n"
						<< "Which Card Would You Like To Add? -->  ";
					getline(std::cin, select);
				} while (!StringUserInputValidation(select) && StringUserInputIsNotZero(select, ZeroEntered));

				// make sure user hasn't entered 0
				if (ZeroEntered != 0)
				{
					// call a function to parse the string
					UserSelectedWordsToAdd = AddPracticeCardsSeperation(select);
					// call function to add cards to list
					processor->AddMultiplePracticeCards(UserSelectedWordsToAdd);
				}
			}	// while keypress isnt ESCAPE
			ShowOptions();
		}
		if (choice == 4) // delete practice cards
		{
			int UserPick;
			bool DisplayIsOn = false;  // a flag to make sure processor->DisplayPractice isnt called twice
			std::cout << "\nA list of Cards with a number to left will appear, enter which number you want to delete at the bottom\n";
			system("pause");
			char KeyPress;  // for key presses
			int ASCIIvalue;  // for key presses/key strokes
			if (DisplayIsOn == false) {  // checks if DisplayIsOn is true or false
				processor->DisplayPractice();  //for delete single word function
				DisplayIsOn = true;
			}
			while (1) {
				KeyPress = _getch(); // user keystrokes
				ASCIIvalue = KeyPress; // userkeystrokes
				if (ASCIIvalue == 27)  // if user hits escape
					break;
				if (DisplayIsOn == false) {  // checks if DisplayIsOn is true or false
					RandomWordGenerator::FreshPage();
					std::cout << "\nA list of Cards with a number to left will appear, enter which number you want to delete at the bottom\n";
					processor->DisplayPractice();  //for delete single word function
					DisplayIsOn = true;
				}
				if (processor->SizeofPracticeList() > 0) {
					do {
						std::cout << "\nPress 0 to Cancel\nWhich Card Would You Like To Delete? -->  ";
						std::cin >> select;
					} while (!ValidateNumber(select)); // keep looping unbtil valid input is selected
					UserPick = stoi(select); // copy valid input
											 //std::cout << processor->SizeofPracticeList() << " list size\n";
					if (UserPick > 0 && UserPick <= processor->SizeofPracticeList()) {
						processor->DeleteSinglePracticeCard(UserPick);
						DisplayIsOn = false;  // resets the flag to false so it can display again within the loop
					}
					else
						std::cout << "Invalid Select Please try again.  OR press escape to exit\n\n";
				} // if processor->sizeofpracticelist > 0
				else { std::cout << "There are NO cards to Delete, Push Escape to Return\n"; }
			}	// while
			ShowOptions();
		}
		ShowOptions();
	} while (choice != 0); // wqhile
}
void PracticeCardsOption(std::shared_ptr<RandomWordGenerator::WordProcessor> processor)
{
	std::string select;
	int choice;
	do {
		RandomWordGenerator::FreshPage();
		std::cout << "Practice Card Training!\n---------------------\n\n";
		std::cout << "1.  English to Polish\n2.  Polish to English\n3.  View Practice Cards\n0.  Quit\n";
		do {
			std::cout << "Select:  ";
			std::cin >> select;
		} while (!ValidateNumber(select)); // keep looping if bad user input
		choice = stoi(select); // copies good input to choice
		switch (choice) {  // Practice Cards Options
		case 1:
			processor->PracticeCardOne();
			break;
		case 2:
			processor->PracticeCardTwo();
			break;
		case 3:
			processor->DisplayPractice();
			system("pause");
			break;
		}  // inner switch for case 4
	} while (choice != 0);// while loop...whiule choice != 0
}
void Select(int s, std::shared_ptr<RandomWordGenerator::WordProcessor> processor) {
	/// have to create integer for stoi to pass into to because it keeps vailing upon while conditions because their is nothing to convert
	std::string select;
	int choice = 0, NumOfCards = 0;
	switch (s) {
	case 1: // display the list of words
		processor->DisplayVecs();
		break;
	case 2:  // selecting a set of flash cards
		FlashCardsOption(processor);  // calls flash card option function
		break;
	case 3: // reset cards
		processor->ResetFlashCards();
		break;
	case 4: // Practice Cards
		PracticeCardsOption(processor);
		break;
	case 5:  // management of practice cards
		PracticeCardsManagement(processor);
		break;
	case 0:
		std::cout << "Goodbye !\n";
		break;
	}
}


int main() {
	try {

		SetConsoleTitle(TEXT("Polish Practice"));
		// Assignment.  start parsing number in AddPracticeCardsSeperation

		//int gd = DETECT;  
		//int gm;

		//initgraph(&gd, &gm, "C:\\TC\\BGI");
		//settextstyle(BOLD_FONT, HORIZ_DIR, 8);
		//outtextxy(10, 10, "TResting");
		//system("pause");
		//closegraph();
		std::shared_ptr<RandomWordGenerator::WordProcessor> processor;

		processor = std::make_shared<RandomWordGenerator::WordProcessor>();
		std::string choice;
		do {
			std::cout << "*********************MAIN MENU******************\n\n"
				<< "Please Select An Option\n"
				<< "-----------------------\n"
				<< "1.  See List of Words/Definitions\n"
				<< "2.  Start New/Continue Flash Cards\n"
				<< "3.  Reset Flash Cards\n"
				<< "4.  Begin or Continue Saved Practice Card \n"
				<< "5.  Saved Practice Cards Management\n"
				<< "0.  Quit\n";
				do {
					std::cout << "Select an Option: ";
					std::cin >> choice;
					std::cout << "\n";
				} while (!ValidateNumber(choice));
			Select(stoi(choice), processor);
			RandomWordGenerator::FreshPage();
		} while (stoi(choice) != 0);
	}
	catch (std::string e) {
		std::cout << e << "\n";
	}
	return 0;
}