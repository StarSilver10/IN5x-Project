/*********************************************************************************
*     File Name           :     MusicNote.hpp
*     Created By          :     tiboiser
*     Creation Date       :     [2020-12-23 12:29]
*     Last Modified       :     [2020-12-26 18:46]
*     Description         :      Class MusicNote header
**********************************************************************************/

namespace objdetect {

	/***********************************
	 * Class : MusicNote
	 **********************************/
	class MusicNote {
		/* ---- Attributes ---- */
		private:
			/* _sequencing, a number sequencing notes in a sheet. */
			int _sequencing;

			/* _duration, duration of the note.
			 * 1 is black, 0.5 is semi-black, 2 is white, etc.. */
			float _duration;

			/* _positionLine, position of the note (line number).
			 * It's the pitch of the note */
			float _positionLine;

		public:

		/* ---- Methods ---- */
		public:

			/* -- Constructors/Destructors -- */

			/* Constructor setting all attributes */
			MusicNote(int seq, float dur, float pos);

			/* Default destructor */
			~MusicNote(void);

			/* -- Getter -- */

			/* Getter of _sequencing */
			int sequencing(void);

			/* Getter of _duration */
			float duration(void);

			/* Getter of _positionLine */
			float positionLine(void);
	};
}
