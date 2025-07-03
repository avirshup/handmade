
#ifndef INPUTS_H
#define INPUTS_H

#include "./common.h"
#include "world.h"

using namespace std;

namespace inputs {
typedef int ControllerId;
typedef Uint8 InputId;

struct GPad {
  SDL_JoystickID jid;
  ControllerId cid;
  SDL_GameController* handle;
};

struct InputState {
  unordered_map<SDL_JoystickID, GPad> gamepads;
  ControllerId next_controller_id;
};

/*******************
Input device management
*******************/
InputState init_input();

errcode add_controller(InputState* state, SDL_JoystickID jid);

errcode remove_controller(InputState* state, SDL_JoystickID jid);

/*******************
Input handling
*******************/
errcode keyboard_key(
    InputState* state,
    world::WorldState* world,
    SDL_Keysym key,
    InputId pressed,
    bool repeat);

errcode controller_button(
    InputState* state,
    world::WorldState* world,
    SDL_JoystickID jid,
    SDL_GameControllerButton button,
    InputId pressed);

errcode controller_axis(
    InputState* state,
    world::WorldState* world,
    SDL_JoystickID jid,
    SDL_GameControllerAxis axis,
    Sint32 value);
}  // namespace inputs
#endif  // INPUTS_H
