#pragma once

#include <thread>

#include "agc.h"
#include "clock_recovery.h"
#include "config.h"
#include "costas.h"
#include "publisher.h"
#include "quantize.h"
#include "rrc.h"
#include "source.h"
#include "stats_publisher.h"
#include "types.h"

/**
 * Demodulator Class
 * 
 * @brief Demodulate LRIT or HRIT GOES satellite signals.
 * 
 * @remarks
 * The signal is BPSK modulated.
 * Symbol rate is 293883 for LRIT, 927000 for HRIT
 * The signal is pseudorandomized.
 * 
 * The raw I/Q samples from the SDR will neet to go through several 
 * steps to extract usable data:
 * 
 * 1 - Normalization and decimation (Automatic Gain Control)   {agc.h}
 * 2 - Carrier wave recovery (Syncronization by Costas Loop)   {costas.h}
 * 3 - Root Raised Cosine Filter                               {rrc.h}
 * 4 - Sample clock recovery (Mueller and Müller algorithm.)   {clock_recovery.h}
 * 5 - Quantization to convert I/Q samples to softBits         {quantize.h}
 * 
 * The demodulator returns a stream of softBits of signed integers
 * with -127 = 100% probability of the softBit being 0
 *       127 = 100% probability of the softBit being 1
 * 
 * The demodulator publishes statistics through nanomsg
 * by subscribing to tcp://IP:PORT specified in config file
 */
class Demodulator {
public:
  /// @brief Signal type can be LRIT or HRIT
  enum Type {
    LRIT = 1,
    HRIT = 2,
  };

  /**
   * @brief Demodulator class constructor.
   * @param Type: Can be LRIT or HRIT.
   */
  explicit Demodulator(Type t);

  /**
   * @brief Initialize Demodulator object
   * @param (Config)config: Configuration object for the demodulator
   */
  void initialize(Config& config);

  /**
   * @brief Obtain pointer to the softBit queue
   * @return shared pointer to a queue of int8_t vector
   */
  std::shared_ptr<Queue<std::vector<int8_t> > > getSoftBitsQueue() {
    return softBitsQueue_;
  }

  /**
   * @brief Start demodulator
   */
  void start();

  /**
   * @brief Stop demodulator
   */
  void stop();

protected:
  void publishStats();

  uint32_t symbolRate_;
  uint32_t sampleRate_;

  std::unique_ptr<Source> source_;
  std::unique_ptr<StatsPublisher> statsPublisher_;
  std::thread thread_;

  // DSP blocks
  std::unique_ptr<AGC> agc_;
  std::unique_ptr<Costas> costas_;
  std::unique_ptr<RRC> rrc_;
  std::unique_ptr<ClockRecovery> clockRecovery_;
  std::unique_ptr<Quantize> quantization_;

  // Queues
  std::shared_ptr<Queue<Samples> > sourceQueue_;
  std::shared_ptr<Queue<Samples> > agcQueue_;
  std::shared_ptr<Queue<Samples> > costasQueue_;
  std::shared_ptr<Queue<Samples> > rrcQueue_;
  std::shared_ptr<Queue<Samples> > clockRecoveryQueue_;
  std::shared_ptr<Queue<std::vector<int8_t> > > softBitsQueue_;
};
