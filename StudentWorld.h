#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include "Actor.h"
#include <string>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Avatar;
class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();
    virtual Board* getBoard() {return m_b;};
    void add_actor(Actor* actor);
    // Determine if there is a square at the specified location. Used to determine if a gameobject
      // like a player, enemy or vortex can move onto the specified location
      bool canMove(int dest_x, int dest_y) const;
    virtual Avatar* getPeach() const {return m_peach;};
    virtual Avatar* getYoshi() const {return m_yoshi;};
    virtual int getBankBalance() const {return m_bank_balance;};
    virtual void addToBankBalance(int newbalance) {m_bank_balance += newbalance;};
    virtual void removeSquare(int x, int y);
    virtual void addDroppingSquare(int x, int y);
    virtual void spawnVortex(int x, int y, int dir);
    virtual Actor* isVortexOverlapping(int x, int y); // if vortex overlaps with a baddie, this returns a pointer to the baddie, if not, it returns the nullptr
    ~StudentWorld(); 
private:
    vector<Actor*> m_ac; // a vector that holds pointers to all the actors in the game
    Avatar* m_peach; // peach
    Avatar* m_yoshi; // yoshi
    Board* m_b;
    int m_bank_balance;
};

#endif // STUDENTWORLD_H_
