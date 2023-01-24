#include <stdio.h>
#include <math.h>

#ifndef IO
#include <iostream>
#endif

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

#include "file_parse.h"
#include "chord.cpp" // FIXME: I don't know C++ enough to do or care about header files

float min(float a, float b) {
    if(b < 0) return 0;
    if(a < b) return a;
    else      return b;
} // end min

int main(int argc, char* argv[]) {

    const float bpm = 180;
    std::vector<float> durations;
    std::vector<std::string> strings;
    std::vector<Chord> chord;

    // THIS IS NOT IMPLEMENTED. 
    std::cout << "Parsing something..." << std::endl;
    parseInputFile(NULL, strings, durations);
    std::cout << "Parsed! Found " << chords.size() << " chords." << std::endl;

    double beats = 0.0;
    for(int i = 0; i < strings.size; i++) {
        chords.push_back(Chord(strings[i], beats, durations[i], bpm));
        beats += durations[i];
    }
    
    short data;
    long long index = 0;

    // Speed and volume settings
    float amplitude = 0.2;

    if(argc == 2) {
        std::cout << "New amplitude found in arguments..." << std::endl;
        float checkAmp = std::stof(argv[1]);
        if(checkAmp >= 0.0 && checkAmp <= 1.0) {
            amplitude = checkAmp;
            std::cout << "New amplitude is: " << checkAmp << std::endl;
        }
    }

    const float seconds = beats * 60 / bpm;

    // Initialize WAV    
    // fmt chunk, also can be used as a config kinda?
    const int subchunk1Size = 16;
    const int audioFormat = 1;
    const int numChannels = 1;
    const int sampleRate = 44100;
    const int blockAlign = 4;
    const int byteRate = sampleRate * blockAlign;
    const int bitsPerSample = 8 * blockAlign / numChannels;

    const int subchunk2Size = sampleRate * blockAlign * (int) (seconds+2);

    const int chunkSize = subchunk1Size + subchunk2Size + 20;

    std::cout << "Opening and formatting WAV..." << std::endl;
    // Write the wav header
    FILE* wav = fopen("test.wav", "wb");
    fwrite("RIFF", sizeof(char), 4, wav);
    fwrite(&chunkSize, sizeof(chunkSize), 1, wav);
    fwrite("WAVEfmt ", sizeof(char), 8, wav);

    fwrite(&subchunk1Size , sizeof(int),   1, wav);
    fwrite(&audioFormat   , sizeof(int)/2, 1, wav);
    fwrite(&numChannels   , sizeof(int)/2, 1, wav);
    fwrite(&sampleRate    , sizeof(int),   1, wav);
    fwrite(&byteRate      , sizeof(int),   1, wav);
    fwrite(&blockAlign    , sizeof(int)/2, 1, wav);
    fwrite(&bitsPerSample , sizeof(int)/2, 1, wav);
    fwrite("data", sizeof(char), 4, wav);
    fwrite(&subchunk2Size, sizeof(int), 1, wav);
    
    double decres;
    float notes[10];
    int length;

    std::cout << "Processing " << numChords << " chords..." << std::endl;
    for(int i = 0; i < numChords; i++) {
        parseNotes(chords[i], notes, &length);
        for(int j = 0; j < sampleRate*seconds*durations[i]/beats; j++) {
            data = 0;
            for(auto & chord : chords) {
                data += chord.getAmplitude(amplitude, index);
            } // end k for
            index++;
            // Make int
            //dataint = data * pow(2, bitsPerSample);
            //if(dataint >= pow(2, bitsPerSample)) { std::cout << "Writing impossible value to WAV at sample " << index << "!" << std::endl; }
            // Write left
            fwrite(&dataint, sizeof(data), 1, wav);
            // Write right
            if(numChannels == 2) fwrite(&dataint, sizeof(data), 1, wav);
        } // end j for
    } // end i for

    fclose(wav);

    return 0;

} // end main
