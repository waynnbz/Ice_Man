#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

#include "Actor.h"

//class Actor;
//class Ice;
//class Iceman;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir){}
	virtual ~StudentWorld() {}
	
	//core 3 

	virtual int init()
	{

		//add Ice
		sw_ice = new Ice**[VIEW_WIDTH];
		for (int i = 0; i < VIEW_WIDTH; ++i) {

			sw_ice[i] = new Ice*[VIEW_HEIGHT - 4];
			for (int j = 0; j < VIEW_HEIGHT-4; ++j) {
				if (i >= 30 && i <= 33 && j >= 4 && j <= 59) {
					sw_ice[i][j] = nullptr;
					continue;
				}

				sw_ice[i][j] = new Ice(i, j);
			}
		}


		//add iceman
		sw_iceman = new Iceman(this);

		//add actors
		initActors();


		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move()
	{
		// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
		std::stringstream ss;
		ss << "Lvl: " << getLevel() 
			<< "  Lives: " << getLives()
			<< "  Hlth: " << std::setw(2) << sw_iceman->getHitPoints()
			<< "%  Wtr: " << std::setw(2) << sw_iceman->getWater()
			<< "  Gld: " << std::setw(2) << sw_iceman->getGold()
			<< "  Oil Left: " << std::setw(2) << getOil()
			<< "  Sonar: " << std::setw(2) << sw_iceman->getSonar()
			<< "  Scr: " << std::setw(6) << std::setfill('0') 
			<< getScore();
		std::string s = ss.str();

		setGameStatText(s);

		sw_iceman->doSomething();

		for (auto it = sw_actors.begin(); it != sw_actors.end(); ) {
			//release dead actors
			if (!(*it)->isAlive()) {
				delete *it;
				it = sw_actors.erase(it);
			}
			//all actors do something
			else {
				(*it)->doSomething();
				++it; 
				//important to leave increament out of for statement 
				//so to avoid repeatedly increase the iterator in case of deletion
			}
		}

		if (!sw_iceman->isAlive()) {
			decLives();
			return GWSTATUS_PLAYER_DIED;
		}
		
		return GWSTATUS_CONTINUE_GAME;


		
	}

	virtual void cleanUp()
	{
		//delete ice
		//wrong boundary not <= here
		for (int i = 0; i < VIEW_WIDTH; ++i) {
			for (int j = 0; j < VIEW_HEIGHT-4; ++j) {
				delete sw_ice[i][j];
				sw_ice[i][j] = nullptr;
			}
			delete[] *sw_ice[i];
		}
		delete[] **sw_ice; //messed up

		//delete iceman
		delete sw_iceman;

		//release actors
		for (auto it = sw_actors.begin(); it != sw_actors.end();) {
			delete *it;
			it = sw_actors.erase(it);
		}
	}

	//helpers
	void addActor(Actor* a);
	void clearIce(int x, int y);
	bool canActorMoveTo(Actor* a, int x, int y) const;
	int annoyAllNearbyActors(Actor* annoyer, int points, int radius);

	Actor* findNearbyIceMan(Actor* a, int radius) const;
	Actor* findNearbyPicker(Actor* a, int radius) const;


	//DIY helper
	void initActors();
	void setOil(int oil) {
		m_oil = oil;
	}
	unsigned int getOil() const {
		return m_oil;
	}
	void decOil() {
		--m_oil;
	}



	//DIY helpers
	//bool boulderSupport(int x, int y) {
	//	if (y == 0)
	//		return true;

	//	for (int i = 0; i <= 3; ++i) {
	//		if (sw_ice[x + i][y - 1] != nullptr) //missed minus one here
	//			return true;
	//	}
	//	return false;
	//}

	/*Ice*** getIce() {
		return sw_ice;
	}*/


private:
	//std::unique_ptr<Ice**> sw_ice;
	unsigned int m_oil;
	Ice*** sw_ice;
	Iceman* sw_iceman;
	std::vector<Actor*> sw_actors;
};

#endif // STUDENTWORLD_H_
