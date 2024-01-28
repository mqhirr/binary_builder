/*
    made by mqhirr
    pls dont remove this kthxbai
*/
#ifndef MQH_OBJECT_BUILDER_IMPL_HPP
#define MQH_OBJECT_BUILDER_IMPL_HPP

#include <cstdint>
#include <type_traits>  // std::is_arithmetic_v etc...

#ifdef MQH_BINARY_BUILDER_STL
#define MQH_BINARY_BUILDER_IMPL_STL
#endif  // MQH_BINARY_BUILDER_STL

namespace mqh::object {
/// @brief Binary Builder interface.
template <typename DerivedBuilder>
class IBinaryBuilder {
 public:
  template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
  void Write(const T& value) {
    static_cast<DerivedBuilder*>(this)->Write(value);
  }

  template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
  T Read() {
    return static_cast<DerivedBuilder*>(this)->Read();
  }

  virtual void WriteBytes(uint8_t* data, uint64_t size_of_data) = 0;
  virtual uint8_t* ReadBytes(uint64_t size_of_data) = 0;

  template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
  IBinaryBuilder& operator<<(const T& value) {
    static_cast<DerivedBuilder*>(this)->Write(value);
    return *this;
  }

  template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
  IBinaryBuilder& operator>>(T& value) {
    value = static_cast<DerivedBuilder*>(this)->template Read<T>();
    return *this;
  }

  virtual const uint8_t* get() const = 0;
};
}  // namespace mqh::object

#endif  // MQH_OBJECT_BUILDER_IMPL_HPP