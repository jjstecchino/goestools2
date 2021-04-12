#pragma once

#include <array>
#include <memory>

#include "sample_publisher.h"
#include "types.h"
/**
 * Performs Root Raised Cosine Filter 
 */
class RRC {
public:
  static constexpr size_t NTAPS = 31;

  /**
   * @brief Public class constructor
   * @param  (int)df: Decimation factor
   * @param  (int)sampleRate: Sample rate
   * @param  (int)symbolRate: Symbol rate
   */
  explicit RRC(int df, int sampleRate, int symbolRate);

  /**
   * @brief Set the sample publisher object
   * @param (std::unique_ptr<SamplePublisher>)samplePublisher: Pointer to a SamplePublisher object
   * @remarks RRC samples can be consumed through nanomsg by a subscribed consumer
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

  int decimation_;
  std::vector<float> taps_;

  Samples tmp_;

  std::unique_ptr<SamplePublisher> samplePublisher_;
};
