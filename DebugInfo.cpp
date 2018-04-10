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

	output.append("Current Time:\n");
	output.append(utils::doubleToString(cDuration.count()));
	output.append("\nAvg. Framerate:\n");
	output.append(utils::doubleToString(getAverageFrameRate()));
	output.append("\nMessages:\n");

	for (string message : this->messages) {
		output.append(message);
		output.append("\n");
	}
	this->messages.clear();

	return output;
}

double DebugInfo::getAverageFrameRate() {
	double total = 0.0;

	for (int sample : this->frameSamples) {
		total = total + (double)sample;
	}

	return (this->frameSamples.size() > 0) ? (total / ((double)this->frameSamples.size())) : -1.0;
}