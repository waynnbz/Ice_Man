#include "Actor.h"
#include "StudentWorld.h"

//******Actor
bool Actor::moveToIfPossible(int x, int y) {
	//boundary check only
	if (x >= 0 && x <= VIEW_WIDTH - 4 && y >= 0 && y <= VIEW_HEIGHT - 4) {
		if (getWorld()->canActorMoveTo(this, x, y)) {
			moveTo(x, y);
			return true;
		}
	}
	return false;
}

//*****Obstacles
//********************Boulder
void Boulder::doSomething()
{
	if (isAlive()) {
		if (m_state == stable) {
			if (getWorld()->canActorMoveTo(this, getX(), getY()-1)) {
				m_state = waiting;
				waitTime = 0;
			}
			
		}
		else if (m_state == waiting) {
			if (waitTime == 30) 
				m_state = falling;
			else
			++waitTime;
		}
		else if (m_state == falling) {

			if (moveToIfPossible(getX(), getY() - 1)) {
				getWorld()->playSound(SOUND_FALLING_ROCK);
				getWorld()->annoyAllNearbyActors(this, 100, 3);
			}
			else{
				m_state = dead;
				setDead();
				setVisible(false);
			}
		}
	}
}

//********************Squirt
void Squirt::doSomething()
{
	//annnoy protests and check travel distance
	if (getWorld()->annoyAllNearbyActors(this, 2, 3) 
		|| m_travelDistance == 0) {
		setDead();
		return;
	}

	//travel if no encounter happened
	switch (getDirection()) {
	case up:
		if (moveToIfPossible(getX(), getY() + 1))
			--m_travelDistance;
		else
			setDead();
	case down:
		if (moveToIfPossible(getX(), getY() - 1))
			--m_travelDistance;
		else
			setDead();
	case left:
		if (moveToIfPossible(getX() - 1, getY()))
			--m_travelDistance;
		else
			setDead();
	case right:
		if (moveToIfPossible(getX() + 1, getY()))
			--m_travelDistance;
		else
			setDead();
	default:
		throw 0;
	}

	return;
}


//*****Agents
//********************Ice Man
void Iceman::doSomething()
{
	int ch;
	//implementation of a forward declaration has to be seperate from header file
	if (getWorld()->getKey(ch) == true) {
		switch (ch) {
		case KEY_PRESS_LEFT:
			if (getDirection() != left)
				setDirection(left);
			else {
				moveToIfPossible(getX() - 1, getY());
			}
			break;
		case KEY_PRESS_RIGHT:
			if (getDirection() != right)
				setDirection(right);
			else {
				moveToIfPossible(getX() + 1, getY());
			}
			break;
		case KEY_PRESS_UP:
			if (getDirection() != up)
				setDirection(up);
			else {
				moveToIfPossible(getX(), getY() + 1);
			}
			break;
		case KEY_PRESS_DOWN:
			if (getDirection() != down)
				setDirection(down);
			else {
				moveToIfPossible(getX(), getY() - 1);
			}
			break;
		case KEY_PRESS_SPACE:
			if (m_water > 0) {

			}
			break;
		default:
			break;
		}
	}

	getWorld()->clearIce(getX(), getY());

}