#include <algorithm>
#include <string_view>

namespace nomos::consts
{
template <size_t N>
class FixedString
{
public:
  char m_data[N]{};
  constexpr FixedString(const char (&str)[N]) noexcept
  {
    std::copy_n(str, N, m_data);
  }
  constexpr std::string_view view() const noexcept
  {
    return {m_data, N - 1};
  }

  static constexpr std::string_view extract_param_name(std::string_view str) noexcept
  {
    size_t colon_idx = str.find(':');
    if (colon_idx == std::string_view::npos)
      return "";

    std::string_view sub = str.substr(colon_idx + 1);
    size_t slash_idx = sub.find('/');
    if (slash_idx == std::string_view::npos)
      return sub;

    return sub.substr(0, slash_idx);
  }
};
template <FixedString Path>
struct Param
{
  template <FixedString Key>
  auto get() const noexcept
  {
    constexpr auto expected_param = extract_param_name(Path.view());
    static_assert(Key.view() == expected_param, "Route does not contain this parameter!");

    return "expected_value";
  };
};
} // namespace nomos::consts
