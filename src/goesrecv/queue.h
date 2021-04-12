#pragma once

#include <deque>
#include <memory>
#include <mutex>
#include <condition_variable>

#include <util/error.h>
/**
 * @brief Thread safe Queue template class
 * 
 * @remarks
 * Structured as a read and write queue
 * Elements for write are obtained from the write queue,
 * once written are returned to the read queue.
 * Elements for read are obtained from the read queue,
 * once read are returned to the write queue
 */
template <class T>
class Queue {
public:
  /**
  * @brief Public constructor 
  * @param (size_t)capacity: Queue depth
  */
  Queue(size_t capacity) :
      elements_(0),
      capacity_(capacity),
      closed_(false) {
  }
  /**
   * @brief Returns the queue capacity.
   * @returns (size_t)value
   */
  size_t size() {
    std::unique_lock<std::mutex> lock(m_);
    return elements_;
  }
  /**
   * @brief Returns true if queue is closed
   * @returns (bool)value
   */
  bool closed() {
    std::unique_lock<std::mutex> lock(m_);
    return closed_;
  }
  /**
   * @brief  Close the queue
   */
  void close() {
    std::unique_lock<std::mutex> lock(m_);
    closed_ = true;
    cv_.notify_one();
  }

  /**
  * @brief Returns a unique pointer to an element to write to
  * @remarks Once written pointer needs to be returned with pushWrite()
  * @returns (std::unique_ptr<T>) ptr
  */
  std::unique_ptr<T> popForWrite() {
    std::unique_lock<std::mutex> lock(m_);
    ASSERT(!closed_);

    // Ensure there is an item to return
    if (write_.size() == 0) {
      if (elements_ < capacity_) {
        elements_++;
        write_.push_back(std::make_unique<T>());
      } else {
        // Wait until pushRead makes an item available
        while (write_.size() == 0) {
          cv_.wait(lock);
        }
      }
    }

    auto v = std::move(write_.front());
    write_.pop_front();
    return v;
  }
  /**
  * @brief Returns a unique pointer of written element to read queue
  * @param  (std::unique_ptr<T>)ptr: pointer to the written item
  */
  void pushWrite(std::unique_ptr<T> v) {
    std::unique_lock<std::mutex> lock(m_);
    ASSERT(!closed_);

    read_.push_back(std::move(v));
    cv_.notify_one();
  }

  /**
  * @brief Returns a unique pointer to an existing element to read from
  * @remarks Once written pointer needs to be returned with pushRead()
  * @returns (std::unique_ptr<T>) ptr
  */
  std::unique_ptr<T> popForRead() {
    std::unique_lock<std::mutex> lock(m_);
    while (read_.size() == 0 && !closed_) {
      cv_.wait(lock);
    }

    // Allow read side to drain
    if (read_.size() == 0 && closed_) {
      return std::unique_ptr<T>(nullptr);
    }

    auto v = std::move(read_.front());
    read_.pop_front();
    return v;
  }

  /**
  * @brief Returns a unique pointer of read element to write queue
  * @param  (std::unique_ptr<T>)ptr: pointer to the read item
  */
  void pushRead(std::unique_ptr<T> v) {
    std::unique_lock<std::mutex> lock(m_);
    if (!closed_) {
      write_.push_back(std::move(v));
      cv_.notify_one();
    }
  }

protected:
  std::mutex m_;
  std::condition_variable cv_;

  size_t elements_;
  size_t capacity_;
  bool closed_;

  std::deque<std::unique_ptr<T> > write_;
  std::deque<std::unique_ptr<T> > read_;
};
