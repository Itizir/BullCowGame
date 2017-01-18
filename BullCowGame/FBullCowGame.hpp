//
//  FBullCowGame.hpp
//  BullCowGame
//
//  Created by Diego Becciolini on 13/01/2017.
//  Copyright © 2017 Itizir. All rights reserved.
//

#ifndef FBullCowGame_hpp
#define FBullCowGame_hpp

#include "FBullCowView.hpp"


// ****** MARK: - Class definition ******

/**
 Class through which the Bull Cow Game is accessed.
 It has exclusive access to the FBullCowModel and FBullCowView.
 Need to provide path to a valid dictionary (list of isograms).
 */
class FBullCowGame
{
// ****** MARK: - Public ******
public:
	
	/// @param DictionaryPath Path to dictionary.
	FBullCowGame(const FText& DictionaryPath);
	/// Deletes game instance and prints exit message.
	~FBullCowGame();
	
	/// Start the game!
	void PlayGame() const;
	
	
// ****** MARK: - Private ******
private:
	FBullCowModel* BCModel;
	FBullCowView* BCView;
	
	EBCGameStatus GetStatus() const;
	
	void PlayRound() const;
	/// Asks if player wants to play again.
	/// @return Whether the player does want another round.
	bool WantsToPlayAgain() const;
};

#endif /* FBullCowGame_hpp */
