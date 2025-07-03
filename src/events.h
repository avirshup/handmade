#ifndef EVENTS_H
#define EVENTS_H

#include "./common.h"
#include "./state.h"

namespace events {

void handle_event(state::GameState* state, const SDL_Event* event);

}  // namespace events

#endif  // EVENTS_H
