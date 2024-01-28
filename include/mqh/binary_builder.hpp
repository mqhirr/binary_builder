/*
    made by mqhirr
    pls dont remove this kthxbai
*/
#ifndef MQH_BINARY_BUILDER_HPP
#define MQH_BINARY_BUILDER_HPP

#include <cstdint>
#include <cstdlib>  // malloc
#include <cstring>  // memset
#include <stdexcept>

#include "object/builder_impl.hpp"
#include "object/serializable.hpp"

#if defined(MQH_BINARY_BUILDER_SUPPORT_STL) && \
    MQH_BINARY_BUILDER_SUPPORT_STL == ON
#include "detail/stl_serializable.hpp"
#endif  // defined(MQH_BINARY_BUILDER_SUPPORT_STL) &&
        // MQH_BINARY_BUILDER_SUPPORT_STL == ON

namespace mqh {
/// @brief Copy mode used for initializer
enum class CopyMode { ReadMode, ExtendMode };

/// @brief Generic Binary Builder.
/// @example ```cpp
/// #include <mqh/binary_builder.hpp>
///
/// int main() {
///     mqh::BinaryBuilder builder{};
///     builder << 12 << 1.f << mqh::String("wow");
///
///     mqh::BinaryBuilder reader{builder};
///     int v = 0;
///
///     reader >> v;
///     return v;
/// }
/// ```
class GenericBinaryBuilder
    : public object::IBinaryBuilder<GenericBinaryBuilder> {
 public:
  GenericBinaryBuilder() : m_memory_position(0), m_capacity(1 << 10) {
    m_data = reinterpret_cast<uint8_t*>(malloc(m_capacity));
    memset(m_data, 0, m_capacity);
  }

  GenericBinaryBuilder(GenericBinaryBuilder& other,
                       CopyMode mode = CopyMode::ReadMode) {
    if (mode == CopyMode::ReadMode) {
      m_data = reinterpret_cast<uint8_t*>(malloc(other.m_memory_position));
      m_capacity = other.m_memory_position;
      memcpy(m_data, other.m_data, other.m_memory_position);
    } else {
      m_data = reinterpret_cast<uint8_t*>(malloc(other.m_capacity));
      m_capacity = other.m_capacity;
      m_memory_position = other.m_memory_position;
      memcpy(m_data, other.m_data, other.m_capacity);
    }
  }

  ~GenericBinaryBuilder() {
    m_capacity = 0;
    m_memory_position = 0;

    free(m_data);
    m_data = nullptr;
  }

 public:
  template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
  void Write(const T& value) {
    uint64_t size_of_t =
        sizeof(T);  // takes longer to use sizeof(T) everywhere for some reason

    if (size_of_t + m_memory_position > m_capacity) {
      m_capacity += (m_memory_position + size_of_t + m_capacity) * 2;
    }

    memcpy(m_data + m_memory_position, &value, size_of_t);
    m_memory_position += size_of_t;
  }

  void Write(object::Serializable<GenericBinaryBuilder>& object) {
    object.Serialize(*this);
  }

  template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
  T Read() {
    uint64_t size_of_t = sizeof(T);
    if (m_memory_position + size_of_t > m_capacity) {
      throw std::runtime_error(
          "[mqh::BinaryBuilder] -> attempted to read out of bounds?");
    }

    m_memory_position += size_of_t;
    return *reinterpret_cast<T*>(
        m_data + (m_memory_position - size_of_t));  // funny black magic
  }

  void Read(object::Serializable<GenericBinaryBuilder>& val) {
    val.Deserialize(*this);
  }

  GenericBinaryBuilder& operator<<(
      object::Serializable<GenericBinaryBuilder>& val) {
    val.Serialize(*this);
    return *this;
  }

  template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
  GenericBinaryBuilder& operator<<(const T& val) {
    Write<T>(val);
    return *this;
  }

  template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
  GenericBinaryBuilder& operator>>(T& val) {
    val = Read<T>();
    return *this;
  }

  GenericBinaryBuilder& operator>>(
      object::Serializable<GenericBinaryBuilder>& val) {
    val.Deserialize(*this);
    return *this;
  }

  void WriteBytes(uint8_t* data, uint64_t size_of_data) override {
    if (size_of_data + m_memory_position > m_capacity) {
      m_capacity += (m_memory_position + size_of_data + m_capacity) * 2;
    }

    memcpy(m_data + m_memory_position, data, size_of_data);
    m_memory_position += size_of_data;
  }

  uint8_t* ReadBytes(uint64_t size_of_data) override {
    if (m_memory_position + size_of_data > m_capacity) {
      throw std::runtime_error(
          "[mqh::BinaryBuilder] -> attempted to read out of bounds?");
    }

    uint8_t* data = reinterpret_cast<uint8_t*>(malloc(size_of_data));
    memcpy(data, m_data + m_memory_position, size_of_data);

    m_memory_position += size_of_data;

    return data;
  }

  const uint8_t* get() const override {
    uint8_t* v = reinterpret_cast<uint8_t*>(malloc(m_memory_position));
    memcpy(v, m_data, m_memory_position);

    return v;
  }

 private:
  uint8_t* m_data;  // very safe indeed

  uint64_t m_memory_position;
  uint64_t m_capacity;
};

using BinaryBuilder = GenericBinaryBuilder;
using BinaryBuilderObject = object::IBinaryBuilder<BinaryBuilder>;
using SerializableObject = object::Serializable<BinaryBuilder>;

#if defined(MQH_BINARY_BUILDER_SUPPORT_STL) && \
    MQH_BINARY_BUILDER_SUPPORT_STL == ON
/// @brief
/// @param LengthValue type of length value (uint64, uint32 etc... / uint64_t by
/// default)
using String = detail::String<uint64_t, GenericBinaryBuilder>;

/// @brief string with custom length type.
template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
using StringC = detail::String<T, GenericBinaryBuilder>;

using StringU32 = detail::String<uint32_t, GenericBinaryBuilder>;
using StringU16 = detail::String<uint16_t, GenericBinaryBuilder>;

/// @brief
/// @param T type of element
/// @param LengthValue type of length value (uint64, uint32 etc... / uint64_t by
/// default)
/// @example mqh::Vector<T>(<your vector goes here>)
template <typename T, typename LengthValue = uint64_t>
using Vector = detail::Vector<LengthValue, T, GenericBinaryBuilder>;
#endif  // defined(MQH_BINARY_BUILDER_SUPPORT_STL) &&
        // MQH_BINARY_BUILDER_SUPPORT_STL == ON
}  // namespace mqh

#endif  // MQH_BINARY_BUILDER_HPP