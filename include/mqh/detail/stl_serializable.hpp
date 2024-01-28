/*
    made by mqhirr
    pls dont remove this kthxbai
*/
#if defined(MQH_BINARY_BUILDER_SUPPORT_STL) && \
    MQH_BINARY_BUILDER_SUPPORT_STL == ON
#ifndef MQH_DETAIL_STL_SERLIAZBLE_HPP
#define MQH_DETAIL_STL_SERLIAZBLE_HPP

#include <cstring>
#include <mqh/object/serializable.hpp>
#include <string>
#include <vector>

namespace mqh::detail {
template <typename DerivedBuilder>
using STLSerializableObject = object::Serializable<DerivedBuilder>;

template <typename DerivedBuilder>
using STLBuilder = object::IBinaryBuilder<DerivedBuilder>;

template <typename LengthValue, typename DerivedBuilder,
          std::enable_if_t<std::is_arithmetic_v<LengthValue>, bool> = true>
class String : public STLSerializableObject<DerivedBuilder>,
               public std::basic_string<char> {
 public:
  String() = default;
  //   String(std::string str) : std::string(str) {}
  String(const std::string& str) : std::string(str) {}

 public:
  void Serialize(STLBuilder<DerivedBuilder>& builder) override {
    LengthValue l = length();
    builder << l;
    builder.WriteBytes(reinterpret_cast<uint8_t*>(data()), l);
  }

  void Deserialize(STLBuilder<DerivedBuilder>& builder) override {
    LengthValue length = (LengthValue)-1;
    builder >> length;

    this->resize(length);

    uint8_t* bytes = builder.ReadBytes(length);
    memcpy(data(), bytes, length);

    free(bytes);
    bytes = nullptr;
  }
};

template <typename LengthValue, typename T, typename DerivedBuilder,
          std::enable_if_t<std::is_arithmetic_v<T>, bool> = true,
          std::enable_if_t<std::is_arithmetic_v<LengthValue>, bool> = true>
class Vector : public STLSerializableObject<DerivedBuilder>,
               public std::vector<T> {
 public:
  Vector() = default;
  Vector(const std::vector<T>& v) : std::vector<T>(v) {}
  Vector(std::vector<T>& v) : std::vector<T>(v) {}

 public:
  void Serialize(STLBuilder<DerivedBuilder>& builder) override {
    builder << static_cast<LengthValue>(this->size());
    builder.WriteBytes(reinterpret_cast<uint8_t*>(this->data()),
                       this->size() * sizeof(T));
  }

  void Deserialize(STLBuilder<DerivedBuilder>& builder) override {
    LengthValue length = (LengthValue)-1;
    builder >> length;

    this->resize(length);

    uint8_t* bytes = builder.ReadBytes(length * sizeof(T));
    memcpy(this->data(), bytes, length * sizeof(T));

    free(bytes);
    bytes = nullptr;
  }
};
}  // namespace mqh::detail

#endif  // MQH_DETAIL_STL_SERLIAZBLE_HPP
#endif  // defined(MQH_BINARY_BUILDER_SUPPORT_STL) &&
        // MQH_BINARY_BUILDER_SUPPORT_STL == ON