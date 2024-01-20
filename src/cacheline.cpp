#include "cacheline.h"
#include <fstream>

#include "version.h"

namespace cacheline {

std::fstream file;
std::filebuf buf;
void function() {}
std::string Cacheline::get_version() {
    return "Version: " + version() + " - Commit: " + git_commit_hash();
}

} // namespace cacheline
