//
//  main.cpp
//  Bull Cow Game
//
//  Created by Diego Becciolini on 10/01/2017.
//  Copyright © 2017 Itizir.
//  This work is free. You can redistribute it and/or modify it under the
//  terms of the Do What The Fuck You Want To Public License, Version 2,
//  as published by Sam Hocevar. See the LICENSE file for more details.
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
