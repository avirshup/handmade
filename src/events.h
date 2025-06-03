#ifndef EVENTS_H
#define EVENTS_H

#include "./common.h"
#include "./state.h"

void handle_event(GameState* state, const SDL_Event* event);

#endif  // EVENTS_H
