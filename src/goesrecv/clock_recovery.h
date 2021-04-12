#pragma once

#include <memory>

#include "sample_publisher.h"
#include "types.h"
/**
 * @brief Performs samples clock recovery
 */
class ClockRecovery {
public:
  /**
   * @brief Public class constructor
   * @param  (int)sampleRate: Sample rate
   * @param  (int)symbolRate: Symbol rate
   */
  explicit ClockRecovery(uint32_t sampleRate, uint32_t symbolRate);
  /**
   * @brief Set bandwidth for the Clock Recovery Loop
   * @param (float)bw : Set loop bandwidth, used to calculate mu and omega gains.
   * @link See http://www.trondeau.com/blog/2011/8/13/control-loop-gain-values.html
   */
  void setLoopBandwidth(float bw);

  /**
  * @brief Returns number of samples per symbol.
  * @returns (float)omega - Samples per symbol
  */
  float getOmega() const {
    return omega_;
  }

  /**
   * @brief Set the sample publisher object
   * @param (std::unique_ptr<SamplePublisher>)samplePublisher: Pointer to a SamplePublisher object
   * @remarks ClockRecovery samples can be consumed through nanomsg by a subscribed consumer
   */
  void setSamplePublisher(std::unique_ptr<SamplePublisher> samplePublisher) {
    samplePublisher_ = std::move(samplePublisher);
  }

  /**
   * @brief Start processing samples
   * @param  (std::shared_ptr<Queue<Samples>>)&qin: Shared pointer to the INPUT queue of samples
   * @param  (std::shared_ptr<Queue<Samples>>)&qout: Shared pointer to the OUTPUT queue of samples
   * @remarks Output queue is automatically closed if Costas loop runs out of samples
   */
  void work(
      const std::shared_ptr<Queue<Samples> >& qin,
      const std::shared_ptr<Queue<Samples> >& qout);

protected:
  float omega_;
  float omegaMin_;
  float omegaMax_;
  float omegaGain_;
  float mu_;
  float muGain_;

  // Past samples
  std::complex<float> p0t_;
  std::complex<float> p1t_;
  std::complex<float> p2t_;

  // Past associated quadrants
  std::complex<float> c0t_;
  std::complex<float> c1t_;
  std::complex<float> c2t_;

  Samples tmp_;

  std::unique_ptr<SamplePublisher> samplePublisher_;
};
