#pragma once

#include <memory>

#include "sample_publisher.h"
#include "types.h"

/**
 * @brief Performs Automatic Gain Control for samples normalization
 */
class AGC {
public:
  /**
   * @brief Public class constructor 
   */
  explicit AGC();

  /**
   * @brief Set minimum gain
   * @param (float)minimumGain 
   */
  void setMin(float min) {
    min_ = min;
  }

  /**
   * @brief Set maximum gain
   * @param (float)maximumGain 
   */
  void setMax(float max) {
    max_ = max;
  }

  /**
   * @brief Set the sample publisher object
   * @param (std::unique_ptr<SamplePublisher>)samplePublisher: Pointer to a SamplePublisher object
   * @remarks AGC samples can be consumed through nanomsg by a subscribed consumer
   */
  void setSamplePublisher(std::unique_ptr<SamplePublisher> samplePublisher) {
    samplePublisher_ = std::move(samplePublisher);
  }
  /**
   * @brief Returns the current gain
   * @returns (float)gain 
   */
  float getGain() const {
    return gain_;
  }

  /**
   * @brief Start processing samples
   * @param  (std::shared_ptr<Queue<Samples>>)&qin: Shared pointer to the INPUT queue of samples
   * @param  (std::shared_ptr<Queue<Samples>>)&qout: Shared pointer to the OUTPUT queue of samples
   * @remarks Output queue is automatically closed if AGC runs out of samples
   */
  void work(
      const std::shared_ptr<Queue<Samples> >& qin,
      const std::shared_ptr<Queue<Samples> >& qout);

protected:
  void work(
      size_t nsamples,
      std::complex<float>* fi,
      std::complex<float>* fo);

  float min_;
  float max_;
  float gain_;
  float alpha_;

  std::unique_ptr<SamplePublisher> samplePublisher_;
};
