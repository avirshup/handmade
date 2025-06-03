#include "wave_generators.h"
#include <cmath>
#include <iostream>
#include <limits>
#include "./constants.h"

class Phaser {
  unsigned m_phase = 0;
  unsigned m_period;

 public:
  explicit Phaser(const unsigned period_ticks) : m_period(period_ticks) {}

  unsigned next() {
    m_phase %= m_period;
    return m_phase++;
  }

  /*** phase accessors ***/
  // This could also be called "peek"
  [[nodiscard]] unsigned get_phase() const { return m_phase; }

  void set_phase(const unsigned new_phase) { m_phase = new_phase; }

  /*** period accessors ***/
  [[nodiscard]] unsigned get_period() const { return m_period; }

  void set_period(const unsigned new_period) {
    if (new_period == 0)
      throw std::invalid_argument("Period must be greater than 0");

    m_phase = m_phase * new_period / m_period;
    m_period = new_period;
  }
};

class SineWaveGenerator final : public IWaveGenerator {
 public:
  Phaser phaser;

  explicit SineWaveGenerator(const unsigned period_ticks, const float volume)
      : IWaveGenerator(volume), phaser(Phaser(period_ticks)) {
    m_phase_factor = _calc_phase_factor();
  }

  Sample next() override {
    const auto phase = this->phaser.next();
    return static_cast<Sample>(
        m_volume * std::numeric_limits<Sample>::max() *
        sin(m_phase_factor * static_cast<float>(phase)));
  }

  void set_period(const unsigned new_period) override {
    this->phaser.set_period(new_period);
    m_phase_factor = _calc_phase_factor();
  }

 private:
  float m_phase_factor;

  [[nodiscard]] float _calc_phase_factor() const {
    return M_PI * this->phaser.get_period();
  }
};

class SquareWaveGenerator final : public IWaveGenerator {
  float duty_cycle_ = 0.5;
  unsigned m_duty_subperiod;

 public:
  Phaser phaser;

  SquareWaveGenerator(const unsigned period_ticks, const float volume)
      : IWaveGenerator(volume), phaser(Phaser(period_ticks)) {
    m_duty_subperiod = _calc_duty_period();
  }
  SquareWaveGenerator(
      const unsigned period_ticks,
      const float volume,
      const float duty_cycle)
      : IWaveGenerator(volume),
        duty_cycle_(duty_cycle),
        phaser(Phaser(period_ticks)) {
    m_duty_subperiod = _calc_duty_period();
  }

  Sample next() override {
    const auto phase = this->phaser.next();
    return phase < m_duty_subperiod
               ? static_cast<Sample>(
                     m_volume * std::numeric_limits<Sample>::max())
               : static_cast<Sample>(
                     m_volume * std::numeric_limits<Sample>::min());
  }

  void set_period(const unsigned new_period) override {
    this->phaser.set_period(new_period);
    m_duty_subperiod = _calc_duty_period();
  }

  /*** duty cycle accessors ***/
  [[nodiscard]] float get_duty_cycle() const { return duty_cycle_; }

  void set_duty_cycle(const float new_duty_cycle) {
    if (new_duty_cycle < 0.0 || new_duty_cycle > 1.0)
      throw std::invalid_argument("Duty cycle must be in [0,1].");
    duty_cycle_ = new_duty_cycle;
    m_duty_subperiod = _calc_duty_period();
  }

 private:
  [[nodiscard]] unsigned _calc_duty_period() const {
    return static_cast<unsigned>(
        duty_cycle_ * static_cast<float>(this->phaser.get_period()));
  }
};
