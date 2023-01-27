#ifndef IO
#define IO
#include <iostream>
#endif

#ifndef STRINGLIBS
#define STRINGLIBS
#include <string>
#include <cstring>
#endif

#include "thing/generate.h"

int main(int argc, char** argv) {
    float amplitude = 0.2;
    if (argc == 2) {
        try {
            amplitude = std::stof(argv[1]);
            if(amplitude > 1.0 || amplitude < 0.0) {
                std::cout << "Amplitude argument invalid, defaulting to 0.2." << std::endl;
                amplitude = 0.2;
            }
        } catch(const std::invalid_argument & a) {
            std::cout << "Amplitude argument invalid, defaulting to 0.2." << std::endl;
            amplitude = 0.2;
        }
    }
    generate("mario.stuf1", amplitude);
}
