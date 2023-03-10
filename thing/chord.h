#ifndef CHORD_H
#define CHORD_H

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

#include <math.h>

enum Shape {sineShape, squareShape, triangleShape, sawtoothShape, errorShape}; 

// Attack, Decay, Sustain, Release
class ADSR {
private:
    double attack;
    double decay;
    double sustain;
    double release;
public:
    ADSR();
    ADSR(double a, double d, double s, double r);

    double getShift(int startFrame, int endFrame, int frame);
};

int noteValue(char c, int shift, int octave);
double pitchValue(char c, int shift, int octave);

typedef struct templateNoteStruct {
    double pitch;
    Shape shape;
    double startSec;
    double duration;
} TemplateNote;

class Note {
private:
    double pitch;
    Shape shape;
    int startFrame;
    int endFrame;
    bool done;
    double lastAmp;
    ADSR adsr;
public:
    Note(double pitch, Shape shape, double startSec, double duration);

    bool isDone();
    bool isNotStart(int frame);

    void testDone(int frame);

    int getAmplitude(float amp, int frame);
};
#endif
