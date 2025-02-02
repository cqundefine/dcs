#include "Util.h"

#include <fstream>
#include <sstream>

#include "Ensure.h"

namespace DCS
{

std::string read_file_to_string(const std::filesystem::path& path)
{
    std::ifstream file_stream{path};
    DCS::ensure(file_stream.is_open(), "failed to open file: {}", path.string());

    std::ostringstream string_stream{};
    string_stream << file_stream.rdbuf();

    return string_stream.str();
}

}
