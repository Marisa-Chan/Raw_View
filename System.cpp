#include <SDL/SDL.h>
#include <inttypes.h>
#include <time.h>


//Returns count of millisecs(1/1000 of second) from system start
uint64_t millisec()
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (uint64_t)t.tv_sec * 1000 + t.tv_nsec / 1000000;
}


uint8_t KeyHits[1024]; //Array with hitted keys (once per press)
bool AnyHit=false;    //it's indicate what any key was pressed
uint8_t *Keys;        //Array with pressed keys (while pressed)

int Mx, My;
uint8_t Mstate;


//Reset state of key hits states
void FlushHits()
{
    AnyHit=false;
    memset(KeyHits,0,1024);
}

//Sets hit state for key
void SetHit(SDLKey key)
{
    AnyHit=true;
    KeyHits[key]=1;
}

//Returns hit state of the key
bool KeyHit(SDLKey key)
{
    if (KeyHits[key])
        return true;
    else
        return false;
}

//return true if any key was hitted(by key hit)
bool KeyAnyHit()
{
    return AnyHit;
}

//Update key pressed states
void UpdateKeyboard()
{
    Keys=SDL_GetKeyState(NULL);
    Mstate=SDL_GetMouseState(&Mx,&My);
}

//return true if key was pressed(continously)
bool KeyDown(SDLKey key)
{
    if (Keys[key])
        return true;
    else
        return false;
}

int MouseX()
{
    return Mx;
}

int MouseY()
{
    return My;
}

bool MouseDown(int btn)
{
    if (Mstate & SDL_BUTTON(btn))
        return true;
    else
        return false;
}






uint64_t mtime=0;    //Game timer ticks [after ~23 milliards years will came overflow of this var, don't play so long]
bool    btime=false; //Indicates new Tick
uint64_t reltime=0;  //Realtime ticks for calculate game ticks

//Resets game timer and set next realtime point to incriment game timer
void InitMTime()
{
    mtime=0;
    btime=false;
    reltime=millisec() + 35;
}

//Process game timer.
void ProcMTime()
{
    if (reltime < millisec())   //New tick
    {
        mtime++;
        btime=true;
        reltime=millisec() + 35;
    }
    else                        //No new tick
    {
        btime=false;
    }
#ifdef DINGOO
    SDL_Delay(2);
#else
    SDL_Delay(10);
#endif
}

//Resturn true if new tick appeared
bool GetTick()
{
    return btime;
}

//Return count of game timer ticks
uint64_t GetTickCount()
{
    return mtime;
}


