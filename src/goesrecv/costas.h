#pragma once

#include <memory>

#include "sample_publisher.h"
#include "types.h"
/**
 * @brief Costas Loop to perform carrier wave discovery
 */
class Costas {
public:
  /**
   * @brief Public class constructor 
   */
  explicit Costas();

  /**
   * @brief Set maximum frequency deviation in radians per sample (given in Hz)
   * @param (float)maxDeviation 
   */
  void setMaxDeviation(float maxDeviation) {
    maxDeviation_ = maxDeviation;
  }

  /**
   * @brief Returns frequency in radians per sample.
   * @returns (float)frequency 
   */
  float getFrequency() const {
    return freq_;
  }

  /**
   * @brief Set the sample publisher object
   * @param (std::unique_ptr<SamplePublisher>)samplePublisher: Pointer to a SamplePublisher object
   * @remarks Costas samples can be consumed through nanomsg by a subscribed consumer
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
  void work(
      size_t nsamples,
      std::complex<float>* fi,
      std::complex<float>* fo);

  float phase_;
  float freq_;
  float alpha_;
  float beta_;
  float maxDeviation_;

  std::unique_ptr<SamplePublisher> samplePublisher_;
};
