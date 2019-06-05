#include "StudentWorld.h"
//#include "Actor.h"
#include <string>
#include <ctime>
using namespace std;

////////Non-member helper


bool wellDistributed(int x, int y, const vector<Actor*> &va) {
	
	for (auto a : va) {
		if (sqrt(pow(x - a->getX(), 2) + pow(y - a->getY(), 2)) < 6.0) {
			return false;
		}
	}

	return true;
}


double getRadius(Actor* a, Actor* b) {
	double radius = sqrt(pow(a->getX() - b->getX(), 2) + pow(a->getY() - b->getY(), 2));
	return radius;
}


///////////////////////

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp


void StudentWorld::clearIce(int x, int y) 
{
	for (int i = 0; i <= 3; ++i) {
		for (int j = 0; j <= 3; ++j) {
			//x + i < VIEW_WIDTH &&
			if (y + j < VIEW_HEIGHT - 4) { //MADE A WRONG ADDITION!!
				if (sw_ice[x + i][y + j] != nullptr) {
					//if overlapped
					delete sw_ice[x + i][y + j];
					sw_ice[x + i][y + j] = nullptr;
					playSound(SOUND_DIG);
				}
			}
		}
	}
}


int StudentWorld::annoyAllNearbyActors(Actor* annoyer, int points, int radius) 
{
	int count = 0;
	if (annoyer != sw_iceman) {
		if (getRadius(annoyer, sw_iceman) <= radius) {
			sw_iceman->annoy(points);
			++count;
		}

	}

	/*for (auto a : sw_actors) {

	}*/


	return count;
}


void StudentWorld::addActor(std::vector<Actor*> actors) 
{
	std::srand(time(NULL));

	int currentLevel = getLevel();

	//add boulder
	for (int i = 0; i < std::min(currentLevel / 2 + 30, 90); ++i) {

		int x_rand = rand() % (VIEW_WIDTH-4);
		int y_rand = rand() % (VIEW_HEIGHT-8);
		
		while (y_rand < 20 || (x_rand > 26 && x_rand < 34)
			|| !wellDistributed(x_rand, y_rand, sw_actors)) {
			x_rand = rand() % (VIEW_WIDTH-4);
			y_rand = rand() % (VIEW_HEIGHT - 8);
		}
		clearIce(x_rand, y_rand);
		Boulder* temp = new Boulder(this, x_rand, y_rand);
		sw_actors.push_back(temp);
	}



}
