#pragma once

#include <Windows.h>
#include <cstdint>
#include <cassert>

namespace fb
{
    constexpr uint32_t frostbiteHash(const char* str, size_t len)
    {
        uint32_t hash = 5381;
        for (size_t i = 0; i < len; ++i)
        {
            hash = ((hash << 5) + hash) ^ static_cast<uint32_t>(str[i]);
        }
        return hash;
    }
}

constexpr uint32_t operator"" _fbhash(const char* str, size_t len)
{
    return fb::frostbiteHash(str, len);
}

namespace fb
{
    template<typename T> class WeakToken
    {
    public:
        T* m_RealPtr; //0x0000
    };
    //Size=0x0008

    template<typename T> class ConstWeakPtr
    {
    public:
        WeakToken<T>* m_Token; //0x0000

        T* Get()
        {
            __try
            {

                if (m_Token && m_Token->m_RealPtr)
                {
                    DWORD_PTR realPtr = (DWORD_PTR)m_Token->m_RealPtr;
                    return (T*)(realPtr - sizeof(DWORD_PTR));
                }
            }
            __except (1)
            {
                return NULL;
            }
            return NULL;
        }

        T* Get8()
        {
            __try
            {

                if (m_Token && m_Token->m_RealPtr)
                {
                    DWORD_PTR realPtr = (DWORD_PTR)m_Token->m_RealPtr;
                    return (T*)(realPtr - 8);
                }
            }
            __except (1)
            {
                return NULL;
            }
            return NULL;
        }
    };
    //Size=0x0008

    template<typename T> class WeakPtr : public ConstWeakPtr<T>
    {
    };
    //Size=0x0008

    struct Guid
    {
        unsigned long	m_Data1;	//0x0000
        unsigned short	m_Data2;	//0x0004
        unsigned short	m_Data3;	//0x0006
        unsigned char	m_Data4[8];	//0x0008

        bool operator=(const fb::Guid* other)
        {
            return this->m_Data1 == other->m_Data1
                && *(DWORD*)&this->m_Data2 == *(DWORD*)&other->m_Data2
                && *(DWORD*)this->m_Data4 == *(DWORD*)other->m_Data4
                && *(DWORD*)&this->m_Data4[4] == *(DWORD*)&other->m_Data4[4];
        }
    };
    //Size=0x0010

    struct Color32
    {
        union
        {
            struct
            {
                unsigned char m_R;	//0x0000
                unsigned char m_G;	//0x0001
                unsigned char m_B;	//0x0002
                unsigned char m_A;	//0x0003
            };

            unsigned int m_Data;	//0x0000
        };

        Color32(unsigned char r1, unsigned char g1, unsigned char b1, unsigned char a1)
        {
            m_R = r1;
            m_G = g1;
            m_B = b1;
            m_A = a1;
        }
    };
    //Size=0x0004

    template<class T> class Tuple2
    {
    public:
        T m_Element1;
        T m_Element2;

        Tuple2(T element1, T element2)
        {
            m_Element1 = element1;
            m_Element2 = element2;
        }
    };
    //Size=2*T

    struct ArrayHeader
    {
        uint32_t unk;
        uint32_t size;
    };

    template <typename T>
    class Array
    {
    public:
        T* m_firstElement;

    public:
        Array() : m_firstElement(nullptr)
        {
        }

        ArrayHeader* GetHeader() const
        {
            if (!m_firstElement)
                return nullptr;

            return reinterpret_cast<ArrayHeader*>(
                reinterpret_cast<char*>(m_firstElement) - sizeof(ArrayHeader)
                );
        }

        T& At(INT nIndex)
        {
            return *(T*)((intptr_t)m_firstElement + (nIndex * sizeof(T)));
        }

        uint32_t size() const
        {
            ArrayHeader* header = GetHeader();
            if (!header)
                return 0;

            return header->size;
        }

        T* begin()
        {
            return m_firstElement;
        }

        T* end()
        {
            return m_firstElement + size();
        }

        T& operator[](uint32_t index) { return At(index); }
    };
}