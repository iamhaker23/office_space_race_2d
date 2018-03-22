#pragma once

struct InputStates{

	int	mouse_x;
	int mouse_y;
	bool LeftPressed;
	bool	keys[256];

	public:
		inline InputStates() {
			this->LeftPressed = false;
			this->mouse_x = 0;
			this->mouse_y = 0;
			
			for (int i = 0; i < 256; i++) {
				keys[i] = false;
			}

		}

};