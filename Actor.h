#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"
#include "Board.h"
using namespace std;
// for living things
const bool WALKING = true;
const bool PAUSED_OR_WAITING_TO_ROLL = false;
// for squares
const bool ACTIVE = true;
const bool INACTIVE = false;
// for coinsquares
const bool BLUE = true;
const bool RED = false;
// for actors
const bool YES_VORTEX = true;
const bool NO_VORTEX = false;
// to see if you are player one or player 2
const int PEACH = 1;
const int YOSHI = 2;
// to see if you are a new player or not
const bool NEW = true;
const bool OLD = false;
 
// if Event Square should activate on player
const bool YEAH = true;
const bool NOH = false;
//class StudentWorld
//{};
class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(int imageID,int inx, int iny, StudentWorld* world, int indir, int depth, int size, bool state);
    virtual void doSomething() = 0;
    bool getstate() const {return m_state;};
    void setstate(bool newstate) {m_state = newstate;};
    StudentWorld* getWorld() const {return m_world;};
    virtual bool isValidSquareInDir(int dir, Actor* p);
    virtual void teleportToRandomLocation();
    virtual ~Actor();
    virtual bool canBeRemoved() {return true;};  // everything except players and Enemies can be removed
    virtual bool isImpactable() {return false;}; // only Bowser and Boo are impactable by Vortex's
    virtual void Impact() {}; // defines how an object reacts to being impacted
private:
    bool m_state;
    StudentWorld* m_world;
};

// Living
class Living : public Actor
{
public:
    Living(int imageID, int inx, int iny,StudentWorld* world, int indir = right, int depth = 0, int size = 1);
    virtual void doSomething() = 0;
    int getWalkingDir() const{return m_walkingdir;};
    void setWalkingDir(int walkingdir) {m_walkingdir = walkingdir;};
    virtual ~Living() {cerr << "Living" <<endl;};
private:
    int m_walkingdir;
};

// Avatar
class Avatar : public Living
{
public:
    Avatar(int imageID, int inx, int iny,int player, StudentWorld* world, int indir = right, int depth = 0, int size = 1);
    virtual void doSomething();
    virtual int getCoins() const {return m_coins;};
    virtual int getStars() const {return m_stars;};
    virtual void addCoins(int coins) {m_coins += coins;};
    virtual void addStars(int stars) {m_stars += stars;};
    virtual void setCoins(int newcoins) {m_coins = newcoins;};
    virtual void setStars(int newstars) {m_stars = newstars;};
    virtual int getDieRoll() const {return m_die_roll;};
    virtual bool getVortex() const {return m_vortex;};
    virtual void setVortex(bool vortex) {m_vortex = vortex;};
    virtual bool getNew() const {return m_new;};
    virtual void setNew(bool b) {m_new = b;};
//    virtual bool isValidSquareInDir(int dir, Avatar* p); MOVED UP TO ACTOR
//    virtual void teleportToRandomLocation();
    virtual void swapPos(Avatar* p); // swap position with other player
    virtual int getTicksToMove() {return m_ticks_to_move;};
    virtual void setTicksToMove(int tickstomove) {m_ticks_to_move = tickstomove;};
    virtual bool getActivate() {return m_activate;};
    virtual void setActivate(bool yn) {m_activate = yn;};
    virtual bool canBeRemoved() {return false;};
    virtual ~Avatar() {cerr << "Avatar" << endl;};
private:
    int m_ticks_to_move;
    int m_player;
    int m_die_roll;
    int m_coins;
    int m_stars;
    bool m_new;  // default starts as new
    bool m_vortex;
    int m_temp;
    bool m_activate;
};

// ActivatingObject
class ActivatingObject : public Actor // activates on something
{
public:
    ActivatingObject(int imageID, int inx, int iny, StudentWorld *world, int indir, int depth, int size, bool state);
    virtual ~ActivatingObject() {cerr << "ActivatingObject" << endl;};
};

// ActivateOnAvatar
class ActivateOnAvatar : public ActivatingObject // the only thing not one of these is a vortex because it acts on enemies (and avatar itself)
{
public:
    ActivateOnAvatar(int imageID, int inx, int iny, StudentWorld *world, int indir, int depth, int size, bool state);
    virtual void doSomething();
    virtual void doActivity() = 0;
    virtual bool justLandedOrMovedOn(Avatar* p) const;
    virtual bool Landed(Avatar* p) const;
    virtual bool moveButNotLanded(Avatar* p) const;
    virtual ~ActivateOnAvatar() {cerr << "ActivateOnAvatar" << endl;};
    
};

// Square
class Square : public ActivateOnAvatar
{
public:
    Square(int imageID,int inx, int iny,StudentWorld* world, int indir, int depth, int size);
    virtual void doActivity() = 0;
    virtual ~Square() {cerr << "Square" << endl;};
private:
};

// CoinSquare
class CoinSquare : public Square
{
public:
    CoinSquare(int imageID, int inx, int iny,StudentWorld* world,int coinschange, int indir = right, int depth = 1, int size = 1);
    virtual void doActivity();
    virtual ~CoinSquare() {cerr << "CoinSquare" << endl;};
private:
    int m_coins_change;
};

class StarSquare : public Square
{
public:
    StarSquare(int imageID, int inx, int iny,StudentWorld* world, int indir = right, int depth = 1, int size = 1);
    virtual void doActivity();
    virtual ~StarSquare() {cerr << "StarSquare";};
private:
    int m_coins_change;
    int m_stars_change;
    
};

class DirectionalSquare : public Square
{
public:
    DirectionalSquare(int imageID, int inx, int iny,StudentWorld* world, int indir, int depth = 1, int size = 1);
    virtual void doActivity();
    virtual ~DirectionalSquare() {cerr << "DirectionalSquare" << endl;};
private:
    int m_forcing_dir;
};

class BankSquare : public Square
{
public:
    BankSquare(int imageID, int inx, int iny,StudentWorld* world, int indir = right, int depth = 1, int size = 1);
    virtual void doActivity();
    virtual ~BankSquare() {cerr << "BankSquare" << endl;};
private:
    int m_coins_change;
};

class EventSquare : public Square
{
public:
    EventSquare(int imageID, int inx, int iny,StudentWorld* world, int indir = right, int depth = 1, int size = 1);
    virtual void doActivity();
    virtual ~EventSquare() {cerr << "EventSquare" << endl;};
                
};

class DroppingSquare : public Square
{
public:
    DroppingSquare(int imageID, int inx, int iny,StudentWorld* world, int indir = right, int depth = 1, int size = 1);
    virtual void doActivity();
    virtual ~DroppingSquare() {cerr << "DroppingSquare" << endl;};
private:
    int m_coins_change; // deduct 10 coins
    int m_stars_change; // decuct 1 star
};

class Enemy : public ActivateOnAvatar
{
public:
    Enemy(int imageID, int inx, int iny,StudentWorld* world, int indir = right, int depth = 0, int size = 1, bool state = PAUSED_OR_WAITING_TO_ROLL);
    virtual void doSomething();
    virtual void doActivity() = 0;
    virtual void setWalkingOrPaused(bool worp) {m_walking_or_paused = worp;};
    virtual int getTraveldist() {return m_travel_dist;};
    virtual int getPausedCounter() {return m_paused_counter;};
    virtual void changePausedCounter(int change) {m_paused_counter += change;};
    virtual void setPausedCounter(int value) {m_paused_counter = value;};
    virtual ~Enemy() {cerr << "Enemy" << endl;};
    virtual int getMaxSquaresToMove() = 0;
    virtual int getBWalkingDir() {return m_walkingdir;};
    virtual void setBWalkingDir(int newdir) {m_walkingdir = newdir;};
    virtual void BowserTakeCoins(Avatar* p) = 0;
    virtual bool canBeRemoved() {return false;};
    virtual int getBMaxSquaresToMove() = 0;
    virtual void swapPlayerCoins(Avatar* p, Avatar* x) = 0;
    virtual void swapPlayerStars(Avatar* p, Avatar* x) = 0;
    virtual bool isImactable() {return true;}; // Bowser and Boo are impactable by Vortex's
    virtual void Impact();

private:
    // for both boo and bowser
    int m_travel_dist;
    int m_paused_counter;
    bool m_walking_or_paused;
//    Avatar* m_peach; // has pointers to the players
//    Avatar* m_yoshi;
    int m_squares_to_move;
    int m_tickstomove;
    int m_walkingdir;
    
};

class Bowser : public Enemy
{
public:
    Bowser(int imageID, int inx, int iny,StudentWorld* world);
    virtual void doActivity() {}; // this is here because it needs it from activate on Avatar pure virtual function
    virtual ~Bowser() {cerr << "Bowser" << endl;};
    virtual int getMaxSquaresToMove() {return m_max_squares_to_move;};
    virtual void BowserTakeCoins(Avatar* p);
    virtual int getBMaxSquaresToMove() {return m_max_squares_to_move;};
    virtual void swapPlayerCoins(Avatar* p, Avatar* x) {};
    virtual void swapPlayerStars(Avatar* p, Avatar* x) {};
    virtual bool isImpactable() {return true;};
private:
    int m_max_squares_to_move;
};

class Boo : public Enemy
{
public:
    Boo(int imageID, int inx, int iny, StudentWorld* world);
    virtual void doActivity() {}; // this is here because it needs it from activate on Avatar pure virtual function
    virtual int getMaxSquaresToMove() {return m_max_squares_to_move;};
    virtual void BowserTakeCoins(Avatar* p) {};
    virtual int getBMaxSquaresToMove() {return m_max_squares_to_move;};
    virtual void swapPlayerCoins(Avatar* p, Avatar*x);
    virtual void swapPlayerStars(Avatar* p, Avatar*x);
    virtual bool isImpactable() {return true;};
    virtual ~Boo() {cerr << "Boo" << endl;};
private:
    int m_max_squares_to_move;
};

class Vortex : public Actor
{
public:
    Vortex(int imageID, int inx, int iny,StudentWorld* world, int vortexdirection, int indir = right, int depth = 0, int size = 1);
    virtual void doSomething();
    virtual int getVortexDir() {return m_vortex_dir;};
    virtual void setVortexDir(int newdir) {m_vortex_dir = newdir;};
    virtual bool outOfBounds();
    virtual ~Vortex() {cerr << "Vortex" << endl;};
private:
    int m_vortex_dir;
};







// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
