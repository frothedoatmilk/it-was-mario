#include <math.h>
#include <string>
#include <vector>
// #include <iostream>

typedef struct structnote {
    int octave;
    int shift;
    std::string name;
    double pitch;
} Note;

// Double later maybe
int noteValue(char c, int shift, int octave) {
    int value = 12 * octave;
    switch(c) {
        case 'A':
            value += 0;
        case 'B':
            value += 2;
        case 'C':
            value += 4 - 12;
        case 'D':
            value += 5 - 12;
        case 'E':
            value += 7 - 12;
        case 'F':
            value += 8 - 12;
        case 'G':
            value += 10 - 12;
        default:
            value = value; // Just cause
    } // end switch
    if(shift >= 100) value++;
    if(shift <= -100) value--;

    return value;
} // end noteValue

class Chord {
private:
    std::vector<Note> notes;
    int startTime;
    bool done;
    short lastAmp;
    double duration;
    double bpm;
public:
    Chord(std::string str, double startBeat, double dur, double bpmm) {
        lastAmp = 0;
        duration = dur;
        startTime = round(44100 * startBeat * dur / bpmm);
        bpm = bpmm;
        done = false;
        
        int n = str.length();
        int oct = 0;
        int shif = 0;
        std::string nam = "";
        double pit = 0.0;
        for(int i = 0; i < n; i++) {
            if('A' <= str[i] && str[i] <= 'G' && i != 0) {
                Note note;
                note.octave = oct;
                note.shift = shif;
                note.name = nam;
                note.pitch = 27.5 * pow(2.0,noteValue(nam[0], shif, oct) / 12.0);
                notes.push_back(note);
                oct = 0;
                shif = 0;
                nam = "";
                nam.push_back(str[i]);
                pit = 0.0;
            } // end if
            if('0' <= str[i] && str[i] <= '9') {
                oct = 10 * oct + (str[i] - '0');
            }
            if(str[i] == 'b') shif -= 100;
            if(str[i] == '#') shif += 100;
        } // end for
    } // end chord

    bool getDone() {return done;}

    int getAmplitude(float amp, int time) {
        if (time < startTime) return 0;
        isDone();
        if (done) return 0;

        if(amp < 0 || amp > 1) return -1;
        double ret = 0;
        for(auto & elem : notes) {
            double p = sin((M_PI / 44100.0) * elem.pitch * (time - startTime));
            ret += amp * p;
        } // end for
        lastAmp = (short) round(ret * 32767);
        return lastAmp; // 32767 is 2^15-1, i.e. max signed 16-bit int
    } // end getAmplitude

    void isDone() {
        if ((-328 <= lastAmp && lastAmp <= 328)) done = true;
    } // end isDone
};
