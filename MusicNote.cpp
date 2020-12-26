/*********************************************************************************
*     File Name           :     MusicNote.cpp
*     Created By          :     tiboiser
*     Creation Date       :     [2020-12-23 12:28]
*     Last Modified       :     [2020-12-26 18:46]
*     Description         :      Class MusicNote
**********************************************************************************/

#include "MusicNote.hpp"

using namespace objdetect;

/* -- Constructors/Destructors -- */

/* Constructor setting all attributes */
MusicNote::MusicNote(int seq, float dur, float pos) {
	_sequencing = seq;
	_duration = dur;
	_positionLine = pos;
}

/* Default destructor */
MusicNote::~MusicNote(void) {
}


/* -- Getters -- */

/* Getter of _sequencing */
int MusicNote::sequencing(void) {
	return _sequencing;
}

/* Getter of _duration */
float MusicNote::duration(void) {
	return _duration;
}

/* Getter of _positionLine */
float MusicNote::positionLine(void) {
	return _positionLine;
}
