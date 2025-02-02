#pragma once

#include <filesystem>
#include <string>

namespace DCS
{

template <typename T>
using Own = std::unique_ptr<T>;
template <typename T, typename... Args>
constexpr Own<T> MakeOwn(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Ref = std::shared_ptr<T>;
template <typename T, typename... Args>
constexpr Ref<T> MakeRef(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T, typename Base>
constexpr Ref<T> StaticPointerCast(const Ref<Base>& base)
{
    return std::static_pointer_cast<T>(base);
}

std::string read_file_to_string(const std::filesystem::path& path);

}
