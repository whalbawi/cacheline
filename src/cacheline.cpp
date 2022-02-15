#include "cacheline.h"

#include "version.h"

namespace cacheline {

std::string Cacheline::get_version() {
    return "Version: " + version() + " - Commit: " + git_commit_hash();
}

} // namespace cacheline
