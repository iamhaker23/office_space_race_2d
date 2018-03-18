#pragma once
#include <vector>
#include <string>
#include "kgeUtils.h"
#include <time.h>

#include <chrono>

#ifndef DEBUG_INFO_H

	#define DEBUG_INFO_H
	using std::vector;
	using std::string;
	using namespace std::chrono;

	class DebugInfo {
		private:
			int sampleSize;
			vector<int> frameSamples;
			vector<string> messages;
			double time;
			duration<double> cDuration;

		public:
			DebugInfo();
			void addFrameSample(int sample);
			void addMessage(string message);
			void reset();
			string getOutput();
			double getAverageFrameRate();
			void setTime(double time);
			void setDuration(duration<double> _cDuration);

	};

#endif
