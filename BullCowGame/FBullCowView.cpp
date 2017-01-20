//
//  FBullCowView.cpp
//  BullCowGame
//
//  Created by Diego Becciolini on 13/01/2017.
//  Copyright © 2017 Itizir.
//  This work is free. You can redistribute it and/or modify it under the
//  terms of the Do What The Fuck You Want To Public License, Version 2,
//  as published by Sam Hocevar. See the LICENSE file for more details.
//


#include "FBullCowView.hpp"


EBCGameStatus FBullCowViewProtected::GetStatus() const { return BCModel->GetStatus(); }

// ****** MARK: - Constructors ******

FBullCowView::FBullCowView(FBullCowModel* BCPointer) : FBullCowViewProtected(BCPointer) {}
FBullCowViewProtected::FBullCowViewProtected(FBullCowModel* BCPointer) : BCModel(BCPointer) {}


// ****** MARK: - Message selection ******

void FBullCowViewProtected::PrintGameStatus() const
{	
	switch (GetStatus()) {
		case EBCGameStatus::New:
			PrintIntro();
			break;
		case EBCGameStatus::Guess_Accepted:
			PrintGuessFeedback();
			break;
		case EBCGameStatus::Round_Ready:
			PrintRoundStart();
			break;
		case EBCGameStatus::Round_Over:
			PrintRoundSummary();
			break;
		case EBCGameStatus::Guess_Not_Word:
			std::cout << "Nooo! That wasn't a word. Just letters, please.\n";
			break;
		case EBCGameStatus::Guess_Too_Short:
			std::cout << "Nooo! Too short! ";
			std::cout << "The word has " << BCModel->GetHiddenWordLength() << " letters.\n";
			break;
		case EBCGameStatus::Guess_Too_Long:
			std::cout << "Nooo! Too long! ";
			std::cout << "The word has " << BCModel->GetHiddenWordLength() << " letters.\n";
			break;
		case EBCGameStatus::Guess_Not_Isogram:
			std::cout << "Nooo! All the letters need to be different. Pfff. Yeah, I know...\n";
			break;
		case EBCGameStatus::Guess_Not_New:
			std::cout << "Oh come on! You've tried that before!\n";
			break;
		case EBCGameStatus::No_Dictionary:
			NoDictionaryError();
			break;
		case EBCGameStatus::Empty_Dictionary:
			EmptyDictionaryError();
			break;
		case EBCGameStatus::NotWord_Dictionary:
			NotWordDictionaryError();
			break;
		case EBCGameStatus::NotIsogram_Dictionary:
			NotIsogramDictionaryError();
			break;
		default:
			std::cerr << "ERROR! Game in unexpected state no <" << (int)GetStatus() << ">!" << std::endl;
			break;
	}
	return;
	
}

FText FBullCowViewProtected::PromptAndGetInput() const
{
	FText Answer;
	
	switch (GetStatus()) {
		case EBCGameStatus::New:
			PromptForHelp();
			break;
		case EBCGameStatus::Round_Reset:
			PromptForWordLength();
			break;
		case EBCGameStatus::Round_Ready:
		case EBCGameStatus::Guess_Accepted:
			PromptForGuess();
			break;
		case EBCGameStatus::Round_Over:
			PromptForReplay();
			break;
		case EBCGameStatus::Guess_Not_Word:
		case EBCGameStatus::Guess_Too_Short:
		case EBCGameStatus::Guess_Too_Long:
		case EBCGameStatus::Guess_Not_Isogram:
		case EBCGameStatus::Guess_Not_New:
			std::cout << "Try again : ";
			break;
		default:
			std::cerr << "ERROR! Game in unexpected state no <" << (int)GetStatus() << ">!" << std::endl;
			std::cerr << "And awaiting input... ";
			break;
	}
	
    std::getline(std::cin, Answer);
	return Answer;
}


// ****** MARK: - Prompt messages ******

void FBullCowViewProtected::PromptForGuess() const
{
	if (BCModel->GetCurrentTry() == BCModel->GetMaxTries())
		std::cout << "\nOh my! You're on your last attempt?! Good luck with that : ";
	else
	{
		std::cout << "\nGo on then, you're on attempt " << BCModel->GetCurrentTry();
		std::cout << " out of " << BCModel->GetMaxTries() << " : ";
	}
	
	return;
}

void FBullCowViewProtected::PromptForWordLength() const
{
	std::cout << "\nHow eager are you? How many letter do you think you can guess?\n";
	std::cout << "The dictionary knows words between " << BCModel->GetMinLetters();
	std::cout << " and " << BCModel->GetMaxLetters() << " letters long.\n";
	std::cout << "If the dictionary cannot grant your wish or cannot understand what you want,\n";
	std::cout << "it will choose for you...\n";
	std::cout << "So? What shall it be : ";
	
	return;
}

void FBullCowViewProtected::PromptForHelp() const
{
	std::cout << "\nAre you new to the game? Would you like to read the rules?\n[y]up / [n]op : ";
	return;
}

void FBullCowViewProtected::PromptForReplay() const
{
	std::cout << "\nHow about another nice game of 'BULLS 'n' COWS'?\n";
	std::cout << "(with a brand new secret word, how exciting!)\n";
	std::cout << "[y]es please / [n]o fanks : ";
	return;
}


// ****** MARK: - Intro, rules, exit ******

void FBullCowViewProtected::PrintIntro() const
{
	std::cout << R"fence(
                   (___)                               David A. Bader sure has
                   (o o)                               a nice ranch out there...
             /------\ /    (__)
            /   _____O     (oo)
            |  / /----\-----\/
            /\oo===|  /    ||
            | ||  *||^-----||
            *  ^^  ^^      ^^
)fence";
    std::cout << "\nWelcome to 'BULLS 'n' COWS'!\nA silly guess-the-word game.\n\n";

    return;
}

void FBullCowViewProtected::PrintRules() const
{
	std::cout << "\nThe Rules of the 'BULLS 'n' COWS' Game!\n\n";
	std::cout << "The aim of the game is to guess a single word of a given length\n";
	std::cout << "within a limited number of tries.\n";
	std::cout << "The word to guess is an isogram: no letter appears twice!\n";
	std::cout << "\nThe only feedback you get after each attempt is\n";
	std::cout << "the count of BULLS and COWS :\n";
	std::cout << " - correct letters in the right place are called BULLS\n";
	std::cout << " - correct letters in the wrong place are called COWS\n";
	std::cout << "\nOh! And an empty submission will abort the game!\n";
	std::cout << "\nThat's all! Good luck!\nPress Return to continue...";
	FText ThrowAwayString;
	std::getline(std::cin, ThrowAwayString);
	return;
}

void FBullCowViewProtected::PrintExit() const
{
	if (GetStatus() == EBCGameStatus::Round_Over)
		std::cout << "\nBe seein' ya!\n" << std::endl;
	// Could probably combine error status into one and avoid this kind of thing...
	else if (  GetStatus() == EBCGameStatus::No_Dictionary
		    || GetStatus() == EBCGameStatus::Empty_Dictionary
			|| GetStatus() == EBCGameStatus::NotIsogram_Dictionary
			|| GetStatus() == EBCGameStatus::NotWord_Dictionary
			)
		std::cout << "\nFix that dictionary issue, will ya?\n" << std::endl;
	else
		std::cout << "\nHmm, leaving in a hurry, are we? Well, so long...\n" << std::endl;
	return;
}


// ****** MARK: - Game information messages ******

void FBullCowViewProtected::PrintRoundStart() const
{
	std::cout << "\nOkey doke, the word lenght is " << BCModel->GetHiddenWordLength();
	std::cout << "\nand you can only guess " << BCModel->GetMaxTries() << " times.\n";
	std::cout << "Let's hear your guesses!\n(remember, make an empty guess to quit the game early, chicken)\n";
	return;
}

void FBullCowViewProtected::PrintGuessFeedback() const
{
	FBullCowCount Score = BCModel->GetCurrentScore();
	
	FText BullPlural = (Score.Bulls==1? "! " : "s!");
	FText CowPlural = (Score.Cows==1? "... " : "s...");

	// Sorry, this looks ugly here... :s
	std::cout << "All rightee!\n";
	std::cout << "\nYou scored " << std::setw(2) << std::right << Score.Bulls;
	std::cout << " Bull" + BullPlural;
	std::cout << ((Score.Bulls==0)?"  Boo.":"      ");
	std::cout << "          ( )\n";
	std::cout << "and " << std::setw(9) << std::right << Score.Cows << " Cow" + CowPlural;
	std::cout << ((Score.Cows>0)?" Moo.":"     ");
	std::cout << "       /---V   `v'-\n";
	std::cout << std::setw(26) << std::left << "with '" + BCModel->GetCurrentGuess() + "'";
	std::cout << "      * |--|    || |`.\n";
	
	PrintGuessesSoFar();
}

void FBullCowViewProtected::PrintGuessesSoFar() const
{
	// For now print in reverse-chronological order.
	// Other possible design choice: by score (then perhaps use multimap where score is key for that).
	std::cout << "\nYour guesses so far :  BULLS  COWS\n";
	for (auto GuessIter = BCModel->GetGuessChronology().crbegin(), StopIter = BCModel->GetGuessChronology().crend(); GuessIter!=StopIter; ++GuessIter )
	{
		std::cout << std::setw(19) << std::right << (*GuessIter)->first << " :";
		std::cout << "  " << std::setw(7) << std::left << (*GuessIter)->second.Bulls;
		std::cout << (*GuessIter)->second.Cows;
		std::cout << std::endl;
	}
}

void FBullCowViewProtected::PrintRoundSummary() const
{
	if (BCModel->IsGameWon())
	{
		std::cout << R"fence(
                  vv                                       vv
             (__)  \\        (__)            (__)         //   (__)
             (oo)  //        (oo)            (oo)         \\   (oo)
            __\/__//        __\/__          __\/__         \\ __\/__
          //      /       //      \\      //      \\        \       \\
         //|      |      //|      |\\    //|      |\\        |      |\\
         \\|      |      \\|      |//    \\|      |//        |      |//
      Z___||||||||||      ||||||||||      ||||||||||        ||||||||||___Z
      Z--------||          //    \\        //    \\            ||--------Z
               ||          \\    //        \\    //            ||
               ||           \\  //          \\  //             ||
               AA            AA AA           AA AA             AA
)fence";

		
		std::cout << "\nWEEEEHAAA! Ooh yeah! Well done!!!\n";
		std::cout << "The magic word was indeed '" << BCModel->RevealHiddenWord() << "'! Aces!\n";

		int32 MovesLeft = 1 + BCModel->GetMaxTries() - BCModel->GetCurrentTry();
		FText Plural = MovesLeft>1? "s" : "";
		if (BCModel->GetCurrentTry()<=2) // If won on first try.
		{
			std::cout << "Oh wait, " << MovesLeft << " move" + Plural + " left?!? Please don't cheat next time, OK? :(\n";
			// Plural shouldn't actually be an issue: MaxTries shouldn't be set as low as 2 anyway...
		}
		else if (BCModel->GetCurrentTry()==1+BCModel->GetMaxTries())
		{
			std::cout << "Phew! That was a close call...\n";
		}
		else if (BCModel->GetCurrentTry()<=BCModel->GetMaxTries())
		{
			std::cout << "Wow! You even had " << MovesLeft << " attempt" + Plural + " remaining. ";
			std::cout << "Real champ.\n";
		}

	}
	else
	{
		std::cout << R"fence(
                            (___)
                   /--------(x x)                  (__)
                  / \       /\ /                   (xx)------\
                 *   \     /  o                     \/\     / *
          ============-----============================-----==========
)fence";
		std::cout << "\nAw, too bad! Better luck next time...\n";
		std::cout << "The word I was thinking of was '" << BCModel->RevealHiddenWord() << "'. Heyup.\n";
	}

	return;
}


// ****** MARK: - Error messages ******

void FBullCowViewProtected::NoDictionaryError() const
{
	std::cerr << "ERROR! Failed to load dictionary \"" + BCModel->GetDictionaryName() + "\"!" << std::endl;
	return;
}

void FBullCowViewProtected::EmptyDictionaryError() const
{
	std::cerr << "ERROR! The dictionary \"" + BCModel->GetDictionaryName() + "\" seems empty." << std::endl;
	return;
}

void FBullCowViewProtected::NotWordDictionaryError() const
{
	std::cerr << "ERROR! The dictionary \"" + BCModel->GetDictionaryName() + "\" contains non-letter characters!" << std::endl;
	return;
}

void FBullCowViewProtected::NotIsogramDictionaryError() const
{
	std::cerr << "ERROR! The dictionary \"" + BCModel->GetDictionaryName() + "\" contains non-isograms!" << std::endl;
	return;
}
