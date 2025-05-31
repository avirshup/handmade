#include "./inputs.h"

#include <assert.h>

#include <optional>
#include <unordered_map>
#include "./common.h"

#define MAX_PLAYERS 8

using namespace std;

typedef int ControllerId;

typedef struct GPad {
  SDL_JoystickID jid;
  ControllerId cid;
  SDL_GameController* handle;
} GPad;

// TODO: not global vars, COME ON
global_var unordered_map<SDL_JoystickID, GPad> GPAD_MAP;
global_var ControllerId next_controller_id;

internal auto get_gpad(const SDL_JoystickID jid)
    -> optional<reference_wrapper<GPad>> {
  if (const auto it = GPAD_MAP.find(jid); it != GPAD_MAP.end()) {
    return ref(it->second);  // wrap the reference
  }
  return nullopt;
}

/***** Input events ******/
errcode
keyboard_key(const SDL_Keysym key, const InputId pressed, const bool repeat) {
  if (pressed == SDL_PRESSED) {
    printf("Key '%d' pressed (repeat=%d)\n", key.sym, repeat);
  } else {
    printf("Key '%d' released (repeat=%d)\n", key.sym, repeat);
  }

  return 0;
};

errcode controller_button(
    const SDL_JoystickID jid,
    const SDL_GameControllerButton button,
    const Uint8 pressed) {
  const auto gpad = get_gpad(jid);
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
    const SDL_JoystickID jid,
    const SDL_GameControllerAxis axis,
    const Sint32 value) {
  const auto it_gpad = GPAD_MAP.find(jid);
  if (it_gpad == GPAD_MAP.end()) {
    printf("Got unknown controller event for jid %d", jid);
    return 1;
  }
  const GPad gpad = it_gpad->second;

  printf("Axis '%d' on controller %d now: {%d}\n", axis, gpad.cid, value);

  return 0;
}

/***** Device events ******/
errcode add_controller(const SDL_JoystickID jid) {
  assert(!GPAD_MAP.contains(jid));

  const ControllerId cid = next_controller_id++;
  printf("New controller, ID %d", cid);
  GPAD_MAP[jid] = GPad{
      .jid = jid,
      .cid = cid,
      .handle = SDL_GameControllerOpen(jid),
  };

  return 0;
}

errcode remove_controller(const SDL_JoystickID jid) {
  const auto found_gpad_iter = GPAD_MAP.find(jid);
  if (found_gpad_iter == GPAD_MAP.end()) {
    return 1;
  }

  const GPad old_gpad = found_gpad_iter->second;
  printf("Removed controller ID %d", old_gpad.cid);
  GPAD_MAP.erase(jid);

  SDL_GameControllerClose(old_gpad.handle);
  return 0;
}
