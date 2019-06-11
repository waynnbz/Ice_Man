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
void StudentWorld::addActor(Actor* a) {
	sw_actors.push_back(a);
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


//not done
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
			if (!it->canActorsPassThroughMe())	return false;
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

	//will annoyer ever be iceman??
	if (annoyer->huntsIceMan()) {
		if (getRadius(annoyer, sw_iceman) <= radius) {
			sw_iceman->annoy(points);
			++count;
		}
	}

	//annoy all other actors
	for (auto it = sw_actors.begin(); it != sw_actors.end(); ++it) {
		if ((*it)->isAnnoyable()) {
			if (*it == annoyer);
			else if (getRadius(annoyer, *it) <= radius) {
				(*it)->annoy(points);
				++count;
			}
		}
	}

	


	/*for (auto a : sw_actors) {

	}*/


	return count;
}



Actor* StudentWorld::findNearbyIceMan(Actor* a, int radius) const {
	if (getRadius(sw_iceman, a) <= radius)
		return sw_iceman;
	else
		return nullptr;
}


Actor* StudentWorld::findNearbyPicker(Actor* a, int radius) const {
	for (auto it = sw_actors.begin(); it != sw_actors.end(); ++it) {
		if ((*it)->canPickThingsUp()) {
			if (getRadius(*it, a) <= radius)
				return *it;
		}
	}
	return nullptr;
}



//**************DIY helper
void StudentWorld::initActors()
{
	std::srand(time(NULL));

	int currentLevel = getLevel();

	//distribute boulder
	for (int i = 0; i < std::min(currentLevel / 2 + 2, 9); ++i) 
	{
		int x_rand = rand() % (VIEW_WIDTH - 4);
		int y_rand = rand() % (VIEW_HEIGHT - 8);

		while (y_rand < 20 || (x_rand > 26 && x_rand < 34)
			|| !wellDistributed(x_rand, y_rand, sw_actors)) {
			x_rand = rand() % (VIEW_WIDTH - 4);
			y_rand = rand() % (VIEW_HEIGHT - 8);
		}
		clearIce(x_rand, y_rand);
		//Boulder* temp = new Boulder(this, x_rand, y_rand);
		//using rvalue move instead
		sw_actors.push_back(new Boulder(this, x_rand, y_rand));
	}

	//distribute Oil Barrel
	setOil(std::min(2 + currentLevel, 21));
	for (unsigned int i = 0; i < getOil(); ++i)
	{
		int x_rand = rand() % (VIEW_WIDTH - 4);
		int y_rand = rand() % (VIEW_HEIGHT - 8);

		while ((y_rand > 4 && x_rand > 26 && x_rand < 34)
			|| !wellDistributed(x_rand, y_rand, sw_actors)) {
			x_rand = rand() % (VIEW_WIDTH - 4);
			y_rand = rand() % (VIEW_HEIGHT - 8);
		}

		sw_actors.push_back(new OilBarrel(this, x_rand, y_rand));

	}

	//distribute Gold Nugget
	for (int i = 0; i < max(5 - currentLevel / 2, 21); ++i) {

		int x_rand = rand() % (VIEW_WIDTH - 4);
		int y_rand = rand() % (VIEW_HEIGHT - 8);

		while ((y_rand > 4 && x_rand > 26 && x_rand < 34)
			|| !wellDistributed(x_rand, y_rand, sw_actors)) {
			x_rand = rand() % (VIEW_WIDTH - 4);
			y_rand = rand() % (VIEW_HEIGHT - 8);
		}

		sw_actors.push_back(new GoldNugget(this, 
			x_rand, y_rand, true, true, true));

	}


	//temp add sonar
	sw_actors.push_back(new SonarKit(this, 0, 60, currentLevel));
	//temp add water
	sw_actors.push_back(new WaterPool(this, 8, 60, currentLevel));
	sw_actors.push_back(new WaterPool(this, 16, 60, currentLevel));

}

