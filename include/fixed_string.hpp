#include <algorithm>
#include <string_view>

namespace nomos::consts
{
template <size_t N>
class FixedString
{
  char m_data[N]{};
  constexpr FixedString(const char (&str)[N]) noexcept
  {
    std::copy_n(str, N, m_data);
  }
  constexpr std::string_view view() const noexcept
  {
    return {m_data, N - 1};
  }
};
} // namespace nomos::consts
