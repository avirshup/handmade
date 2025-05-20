
#ifndef INPUTS_H
#define INPUTS_H

#include "./common.h"

typedef int ControllerId;
typedef Uint8 InputId;

errcode add_controller(SDL_JoystickID jid);
errcode remove_controller(SDL_JoystickID jid);

errcode keyboard_key(SDL_Keysym key, InputId pressed, bool repeat);

errcode controller_button(
    SDL_JoystickID jid,
    SDL_GameControllerButton button,
    InputId pressed);
errcode
controller_axis(SDL_JoystickID jid, SDL_GameControllerAxis axis, Sint32 value);
#endif  // INPUTS_H
