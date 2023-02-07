#include "chord.h"

// Double later maybe
int noteValue(char c, int shift, int octave) {
    int value = 12 * octave;
    switch(c) {
        case 'A':
            value += 0;
            break;
        case 'B':
            value += 2;
            break;
        case 'C':
            value += 3;
            break;
        case 'D':
            value += 5;
            break;
        case 'E':
            value += 7;
            break;
        case 'F':
            value += 8;
            break;
        case 'G':
            value += 10;
            break;
        default:
            value = value; // Just cause
    } // end switch
    if(shift >= 100) value++;
    if(shift <= -100) value--;

    return value;
} // end noteValue

double pitchValue(char c, int shift, int octave) {
    return 27.5 * pow(2.0,noteValue(c, shift, octave) / 12.0);
} // end pitchValue

Note::Note(double pitc, Shape shap, double startSec, double duration) {
    pitch = pitc;
    shape = shap;
    startFrame = round(44100 * startSec);
    endFrame = round(44100 * (startSec + duration));
    done = false;
} // end Chord

bool Note::isDone() {return done;}
bool Note::isNotStart(int frame) {return startFrame > frame;}

double tent(double x) {
    if(x <= 0.25) return 4*x;
    if(x <= 0.75) return 2-4*x;
    else          return 4*x-4;
}

int Note::getAmplitude(float amp, int frame) {
    testDone(frame);
    if (done) {
	return 0;
    }
    if (frame < startFrame) {
	return 0;
    }
    if(amp < 0 || amp > 1) return -1;

    double val = pitch * (frame - startFrame) / 44100; 
    if     (shape == sineShape)     val = sin(2 * M_PI * val);
    else if(shape == squareShape)   val = round(val - floor(val));
    else if(shape == sawtoothShape) val = 0.5 * (val - floor(val));
    else if(shape == triangleShape) val = tent(val - floor(val));
    else                            val = 0;
    lastAmp = (short) round(val * 32767);
    return (short) round(amp * val * 32767); // 32767 is 2^15-1, i.e. max signed 16-bit int
} // end getAmplitude

void Note::testDone(int frame) {
    if (frame < endFrame) return;
    //done = true;
    if ((-328 <= lastAmp && lastAmp <= 328)) done = true;
} // end isDone
