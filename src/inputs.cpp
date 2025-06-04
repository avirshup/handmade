#include "./inputs.h"

#include <optional>
#include <unordered_map>
#include "./common.h"
#include "state.h"

using namespace std;

InputState init_input() {
  return InputState{};
}

internal auto get_gpad(InputState* state, const SDL_JoystickID jid)
    -> optional<reference_wrapper<GPad>> {
  if (const auto it = state->gamepads.find(jid); it != state->gamepads.end()) {
    return ref(it->second);  // wrap the reference
  }
  return nullopt;
}

/***** Input events ******/
errcode keyboard_key(
    InputState* state,
    WorldState* world,
    const SDL_Keysym key,
    const InputId pressed,
    const bool repeat) {

  if (pressed == SDL_PRESSED) {
    spdlog::trace("Key '%d' pressed (repeat=%d)\n", key.sym, repeat);
    
    // Handle arrow key presses
    switch (key.sym) {
      case SDLK_UP:
        world->user_acc.y += 1.0f;
        break;
      case SDLK_DOWN:
        world->user_acc.y -= 1.0f;
        break;
      case SDLK_LEFT:
        world->user_acc.x -= 1.0f;
        break;
      case SDLK_RIGHT:
        world->user_acc.x += 1.0f;
        break;
    }
  } else {
    spdlog::trace("Key '%d' released (repeat=%d)\n", key.sym, repeat);
    
    // Handle arrow key releases
    switch (key.sym) {
      case SDLK_UP:
        world->user_acc.y -= 1.0f;
        break;
      case SDLK_DOWN:
        world->user_acc.y += 1.0f;
        break;
      case SDLK_LEFT:
        world->user_acc.x += 1.0f;
        break;
      case SDLK_RIGHT:
        world->user_acc.x -= 1.0f;
        break;
    }
  }

  return 0;
};

errcode controller_button(
    InputState* state,
    WorldState* world,
    const SDL_JoystickID jid,
    const SDL_GameControllerButton button,
    const Uint8 pressed) {
  const auto gpad = get_gpad(state, jid);
  if (!gpad)
    return 1;

  if (pressed == SDL_PRESSED) {
    printf("Button '%d' pressed on controller %d\n", button, gpad->get().cid);
  } else {
    printf("Button '%d' released on controller %d\n", button, gpad->get().cid);
  }

  return 0;
}

errcode controller_axis(
    InputState* state,
    WorldState* world,

    const SDL_JoystickID jid,
    const SDL_GameControllerAxis axis,
    const Sint32 value) {
  const auto it_gpad = state->gamepads.find(jid);
  if (it_gpad == state->gamepads.end()) {
    printf("Got unknown controller event for jid %d", jid);
    return 1;
  }
  const GPad gpad = it_gpad->second;

  printf("Axis '%d' on controller %d now: {%d}\n", axis, gpad.cid, value);

  return 0;
}

/***** Device events ******/
errcode add_controller(InputState* state, const SDL_JoystickID jid) {
  assert(!state->gamepads.contains(jid));

  const ControllerId cid = state->next_controller_id++;
  printf("New controller, ID %d", cid);
  state->gamepads[jid] = GPad{
      .jid = jid,
      .cid = cid,
      .handle = SDL_GameControllerOpen(jid),
  };

  return 0;
}

errcode remove_controller(InputState* state, const SDL_JoystickID jid) {
  const auto gpad = get_gpad(state, jid);
  if (!gpad)
    return 1;

  // TODO: really not sure about how memory safety works here!!!
  const GPad old_gpad = gpad->get();
  printf("Removed controller ID %d", old_gpad.cid);

  SDL_GameControllerClose(old_gpad.handle);
  state->gamepads.erase(jid);
  return 0;
}
