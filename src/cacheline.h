#ifndef CACHELINE_CACHELINE_H_
#define CACHELINE_CACHELINE_H_

#include <string>

namespace cacheline {

class Cacheline {
  public:
    static std::string get_version();
};

} // namespace cacheline

#endif // CACHELINE_CACHELINE_H_
