#include <math.h>
#include <iostream>

class Phased {
 protected:
  unsigned phase_ = 0;
  unsigned period_;

 public:
  explicit Phased(const unsigned period_ticks) : period_(period_ticks) {}

  unsigned next() {
    phase_ %= period_;
    return phase_++;
  }

  unsigned get_phase() const { return phase_; }

  void set_phase(const unsigned new_phase) { phase_ = new_phase; }

  unsigned get_period() const { return period_; }

  void set_period(const unsigned new_period) {
    if (new_period == 0)
      throw std::invalid_argument("Period must be greater than 0");

    phase_ = phase_ * new_period / period_;
    period_ = new_period;
  }
};

// FIXME: I had the sine wave generators inherit from Phased because
//   I wanted to learn how inheritance works in C++, not because it
//   makes any sense at all; replace this with
//   with composition and/or virtual interfaces.

class SineWaveGenerator : public Phased {
 public:
  explicit SineWaveGenerator(const unsigned period_ticks, const int32_t volume)
      : Phased(period_ticks), volume_(volume) {
    _phase_factor = _calc_phase_factor();
  }

  int32_t next() {
    const auto phase = Phased::next();
    return static_cast<int32_t>(volume_ * sin(_phase_factor * phase));
  }

  void set_volume(const int32_t volume) { volume_ = volume; }

  void set_period(const unsigned new_period) {
    Phased::set_period(new_period);
    _phase_factor = _calc_phase_factor();
  }

  int32_t get_volume() const { return volume_; }

 private:
  int32_t volume_;
  float _phase_factor;

  unsigned _calc_phase_factor() const {
    return static_cast<unsigned>(M_PI * period_);
  }
};

class SquareWaveGenerator : public Phased {
 public:
  SquareWaveGenerator(const unsigned period_ticks, const int32_t volume)
      : Phased(period_ticks), volume_(volume) {
    _duty_ticks = _calc_duty_ticks();
  }
  SquareWaveGenerator(
      const unsigned period_ticks,
      const int32_t volume,
      const float duty_cycle)
      : Phased(period_ticks), volume_(volume), duty_cycle_(duty_cycle) {
    _duty_ticks = _calc_duty_ticks();
  }

  int32_t next() {
    const auto phase = Phased::next();
    return phase < _duty_ticks ? volume_ : -volume_;
  };

  float duty_cycle() const { return duty_cycle_; }

  void set_duty_cycle(const float new_duty_cycle) {
    if (new_duty_cycle < 0.0 || new_duty_cycle > 1.0)
      throw std::invalid_argument("Duty cycle must be in [0,1].");
    duty_cycle_ = new_duty_cycle;
    _duty_ticks = _calc_duty_ticks();
  }

  void set_period(const unsigned new_period) {
    Phased::set_period(new_period);
    _duty_ticks = _calc_duty_ticks();
  }

  void set_volume(const int32_t volume) { volume_ = volume; }

  int32_t get_volume() const { return volume_; }

 private:
  int32_t volume_;
  float duty_cycle_ = 0.5;
  unsigned _duty_ticks;

  unsigned _calc_duty_ticks() const {
    return static_cast<unsigned>(duty_cycle_ * static_cast<float>(period_));
  }
};
