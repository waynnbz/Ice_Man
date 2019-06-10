#include "StudentWorld.h"
//#include "Actor.h"
#include <string>
#include <ctime>
#include <queue>
using namespace std;


GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}


//*******************Non-member helper

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


//*******************Member Helper***********

void StudentWorld::addActor(std::vector<Actor*> actors)
{
	std::srand(time(NULL));

	int currentLevel = getLevel();

	//add boulder
	for (int i = 0; i < std::min(currentLevel / 2 + 2, 9); ++i) {

		int x_rand = rand() % (VIEW_WIDTH - 4);
		int y_rand = rand() % (VIEW_HEIGHT - 8);

		while (y_rand < 20 || (x_rand > 26 && x_rand < 34)
			|| !wellDistributed(x_rand, y_rand, sw_actors)) {
			x_rand = rand() % (VIEW_WIDTH - 4);
			y_rand = rand() % (VIEW_HEIGHT - 8);
		}
		clearIce(x_rand, y_rand);
		Boulder* temp = new Boulder(this, x_rand, y_rand);
		sw_actors.push_back(temp);
	}



}


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


bool StudentWorld::canActorMoveTo(Actor* a, int x, int y) const
{
	if (x<0 || x > VIEW_WIDTH - 4 || y <0 || y > VIEW_HEIGHT - 4)
		return false;

	//check for ice
	if (!a->canDigThroughIce()) {
		for (int i = 0; i <= 3; ++i) {
			for (int j = 0; j <= 3; ++j) {
				//ice field boundary
				if (y + j < VIEW_HEIGHT - 4) {
					if (sw_ice[x + i][y + j] != nullptr) {
						//there is ice overlapping
						return false;
					}
				}
			}
		}
	}

	//check for obstacle actor cant passing through
	for (auto it : sw_actors) {
		if (it == a);
		else if (abs(x - it->getX()) <= 3 && abs(y - it->getY()) <= 3)
			if (!it->canActorPassThroughMe())	return false;
	}

	//checcan the actor move the destination within one move
	if ((x - 1 == a->getX() && y == a->getY())
		|| (x + 1 == a->getX() && y == a->getY())
		|| (x == a->getX() && y - 1 == a->getY())
		|| (x == a->getX() && y + 1 == a->getY()))
		return true;

	//can actor pass through??
		//is there ice?? can actor dig through??
		//maybe having ice clear here

	//recursion??
	return false;
}


//Not done
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



