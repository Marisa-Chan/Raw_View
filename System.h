#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include <inttypes.h>
#include <SDL/SDL.h>

//System time functions
uint64_t millisec();

//Game timer functions
void InitMTime();
void ProcMTime();
bool GetTick();
uint64_t GetTickCount();

//Keyboard functions
void FlushHits();
void UpdateKeyboard();
bool KeyDown(SDLKey key);
bool KeyAnyHit();
int MouseX();
int MouseY();
bool MouseDown(int btn);
void SetHit(SDLKey key);
bool KeyHit(SDLKey key);


#endif // SYSTEM_H_INCLUDED
