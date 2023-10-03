#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
// Actor

Actor::~Actor()
{
    cerr << "actor" << endl;
}

Actor::Actor(int imageID, int inx, int iny,StudentWorld* world, int indir, int depth, int size, bool state)
:GraphObject(imageID,inx,iny,indir,depth,size),m_state(state),m_world(world)
{
}

// ActivatingObject
ActivatingObject::ActivatingObject(int imageID, int inx, int iny, StudentWorld *world, int indir, int depth, int size, bool state)
:Actor(imageID,inx,iny,world,indir,depth,size,state)
{
}

//ActivateOnAvatar
ActivateOnAvatar::ActivateOnAvatar(int imageID, int inx, int iny, StudentWorld *world, int indir, int depth, int size, bool state)
:ActivatingObject(imageID,inx,iny,world,indir,depth,size,state)
{
}

void ActivateOnAvatar::doSomething()
{
//    if(justLandedOrMovedOn(getWorld()->getYoshi()) || justLandedOrMovedOn(getWorld()->getPeach()))
        doActivity();
}

bool ActivateOnAvatar::justLandedOrMovedOn(Avatar* p) const // by avatar p
{
    
        if(getX() == p->getX() && getY() == p->getY() && p->getNew() == NEW) // if the player lands on or walks over this object
        {
            
            return true;
        }
        else
            return false;

}

bool ActivateOnAvatar::Landed(Avatar* p) const
{
    if(p->getstate() == PAUSED_OR_WAITING_TO_ROLL && getX() == p->getX() && getY() == p->getY() && p->getNew() == NEW) // if the player lands on this object
    {

        return true;
    }
    else
        return false;
}

bool ActivateOnAvatar::moveButNotLanded(Avatar* p) const
{
    if(p->getstate() == WALKING && getX() == p->getX() && getY() == p->getY() && p->getNew() == NEW) // if the player walks over this object
    {
        return true;
    }
    else
        return false;
}

// Square
Square::Square(int imageID,int inx, int iny,StudentWorld* world, int indir, int depth, int size)
:ActivateOnAvatar(imageID,inx,iny,world,indir,depth,size, ACTIVE)
{
}


// CoinSquare
CoinSquare::CoinSquare(int imageID, int inx, int iny,StudentWorld* world, int coinschange,int indir, int depth, int size)
:Square(imageID,inx,iny,world,indir,depth,size),m_coins_change(coinschange)
{
}

void CoinSquare::doActivity()
{
    if(getstate() == INACTIVE) // if the CoinSquare has been destroyed by Bowser
        return; // do nothing
    // if peach is waiting to roll and is a "new" player to this coinsquare
    Avatar* peach = getWorld()->getPeach(); // local peach
    Avatar* yoshi = getWorld()->getYoshi(); // local yoshi
    if(peach->getstate() == PAUSED_OR_WAITING_TO_ROLL && getX() == peach->getX() && getY() == peach->getY() && peach->getNew() == NEW) // we know peach landed or  moved on, now just checking for landed
    {
        if(peach->getCoins() < 3 && m_coins_change < 0)
            peach->addCoins((-1)*peach->getCoins());
        else
            peach->addCoins(m_coins_change);
        if(m_coins_change < 0)
            getWorld()->playSound(SOUND_TAKE_COIN);
        else
            getWorld()->playSound(SOUND_GIVE_COIN);
            
        peach->setNew(OLD);
    }
    if(yoshi->getstate() == PAUSED_OR_WAITING_TO_ROLL && getX() == yoshi->getX() && getY() == yoshi->getY() && yoshi->getNew() == NEW)
    {
        if(yoshi->getCoins() < 3 && m_coins_change < 0)
            yoshi->addCoins((-1)*yoshi->getCoins());
        else
            yoshi->addCoins(m_coins_change);
        if(m_coins_change < 0)
            getWorld()->playSound(SOUND_TAKE_COIN);
        else
            getWorld()->playSound(SOUND_GIVE_COIN);
        yoshi->setNew(OLD);
    }
}

// StarSquare
StarSquare::StarSquare(int imageID, int inx, int iny, StudentWorld* world, int indir, int depth, int size)
:Square(imageID,inx,iny,world,indir,depth,size),m_coins_change(-1*20),m_stars_change(1)
{
    
}

void StarSquare::doActivity()
{
    if(getstate() == INACTIVE) // if the CoinSquare has been destroyed by Bowser
        return; // do nothing
    Avatar* peach = getWorld()->getPeach(); // local peach
    Avatar* yoshi = getWorld()->getYoshi(); // local yoshi
    //if peach or yoshi just landed or moved on the starsquare
    if(justLandedOrMovedOn(peach))
    {
        peach->setNew(OLD);
        if(peach->getCoins() < 20)
            return;
        else
        {
            peach->addCoins(m_coins_change);
            peach->addStars(m_stars_change);
            getWorld()->playSound(SOUND_GIVE_STAR);
        }
    }
    if(justLandedOrMovedOn(yoshi))
    {
        yoshi->setNew(OLD);
        if(yoshi->getCoins() < 20)
            return;
        else
        {
            yoshi->addCoins(m_coins_change);
            yoshi->addStars(m_stars_change);
            getWorld()->playSound(SOUND_GIVE_STAR);
        }
    }
}

// Directional Square
DirectionalSquare::DirectionalSquare(int imageID, int inx, int iny,StudentWorld* world, int indir, int depth, int size)
:Square(imageID,inx,iny,world,indir,depth,size),m_forcing_dir(indir)
{
    
}


void DirectionalSquare::doActivity()
{
    if(getstate() == INACTIVE) // if the CoinSquare has been destroyed by Bowser
        return; // do nothing
    Avatar* peach = getWorld()->getPeach(); // local peach
    Avatar* yoshi = getWorld()->getYoshi(); // local yoshi
    // if peach or yoshi just landed or moved on the directional square
    if(justLandedOrMovedOn(peach))
    {
        peach->setNew(OLD);
        peach->setWalkingDir(m_forcing_dir);
        if(m_forcing_dir == right || m_forcing_dir == up || m_forcing_dir == down)
        {
            peach->setDirection(right);
        }
        else
            peach->setDirection(left);
    }
    if(justLandedOrMovedOn(yoshi))
    {
        yoshi->setNew(OLD);
        yoshi->setWalkingDir(m_forcing_dir);
        if(m_forcing_dir == right || m_forcing_dir == up || m_forcing_dir == down)
        {
            yoshi->setDirection(right);
        }
        else
            yoshi->setDirection(left);
//        cerr << "newy" << endl;
    }
}

// BankSquare
BankSquare::BankSquare(int imageID, int inx, int iny,StudentWorld* world, int indir, int depth, int size)
:Square(imageID,inx,iny,world,indir,depth,size),m_coins_change((-1)*5)
{
}

void BankSquare::doActivity()
{
    if(getstate() == INACTIVE) // if the CoinSquare has been destroyed by Bowser
        return; // do nothing
    Avatar* peach = getWorld()->getPeach(); // local peach
    Avatar* yoshi = getWorld()->getYoshi(); // local yoshi
    // if peach or yoshi just landed or moved on the directional square
    if(Landed(peach)) // if peach lands on the bank
    {
        peach->setNew(OLD);
        peach->addCoins(getWorld()->getBankBalance()); // add the bank balance to her coins
        getWorld()->addToBankBalance((-1)*getWorld()->getBankBalance()); // subtract bank balance from bank balance
        getWorld()->playSound(SOUND_WITHDRAW_BANK);
        
    }
    if(Landed(yoshi)) // if yoshi lands on the bank
    {
        yoshi->setNew(OLD);
        yoshi->addCoins(getWorld()->getBankBalance()); // add the bank balance to his coins
        getWorld()->addToBankBalance((-1)*getWorld()->getBankBalance()); // subtract the bank balance from bank balance (set it to zero)
        getWorld()->playSound(SOUND_WITHDRAW_BANK);
    }
    if(moveButNotLanded(peach)) // if peach moves over the bank
    {
        peach->setNew(OLD);
        if(peach->getCoins() < 5) // if peach has less than 5 coins
        {
            int deducted = peach->getCoins();
            peach->addCoins((-1)*peach->getCoins()); // subtract as many coins as peach has
            getWorld()->addToBankBalance(deducted); // add that many coins to the bank
        }
        else // peach has 5 or more coins
        {
            peach->addCoins(m_coins_change); // subtract 5 coins from peach
            getWorld()->addToBankBalance((-1)*m_coins_change); // add 5 coins to bank
        }
        getWorld()->playSound(SOUND_DEPOSIT_BANK);
    }
    if(moveButNotLanded(yoshi))
    {
        yoshi->setNew(OLD);
        if(yoshi->getCoins() < 5) // if peach has less than 5 coins
        {
            int deducted = yoshi->getCoins();
            yoshi->addCoins((-1)*yoshi->getCoins()); // subtract as many coins as peach has
            getWorld()->addToBankBalance(deducted); // add that many coins to the bank
        }
        else // peach has 5 or more coins
        {
            yoshi->addCoins(m_coins_change); // subtract 5 coins from peach
            getWorld()->addToBankBalance((-1)*m_coins_change); // add 5 coins to bank
        }
        getWorld()->playSound(SOUND_DEPOSIT_BANK);
    }
}

// EventSquare
EventSquare::EventSquare(int imageID, int inx, int iny,StudentWorld* world, int indir, int depth, int size)
:Square(imageID,inx,iny,world,indir,depth,size)
{
}

void EventSquare::doActivity()
{
    if(getstate() == INACTIVE) // if the EventSquare has been destroyed by Bowser
        return; // do nothing
    const int TELEPORT = 1;
    const int SWAP = 2;
    const int VORTEX = 3;
    int action = randInt(1, 3); // get a random action
    Avatar* peach = getWorld()->getPeach(); // local peach
    Avatar* yoshi = getWorld()->getYoshi(); // local yoshi
    // ADD IMPLEMENTATION OF EVENTSQUARE NOT ACTIVATING ON OTHER PLAYER AFTER SWAPPING POSTITIONS
    if(Landed(peach) && peach->getActivate() == YEAH) // if peach lands on the event square (not through a swap)
    {
        peach->setNew(OLD);
        switch(action)
        {
            case TELEPORT:
                peach->teleportToRandomLocation();
                getWorld()->playSound(SOUND_PLAYER_TELEPORT);
                break;
            case SWAP:
                peach->swapPos(yoshi);
                getWorld()->playSound(SOUND_PLAYER_TELEPORT);
                break;
            case VORTEX:
                peach->setVortex(YES_VORTEX); // give peach a vortex
                getWorld()->playSound(SOUND_GIVE_VORTEX);
                break;
        }
        
    }
    if(Landed(yoshi) && yoshi->getActivate() == YEAH) // if yoshi lands on the event square but not through a swap by peach
    {
        yoshi->setNew(OLD);
        switch(action)
        {
            case TELEPORT:
                yoshi->teleportToRandomLocation();
                getWorld()->playSound(SOUND_PLAYER_TELEPORT);
                break;
            case SWAP:
                yoshi->swapPos(peach);
                getWorld()->playSound(SOUND_PLAYER_TELEPORT);
                break;
            case VORTEX:
                yoshi->setVortex(YES_VORTEX); // give yoshi a vortex
                getWorld()->playSound(SOUND_GIVE_VORTEX);
                break;
        }
    }
}

// DroppingSquare
DroppingSquare::DroppingSquare(int imageID, int inx, int iny,StudentWorld* world, int indir, int depth, int size)
:Square(imageID,inx,iny,world,indir,depth,size),m_coins_change(-1*(10)),m_stars_change(-1)
{
}

void DroppingSquare::doActivity()
{
    if(getstate() == INACTIVE) // if the DroppingSquare has been destroyed by Bowser
        return; // do nothing
    const int DEDUCT_COINS = 1;
    const int DEDUCT_STARS = 2;
    int action = randInt(1, 2);
    Avatar* peach = getWorld()->getPeach(); // local peach
    Avatar* yoshi = getWorld()->getYoshi(); // local yoshi
    if(Landed(peach)) // if peach lands on the droppingsquare
    {
        peach->setNew(OLD);
        switch(action)
        {
            case DEDUCT_COINS:
                if(peach->getCoins() < 10) // if peach has less than 10 coins
                {
                    peach->addCoins((-1)*peach->getCoins()); // set her coins to 0
                }
                else // else she has 10 or more coins
                {
                    peach->addCoins(m_coins_change); // subtract 10 coins from her
                }
                getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
                break;
            case DEDUCT_STARS:
                if(peach->getStars() >= 1) // if peach has at least one star
                {
                    peach->addStars(m_stars_change); // remove that star
                }
                getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
                break;
        }
    }
    if(Landed(yoshi)) // if yoshi lands on the droppingsquare
    {
        yoshi->setNew(OLD);
        switch(action)
        {
            case DEDUCT_COINS:
                if(yoshi->getCoins() < 10) // if yoshi has less than 10 coins
                {
                    yoshi->addCoins((-1)*yoshi->getCoins()); // set his coins to 0
                }
                else // else he has 10 or more coins
                {
                    yoshi->addCoins(m_coins_change); // subtract 10 coins from him
                }
                getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
                break;
            case DEDUCT_STARS:
                if(yoshi->getStars() >= 1) // if yoshi has at least one star
                {
                    yoshi->addStars(m_stars_change); // remove that star
                }
                getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
                break;
        }
    }
}


Living::Living(int imageID, int inx, int iny,StudentWorld* world, int indir, int depth, int size)
:Actor(imageID,inx,iny,world,indir,depth,size,PAUSED_OR_WAITING_TO_ROLL),m_walkingdir(right) // all living things start out in paused or waiting to roll state
{
    
}

Avatar::Avatar(int imageID, int inx, int iny,int player,StudentWorld* world, int indir, int depth, int size)
:Living(imageID,inx,iny,world,indir,depth,size),m_ticks_to_move(0),m_die_roll(0),m_coins(0),m_stars(0),m_player(player),m_new(OLD),m_vortex(NO_VORTEX),m_activate(YEAH)
{
}

void Avatar::doSomething()
{
    if(getstate() == PAUSED_OR_WAITING_TO_ROLL)
    {
        int action = getWorld()->getAction(m_player);
        if(action == ACTION_ROLL) // if one of the players rolls
        {
            m_die_roll = randInt(1,10); // give them their die roll value from 1 to 10
            m_ticks_to_move = m_die_roll*8;
            setstate(WALKING); // set the player's state to Walking
            setNew(NEW); // the player has rolled again so they are a new player
            setActivate(YEAH); // allow the player to be triggered by event squares now
            
        }
        if(action == ACTION_FIRE && getVortex() == YES_VORTEX) // if one of the players fires and they have a vortex
        {
            // spawn a vortex directly in front of the player
            switch(getWalkingDir())
            {
                case right:
                    getWorld()->spawnVortex(getX()+SPRITE_WIDTH,getY(), getWalkingDir());
                    break;
                case left:
                    getWorld()->spawnVortex(getX()-SPRITE_WIDTH,getY(), getWalkingDir());
                    break;
                case up:
                    getWorld()->spawnVortex(getX(),getY()+SPRITE_HEIGHT, getWalkingDir());
                    break;
                case down:
                    getWorld()->spawnVortex(getX(),getY()-SPRITE_HEIGHT, getWalkingDir());
                    break;
            }
            setVortex(NO_VORTEX); // get rid of the player's vortex
        }
        else
            return; // because the player didn't do anything
    }
//    if((getWorld())->getAction(m_player) == ACTION_NONE)
//    {
//    }
    if(getstate() == WALKING)
    {
        // we only 
        
        int check1 = getX();
        int check2 = getY();
        if(check1%16 == 0 && check2%16 == 0) // check to make sure we only check bounds when we are in the center of a square
        {
            //             constantly check if there are more than two directions the player can go
            int numvalidirs = 0;
            // count the number of valid dirs at the current position
            if(isValidSquareInDir(right,this))
                numvalidirs++;
            if(isValidSquareInDir(up,this))
                numvalidirs++;
            if(isValidSquareInDir(left,this))
                numvalidirs++;
            if(isValidSquareInDir(down,this))
                numvalidirs++;
            if(numvalidirs > 2 && getWorld()->getBoard()->getContentsOf((getX()/SPRITE_WIDTH), getY()/SPRITE_HEIGHT) != Board::left_dir_square && getWorld()->getBoard()->getContentsOf((getX()/SPRITE_WIDTH), getY()/SPRITE_HEIGHT) != Board::right_dir_square && getWorld()->getBoard()->getContentsOf((getX()/SPRITE_WIDTH), getY()/SPRITE_HEIGHT) != Board::down_dir_square && getWorld()->getBoard()->getContentsOf((getX()/SPRITE_WIDTH), getY()/SPRITE_HEIGHT) != Board::up_dir_square) // the player reaches a fork and is not on a directional square
            {
                // IF *LANDS* ON FORK, skip this and wait for a roll and then get player input
                // If player is stuck at a fork
                if(m_ticks_to_move != 0)
                {
                    int action = getWorld()->getAction(m_player); // get the player action
                    switch(action)
                    {
                        case ACTION_NONE: // keep returning during the ticks they don't do anything
                            return;
                        case ACTION_RIGHT:
                            if(isValidSquareInDir(right, this) && getWalkingDir() != left) // if the square to the right is valid and the player did not come from the right
                            {
                                setWalkingDir(right);
                                setDirection(right);
//                                m_ticks_to_move = m_temp;
                            }
                            else
                                return;
                            break;
                        case ACTION_LEFT:
                            if(isValidSquareInDir(left, this) && getWalkingDir() != right) // if the square to the left is valid and the player did not come from the left
                            {
                                setWalkingDir(left);
                                setDirection(left);
//                                m_ticks_to_move = m_temp;
                            }
                            else
                                return;
                            break;
                        case ACTION_UP:
                            if(isValidSquareInDir(up, this) && getWalkingDir() != down) // if the square upwards is valid and the player did not come from above
                            {
                                setWalkingDir(up);
                                setDirection(right);
//                                m_ticks_to_move = m_temp;
                            }
                            else
                                return;
                            break;
                        case ACTION_DOWN:
                            if(isValidSquareInDir(down, this) && getWalkingDir() != up) // if the square downwards is valid and the player did not come below
                            {
                                setWalkingDir(down);
                                setDirection(right);
//                                m_ticks_to_move = m_temp;
                            }
                            else
                                return;
                            break;
                        default:
                            return;
                    }
                }
            }
            else
                setNew(NEW);
            // All the checks to see if the next square is empty and make it change direction
            if(getWalkingDir() == right) // if the character is going to the right
            {
                Board::GridEntry ge;
                ge = getWorld()->getBoard()->getContentsOf((getX()/SPRITE_WIDTH)+1, getY()/SPRITE_HEIGHT); // set ge to one ge to the right
                //            cout << ge << endl;
                //            cout << getX() << "," << getY() << endl;
                
                if(ge == Board::empty) // if the next space to the right is empty
                {
                    if(getWorld()->getBoard()->getContentsOf(getX()/SPRITE_WIDTH, (getY()/SPRITE_HEIGHT)+1) != Board::empty) // if it can move up
                    {
                        setWalkingDir(up); // set the character's walking direction to up and don't change direction it's facing
                    }
                    else // if it can't move up it has to move down
                        setWalkingDir(down);
                    setDirection(right);
                }
            }
            else if(getWalkingDir() == left) // if the character is going to the left
            {

                if(getWorld()->getBoard()->getContentsOf((getX()/SPRITE_WIDTH-1), getY()/SPRITE_HEIGHT) == Board::empty) // if the ge one to the left is empty
                {
                    if(getWorld()->getBoard()->getContentsOf(getX()/SPRITE_WIDTH, (getY()/SPRITE_HEIGHT)+1) != Board::empty) // if it can move up
                    {
                        setWalkingDir(up);
                    }
                    else // if it can't move up it has to move down
                        setWalkingDir(down);
                    setDirection(right); // set the direction it faces to the right because the avatar will be moving up, down or right
                }
            }
            else if(getWalkingDir() == up) // if the character is going up
            {
                if(getWorld()->getBoard()->getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT+1) == Board::empty) // if the ge one above is empty
                {
                    if(getWorld()->getBoard()->getContentsOf(getX()/SPRITE_WIDTH+1, getY()/SPRITE_HEIGHT) != Board::empty) // if it can move right
                    {
                        setWalkingDir(right); // set the walking dir to right
                        setDirection(right);
                    }
                    else // it can't move right so it has to move left
                    {
                        setWalkingDir(left);
                        setDirection(left);
                    }
                    
                }
            }
            else if(getWalkingDir() == down) // if the character is going down
            {
                if(getWorld()->getBoard()->getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT - 1) == Board::empty) // if the ge one below is empty
                {
                    if(getWorld()->getBoard()->getContentsOf(getX()/SPRITE_WIDTH+1, getY()/SPRITE_HEIGHT) != Board::empty) // if it can move right
                    {
                        setWalkingDir(right); // set the walking dir to right
                        setDirection(right);
                    }
                    else // it can't move right so it has to move left
                    {
                        setWalkingDir(left);
                        setDirection(left);
                    }
                }
            }
            
        }
        
    
        
        
//         move two pixels in the walking direction if the avatar has not been stopped
        if(m_ticks_to_move != 0)
        {
            switch(getWalkingDir())
            {
                case right:
                    moveTo(getX()+2, getY());
                    m_ticks_to_move--;
                    if(m_ticks_to_move == 0)
                        setstate(PAUSED_OR_WAITING_TO_ROLL);
                    break;
                case left:
                    moveTo(getX()-2, getY());
                    m_ticks_to_move--;
                    if(m_ticks_to_move == 0)
                        setstate(PAUSED_OR_WAITING_TO_ROLL);
                    break;
                case up:
                    moveTo(getX(), getY()+2);
                    m_ticks_to_move--;
                    if(m_ticks_to_move == 0)
                        setstate(PAUSED_OR_WAITING_TO_ROLL);
                    break;
                case down:
                    moveTo(getX(), getY()-2);
                    m_ticks_to_move--;
                    if(m_ticks_to_move == 0)
                        setstate(PAUSED_OR_WAITING_TO_ROLL);
                    break;
            }
            int check3 = getX();
            int check4= getY();
            if(check3%16 == 0 && check4%16 == 0 && m_die_roll != 0)
            {
                m_die_roll--;
            }

        }
    }

}

// returns if the next square in dir for Actor p is not empty
bool Actor::isValidSquareInDir(int dir, Actor* p)
{
    Board::GridEntry ge;
    switch(dir)
    {
        case right:
            ge = getWorld()->getBoard()->getContentsOf((p->getX()/SPRITE_WIDTH)+1, p->getY()/SPRITE_HEIGHT);
            break;
        case left:
            ge = getWorld()->getBoard()->getContentsOf((p->getX()/SPRITE_WIDTH)-1, p->getY()/SPRITE_HEIGHT);
            break;
        case up:
            ge = getWorld()->getBoard()->getContentsOf((p->getX()/SPRITE_WIDTH), (p->getY()/SPRITE_HEIGHT) + 1);
            break;
        case down:
            ge = getWorld()->getBoard()->getContentsOf((p->getX()/SPRITE_WIDTH), (p->getY()/SPRITE_HEIGHT)-1);
            break;
    }
    if(ge != Board::empty) // if that direction is not empty
    {
        return true;
    }
    else
        return false;
}

void Actor::teleportToRandomLocation() // teleports player or baddie to random location
{
    int xcoord;
    int ycoord;
    while(getWorld()->getBoard()->getContentsOf(xcoord/SPRITE_WIDTH, ycoord/SPRITE_HEIGHT) == Board::empty)
    {
        xcoord = randInt(0, 255); // new xcoord
        xcoord = xcoord - xcoord%16;
        ycoord = randInt(0,255); // new ycoord
        ycoord = ycoord - ycoord%16;
    }
    moveTo(xcoord, ycoord);
}

void Avatar::swapPos(Avatar* p)
{
    int x = getX(); // temp x and y coord to save them before they are changed
    int y = getY();
    moveTo(p->getX(), p->getY()); //switch player positions
    p->moveTo(x,y);
    
    // swap their ticks to move
    int temp = m_ticks_to_move;
    m_ticks_to_move = p->getTicksToMove();
    p->setTicksToMove(temp);
    
    // swap their walking directions
    int tempdir = getWalkingDir();
    setWalkingDir(p->getWalkingDir());
    p->setWalkingDir(tempdir);
    
    // swap their sprite directions
    int tempd = getDirection();
    setDirection(p->getDirection());
    p->setDirection(tempd);
    
    // swap their walking/waitingtoroll states
    int tempstate = getstate();
    setstate(p->getstate());
    p->setstate(tempstate);
    
    // make sure event square doesn't activate on other player
    p->setActivate(NOH); // this will be reset for the other player after they roll
}

// Enemy
Enemy::Enemy(int imageID, int inx, int iny,StudentWorld* world, int indir, int depth, int size, bool state)
:ActivateOnAvatar(imageID,inx,iny,world,indir,depth,size, ACTIVE),m_travel_dist(0),m_paused_counter(180),m_walking_or_paused(PAUSED_OR_WAITING_TO_ROLL),m_walkingdir(right) //have to pass in that it is active so that the move function calls do something on it
{
}


void Enemy::doSomething() // this will do everything for Boo and Bowser that is the same and call their differing functions if needed
{
    if(m_walking_or_paused == PAUSED_OR_WAITING_TO_ROLL) // if baddie is in paused state
    {
        if(Landed(getWorld()->getPeach()) && getWorld()->getPeach()->getstate() == PAUSED_OR_WAITING_TO_ROLL) // if peach is waiting to roll and she is on the same square as the baddie
        {
            int x = randInt(1,2);
            if(x == 1) // 50% chance
            {
                BowserTakeCoins(getWorld()->getPeach()); // bowser takes peach's coins
                swapPlayerCoins(getWorld()->getPeach(),getWorld()->getYoshi()); // boo swaps coins
            }
            else // other 50%
            {
                swapPlayerStars(getWorld()->getPeach(), getWorld()->getYoshi()); // boo swaps stars
            }
            
            // separate parts for bowser and boo
        }
        if(Landed(getWorld()->getYoshi()) && getWorld()->getYoshi()->getstate() == PAUSED_OR_WAITING_TO_ROLL) // if yoshi is waiting to roll and he is on the same square as the baddie
        {
            int x = randInt(1,2);
            if(x == 1) // 50% chance
            {
                BowserTakeCoins(getWorld()->getYoshi()); // bowser takes yoshi's coins
                swapPlayerCoins(getWorld()->getYoshi(),getWorld()->getPeach()); // boo swaps coins
            }
            else // other 50%
            {
                swapPlayerStars(getWorld()->getYoshi(), getWorld()->getPeach()); // boo swaps stars
            }
            // separate parts for bowser and boo
        }
        changePausedCounter(-1); // decrement the paused counter
        if(getPausedCounter() <= 0) // if the pause counter has run out
        {
            m_squares_to_move = randInt(1,getMaxSquaresToMove()); // set squares to move to a random max squares to move
            m_tickstomove = m_squares_to_move*8;

            // pick a random new direction for Bowser to move onto that is valid
            const int UP = 1;
            const int DOWN = 2;
            const int LEFT = 3;
            const int RIGHT = 4;
            int newdirectiontowalk = randInt(1,4);
            switch(newdirectiontowalk) // set newdirection to actual angle value
            {
                case UP:
                    newdirectiontowalk = up;
                    break;
                case DOWN:
                    newdirectiontowalk = down;
                    break;
                case RIGHT:
                    newdirectiontowalk = right;
                    break;
                case LEFT:
                    newdirectiontowalk = left;
                    break;
            }
            while(!isValidSquareInDir(newdirectiontowalk, this)) // while it isn't a valid direction keep setting it to a random direction until it gets one that is valid
            {
                newdirectiontowalk = randInt(1,4);
                switch(newdirectiontowalk)
                {
                    case UP:
                        newdirectiontowalk = up;
                        break;
                    case DOWN:
                        newdirectiontowalk = down;
                        break;
                    case RIGHT:
                        newdirectiontowalk = right;
                        break;
                    case LEFT:
                        newdirectiontowalk = left;
                        break;
                }
            }
            setBWalkingDir(newdirectiontowalk); // set Baddie's walking dir to the new direction to walk
            if(newdirectiontowalk == left) // if new direction is left
            {
                setDirection(left); // set baddie to face left
            }
            else // if new direction isn't left
            {
                setDirection(right); // set baddie to face right
            }

            setWalkingOrPaused(WALKING); // set the baddie into the walking state
        }
    }
    else if(m_walking_or_paused == WALKING)// if baddie is in walking state
    {
        int check1 = getX();
        int check2 = getY();
        if(check1%16 == 0 && check2%16 == 0) // check to make sure we only check bounds when we are in the center of a square
        {
            //             constantly check if there are more than two directions the player can go
            int numvalidirs = 0;
            // count the number of valid dirs at the current position
            if(isValidSquareInDir(right,this))
                numvalidirs++;
            if(isValidSquareInDir(up,this))
                numvalidirs++;
            if(isValidSquareInDir(left,this))
                numvalidirs++;
            if(isValidSquareInDir(down,this))
                numvalidirs++;
            if(numvalidirs > 2 && getWorld()->getBoard()->getContentsOf((getX()/SPRITE_WIDTH), getY()/SPRITE_HEIGHT) != Board::left_dir_square && getWorld()->getBoard()->getContentsOf((getX()/SPRITE_WIDTH), getY()/SPRITE_HEIGHT) != Board::right_dir_square && getWorld()->getBoard()->getContentsOf((getX()/SPRITE_WIDTH), getY()/SPRITE_HEIGHT) != Board::down_dir_square && getWorld()->getBoard()->getContentsOf((getX()/SPRITE_WIDTH), getY()/SPRITE_HEIGHT) != Board::up_dir_square) // if the baddie reaches a fork
            {
                // pick a random new direction for Baddie to move onto that is valid
                const int UP = 1;
                const int DOWN = 2;
                const int LEFT = 3;
                const int RIGHT = 4;
                int newdirectiontowalk = randInt(1,4);
                switch(newdirectiontowalk) // set newdirection to actual angle value
                {
                    case UP:
                        newdirectiontowalk = up;
                        break;
                    case DOWN:
                        newdirectiontowalk = down;
                        break;
                    case RIGHT:
                        newdirectiontowalk = right;
                        break;
                    case LEFT:
                        newdirectiontowalk = left;
                        break;
                }
                while(!isValidSquareInDir(newdirectiontowalk, this)) // while it isn't a valid direction keep setting it to a random direction until it gets one that is valid
                {
                    newdirectiontowalk = randInt(1,4);
                    switch(newdirectiontowalk)
                    {
                        case UP:
                            newdirectiontowalk = up;
                            break;
                        case DOWN:
                            newdirectiontowalk = down;
                            break;
                        case RIGHT:
                            newdirectiontowalk = right;
                            break;
                        case LEFT:
                            newdirectiontowalk = left;
                            break;
                    }
                }
                setBWalkingDir(newdirectiontowalk); // set the baddies walking dir to the new direction
                if(newdirectiontowalk == left) // if new direction is left
                {
                    setDirection(left); // set baddie to face left
                }
                else // if new direction isn't left
                {
                    setDirection(right); // set baddie to face right
                }
            }
            else if(!isValidSquareInDir(getBWalkingDir(), this)) // if the baddie is on top of a square and there is not a valid square in the direction of it's current walkindir
            {
                // ALL CHECKS TO AUTOCHANGE BADDIE DIRECTION IF NEXT SQUARE IS EMPTY
                if(getBWalkingDir() == right) // if the character is going to the right
                {
                    Board::GridEntry ge;
                    ge = getWorld()->getBoard()->getContentsOf((getX()/SPRITE_WIDTH)+1, getY()/SPRITE_HEIGHT); // set ge to one ge to the right
                    //            cout << ge << endl;
                    //            cout << getX() << "," << getY() << endl;
                    
                    if(ge == Board::empty) // if the next space to the right is empty
                    {
                        if(getWorld()->getBoard()->getContentsOf(getX()/SPRITE_WIDTH, (getY()/SPRITE_HEIGHT)+1) != Board::empty) // if it can move up
                        {
                            setBWalkingDir(up); // set the character's walking direction to up and don't change direction it's facing
                        }
                        else // if it can't move up it has to move down
                            setBWalkingDir(down);
                        setDirection(right);
                    }
                }
                else if(getBWalkingDir() == left) // if the character is going to the left
                {
                    
                    if(getWorld()->getBoard()->getContentsOf((getX()/SPRITE_WIDTH-1), getY()/SPRITE_HEIGHT) == Board::empty) // if the ge one to the left is empty
                    {
                        if(getWorld()->getBoard()->getContentsOf(getX()/SPRITE_WIDTH, (getY()/SPRITE_HEIGHT)+1) != Board::empty) // if it can move up
                        {
                            setBWalkingDir(up);
                        }
                        else // if it can't move up it has to move down
                            setBWalkingDir(down);
                        setDirection(right); // set the direction it faces to the right because the avatar will be moving up, down or right
                    }
                }
                else if(getBWalkingDir() == up) // if the character is going up
                {
                    if(getWorld()->getBoard()->getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT+1) == Board::empty) // if the ge one above is empty
                    {
                        if(getWorld()->getBoard()->getContentsOf(getX()/SPRITE_WIDTH+1, getY()/SPRITE_HEIGHT) != Board::empty) // if it can move right
                        {
                            setBWalkingDir(right); // set the walking dir to right
                            setDirection(right);
                        }
                        else // it can't move right so it has to move left
                        {
                            setBWalkingDir(left);
                            setDirection(left);
                        }
                        
                    }
                }
                else if(getBWalkingDir() == down) // if the character is going down
                {
                    if(getWorld()->getBoard()->getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT - 1) == Board::empty) // if the ge one below is empty
                    {
                        if(getWorld()->getBoard()->getContentsOf(getX()/SPRITE_WIDTH+1, getY()/SPRITE_HEIGHT) != Board::empty) // if it can move right
                        {
                            setBWalkingDir(right); // set the walking dir to right
                            setDirection(right);
                        }
                        else // it can't move right so it has to move left
                        {
                            setBWalkingDir(left);
                            setDirection(left);
                        }
                    }
                }
            }
        }
                // move to pixels in the walking direction
        int randomnumber = randInt(1,4); // get a random number
                switch(getBWalkingDir())
                {
                    case right:
                        moveTo(getX()+2, getY());
                        m_tickstomove--;
                        if(m_tickstomove == 0)
                        {
                            setWalkingOrPaused(PAUSED_OR_WAITING_TO_ROLL); // set baddie back to paused
                            changePausedCounter(180); // set the paused counter to 180
                            if(randomnumber == 1 && getBMaxSquaresToMove() == 10) // 25% chance that Bowser will deposit a dropping
                            {
                                cerr << "deposit dropping" << endl;
                                getWorld()->removeSquare(getX(), getY());
                                getWorld()->addDroppingSquare(getX(), getY());
                                
                            }
                        }
                        break;
                    case left:
                        moveTo(getX()-2, getY());
                        m_tickstomove--;
                        if(m_tickstomove == 0)
                        {
                            setWalkingOrPaused(PAUSED_OR_WAITING_TO_ROLL);
                            changePausedCounter(180);
                            if(randomnumber == 1 && getBMaxSquaresToMove() == 10) // 25% chance that Bowser will deposit a dropping
                            {
                                cerr << "deposit dropping" << endl;
                                getWorld()->removeSquare(getX(), getY());
                                getWorld()->addDroppingSquare(getX(), getY());
                            }
                        }
                        break;
                    case up:
                        moveTo(getX(), getY()+2);
                        m_tickstomove--;
                        if(m_tickstomove == 0)
                        {
                            setWalkingOrPaused(PAUSED_OR_WAITING_TO_ROLL);
                            changePausedCounter(180);
                            if(randomnumber == 1 && getBMaxSquaresToMove() == 10) // 25% chance that Bowser will deposit a dropping
                            {
                                cerr << "deposit dropping" << endl;
                                getWorld()->removeSquare(getX(), getY());
                                getWorld()->addDroppingSquare(getX(), getY());
                            }
                        }
                        break;
                    case down:
                        moveTo(getX(), getY()-2);
                        m_tickstomove--;
                        if(m_tickstomove == 0)
                        {
                            setWalkingOrPaused(PAUSED_OR_WAITING_TO_ROLL);
                            changePausedCounter(180);
                            if(randomnumber == 1 && getBMaxSquaresToMove() == 10) // 25% chance that Bowser will deposit a dropping
                            {
                                cerr << "deposit dropping" << endl;
                                getWorld()->removeSquare(getX(), getY());
                                getWorld()->addDroppingSquare(getX(), getY());
                            }
                        }
                        break;
                }
    }
}

void Enemy::Impact() // When enemies are impacted, they teleport to a random location
{
    teleportToRandomLocation();
    setBWalkingDir(right);
    setWalkingOrPaused(PAUSED_OR_WAITING_TO_ROLL);
    setPausedCounter(180); //sets pause to 180
}

// Bowser
Bowser::Bowser(int imageID, int inx, int iny,StudentWorld* world)
:Enemy(imageID,inx,iny,world),m_max_squares_to_move(10)
{
}

void Bowser::BowserTakeCoins(Avatar* p) // bowser takes the players coins
{
    p->addCoins((-1)*p->getCoins()); // remove all of the players coins
    getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
}

// Boo
Boo::Boo(int imageID, int inx, int iny, StudentWorld* world)
:Enemy(imageID,inx,iny,world),m_max_squares_to_move(3)
{
}

void Boo::swapPlayerCoins(Avatar* p, Avatar* x)
{
    //swap coins
    int tempcoins = p->getCoins();
    p->setCoins(x->getCoins());
    x->setCoins(tempcoins);
    //play sound
    getWorld()->playSound(SOUND_BOO_ACTIVATE);
    cerr << "boo sound coins" << endl;
}

void Boo::swapPlayerStars(Avatar* p, Avatar* x)
{
    //swap stars
    int tempstars = p->getStars();
    p->setStars(x->getStars());
    x->setStars(tempstars);
    //play sounds
    getWorld()->playSound(SOUND_BOO_ACTIVATE);
    cerr << "boo sound stars" << endl;
    
}

// Vortex
Vortex::Vortex(int imageID, int inx, int iny,StudentWorld* world, int vortexdirection, int indir, int depth, int size)
:Actor(imageID,inx,iny,world,indir,depth,size,ACTIVE),m_vortex_dir(vortexdirection) // set the vortex direction to the direction specified by player
{
}

void Vortex::doSomething()
{
    if(getstate() == INACTIVE)
        return;
    
    // move two pixels in vortex direction
    switch(getVortexDir())
    {
        case right:
            moveTo(getX()+2, getY());
            break;
        case left:
            moveTo(getX()-2, getY());
            break;
        case up:
            moveTo(getX(), getY()+2);
            break;
        case down:
            moveTo(getX(), getY()-2);
            break;
    }
    if(outOfBounds()) // if the Vortex goes out of bounds
    {
        setstate(INACTIVE); // set it to inactive
    }
    Actor* a = getWorld()->isVortexOverlapping(getX(), getY());
    if(a != nullptr) // if the vortex is overlapping with something that is impactable
    {
        cerr << "overlapping with baddie" << endl;
        a->Impact();// call impact on the object
        setstate(INACTIVE); // set the vortex to inactive
        getWorld()->playSound(SOUND_HIT_BY_VORTEX);
    }
    
}

bool Vortex::outOfBounds()
{
    if((getX() < 0 || getX() >= VIEW_WIDTH) || (getY() < 0 || getY() >= VIEW_HEIGHT)) // if vortex is out of bounds
    {
        return true;
    }
    else
        return false;
}


