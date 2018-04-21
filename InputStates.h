#pragma once

#include <chrono>
#include <time.h>

using namespace std::chrono;

struct InputStates{


	char character;
	bool characterIsNew;
	int	mouse_x;
	int mouse_y;
	bool LeftPressed;
	bool*	keys = new bool[256];
	system_clock::time_point* keysActivated = new system_clock::time_point[256];

	public:
		inline InputStates() {

			this->character = NULL;
			this->characterIsNew = false;

			this->LeftPressed = false;
			this->mouse_x = 0;
			this->mouse_y = 0;

			for (int i = 0; i < 256; i++) {
				keys[i] = false;
			}

			system_clock::time_point n = system_clock::now();

			for (int i = 0; i < 256; i++) {
				keysActivated[i] = n;
			}

		}

		inline ~InputStates() {
			delete[] keys;
			delete[] keysActivated;
		}

		inline std::chrono::milliseconds timeSinceKey(int vkey) {
			return std::chrono::duration_cast<std::chrono::milliseconds>(system_clock::now() - keysActivated[vkey]);
		}

};