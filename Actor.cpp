#include "Actor.h"
#include "StudentWorld.h"

//******Actor
bool Actor::moveToIfPossible(int x, int y) {

	if (getWorld()->canActorMoveTo(this, x, y)) {
		moveTo(x, y);
		return true;
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
	
	//travel if no encounter happened
	switch (getDirection()) {
	case up:
		if (moveToIfPossible(getX(), getY() + 1)) {
			--m_travelDistance;
			//setVisible(true);
		}
		else {
			setDead();
			return;
		}
		break;
	case down:
		if (moveToIfPossible(getX(), getY() - 1)) {
			--m_travelDistance;
			//setVisible(true);
		}
		else {
			setDead();
			return;
		}
		break;
	case left:
		if (moveToIfPossible(getX() - 1, getY())) {
			--m_travelDistance;
			//setVisible(true);
		}
		else {
			setDead();
			return;
		}
		break;
	case right:
		if (moveToIfPossible(getX() + 1, getY())) {
			--m_travelDistance;
			//setVisible(true);
		}
		else {
			setDead();
			return;
		}
		break;
	default:
		break;
	}
	setVisible(true);

	//annnoy protests and check travel distance
	if (getWorld()->annoyAllNearbyActors(this, 2, 3)
		|| m_travelDistance == 0) {
		setDead();
		return;
	}


	return;
}


//*****Activating Object

//********************Oil Barrel
void OilBarrel::doSomething() {
	if (!isAlive())	return;

	if (getWorld()->findNearbyIceMan(this, 4) != nullptr)
		setVisible(true);
	else
		return;

	Actor* t_iceMan = getWorld()->findNearbyIceMan(this, 3);
	if (t_iceMan != nullptr) {
		setDead();
		getWorld()->playSound(SOUND_FOUND_OIL);
		getWorld()->increaseScore(1000);
		getWorld()->decOil();
	}
	return;
}

//********************Gold Nugget
void GoldNugget::doSomething() 
{
	if (!isAlive()) return;

	if (!isPermanent())
		setTicksToLive();

	if (getWorld()->findNearbyIceMan(this, 4) != nullptr)
		setVisible(true);
	else
		return;

	Actor* picker;

	//pickable by Ice man
	if (isPickableByIceman()) {
		picker = getWorld()->findNearbyIceMan(this, 3);

		if (picker != nullptr) {
			setDead();
			getWorld()->playSound(SOUND_GOT_GOODIE);
			getWorld()->increaseScore(10);
			picker->addGold();
		}
	}
	//pickable by protester
	else {
		picker = getWorld()->findNearbyPicker(this, 3);

		if (picker != nullptr) {
			setDead();
			getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
			//activate bribed function in protestor
			getWorld()->increaseScore(25);
		}
	}
}

//********************Sonar Kit
void SonarKit::doSomething()
{
	if (!isAlive()) return;

	setTicksToLive();

	Actor* t_iceMan = getWorld()->findNearbyIceMan(this, 3);
	if (t_iceMan != nullptr) {
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		t_iceMan->addSonar();
		getWorld()->increaseScore(75);
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
				switch (getDirection()) {
				case up:
					getWorld()->addActor(new Squirt(getWorld(), 
						getX(), getY() + 4, getDirection()));
					break;
				case down:
					getWorld()->addActor(new Squirt(getWorld(),
						getX(), getY() - 4, getDirection()));
					break;
				case left:
					getWorld()->addActor(new Squirt(getWorld(),
						getX() - 4, getY(), getDirection()));
					break;
				case right:
					getWorld()->addActor(new Squirt(getWorld(),
						getX() + 4, getY(), getDirection()));
					break;
				}
				getWorld()->playSound(SOUND_PLAYER_SQUIRT);
				--m_water;
			}
			break;
		case KEY_PRESS_TAB:
			if (getGold() > 0) {
				getWorld()->addActor(new GoldNugget(getWorld(),
					getX(), getY(), false, false, true));
				decGold();
			}

			break;
		default:
			break;
		}
	}

	getWorld()->clearIce(getX(), getY());

}