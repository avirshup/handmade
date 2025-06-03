#include <math.h>
#include <iostream>

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
  unsigned get_phase() const { return m_phase; }

  void set_phase(const unsigned new_phase) { m_phase = new_phase; }

  /*** period accessors ***/
  unsigned get_period() const { return m_period; }

  void set_period(const unsigned new_period) {
    if (new_period == 0)
      throw std::invalid_argument("Period must be greater than 0");

    m_phase = m_phase * new_period / m_period;
    m_period = new_period;
  }
};

class SineWaveGenerator {
 public:
  Phaser phaser;

  explicit SineWaveGenerator(const unsigned period_ticks, const int32_t volume)
      : phaser(Phaser(period_ticks)), m_volume(volume) {
    m_phase_factor = _calc_phase_factor();
  }

  int32_t next() {
    const auto phase = this->phaser.next();
    return static_cast<int32_t>(m_volume * sin(m_phase_factor * phase));
  }

  void set_volume(const int32_t volume) { m_volume = volume; }

  void set_period(const unsigned new_period) {
    this->phaser.set_period(new_period);
    m_phase_factor = _calc_phase_factor();
  }

  int32_t get_volume() const { return m_volume; }

 private:
  int32_t m_volume;
  float m_phase_factor;

  unsigned _calc_phase_factor() const {
    return static_cast<unsigned>(M_PI * this->phaser.get_period());
  }
};

class SquareWaveGenerator {
  int32_t m_volume;
  float duty_cycle_ = 0.5;
  unsigned m_duty_subperiod;

 public:
  Phaser phaser;

  SquareWaveGenerator(const unsigned period_ticks, const int32_t volume)
      : m_volume(volume), phaser(Phaser(period_ticks)) {
    m_duty_subperiod = _calc_duty_period();
  }
  SquareWaveGenerator(
      const unsigned period_ticks,
      const int32_t volume,
      const float duty_cycle)
      : m_volume(volume),
        duty_cycle_(duty_cycle),
        phaser(Phaser(period_ticks)) {
    m_duty_subperiod = _calc_duty_period();
  }

  int32_t next() {
    const auto phase = this->phaser.next();
    return phase < m_duty_subperiod ? m_volume : -m_volume;
  };

  /*** duty cycle accessors ***/
  float get_duty_cycle() const { return m_duty_subperiod; }

  void set_duty_cycle(const float new_duty_cycle) {
    if (new_duty_cycle < 0.0 || new_duty_cycle > 1.0)
      throw std::invalid_argument("Duty cycle must be in [0,1].");
    duty_cycle_ = new_duty_cycle;
    m_duty_subperiod = _calc_duty_period();
  }

  void set_period(const unsigned new_period) {
    this->phaser.set_period(new_period);
    m_duty_subperiod = _calc_duty_period();
  }

  void set_volume(const int32_t volume) { m_volume = volume; }

  int32_t get_volume() const { return m_volume; }

 private:
  unsigned _calc_duty_period() const {
    return static_cast<unsigned>(
        duty_cycle_ * static_cast<float>(this->phaser.get_period()));
  }
};
