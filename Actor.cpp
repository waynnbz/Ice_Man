#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp


void Iceman::doSomething()
{

	//check player's area for ice overlapping
	Ice*** ice = getWorld()->getIce();
	for (int i = 0; i <= 3; ++i) {
		for (int j = 0; j <= 3; ++j) {
			if (getX() + i <= 64 && getY() + j <= 59) { //MADE A WRONG ADDITION!!
				if (ice[getX() + i][getY() + j] != nullptr) {
					//if overlapped
					delete ice[getX() + i][getY() + j];
					ice[getX() + i][getY() + j] = nullptr;
					getWorld()->playSound(SOUND_DIG);
				}
			}
		}
	}

	//new comment

	int ch;
	//forward declaration; implementation has to be seperate from header file
	if (getWorld()->getKey(ch) == true) {

		switch (ch) {
		case KEY_PRESS_LEFT: //need to add obstacle check
			if (getDirection() != left)
				setDirection(left);
			else {
				if(getX()-1 >= 0)
				moveTo(getX() - 1, getY());
			}
			break;
		case KEY_PRESS_RIGHT:
			if (getDirection() != right)
				setDirection(right);
			else {
				if(getX()+1 <= 61)
				moveTo(getX() + 1, getY());
			}
			break;
		case KEY_PRESS_UP:
			if (getDirection() != up)
				setDirection(up);
			else {
				if(getY() + 1 <= 60)
				moveTo(getX(), getY() + 1);
			}
			break;
		case KEY_PRESS_DOWN:
			if (getDirection() != down)
				setDirection(down);
			else {
				if(getY()-1 >= 0)
				moveTo(getX(), getY() - 1);
			}
			break;
		case KEY_PRESS_SPACE:
			break;
		default:
			break;
		}
	}



}