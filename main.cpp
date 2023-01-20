#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string>
#include <cstring>

// Easier to process notes with these enums
enum State{NO_NOTE, NOTE, VALID};
enum Accidental{NONE, SHARP, FLAT};

void parseNotes(const std::string* CHORD, float notes[10], int* length) {
	// create the twelvth roots of 2
	float twelve[12];
	for(int i = 0; i < 12; i++) {
		twelve[i] = pow(2,i/12.);
	} // end for

	*length = 0;
	enum State state = NO_NOTE;
	enum Accidental acc = NONE;
	char note;
	int num;
	char* index = new char[(*CHORD).length() + 1];
    strcpy(index, (*CHORD).c_str());
	while(*index != 0) {
		switch(state) {
			case NO_NOTE:
				if('A' <= *index && *index <= 'G') {
					note = *index;
					state = NOTE;
				}
				break;
			case NOTE:
				if(*index == '#') acc = SHARP;
				if(*index == 'b') acc = FLAT;
				if('0' <= *index && *index <= '9') {
					num = *index - '0';
					state = VALID;
				} 
				break;
			default:
				0; // Empty statement
		} // end switch
		if(state == VALID) {
			float offset = 27.5 * pow(2, num);
			int shift;
			float letter;
			switch(note) {
				case 'A':
					shift = 0;
					break;
				case 'B':
					shift = 2;
					break;
				case 'C':
					shift = 3;
					break;
				case 'D':
					shift = 5;
					break;
				case 'E':
					shift = 7;
					break;
				case 'F':
					shift = 8;
					break;
				case 'G':
					shift = 10;
					break;
				default:
					0;
			} // end switch
			if(acc == SHARP) shift++;
			if(acc == FLAT)  shift--;
			letter = offset * pow(2, shift/12.);

			notes[(int) *length] = letter;
			*length += 1;
			
			state = NO_NOTE;
			acc = NONE;
		} // end if
		index++;
	} // end while
}

float min(float a, float b) {
	if(b < 0) return 0;
	if(a < b) return a;
	else      return b;
} // end min

int main() {

    // 10 is arbitrary, use the open file to dictate the size of the arrays...
    float* durations = new float[10];
    std::string** chords = new std::string* [10];
    // TODO: read a file and parse into a paired set of arrays: durations[] and chords[]

	float beats = 0;
	int numChords = 0;
	for(int i = 0; durations[i] != 0; i++) {
		beats += durations[i];
		numChords++;
	} // end for

	float data;
	int dataint;
	long long index = 0;

    // Speed and volume settings
    float amplitude = 0.2;
	const float bpm = 180;
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

	for(int i = 0; i < numChords; i++) {
		parseNotes(chords[i], notes, &length);
		printf("%i", length);
		for(int j = 0; j < sampleRate*seconds*durations[i]/beats; j++) {
			data = 0;
			decres = min(1.0, 0.00012*(sampleRate*seconds-index-2500));
			for(int k = 0; k < length; k++) {
				data += decres * amplitude * sin(2*M_PI*index*notes[k]/sampleRate);
			} // end k for
			index++;
			// Make int
			dataint = data * pow(2, bitsPerSample);
			// Write left
			fwrite(&dataint, sizeof(data), 1, wav);
			// Write right
			if(numChannels == 2) fwrite(&dataint, sizeof(data), 1, wav);
		} // end j for
	} // end i for

	fclose(wav);

	return 0;

} // end main