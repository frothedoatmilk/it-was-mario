#ifndef STRINGLIBS
#include <string>
#include <cstring>
#endif

#ifndef FILELIB
#include <cstdio>
#endif

#ifndef VECTOR
#include <vector>
#endif

// Function to turn the .stuff format into usable arrays for the synth
// WILL NOT CLOSE THE FILE!
// TODO: IMPLEMENT CORRECTLY.
void parseInputFile (FILE* toParse, std::vector<std::string>& chordArray, std::vector<float>& durationArray);