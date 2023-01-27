#ifndef FILEPARSE_H
#define FILEPARSE_H

#ifndef STRINGLIBS
#define STRINGLIBS
#include <string>
#include <cstring>
#endif

#ifndef FILELIB
#define FILELIB
#include <cstdio>
#endif

#ifndef VECTOR
#define VECTOR
#include <vector>
#endif

#ifndef IO
#define IO
#include <iostream>
#include <fstream>
#endif

#ifndef ALGO
#define ALGO
#include <algorithm>
#endif

#include "chord.h"

float parseInputFile (std::string filename, std::vector<Note> & noteArray);
#endif
