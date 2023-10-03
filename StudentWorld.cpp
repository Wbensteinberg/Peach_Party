#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <string>
#include <cmath>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

void StudentWorld::add_actor(Actor* actor) // add a new actor to the studentworld
{
    
}

bool canMove(int dest_x, int dest_y)
{
    return true;
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_peach = nullptr;
    m_yoshi = nullptr;
    m_b = nullptr;
    m_bank_balance = 0;
}

int StudentWorld::init()
{
//    CoinSquare cs(IID_BLUE_COIN_SQUARE,0,0,this);
//	startCountdownTimer(5);  // this placeholder causes timeout after 5 seconds
    string board_file = assetPath() += "board0";
    board_file += to_string(getBoardNumber());
    board_file += ".txt"; // finish constructing the path to the specified board
    m_b = new Board; // create a pointer to a new board
    m_b->loadBoard(board_file); // load the specified board into b
    
    Board::LoadResult result = m_b->loadBoard(board_file);
    if(result == Board::load_fail_file_not_found)
        return GWSTATUS_BOARD_ERROR;
    
    // loop through the board 
    for(int k = 0; k < BOARD_WIDTH; k++) // loop through the rows
        for(int i = 0;i < BOARD_HEIGHT;i++) // for each row, loop through each item
        {
            Board::GridEntry ge = m_b->getContentsOf(k, i); // set ge to the character on the board grid of the text board
            switch(ge)
            {
                case Board::blue_coin_square: // if it represents a blue coin square
                {
                    CoinSquare* bcs = new CoinSquare(IID_BLUE_COIN_SQUARE,k*SPRITE_WIDTH,i*SPRITE_HEIGHT,this,3);
                    m_ac.push_back(bcs);
                }
                    break;
                case Board::player:
                {
                    CoinSquare* bcs2 = new
                    CoinSquare(IID_BLUE_COIN_SQUARE,k*SPRITE_WIDTH,i*SPRITE_HEIGHT,this,3);
                    m_ac.push_back(bcs2);
                    Avatar* p = new Avatar(IID_PEACH,k*SPRITE_WIDTH,i*SPRITE_HEIGHT,PEACH,this);
                    m_peach = p;
                    Avatar* y = new Avatar(IID_YOSHI,k*SPRITE_WIDTH,i*SPRITE_HEIGHT,YOSHI,this);
                    m_yoshi = y;
                    break;
                }
                case Board::red_coin_square:
                {
                    CoinSquare* rcs = new CoinSquare(IID_RED_COIN_SQUARE,k*SPRITE_WIDTH,i*SPRITE_HEIGHT,this,-3);
                    m_ac.push_back(rcs);
                    break;
                }
                case Board::up_dir_square:
                {
                    DirectionalSquare* uds = new DirectionalSquare(IID_DIR_SQUARE,k*SPRITE_WIDTH,i*SPRITE_HEIGHT,this,GraphObject::up);
                    m_ac.push_back(uds);
                    break;
                }
                case Board::down_dir_square:
                {
                    DirectionalSquare* dds = new DirectionalSquare(IID_DIR_SQUARE,k*SPRITE_WIDTH,i*SPRITE_HEIGHT,this,GraphObject::down);
                    m_ac.push_back(dds);
                    break;
                }
                case Board::right_dir_square:
                {
                    DirectionalSquare* rds = new DirectionalSquare(IID_DIR_SQUARE,k*SPRITE_WIDTH,i*SPRITE_HEIGHT,this,GraphObject::right);
                    m_ac.push_back(rds);
                    break;
                }
                case Board::left_dir_square:
                {
                    DirectionalSquare* lds = new DirectionalSquare(IID_DIR_SQUARE,k*SPRITE_WIDTH,i*SPRITE_HEIGHT,this,GraphObject::left);
                    m_ac.push_back(lds);
                    break;
                }
                case Board::star_square:
                {
                    StarSquare* ss = new StarSquare(IID_STAR_SQUARE,k*SPRITE_WIDTH,i*SPRITE_HEIGHT,this);
                    m_ac.push_back(ss);
                    break;
                }
                case Board::bank_square:
                {
                    BankSquare* bs = new BankSquare(IID_BANK_SQUARE,k*SPRITE_WIDTH,i*SPRITE_HEIGHT,this);
                    m_ac.push_back(bs);
                    break;
                }
                case Board::event_square:
                {
                    EventSquare* es = new EventSquare(IID_EVENT_SQUARE,k*SPRITE_WIDTH,i*SPRITE_HEIGHT,this);
                    m_ac.push_back(es);
                    break;
                }
                case Board::bowser:
                {
                    Bowser* bwsr = new Bowser(IID_BOWSER,k*SPRITE_WIDTH,i*SPRITE_HEIGHT,this);
                    m_ac.push_back(bwsr);
                    CoinSquare* bcs3 = new
                    CoinSquare(IID_BLUE_COIN_SQUARE,k*SPRITE_WIDTH,i*SPRITE_HEIGHT,this,3);
                    m_ac.push_back(bcs3);
                }
                case Board::boo:
                {
                    Boo* bo = new Boo(IID_BOO,k*SPRITE_WIDTH,i*SPRITE_HEIGHT,this);
                    m_ac.push_back(bo);
                    CoinSquare* bcs4 = new
                    CoinSquare(IID_BLUE_COIN_SQUARE,k*SPRITE_WIDTH,i*SPRITE_HEIGHT,this,3);
                    m_ac.push_back(bcs4);
                }
            }
        }
        
    
    startCountdownTimer(99);
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.

//    setGameStatText("Game will end in a few seconds");
    
    m_peach->doSomething(); // give peach a chance to do something
    m_yoshi->doSomething(); // give yoshi a chance to do something
    for(vector<Actor*>::iterator p = m_ac.begin();p != m_ac.end();p++) // loop through actors
    {
        if((*p)->getstate() == ACTIVE) // if the actor is active
        {
            (*p)->doSomething(); // give all actors a chance to do something
        }
    }
    for(vector<Actor*>::iterator p = m_ac.begin();p != m_ac.end();p++) // loop through actors
    {
        if((*p) && (*p)->canBeRemoved() && (*p)->getstate() == INACTIVE) // if *p isnt null pointer, can be removed and has an Inactive state
        {
            delete *p; // delete any that have become inactive
            p = m_ac.erase(p); // remove their dangling pointers from the vector
            p--;
        }
    }
    
    // Displaying stats string
    string gamestats = "P1 Roll: ";
    gamestats += to_string(m_peach->getDieRoll());
    gamestats += " Stars: ";
    gamestats += to_string(m_peach->getStars());
    gamestats += " $$: ";
    gamestats += to_string(m_peach->getCoins());
    if(m_peach->getVortex() == YES_VORTEX)
    {
        gamestats += " VOR";
    }
    // IMPLEMENT VORTEXs INTO STRING
    gamestats += " | Time: ";
    gamestats += to_string(timeRemaining());
    gamestats += " | Bank: ";
    gamestats += to_string(getBankBalance());
    gamestats += " | P2 Roll: ";
    gamestats += to_string(m_yoshi->getDieRoll());
    gamestats += " Stars: ";
    gamestats += to_string(m_yoshi->getStars());
    gamestats += " $$: ";
    gamestats += to_string(m_yoshi->getCoins());
    if(m_yoshi->getVortex() == YES_VORTEX)
    {
        gamestats += " VOR";
    }
    // IMPLEMENT VORTEXs INTO STRING
    setGameStatText(gamestats);
        
    if (timeRemaining() <= 0) // if the game is over
    {
        playSound(SOUND_GAME_FINISHED); // play game ending sound
        int winner = 0;
        if(m_peach->getStars() > m_yoshi->getStars()) // if the game is over and peach has more starts
        {
            winner = GWSTATUS_PEACH_WON; // peach is the winner
        }
        if(m_yoshi->getStars() > m_peach->getStars()) // if the game is over and yoshi has more stars
        {
            winner = GWSTATUS_YOSHI_WON;
        }
        if(m_peach->getStars() == m_yoshi->getStars()) // if the game is over they have same stars
        {
            if(m_peach->getCoins() > m_yoshi->getCoins()) // if peach more coins
            {
                winner = GWSTATUS_PEACH_WON; // peach is the winner
            }
            if(m_yoshi->getCoins() > m_peach->getCoins()) // if yoshi more coins
            {
                winner = GWSTATUS_YOSHI_WON; //
            }
            if(m_yoshi->getCoins() == m_peach->getCoins()) // if they have the same stars and coins
            {
                int x = randInt(1, 2); // get random winner
                if(x == 1)
                    winner = GWSTATUS_PEACH_WON;
                if(x == 2)
                    winner = GWSTATUS_YOSHI_WON;
            }
        }
        // display final scores
        if(winner == GWSTATUS_PEACH_WON)
            setFinalScore(m_peach->getStars(), m_peach->getCoins());
        if(winner == GWSTATUS_YOSHI_WON)
            setFinalScore(m_yoshi->getStars(), m_yoshi->getCoins());
        
        // return the winner
        return winner;
    }
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for(vector<Actor*>::iterator p = m_ac.begin();p != m_ac.end();p++)
    {
        if(*p == nullptr)
            break;
        delete *p;
        p = m_ac.erase(p);
        p--;
    }
    delete m_peach;
    m_peach = nullptr;
    delete m_yoshi;
    m_yoshi = nullptr;
    delete m_b;
    m_b = nullptr;
}

StudentWorld::~StudentWorld()
{cleanUp();}

void StudentWorld::removeSquare(int x, int y)
{
    for(vector<Actor*>::iterator p = m_ac.begin();p != m_ac.end();p++)
    {
        if((*p)->getX() == x && (*p)->getY() == y && (*p)->canBeRemoved() == true) // if the x and y coordinates match and it can be removed
        {
            (*p)->setstate(INACTIVE); // set state to inactive so that it is deleted in the move function
        }
    }
}

void StudentWorld::addDroppingSquare(int x, int y)
{
    DroppingSquare* ds = new DroppingSquare(IID_DROPPING_SQUARE, x, y,this);
    m_ac.push_back(ds);
    playSound(SOUND_DROPPING_SQUARE_CREATED);
}

void StudentWorld::spawnVortex(int x, int y, int dir)
{
    Vortex* v = new Vortex(IID_VORTEX,x,y,this,dir);
    m_ac.push_back(v);
    playSound(SOUND_PLAYER_FIRE);
}

Actor* StudentWorld::isVortexOverlapping(int x, int y)
{
    for(vector<Actor*>::iterator p = m_ac.begin();p != m_ac.end();p++)
    {
        if((*p)->isImpactable() == true && abs((*p)->getX() - x) < 16 && abs((*p)->getY() - y) < 16) // if the object is impactable by vortexs and the vortex overlaps with it
        {
            return *p; // return a pointer to the baddie
        }
    }
    // if nothing is overlapping
    return nullptr; // return the nullptr
}
