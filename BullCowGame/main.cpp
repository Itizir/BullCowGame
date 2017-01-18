//
//  main.cpp
//  Bull Cow Game
//
//  Created by Diego Becciolini on 10/01/2017.
//  Copyright © 2017 Itizir. All rights reserved.
//


#include "FBullCowGame.hpp"


int main(int argc, const char * argv[])
{
	FText DictionaryPath("isograms.txt"); // set a default...
	if (argc > 1)
		DictionaryPath = argv[1];

	FBullCowGame BCGame(DictionaryPath);
	
	BCGame.PlayGame();
	
	return 0;
}
