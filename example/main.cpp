#include <mqh/object/builder_impl.hpp>
#include <mqh/binary_builder.hpp>

class ExampleObject : public mqh::SerializableObject {
public:
    uint16_t b{};

public:
    ExampleObject(uint16_t t) : b(t) {}

    void Serialize(mqh::BinaryBuilderObject& obj) override {
        obj << b;
    }

    void Deserialize(mqh::BinaryBuilderObject& obj) override {
        obj >> b;
    }
};

int main() {
    mqh::BinaryBuilder b{};

    int b2 = 12;
    mqh::StringC<int16_t> a("abc");
    mqh::Vector<int> v({
        0, 1, 2
    });

    ExampleObject o{7};

    b << b2;
    b << a;
    b << v;
    b << o;

    mqh::BinaryBuilder s{b};

    int r = -1;
    s >> r;

    mqh::StringC<int16_t> f{};

    s >> f;

    mqh::Vector<int> t;

    s >> t;

    ExampleObject g{0};
    s >> g;

    std::printf("%s\n", f.c_str());

    return t[1] + g.b;
}