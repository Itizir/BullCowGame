//
//  FBullCowGame.cpp
//  BullCowGame
//
//  Created by Diego Becciolini on 13/01/2017.
//  Copyright © 2017 Itizir. All rights reserved.
//

#include "FBullCowGame.hpp"


EBCGameStatus FBullCowGame::GetStatus() const { return BCModel->GetStatus(); }

// ****** MARK: - Constructors ******

FBullCowGame::FBullCowGame(const FText& DictionaryPath)
{
	BCModel = new FBullCowModel(DictionaryPath);
	BCView  = new FBullCowView(BCModel);
	
	BCView->PrintGameStatus(); // Prints intro or error.
	
	if(GetStatus()==EBCGameStatus::New)
	{
		FText Answer = BCView->PromptAndGetInput(); // Asks if need help with rules.
		if (tolower(Answer[0]) == 'y')
			BCView->PrintRules();
		BCModel->ResetRound();
	}
}
FBullCowGame::~FBullCowGame()
{
	BCView->PrintExit();
	
	delete BCView;
	delete BCModel;
}


// ****** MARK: - Public interface ******

void FBullCowGame::PlayGame() const
{
	while ( GetStatus() == EBCGameStatus::Round_Reset || ( GetStatus() == EBCGameStatus::Round_Over && WantsToPlayAgain() ) )
	{
		PlayRound();
	}
}


// ****** MARK: - Game interaction ******

void FBullCowGame::PlayRound() const
{
	FText PlayerAnswer;
	
	BCModel->ResetRound(); // Happens a second time if the game was brand new. Oh well.
	PlayerAnswer = BCView->PromptAndGetInput();
	BCModel->SetRandomHiddenWord(std::atoi(PlayerAnswer.c_str()));
	
	BCView->PrintGameStatus(); // Reveals word length and max tries.
	
	while ( GetStatus() != EBCGameStatus::Round_Over )
	{
		PlayerAnswer = BCView->PromptAndGetInput();
		BCModel->SubmitGuess(PlayerAnswer);
		
		if(PlayerAnswer.empty()) // *After* SubmitGuess to make sure status cannot have stayed as Round_Ready.
			return;

		BCView->PrintGameStatus();
	}
}

bool FBullCowGame::WantsToPlayAgain() const
{
	FText PlayerAnswer;
	PlayerAnswer = BCView->PromptAndGetInput();
	
	return (tolower(PlayerAnswer[0]) == 'y');
}
