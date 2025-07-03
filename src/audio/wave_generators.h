#pragma once
#include "./constants.h"

/*******************
  Phaser class decl
*******************/
namespace audio {
class Phaser {
 public:
  explicit Phaser(const unsigned period_ticks);

  unsigned next();

  [[nodiscard]] unsigned get_phase() const;
  void set_phase(const unsigned new_phase);

  [[nodiscard]] unsigned get_period() const;
  void set_period(const unsigned new_period);

 private:
  unsigned m_phase = 0;
  unsigned m_period;
};

/*******************
  IWaveGenerator - wave generator interface
*******************/
class IWaveGenerator {
 public:
  virtual ~IWaveGenerator() = default;
  explicit IWaveGenerator(float const volume) : m_volume(volume) {};

  // virtual functions
  virtual Sample next() = 0;
  virtual void set_period(unsigned new_period) = 0;

  // volume interface
  void set_volume(const float volume) { m_volume = volume; };
  [[nodiscard]] float get_volume() const;

 protected:
  float m_volume = 0.005;  // don't use 1.0, it's VERY loud
};

/*******************
  SineWaveGenerator class decl
*******************/
class SineWaveGenerator final : public IWaveGenerator {
 public:
  Phaser phaser;

  explicit SineWaveGenerator(const unsigned period_ticks, const float volume);
  Sample next() override;
  void set_period(const unsigned new_period) override;

 private:
  float m_phase_factor;
  [[nodiscard]] float _calc_phase_factor() const;
};

/*******************
  SquareWaveGenerator class decl
*******************/
class SquareWaveGenerator final : public IWaveGenerator {
 public:
  Phaser phaser;

  SquareWaveGenerator(const unsigned period_ticks, const float volume);
  SquareWaveGenerator(
      const unsigned period_ticks,
      const float volume,
      const float duty_cycle);
  Sample next() override;
  void set_period(const unsigned new_period) override;

  [[nodiscard]] float get_duty_cycle() const;
  void set_duty_cycle(const float new_duty_cycle);

 private:
  float duty_cycle_ = 0.5;
  unsigned m_duty_subperiod;
  [[nodiscard]] unsigned _calc_duty_period() const;
};
}  // namespace audio