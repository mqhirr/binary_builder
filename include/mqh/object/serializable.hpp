/*
    made by mqhirr
    pls dont remove this kthxbai
*/
#ifndef MQH_OBJECT_SERIALIZABLE_HPP
#define MQH_OBJECT_SERIALIZABLE_HPP

// #include <mqh/detail/is_builder.hpp>
#include <mqh/object/builder_impl.hpp>

namespace mqh::object {
/// @brief makes objects serializable (if implemented)
template <typename DerivedBuilder
          //, std::enable_if_t<detail::is_builder_v<DerivedBuilder>, bool> =
          // true
          >
class Serializable {
 public:
  virtual void Serialize(IBinaryBuilder<DerivedBuilder>& builder) = 0;
  virtual void Deserialize(IBinaryBuilder<DerivedBuilder>& builder) = 0;
};
}  // namespace mqh::object

#endif  // MQH_OBJECT_SERIALIZABLE_HPP