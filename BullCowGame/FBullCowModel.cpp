//
//  FBullCowModel.cpp
//  Bull Cow Game Model
//
//  Created by Diego Becciolini on 11/01/2017.
//  Copyright © 2017 Itizir. All rights reserved.
//

#include "FBullCowModel.hpp"


// ****** MARK: - Constructors ******

FBullCowModel::FBullCowModel(const FString& DictionaryPath) : FBullCowModelProtected(DictionaryPath) {}
FBullCowModelProtected::FBullCowModelProtected(const FString& DictionaryPath) :
DictionaryName(DictionaryPath),
MinLetters(27),MaxLetters(0)
{
	LoadDictionary(DictionaryPath);
}


// ****** MARK: - Getters ******

EBCGameStatus FBullCowModelProtected::GetStatus() const { return CurrentStatus; }
int32 FBullCowModelProtected::GetMaxTries() const { return MyMaxTries; }
int32 FBullCowModelProtected::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowModelProtected::GetHiddenWordLength() const
{
	return (int32)MyHiddenWord.length();
}
int32 FBullCowModelProtected::GetMinLetters() const { return MinLetters; }
int32 FBullCowModelProtected::GetMaxLetters() const { return MaxLetters; }
FBullCowCount FBullCowModelProtected::GetCurrentScore() const { return MyCurrentScore; }
FString FBullCowModelProtected::GetCurrentGuess() const { return MyCurrentGuess; }
TMap<FString, FBullCowCount> const& FBullCowModelProtected::GetGuessHistory() const { return MyGuessHistory; }
std::vector< TMap<FString, FBullCowCount>::const_iterator > const& FBullCowModelProtected::GetGuessChronology() const { return MyGuessChronology; }
FString FBullCowModelProtected::GetDictionaryName() const { return DictionaryName; }
bool FBullCowModelProtected::IsGameWon() const { return bGameIsWon; }

FString FBullCowModelProtected::RevealHiddenWord() const
{
	if (GetStatus() == EBCGameStatus::Round_Over)
		return MyHiddenWord;
	else
		return "NO CHEATING, NAUGHTY!";
}


// ****** MARK: - Game state update / game logic ******

void FBullCowModelProtected::ResetRound()
{
	MyCurrentGuess = "";
	MyGuessChronology.clear();
	MyGuessHistory.clear();
	MyCurrentTry = 1;
	bGameIsWon = false;
	
	// Needs to be set afterwards by calling SetRandomHiddenWord()!
	MyHiddenWord = "";
	MyMaxTries = 0;
	
	CurrentStatus = EBCGameStatus::Round_Reset;
	
	return;
}

void FBullCowModelProtected::SetRandomHiddenWord(int32 WordLength=0)
{
	std::uniform_int_distribution<unsigned long> PickWordLength(GetMinLetters(),GetMaxLetters());
	
	while (WordLength < GetMinLetters() || WordLength > GetMaxLetters() || Dictionary[WordLength].empty())
		WordLength = (int32)PickWordLength(RD);
	
	
	std::uniform_int_distribution<unsigned long> UniformDistr(0,Dictionary[WordLength].size()-1);
	
	MyHiddenWord = Dictionary[WordLength][UniformDistr(RD)];
	
	SetMaxTries();
	
	CurrentStatus = EBCGameStatus::Round_Ready;
	return;
}

void FBullCowModelProtected::SetMaxTries()
{
	// !!!: Still unsure what the scaling should be.
	MyMaxTries = (int32)MyHiddenWord.length()+5;
	return;
}

void FBullCowModelProtected::SubmitGuess(const FString& Guess)
{
	// May not be valid!
	// Store to make sure it is capitalised (to check against history).
	MyCurrentGuess = Guess;
	transform(MyCurrentGuess.begin(), MyCurrentGuess.end(), MyCurrentGuess.begin(), toupper);
	
	
	if (!IsAlpha(GetCurrentGuess()))
		CurrentStatus = EBCGameStatus::Guess_Not_Word;
	
	else if (GetCurrentGuess().length() < GetHiddenWordLength())
		CurrentStatus = EBCGameStatus::Guess_Too_Short;
	
	else if (GetCurrentGuess().length() > GetHiddenWordLength())
		CurrentStatus = EBCGameStatus::Guess_Too_Long;
	
	else if (!IsIsogram(GetCurrentGuess()))
		CurrentStatus = EBCGameStatus::Guess_Not_Isogram;
	
	// Looks like the count(k) analogue in TMap would be Contains(k).
	else if ( GetGuessHistory().count(GetCurrentGuess()) )
		CurrentStatus = EBCGameStatus::Guess_Not_New;

	
	// Could put a 'return' here, to make it clear last condition only if all tests passed.
	
	else // If guess passed all tests: is valid.
	{
		++MyCurrentTry;
		if (GetCurrentTry() > GetMaxTries())
			CurrentStatus = EBCGameStatus::Round_Over;
		else
			CurrentStatus = EBCGameStatus::Guess_Accepted;
		
		ScoreCurrentGuess();
	}
	
	return;
}

void FBullCowModelProtected::ScoreCurrentGuess()
{
	int32 WordLength = GetHiddenWordLength();
	
	
	// Before: if (GetCurrentGuess().size() != WordLength)
	// Replaced with status check:
	if (GetStatus() != EBCGameStatus::Guess_Accepted && GetStatus() != EBCGameStatus::Round_Over)
		return;
	// In that case, current guess cannot be valid, thus score is 0 (i.e. untouched).
	// But should not be happening.
	
	
	MyCurrentScore = FBullCowCount(); // Reset current score!
	for(int32 MHWi = 0; MHWi < WordLength; ++MHWi)
	for(int32 Gi = 0; Gi < WordLength; ++Gi)
	{
		if (MyHiddenWord[MHWi] == GetCurrentGuess()[Gi])
		{
			if (MHWi==Gi)
				++MyCurrentScore.Bulls;
			else
				++MyCurrentScore.Cows;
		}
	}
	
	// Record new score in history.
	MyGuessChronology.push_back(
		MyGuessHistory.emplace(GetCurrentGuess(), GetCurrentScore()).first
	);
	
	bGameIsWon = (GetCurrentScore().Bulls == WordLength);
	if (bGameIsWon)
		CurrentStatus = EBCGameStatus::Round_Over;
	
	return;
}


// ****** MARK: - Word checking ******
		
bool FBullCowModelProtected::IsAlpha(const FString& Word) const
{
	return ( find_if_not(Word.cbegin(), Word.cend(), isalpha) == Word.end() );
}

bool FBullCowModelProtected::IsIsogram(const FString& Word) const
{
	if (Word.length()<2) { return true; }
	
	// Idea of using bitset from Lukáš Venhoda.
	// Take full char range so to not rely on Word being only letters.
	std::bitset< 1<<8*sizeof(Word[0]) > LetterSeen; // Should be 256 for standard 8-bit chars.
	LetterSeen.reset();
	
	for (auto Letter : Word)
	{
		// Here just to make sure mixed case still counts as same letter.
		// Word still recorded in consistent case at this point, potentially
		Letter = toupper(Letter);
		
		if (LetterSeen[Letter])
			return false;
		else
			LetterSeen[Letter] = 1;
	}
	
	return true;
}


// ****** MARK: - Load and use dictionary ******

void FBullCowModelProtected::LoadDictionary(const FString& DictionaryName)
{
	FString Word;
	std::ifstream File;
	File.open(DictionaryName);
	
	if (!File.is_open())
	{
		CurrentStatus = EBCGameStatus::No_Dictionary;
		return;
	}
	
	while (File >> Word)
	{
		transform(Word.begin(), Word.end(), Word.begin(), toupper); // Make sure all in upper-case.
		
		if (!IsAlpha(Word))
		{
			CurrentStatus = EBCGameStatus::NotWord_Dictionary;
			return;
		}
		else if (!IsIsogram(Word))
		{
			CurrentStatus = EBCGameStatus::NotIsogram_Dictionary;
			return;
		}
		else
		{
			MaxLetters = std::max(MaxLetters, (int32)Word.length());
			MinLetters = std::min(MinLetters, (int32)Word.length());
			
			Dictionary[(int32)Word.length()].push_back(Word);
		}
		
	//	// Could also simply skip invalid words instead of putting game in error state:
	//	if (IsAlpha(Word) && IsIsogram(Word))
	//		Dictionary[(int32)Word.length()].push_back(Word);
	}
	
	if (Dictionary.empty())
	{
		CurrentStatus = EBCGameStatus::Empty_Dictionary;
		return;
	}
		
	CurrentStatus = EBCGameStatus::New;
	return;
}
