/*********************************************************************************
 *     File Name           :     MidiConversion.cpp
 *     Created By          :     tiboiser
 *     Creation Date       :     [2020-12-22 09:20]
 *     Last Modified       :     [2020-12-26 20:23]
 *     Description         :     Convert into a midi file
 **********************************************************************************/

//#include <MidiFile.h>
//#include <Options.h>
#include "craigsapp-midifile-988d55b/include/MidiFile.h"
#include "craigsapp-midifile-988d55b/include/Options.h"
#include <iostream>
#include <math.h>

#include "MidiConversion.hpp"

using namespace std;
using namespace smf;
using namespace objdetect;

/* noteToKeyNumber convert a note in a line from 0 to 6 (1 below the 5-lines, 1 above the 5-lines)
 * And convert it to frenquencies then to equivalent keyNumber in Midi */
int MidiConversion::noteToKeyNumber(float note) {
	float arrayNoteToFrequency[NUMBER_NOTES][2] = {
		{0.00f, 261.626f}, // Do3
		{0.25f, 277.183f},
		{0.50f, 293.665f},
		{0.75f, 311.127f},
		{1.00f, 329.628f},
		{1.50f, 349.228f},
		{1.75f, 369.994f},
		{2.00f, 391.995f},
		{2.25f, 415.305f},
		{2.50f, 440.0f},
		{2.75f, 466.164f},
		{3.00f, 493.883f},
		{3.50f, 523.251f}, // Do4
		{3.75f, 554.365f},
		{4.00f, 587.33f},
		{4.25f, 622.254f},
		{4.50f, 659.255f},
		{5.00f, 698.456f},
		{5.25f, 739.989f}  // Fa4#
	};

	int i = 0;
	while (i < NUMBER_NOTES && arrayNoteToFrequency[i][0] != note) {
		i++;
	}

	if (i == NUMBER_NOTES) {
		cout << "Error : undefined note" << endl;
		return 0;
	}

	return round(12.f * log2(arrayNoteToFrequency[i][1]/440.f) + 69.f);
}

/* notesToMidi convert a vector of note representing a music sheet to a midi audio. */
int MidiConversion::notesToMidi(string outName, vector<MusicNote> mn, int tempo) {

	MidiFile midifile = MidiFile();
	midifile.addTrack();

	int previousTick = 0;
	for (vector<MusicNote>::iterator i = mn.begin(); i != mn.end(); i++) {
		MusicNote& note = *i;
		int pitch = noteToKeyNumber(note.positionLine());

		MidiMessage m2 = MidiMessage();
		MidiMessage m3 = MidiMessage();
		MidiMessage& m = m2;
		MidiMessage& m4 = m3;
		m.setCommand(0x90, pitch, 127); // Start of note
		m4.setCommand(0x80, pitch, 127); // End of note
		//midifile.addEvent(0, note.sequencing()*tempo, m);
		//midifile.addEvent(0, note.sequencing()*tempo + note.duration() * tempo, m4);

		//If note is not a silence
		if (!note.silence()) {
			midifile.addEvent(0, previousTick, m);
			midifile.addEvent(0, previousTick + note.duration() * tempo, m4);
		}
		
		previousTick = previousTick + note.duration() * tempo;
	}
	midifile.sortTracks();
	midifile.write(outName);

	return 1;
}
