#pragma once
#include "./constants.h"



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
  float m_volume = 0.5;
};

