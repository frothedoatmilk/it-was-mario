#ifndef GEN
#define GEN

#include <stdio.h>
#include <math.h>

#ifndef IO
#define IO
#include <iostream>
#include <fstream>
#endif

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

#include "file_parse.h"
#include "chord.h"

float min(float a, float b);

void generate(std::string filename, float amplitude);

#endif
