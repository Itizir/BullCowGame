//
//  FBullCowGame.cpp
//  BullCowGame
//
//  Created by Diego Becciolini on 13/01/2017.
//  Copyright © 2017 Itizir.
//  This work is free. You can redistribute it and/or modify it under the
//  terms of the Do What The Fuck You Want To Public License, Version 2,
//  as published by Sam Hocevar. See the LICENSE file for more details.
//


#include "FBullCowGame.hpp"


EBCGameStatus FBullCowGame::GetStatus() const { return BCModel.GetStatus(); }

// ****** MARK: - Constructors ******

// Can actually already pass the address of BCModel at this point: no need to manually allocate.
FBullCowGame::FBullCowGame(const FText& DictionaryPath) : BCModel(DictionaryPath), BCView(&BCModel)
{
	BCView.PrintGameStatus(); // Prints intro or error.
	
	// Game successfully created.
	if(GetStatus()==EBCGameStatus::New)
	{
		FText Answer = BCView.PromptAndGetInput(); // Asks if need help with rules.
		if (tolower(Answer[0]) == 'y')
			BCView.PrintRules();
		BCModel.ResetRound(); // Now game is in Round_Reset state, ready to start a round.
	}
}
FBullCowGame::~FBullCowGame()
{
	BCView.PrintExit();
}


// ****** MARK: - Public interface ******

void FBullCowGame::PlayGame()
{
	// Game will be Round_Reset state if created successfully.
	while ( GetStatus() == EBCGameStatus::Round_Reset || ( GetStatus() == EBCGameStatus::Round_Over && WantsToPlayAgain() ) )
	{
		PlayRound();
	}
}


// ****** MARK: - Game interaction ******

void FBullCowGame::PlayRound()
{
	FText PlayerAnswer;
	
	BCModel.ResetRound(); // Happens a second time if the game was brand new. Oh well.
	PlayerAnswer = BCView.PromptAndGetInput(); // Asks for desired word length.
	BCModel.SetRandomHiddenWord(std::atoi(PlayerAnswer.c_str()));
	
	BCView.PrintGameStatus(); // Reveals word length and max tries.
	
	while ( GetStatus() != EBCGameStatus::Round_Over )
	{
		PlayerAnswer = BCView.PromptAndGetInput();
		if(PlayerAnswer.empty()) // Now that Round_Reset and Round_Ready are separate states, can exit the loop here already.
			return;

		BCModel.SubmitGuess(PlayerAnswer);
		BCView.PrintGameStatus();
	}
}

bool FBullCowGame::WantsToPlayAgain() const
{
	FText PlayerAnswer;
	PlayerAnswer = BCView.PromptAndGetInput();
	
	return (tolower(PlayerAnswer[0]) == 'y');
}
