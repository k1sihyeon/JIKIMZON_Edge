#ifndef JIKIMZON_ISERIALIZEINTERFACE_H
#define JIKIMZON_ISERIALIZEINTERFACE_H

#define OUT

namespace common
{
    class ISerializeInterface
    {
    public:
        virtual void Serialize(std::vector<uint8_t>& OUT buffer) = 0;
        virtual void Deserialize(std::vector<uint8_t>& buffer) = 0;
        virtual std::vector<uint8_t> Serialize() = 0;
    };
}

#endif // JIKIMZON_ISERIALIZEINTERFACE_H