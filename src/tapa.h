#ifndef TASK_LEVEL_PARALLELIZATION_H_
#define TASK_LEVEL_PARALLELIZATION_H_

#ifdef __SYNTHESIS__
#error this header is not synthesizable
#endif  // __SYNTHESIS__

#include <climits>
#include <cstdarg>
#include <cstdint>
#include <cstdlib>

#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include <glog/logging.h>

#include "tapa/mmap.h"
#include "tapa/stream.h"
#include "tapa/synthesizable/traits.h"
#include "tapa/synthesizable/util.h"
#include "tapa/synthesizable/vec.h"

namespace tapa {

struct seq {};

struct task {
  task();
  task(task&&) = delete;
  task(const task&) = delete;
  ~task();

  task& operator=(task&&) = delete;
  task& operator=(const task&) = delete;

  template <int step, typename Function, typename... Args>
  task& invoke(Function&& f, Args&&... args) {
    return invoke<step>(f, "", args...);
  }

  template <int step, typename Function, typename... Args, size_t S>
  task& invoke(Function&& f, const char (&name)[S], Args&&... args) {
    schedule(/* detach= */ step < 0, std::bind(f, std::forward<Args>(args)...));
    return *this;
  }

  // invoke task vector without a name
  template <int step, uint64_t length, typename Function, typename... Args>
  task& invoke(Function&& f, Args&&... args) {
    return invoke<step, length>(f, "", args...);
  }

  // invoke task vector with a name
  template <int step, uint64_t length, typename Function, typename... Args,
            size_t S>
  task& invoke(Function&& f, const char (&name)[S], Args&&... args) {
    for (uint64_t i = 0; i < length; ++i) {
      this->invoke<step>(f, access(args, i)...);
    }
    return *this;
  }

 private:
  // scalar
  template <typename T>
  static T& access(T& arg, uint64_t idx) {
    return arg;
  }

  // sequence
  static int access(seq, uint64_t idx) { return idx; }

  // access streams in vector invoke
  template <typename T, uint64_t length, uint64_t depth>
  static stream<T, depth>& access(streams<T, length, depth>& arg,
                                  uint64_t idx) {
    LOG_IF(INFO, idx >= length) << "invocation #" << idx << " accesses "
                                << "stream #" << idx % length;
    return arg[idx];
  }

  // access async_mmaps in vector invoke
  template <typename T, uint64_t length>
  static async_mmap<T>& access(async_mmaps<T, length>& arg, uint64_t idx) {
    LOG_IF(INFO, idx >= length) << "invocation #" << idx << " accesses "
                                << "async_mmap #" << idx % length;
    return arg[idx % length];
  }

  void schedule(bool detach, const std::function<void()>&);
};

template <typename T, uint64_t N>
inline std::ostream& operator<<(std::ostream& os, const vec_t<T, N>& obj) {
  os << "{";
  for (uint64_t i = 0; i < N; ++i) {
    if (i > 0) os << ", ";
    os << "[" << i << "]: " << obj[i];
  }
  return os << "}";
}

namespace internal {
void* allocate(size_t length);
void deallocate(void* addr, size_t length);
}  // namespace internal

template <typename T>
struct aligned_allocator {
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  T* allocate(size_t count) {
    return reinterpret_cast<T*>(internal::allocate(count * sizeof(T)));
  }
  void deallocate(T* ptr, std::size_t count) {
    internal::deallocate(ptr, count * sizeof(T));
  }
};

}  // namespace tapa

#endif  // TASK_LEVEL_PARALLELIZATION_H_
