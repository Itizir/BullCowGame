//
//  FBullCowView.hpp
//  BullCowGame
//
//  Created by Diego Becciolini on 13/01/2017.
//  Copyright © 2017 Itizir.
//  This work is free. You can redistribute it and/or modify it under the
//  terms of the Do What The Fuck You Want To Public License, Version 2,
//  as published by Sam Hocevar. See the LICENSE file for more details.
//


#ifndef FBullCowView_hpp
#define FBullCowView_hpp

#include <iostream>
#include <iomanip>
#include "FBullCowModel.hpp"


// ****** MARK: - Helper definitions ******

// Unreal-like type names.
using FText = std::string;
using int32 = int;


// ****** MARK: - Class definition ******

/**
 Interface class for the Bull Cow Game.
 Can only be accessed by FBullCowGame through FBullCowView.
 */
class FBullCowViewProtected
{
// ****** MARK: - Protected: reserved for FBullCowGame ******
protected:
	/// Need to pass pointer to the FBullCowModel.
	FBullCowViewProtected(FBullCowModel*);
	
	/// Prints the rules of the game.
	void PrintRules() const;
	
	/// Prints relevant information depending on state of the game.
	void PrintGameStatus() const;
	
	/// Message printed at the destruction of a game.
	void PrintExit() const;
	
	/// Prompts the player in a way relevant to the current state of the game.
	/// @return The player answer.
	FText PromptAndGetInput() const;
	
	
// ****** MARK: - Private ******
private:
	/// Private: should only be created with FBullCowModel provided!
	FBullCowViewProtected();
	
	// Game model should not change and be changed by the view.
	const FBullCowModel* const BCModel;
	
	EBCGameStatus GetStatus() const;
	
	
	/// Prints splash screen.
	void PrintIntro() const;
	
	/// Prompt at creation of game for game rules.
	void PromptForHelp() const;
	/// Asks requested word length for round setup.
	void PromptForWordLength() const;
	/// Initial prompt for a given guess number.
	void PromptForGuess() const;
	/// Prompt after round is over.
	void PromptForReplay() const;
	
	/// Prints the number of letter in the hidden word.
	void PrintRoundStart() const;
	
	/// Prints the Bull and Cow tally.
	void PrintGuessFeedback() const;
	void PrintGuessesSoFar() const;
	
	/// Prints win/lose status.
	void PrintRoundSummary() const;
	
	// Dictionary ERROR messages.
	void NoDictionaryError() const;
	void EmptyDictionaryError() const;
	void NotWordDictionaryError() const;
	void NotIsogramDictionaryError() const;
};


// ****** MARK: - Giving access to friends ******

/**
 FBullCowGame gets exclusive access to protected method of FBullCowViewProtected.
 */
class FBullCowView : private FBullCowViewProtected
{
	friend FBullCowGame;
private:
	FBullCowView(FBullCowModel*);
};


#endif /* FBullCowView_hpp */
