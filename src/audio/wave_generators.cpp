#include "wave_generators.h"
#include <iostream>
#include <limits>
#include "./constants.h"
/*******************
  Phaser class impl
*******************/
Phaser::Phaser(const unsigned period_ticks) : m_period(period_ticks) {}

unsigned Phaser::next() {
  // TODO: is using integer periods a problem?
  //  Q: Will these discrete periods cause tone aliasing?
  //  1. At high frequencies: might be an issue in theory, if we're anywhere
  //     near the nyquist frequency - but practically I think we're well below
  //     that?
  //  2. At low frequencies: no problems, we have lots of
  //    period-domain resolution there.
  m_phase %= m_period;
  return m_phase++;
}

[[nodiscard]] unsigned Phaser::get_phase() const {
  return m_phase;
}

void Phaser::set_phase(const unsigned new_phase) {
  m_phase = new_phase;
}

[[nodiscard]] unsigned Phaser::get_period() const {
  return m_period;
}

void Phaser::set_period(const unsigned new_period) {
  if (new_period == 0)
    throw std::invalid_argument("Period must be greater than 0");

  m_phase = m_phase * new_period / m_period;
  m_period = new_period;
}

/*******************
  SineWaveGenerator class impl
*******************/
SineWaveGenerator::SineWaveGenerator(
    const unsigned period_ticks,
    const float volume)
    : IWaveGenerator(volume), phaser(Phaser(period_ticks)) {
  m_phase_factor = _calc_phase_factor();
}

Sample SineWaveGenerator::next() {
  const auto phase = this->phaser.next();
  return static_cast<Sample>(
      m_volume * std::numeric_limits<Sample>::max() *
      sin(m_phase_factor * static_cast<float>(phase)));
}

void SineWaveGenerator::set_period(const unsigned new_period) {
  this->phaser.set_period(new_period);
  m_phase_factor = _calc_phase_factor();
}

float SineWaveGenerator::_calc_phase_factor() const {
  return M_PI * this->phaser.get_period();
}

/*******************
  SquareWaveGenerator class impl
*******************/
SquareWaveGenerator::SquareWaveGenerator(
    const unsigned period_ticks,
    const float volume)
    : IWaveGenerator(volume), phaser(Phaser(period_ticks)) {
  m_duty_subperiod = _calc_duty_period();
}

SquareWaveGenerator::SquareWaveGenerator(
    const unsigned period_ticks,
    const float volume,
    const float duty_cycle)
    : IWaveGenerator(volume),
      duty_cycle_(duty_cycle),
      phaser(Phaser(period_ticks)) {
  m_duty_subperiod = _calc_duty_period();
}

Sample SquareWaveGenerator::next() {
  const auto phase = this->phaser.next();
  return static_cast<Sample>(
      phase < m_duty_subperiod ? m_volume * std::numeric_limits<Sample>::max()
                               : m_volume * std::numeric_limits<Sample>::min());
}

void SquareWaveGenerator::set_period(const unsigned new_period) {
  this->phaser.set_period(new_period);
  m_duty_subperiod = _calc_duty_period();
}

[[nodiscard]] float SquareWaveGenerator::get_duty_cycle() const {
  return duty_cycle_;
}

void SquareWaveGenerator::set_duty_cycle(const float new_duty_cycle) {
  if (new_duty_cycle < 0.0 || new_duty_cycle > 1.0)
    throw std::invalid_argument("Duty cycle must be in [0,1].");
  duty_cycle_ = new_duty_cycle;
  m_duty_subperiod = _calc_duty_period();
}

unsigned SquareWaveGenerator::_calc_duty_period() const {
  return static_cast<unsigned>(
      duty_cycle_ * static_cast<float>(this->phaser.get_period()));
}
