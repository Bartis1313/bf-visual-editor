#include "typeinfo.h"

namespace fb
{

    BasicTypesEnum MemberInfo::GetTypeCode()
    {
        fb::MemberInfo::MemberInfoData* memberInfoData = GetMemberInfoData();
        if (memberInfoData)
        {
            return (BasicTypesEnum)((memberInfoData->m_Flags.m_FlagBits & 0x1F0) >> 0x4);
        }
        return kTypeCode_BasicTypeCount;
    }


    std::string MemberInfo::GetTypeName()
    {
        switch (GetTypeCode())
        {
        case kTypeCode_Void: return "Void";
        case kTypeCode_DbObject: return "DbObject";
        case kTypeCode_ValueType: return "ValueType";
        case kTypeCode_Class: return "Class";
        case kTypeCode_Array: return "Array";
        case kTypeCode_FixedArray: return "FixedArray";
        case kTypeCode_String: return "String";
        case kTypeCode_CString: return "CString";
        case kTypeCode_Enum: return "Enum";
        case kTypeCode_FileRef: return "FileRef";
        case kTypeCode_Boolean: return "Boolean";
        case kTypeCode_Int8: return "Int8";
        case kTypeCode_Uint8: return "Uint8";
        case kTypeCode_Int16: return "Int16";
        case kTypeCode_Uint16: return "Uint16";
        case kTypeCode_Int32: return "Int32";
        case kTypeCode_Uint32: return "Uint32";
        case kTypeCode_Int64: return "Int64";
        case kTypeCode_Uint64: return "Uint64";
        case kTypeCode_Float32: return "Float32";
        case kTypeCode_Float64: return "Float64";
        case kTypeCode_Guid: return "Guid";
        case kTypeCode_SHA1: return "SHA1";
        case kTypeCode_ResourceRef: return "ResourceRef";
        default:
            char buffer[32];
            sprintf_s(buffer, "Undefined[%i]", GetTypeCode());
            return buffer;
        }
    }


    MemberInfo::MemberInfoData* MemberInfo::GetMemberInfoData()
    {
        return ((MemberInfoData*)m_InfoData);
    }


    TypeInfo::TypeInfoData* TypeInfo::GetTypeInfoData()
    {
        return ((TypeInfoData*)m_InfoData);
    }


    FieldInfo::FieldInfoData* FieldInfo::GetFieldInfoData()
    {
        return ((FieldInfoData*)m_InfoData);
    }


    ClassInfo::ClassInfoData* ClassInfo::GetClassInfoData()
    {
        return ((ClassInfoData*)m_InfoData);
    }


    bool ClassInfo::isSubclassOf(ClassInfo* classptr)
    {
        return this->m_ClassId - (unsigned int)classptr->m_ClassId <= classptr->m_LastSubClassId
            - (unsigned int)classptr->m_ClassId;
    }


    ArrayTypeInfo::ArrayTypeInfoData* ArrayTypeInfo::GetArrayTypeInfoData()
    {
        return ((ArrayTypeInfoData*)m_InfoData);
    }


    EnumFieldInfo::EnumFieldInfoData* EnumFieldInfo::GetEnumInfoData()
    {
        return ((EnumFieldInfoData*)m_InfoData);
    }


    ValueTypeInfo::ValueTypeInfoData* ValueTypeInfo::GetValueInfoData()
    {
        return ((ValueTypeInfoData*)m_InfoData);
    }
}