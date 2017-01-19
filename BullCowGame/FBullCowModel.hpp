//
//  FBullCowModel.hpp
//  Bull Cow Game Model
//
//  Created by Diego Becciolini on 11/01/2017.
//  Copyright © 2017 Itizir.
//  This work is free. You can redistribute it and/or modify it under the
//  terms of the Do What The Fuck You Want To Public License, Version 2,
//  as published by Sam Hocevar. See the LICENSE file for more details.
//


#ifndef FBullCowModel_hpp
#define FBullCowModel_hpp

#include <fstream>
#include <random>
#include <string>
#include <unordered_map> // For 'TMap'
#include <bitset> // Used in IsIsogram
#include <vector> // For dictionary and MyGuessChronology
#include <algorithm> // min,max,find


// ****** MARK: - Helper definitions ******

// Unreal-like type names.
#define TMap std::unordered_map
using FString = std::string;
using int32 = int;

/// Game status!
enum class EBCGameStatus
{
	New, // Newly created, dictionary loaded.
	Round_Reset, // Parameters reinitialised, waiting for hidden word to be set.
	Round_Ready, // Hidden word set, everything ready.
	Round_Over, // Win or loss.
	
	Guess_Not_Word, // Contains non-letter chars.
	Guess_Too_Short,
	Guess_Too_Long,
	Guess_Not_Isogram, // Duplicate letters.
	Guess_Not_New, // Already tried.
	Guess_Accepted,
	
	No_Dictionary,
	Empty_Dictionary,
	NotWord_Dictionary,
	NotIsogram_Dictionary
};

/// Bulls and Cows initialised to 0.
struct FBullCowCount
{
	int32 Bulls = 0;
	int32 Cows = 0;
};


// ****** MARK: - Class definition ******

class FBullCowModelProtected
{
// ****** MARK: - Protected: reserved for FBullCowGame (and FBullCowView) ******
protected:
	
	/// @param DictionaryPath Path to dictionary.
	FBullCowModelProtected(const FString& DictionaryPath);
	
	// Getters.
	EBCGameStatus GetStatus() const;
	int32 GetMaxTries() const;
	int32 GetCurrentTry() const;
	int32 GetHiddenWordLength() const;
	int32 GetMinLetters() const;
	int32 GetMaxLetters() const;
	FBullCowCount GetCurrentScore() const;
	FString GetCurrentGuess() const;
	TMap<FString, FBullCowCount> const& GetGuessHistory() const;
	std::vector< TMap<FString, FBullCowCount>::const_iterator > const& GetGuessChronology() const;
	FString GetDictionaryName() const;
	bool IsGameWon() const;
	/**
	 Reveals what the hidden word was once the game is over.
	 
	 @return The Hidden word.
	 */
	FString RevealHiddenWord() const;
	
	/**
	 Reinitialises all game parameters, but does not set the hidden word!
	 Sets game in Round_Reset state.
	 */
	void ResetRound();
	/**
	 Picks a random word from the dictionary and sets it as the new hidden word.
	 If invalid or unavailable WordLength, picks a word length at random.
	 Sets game in Round_Ready state.
	 
	 @param WordLength Word length requested.
	 */
	void SetRandomHiddenWord(int32 WordLength);
	
	/**
	 Main interface used by FBullCowGame.
	 Check if given guess is isogram of valid format
	 (word with all different letters and right length).
	 If yes, increments try number and records current guess
	 (calls then ScoreCurrentGuess() to keep state of game up-to-date).
	 
	 @param Guess Guessed string.
	 */
	void SubmitGuess(const FString& Guess);
	
	/**
	 Checks score of the current guess:
	 - counts bulls and cows to update MyCurrentScore
	 - updates win status
	 */
	void ScoreCurrentGuess();
	
	
// ****** MARK: - Private ******
private:
	
	// Private: should not be created without a dictionary!
	FBullCowModelProtected();
	
	EBCGameStatus CurrentStatus;
	FBullCowCount MyCurrentScore;
	bool bGameIsWon;
	
	FString MyHiddenWord;
	FString MyCurrentGuess;
	
	// Map for easy lookup. Vector of iterators to access elements of MyGuessHistory in chronological order.
	TMap<FString, FBullCowCount> MyGuessHistory;
	std::vector< TMap<FString, FBullCowCount>::const_iterator > MyGuessChronology;
	
	int32 MyCurrentTry;
	int32 MyMaxTries;
	
	FString DictionaryName;
	TMap< int32, std::vector<FString> > Dictionary; // Words accessed by their length.
	int32 MinLetters; // Shortest word length in dictionary.
	int32 MaxLetters; // Longest word length in dictionary.
	
	// Random generator!
	// Thanks to Dan Marshall for pointing out alternative to deterministic engines!
	std::random_device RD;
	
	/**
	 Checks if the argument is a word of letters only.

	 @param Word Any FString.
	 */
	bool IsAlpha(const FString& Word) const;
	
	/**
	 Checks if the argument is an isogram:
	 a word without repeating characters.

	 @param Word Any FString
	 */
	bool IsIsogram(const FString& Word) const;
	
	
	/**
	 Tries to load the provided dictionary at creation.
	 Failing so puts the game in an error state (EBCGameStatus::No_Dictionary, etc.).
	 If success, game is in EBCGameStatus::New state.

	 @param DictionaryName Path to the dictionary file.
	 */
	void LoadDictionary(const FString& DictionaryName);
	/**
	 Sets the allowed number of tries.
	 Called by SetRandomHiddenWord because potentially could be made to depend on
	 length of chosen word.
	 */
	void SetMaxTries();
};


// ****** MARK: - Giving access to friends ******

// Pre-declarations to give them friend status.
class FBullCowGame;
class FBullCowViewProtected;

/**
 FBullCowGame and FBullCowView get exclusive access to protected method of FBullCowModelProtected.
 FBullCowGame to act on the game state.
 FBullCowView to read the game state.
 */
class FBullCowModel : private FBullCowModelProtected
{
	friend FBullCowGame;
	friend FBullCowViewProtected;
private:
	FBullCowModel(const FString&);
};


#endif /* FBullCowModel_hpp */
