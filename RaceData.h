#pragma once

#include <string>

using std::string;

struct RaceData {
	private:
		int segmentsCompleted;
		int currentSegment;
		int laps;
		
		float progressOnCurrentSegment;
		int position;


	public:
		inline RaceData() {
			this->laps = 0;
			this->currentSegment = 0;
			this->segmentsCompleted = 0;

			this->progressOnCurrentSegment = 0.0f;
			this->position = -1;
		}


		inline void setPosition(int position) {
			this->position = position;
		}

		inline int getPosition() {
			return this->position;
		}

		inline void setProgressOnCurrentSegment (float progressOnCurrentSegment) {
			this->progressOnCurrentSegment = progressOnCurrentSegment;
		}

		inline float getProgressOnCurrentSegment() {
			return this->progressOnCurrentSegment;
		}



		inline void incrementLaps() {
			this->laps += 1;
		}

		inline void setCurrentSegment(int segment) {
			this->currentSegment = segment;
		}

		inline int getCurrentSegment() {
			return this->currentSegment;
		}
		inline int getLaps() {
			return this->laps;
		}
		inline int getSegmentsCompleted() {
			return this->segmentsCompleted;
		}

		inline void setSegmentsCompleted(int completed) {
			this->segmentsCompleted = completed;
		}

		inline void incrementSegmentsCompleted() {
			this->segmentsCompleted += 1;;
		}

		inline bool hasCompletedSegments(int total) {
			return ((this->segmentsCompleted) == total);
		}


};