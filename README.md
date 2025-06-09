# Known problems

## casts from floats/doubles to ints

- It's UB (!!!) to cast a floating point number into an int type if it's too big.
    - (seriously WHY???)
- Probably should be using [std::round](https://en.cppreference.com/w/cpp/numeric/math/round)
  and/or [std::clamp](https://en.cppreference.com/w/cpp/algorithm/clamp.html)

## Frequency domain aliasing

- The `Phaser` + waveform generators to be able to handle non-integer periods
  and phases; frequency aliasing is clearly audible at not-even-particularly-high
  frequencies (~500 Hz-ish I think?)

