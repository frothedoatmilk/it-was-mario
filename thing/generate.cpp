#include "generate.h"

float min(float a, float b) {
    if(b < 0) return 0;
    if(a < b) return a;
    else      return b;
} // end min

void generate(std::string filename, float amplitude) {

    float seconds = 0.0;
    std::vector<Note> notes;

    std::cout << "Parsing something..." << std::endl;
    seconds = parseInputFile("stuf/" + filename, notes);
    std::cout << "Parsed! Found " << notes.size() << " notes." << std::endl;

    short data;

    // Speed and volume settings
    if(amplitude != 0.2) {
        std::cout << "New amplitude found in arguments..." << std::endl;
        if(amplitude < 0.0 || amplitude > 1.0) {
            std::cout << "Amplitude outside acceptable range" << std::endl;
	    return;
        }
    }

    // Initialize WAV    
    const int subchunk1Size = 16;
    const int audioFormat = 1;
    const int numChannels = 1;
    const int sampleRate = 44100;
    const int blockAlign = 2;
    const int byteRate = sampleRate * blockAlign;
    const int bitsPerSample = 8 * blockAlign / numChannels;
    const int subchunk2Size = sampleRate * blockAlign * (int) (seconds+2);
    const int chunkSize = subchunk1Size + subchunk2Size + 20;

    std::cout << "Opening and formatting WAV..." << std::endl;

    // Write the wav header
    FILE* wav = fopen("music.wav", "wb");
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
    
    std::cout << "Processing " << notes.size() << " chords..." << std::endl;

    for(int index = 0; index < sampleRate*seconds; index++) {
        data = 0;
        for(int i = 0; i < notes.size(); i++) {
            if(notes[i].isNotStart(index)) break;
            data += notes[i].getAmplitude(amplitude, index);
            if(notes[i].isDone()) {notes.erase(notes.begin()+i); i--;}
        } // end k for
        fwrite(&data, sizeof(data), 1, wav);
        // Write right
        if(numChannels == 2) fwrite(&data, sizeof(data), 1, wav);
    } // end j for

    fclose(wav);
} // end main
