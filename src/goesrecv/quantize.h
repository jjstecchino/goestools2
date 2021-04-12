#pragma once

#include <memory>

#include "soft_bit_publisher.h"
#include "types.h"
/**
 * @brief Perform quantization of complex floats into softBits
 */
class Quantize {
public:
  explicit Quantize();

  /**
   * @brief Set the sample publisher object
   * @param (std::unique_ptr<SamplePublisher>)samplePublisher: Pointer to a SamplePublisher object
   * @remarks Quantization softBits can be consumed through nanomsg by a subscribed consumer
   */
  void setSoftBitPublisher(std::unique_ptr<SoftBitPublisher> softBitPublisher) {
    softBitPublisher_ = std::move(softBitPublisher);
  }

  /**
   * @brief Start processing samples
   * @param  (std::shared_ptr<Queue<sSamples>>)&qin: Shared pointer to the INPUT queue of vector of complex floats
   * @param  (std::shared_ptr<Queue<std::vector<int8_t>>>)&qout: Shared pointer to the OUTPUT queue of vector of softBits
   * @remarks Output queue is automatically closed if Quantization loop runs out of samples
   */
  void work(
      const std::shared_ptr<Queue<Samples > >& qin,
      const std::shared_ptr<Queue<std::vector<int8_t> > >& qout);

protected:
  std::unique_ptr<SoftBitPublisher> softBitPublisher_;
};
