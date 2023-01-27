#include "file_parse.h"

enum ReadMode {chordMode, templMode, voiceMode, paramMode, noMode};

Shape stoshape(std::string str) {
    if(str == "SINE") return sineShape;
    if(str == "SQUARE") return squareShape;
    if(str == "TRIANGLE") return triangleShape;
    if(str == "SAWTOOTH") return sawtoothShape;
    return errorShape;
} // end stoshape

TemplateNote tNote(double pitch, Shape shape, double startSec, double duration) {
    TemplateNote t;
    t.pitch = pitch;
    t.shape = shape;
    t.startSec = startSec;
    t.duration = duration;
    return t;
} // end tNode

Note makeNoteFromTemplate(TemplateNote tn, float startSec) {
    return Note(tn.pitch, tn.shape, tn.startSec + startSec, tn.duration);
} // end makeNoteFromTemplate

float parseInputFile1 (std::string filename, std::vector<Note> & notes);

float parseInputFile (std::string filename, std::vector<Note> & notes){
    if(filename.back() == '1') return parseInputFile1(filename, notes);
    return -1; // error
} // end parseFile

////////////////////////////////////////////////////////////////////////

float parseInputFile1 (std::string filename, std::vector<Note> & notes){
    float seconds = 0.0;

    notes = {};
    std::vector<std::vector<TemplateNote>> templates = {};
    std::vector<std::string> templateNames = {};
    std::vector<float> templateLengths = {};
    std::vector<TemplateNote> currTemplate = {};
    std::string currTemplateName = "";
    float currTemplateLength = 0.0;

    std::string line;
    std::ifstream musicfile(filename);
    int bpm = 120;
    int protocol = 1;
    if (musicfile.is_open()) {
        ReadMode mode = noMode;
        std::vector<Shape> shapes;
        while(getline(musicfile, line)) {
            switch(mode) {
                case templMode:
                case chordMode: {
                    if(line == "</chord>") {mode = noMode; break;}
                    if(line == "</templ>") {mode = noMode; break;}
                    if(mode == templMode) {
                        if(line[0] == '>') {
                            currTemplateName = "";
                            for (auto & c : line) if (c != '>') currTemplateName.push_back(c);
                            currTemplate = {};
                            currTemplateLength = 0.0;
                            continue;
                            } // end if
                        if(line[0] == '<') {
                            templateNames.push_back(currTemplateName);
                            templates.push_back(currTemplate);
                            templateLengths.push_back(currTemplateLength);
                            continue;
                        } // end if
                    } // end if
                    if(mode == chordMode) {
                        if(line[0] == '>') {
                            currTemplateName = "";
                            for (auto & c : line) if (c != '>') currTemplateName.push_back(c);
                            std::vector<std::string>::iterator itr = std::find(templateNames.begin(), templateNames.end(), currTemplateName);
                            int loc = std::distance(templateNames.begin(), itr);
                            std::vector<TemplateNote> temp = templates[loc];
                            for(auto & tn : temp) {
                                notes.push_back(makeNoteFromTemplate(tn, seconds));
                            } // end for
                            seconds += templateLengths[loc];
                            continue;
                        } // end if
                    } // end if
                    int voiceIndex = 0;
                    float dur = 0.0;
                    float notedur = 0.0;
                    float lenshift = 1.0;
                    std::string name = "";
                    int octave = 0;
                    int shift = 0;
                    double pitch = 0.0;
                    bool doNotes = false;
                    for (auto& c : line) {
                        if(c == ' ') doNotes = true;
                        if(!doNotes) {
                            if(c == 'w') dur = 4.0;
                            if(c == 'h') dur = 2.0;
                            if(c == 'q') dur = 1.0;
                            if(c == 'e') dur = 1.0/2;
                            if(c == 't') dur = 1.0/3;
                            if(c == 'T') dur = 2.0/3;
                            if(c == 's') dur = 1.0/4;
                            if(c == 'x') dur = 1.0/6;
                            if(c == '.') dur *= 1.5;
                            if(c == '-') break; // These are for comments
                        } // end if
                        else{
                            if(c == ' ') {
                                if(name != "") {
                                    pitch = pitchValue(name[0], shift, octave);
                                    if (mode == chordMode) {
                                        if(notedur != 0.0) notes.push_back(Note(pitch, shapes[voiceIndex], seconds, lenshift * notedur * 60 / bpm));
                                        else               notes.push_back(Note(pitch, shapes[voiceIndex], seconds, lenshift * dur * 60 / bpm));
                                    } // end if
                                    if (mode == templMode) {
                                        if(notedur != 0.0) currTemplate.push_back(tNote(pitch, shapes[voiceIndex], currTemplateLength, lenshift * notedur * 60 / bpm));
                                        else               currTemplate.push_back(tNote(pitch, shapes[voiceIndex], currTemplateLength, lenshift * dur * 60 / bpm));
                                    } // end if
                                    voiceIndex++;

                                    notedur = 0.0;
                                    lenshift = 1.0;
                                    name = "";
                                    octave = 0;
                                    shift = 0;
                                    pitch = 0.0;
                                } // end if
                            } // end if
                            if('A' <= c && c <= 'G') name.push_back(c);
                            if(c == 'b') shift -= 100;
                            if(c == '#') shift += 100;
                            if('0' <= c && c <= '9') octave = 10 * octave + (c - '0');
                            if(c == 'w') notedur = 4.0;
                            if(c == 'h') notedur = 2.0;
                            if(c == 'q') notedur = 1.0;
                            if(c == 'T') notedur = 2.0/3;
                            if(c == 'e') notedur = 1.0/2;
                            if(c == 't') notedur = 1.0/3;
                            if(c == 's') notedur = 1.0/4;
                            if(c == 'x') notedur = 1.0/6;
                            if(c == '.') notedur *= 1.5;
                            if(c == '!') lenshift *= 0.3;
                            if(c == ':') lenshift *= 0.5;
                            if(c == ';') lenshift *= 0.8;
                            if(c == ',') lenshift *= 0.9;
                            if(c == '-') voiceIndex++;
                        }
                    } // end for
                    if(name != "") {
                        pitch = pitchValue(name[0], shift, octave);
                        if (mode == chordMode) {
                            if(notedur != 0.0) notes.push_back(Note(pitch, shapes[voiceIndex], seconds, lenshift * notedur * 60 / bpm));
                            else               notes.push_back(Note(pitch, shapes[voiceIndex], seconds, lenshift * dur * 60 / bpm));
                        } // end if
                        if (mode == templMode) {
                            if(notedur != 0.0) currTemplate.push_back(tNote(pitch, shapes[voiceIndex], currTemplateLength, lenshift * notedur * 60 / bpm));
                            else               currTemplate.push_back(tNote(pitch, shapes[voiceIndex], currTemplateLength, lenshift * dur * 60 / bpm));
                        } // end if
                        voiceIndex++;

                        notedur = 0.0;
                        lenshift = 1.0;
                        name = "";
                        octave = 0;
                        shift = 0;
                        pitch = 0.0;
                    } // end if
                    if(mode == chordMode) seconds += dur * 60 / bpm;
                    if(mode == templMode) currTemplateLength += dur * 60 / bpm;
                    break;
                }
                case voiceMode: {
                    if(line == "</voice>") {mode = noMode; break;}
                    shapes = {};
                    std::string voice = "";
                    for (auto& c : line) {
                        if(c == ' ') {
                            shapes.push_back(stoshape(voice));
                            voice = "";
                        } // end if
                        if('A' <= c && c <= 'Z') voice.push_back(c);
                    } // end for
                    if(voice != "") shapes.push_back(stoshape(voice));
                    break;
                }
                case paramMode: {
                    if(line == "</param>") {mode = noMode; break;}
                    std::string param = "";
                    std::string value = "";
                    bool findParam = true;
                    for (auto& c : line) {
                        if(c == ' ') {findParam = false; continue;}
                        if(findParam) {param.push_back(c); continue;}
                        if(('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || ('0' <= c && c <= '9')) value.push_back(c);
                    } // end for
                    if(param == "bpm") bpm = stoi(value);
                    break;
                }
                case noMode: {
                    if(line == "<chord>") mode = chordMode;
                    if(line == "<templ>") mode = templMode;
                    if(line == "<voice>") mode = voiceMode;
                    if(line == "<param>") mode = paramMode;
                    break;
                }
                default:
                    break;
             } // end switch
        } // end while
    } // end if
    else std::cout << "File not found" << std::endl;

    return seconds;
}
