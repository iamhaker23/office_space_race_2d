#include "DebugInfo.h"

DebugInfo::DebugInfo() {
	this->sampleSize = 2;
	this->reset();
}

double DebugInfo::getTime() {
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(this->cDuration);
	return (double)ms.count()/1000.0;
}

void DebugInfo::setDuration(duration<double> _cDuration) {
	this->cDuration = _cDuration;
}

void DebugInfo::addFrameSample(int sample) {
	
	if ((int)this->frameSamples.size() >= this->sampleSize) this->frameSamples.erase(this->frameSamples.begin());
	
	this->frameSamples.push_back(sample);
}

void DebugInfo::addMessage(string message) {

	this->messages.push_back(message);

}

void DebugInfo::reset() {
	this->frameSamples.clear();
	this->messages.clear();
}

string DebugInfo::getOutput() {
	string output = "";

	output.append(utils::floatLabel("Time:", (float)cDuration.count(), "s\n"));
	output.append(utils::floatLabel("", (float)getAverageFrameRate(), "fps\n"));

	for (string message : this->messages) {
		output.append(message);
		output.append("\n");
	}
	this->messages.clear();

	return output;
}

string DebugInfo::getMessages() {
	string output = "";

	for (string message : this->messages) {
		output.append(message);
		output.append("\n");
	}
	this->messages.clear();

	return output;
}

double DebugInfo::getAverageFrameRate() {
	double total = 0.0;
	int sampleCount = 0;

	for (int sample : this->frameSamples) {
		sampleCount++;
		total = total + (double)sample;
	}

	return (sampleCount > 0) ? (total / ((double)sampleCount)) : -1.0;
}


DebugInfo::~DebugInfo() {
	this->reset();
}

DebugInfo::DebugInfo(const DebugInfo &copy) {
	this->sampleSize = copy.sampleSize;
	
}