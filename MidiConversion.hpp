/*********************************************************************************
*     File Name           :     MidiConversion.hpp
*     Created By          :     tiboiser
*     Creation Date       :     [2020-12-23 08:47]
*     Last Modified       :     [2020-12-26 18:48]
*     Description         :      MidiConversion class header
**********************************************************************************/

#include <MidiFile.h>
#include <Options.h>
#include <iostream>

#include "MusicNote.hpp"

#define NUMBER_NOTES 19

namespace objdetect {

	/***********************************
	 * Class : MidiConversion
	 **********************************/
	class MidiConversion {
		/* ---- Attributes ---- */
		private:

		public:

		/* ---- Methods ---- */
		public:
			/* noteToKeyNumber convert a note in a line from 0 to 6 (1 below the 5-lines, 1 above the 5-lines)
			 * And convert it to frenquencies then to equivalent keyNumber in Midi */
			static int noteToKeyNumber(float note);

			/* notesToMidi convert a vector of note representing a music sheet to a midi audio. */
			static int notesToMidi(std::string outName, std::vector<MusicNote> mn, int tempo);
	};
}
