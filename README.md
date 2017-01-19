# BullCowGame
The Game of Bulls 'n' Cows — Little Mastermind-like word-guessing game made as part of Ben Tristem's [Unreal course on Udemy](https://www.udemy.com/unrealcourse/).

The reference version of the game is available [here](https://github.com/UnrealCourse/02_BullCowGame).

## Features
Major changes and additions include:
- Redesigned structure of the code with all the code for printing to the console in its own [FBullCowView](BullCowGame/FBullCowView.hpp) class, and control of the game moved from main to a dedicated [FBullCowGame](BullCowGame/FBullCowGame.hpp) class.
- Support of custom external dictionaries. The path to a dictionary file can be passed as an argument to the program.
- Word-length selection.
- The list of past guesses is updated and printed each turn.

## Dictionary files
Three different dictionaries are provided:
- [isograms.txt](BullCowGame/isograms.txt), the default, is derived from [one of Wiktionary's frequency lists](https://en.wiktionary.org/wiki/Wiktionary:Frequency_lists/PG/2006/04/1-10000) (3683 words)
- [isograms_nouns.txt](BullCowGame/isograms_nouns.txt) is inspired by [a reduced list of common nouns](http://www.talkenglish.com/vocabulary/top-1500-nouns.aspx) for an easier selection. (579 words)
- [isograms_all.txt](BullCowGame/isograms_all.txt) is a much bigger list containing potentially [all English words](https://github.com/dwyl/english-words). (62229 words)

All of these are filtered for letter-only isograms and trimmed of words that are less than 3 letters, plus some additional selection depending on the list (removing too long words to keep large enough sets of each size, etc.).

## Acknowledgments
The game features ASCII cows from the [vast collections](http://www.chris.com/ascii/joan/www.geocities.com/SoHo/7373/cows.html) curated by David A. Bader.

I also want to thank Lukáš Venhoda and Dan Marshall for useful comments.
