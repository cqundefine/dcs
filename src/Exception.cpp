#include "Exception.h"

namespace DCS
{

std::string Exception::stacktrace() const
{
    return std::to_string(m_trace);
}

}
