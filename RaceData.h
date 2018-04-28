#pragma once

#include <string>

using std::string;

struct RaceData {
	private:
		int currentSegment;
		int laps;
		string name;
		float progressOnCurrentSegment;
		int position;
		bool dnf;

	public:
		inline RaceData() {
			this->laps = 0;
			this->currentSegment = 0;
			this->name = "UNKNOWN";
			this->progressOnCurrentSegment = 0.0f;
			this->position = -1;
			this->dnf = false;
		}

		inline ~RaceData() {
			this->name.clear();
		}

		inline RaceData(const RaceData &copy) {
			this->name = copy.name;
			this->laps = copy.laps;
			this->progressOnCurrentSegment = copy.progressOnCurrentSegment;
			this->position = copy.position;
			this->currentSegment = copy.currentSegment;
		}

		inline void setName(string name) {
			this->name = name;
		}

		inline string getName() {
			return this->name;
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

		inline bool hasCompletedSegments(int total) {
			return (this->currentSegment == total);
		}

		inline void setDNF(bool flag) {
			this->dnf = flag;
		}

		inline bool didNotFinish() {
			return this->dnf;
		}


};