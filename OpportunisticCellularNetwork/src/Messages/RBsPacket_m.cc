//
// Generated file, do not edit! Created by opp_msgtool 6.0 from Messages/RBsPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "RBsPacket_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

namespace opportunisticcellularnetwork {

Register_Class(RBsPacket)

RBsPacket::RBsPacket(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

RBsPacket::RBsPacket(const RBsPacket& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

RBsPacket::~RBsPacket()
{
    delete [] this->arrivalTimes;
}

RBsPacket& RBsPacket::operator=(const RBsPacket& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void RBsPacket::copy(const RBsPacket& other)
{
    this->destinationUser = other.destinationUser;
    this->usedRBs = other.usedRBs;
    this->usedBytes = other.usedBytes;
    delete [] this->arrivalTimes;
    this->arrivalTimes = (other.arrivalTimes_arraysize==0) ? nullptr : new ::omnetpp::simtime_t[other.arrivalTimes_arraysize];
    arrivalTimes_arraysize = other.arrivalTimes_arraysize;
    for (size_t i = 0; i < arrivalTimes_arraysize; i++) {
        this->arrivalTimes[i] = other.arrivalTimes[i];
    }
}

void RBsPacket::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->destinationUser);
    doParsimPacking(b,this->usedRBs);
    doParsimPacking(b,this->usedBytes);
    b->pack(arrivalTimes_arraysize);
    doParsimArrayPacking(b,this->arrivalTimes,arrivalTimes_arraysize);
}

void RBsPacket::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->destinationUser);
    doParsimUnpacking(b,this->usedRBs);
    doParsimUnpacking(b,this->usedBytes);
    delete [] this->arrivalTimes;
    b->unpack(arrivalTimes_arraysize);
    if (arrivalTimes_arraysize == 0) {
        this->arrivalTimes = nullptr;
    } else {
        this->arrivalTimes = new ::omnetpp::simtime_t[arrivalTimes_arraysize];
        doParsimArrayUnpacking(b,this->arrivalTimes,arrivalTimes_arraysize);
    }
}

int RBsPacket::getDestinationUser() const
{
    return this->destinationUser;
}

void RBsPacket::setDestinationUser(int destinationUser)
{
    this->destinationUser = destinationUser;
}

int RBsPacket::getUsedRBs() const
{
    return this->usedRBs;
}

void RBsPacket::setUsedRBs(int usedRBs)
{
    this->usedRBs = usedRBs;
}

int RBsPacket::getUsedBytes() const
{
    return this->usedBytes;
}

void RBsPacket::setUsedBytes(int usedBytes)
{
    this->usedBytes = usedBytes;
}

size_t RBsPacket::getArrivalTimesArraySize() const
{
    return arrivalTimes_arraysize;
}

::omnetpp::simtime_t RBsPacket::getArrivalTimes(size_t k) const
{
    if (k >= arrivalTimes_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)arrivalTimes_arraysize, (unsigned long)k);
    return this->arrivalTimes[k];
}

void RBsPacket::setArrivalTimesArraySize(size_t newSize)
{
    ::omnetpp::simtime_t *arrivalTimes2 = (newSize==0) ? nullptr : new ::omnetpp::simtime_t[newSize];
    size_t minSize = arrivalTimes_arraysize < newSize ? arrivalTimes_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        arrivalTimes2[i] = this->arrivalTimes[i];
    for (size_t i = minSize; i < newSize; i++)
        arrivalTimes2[i] = SIMTIME_ZERO;
    delete [] this->arrivalTimes;
    this->arrivalTimes = arrivalTimes2;
    arrivalTimes_arraysize = newSize;
}

void RBsPacket::setArrivalTimes(size_t k, ::omnetpp::simtime_t arrivalTimes)
{
    if (k >= arrivalTimes_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)arrivalTimes_arraysize, (unsigned long)k);
    this->arrivalTimes[k] = arrivalTimes;
}

void RBsPacket::insertArrivalTimes(size_t k, ::omnetpp::simtime_t arrivalTimes)
{
    if (k > arrivalTimes_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)arrivalTimes_arraysize, (unsigned long)k);
    size_t newSize = arrivalTimes_arraysize + 1;
    ::omnetpp::simtime_t *arrivalTimes2 = new ::omnetpp::simtime_t[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        arrivalTimes2[i] = this->arrivalTimes[i];
    arrivalTimes2[k] = arrivalTimes;
    for (i = k + 1; i < newSize; i++)
        arrivalTimes2[i] = this->arrivalTimes[i-1];
    delete [] this->arrivalTimes;
    this->arrivalTimes = arrivalTimes2;
    arrivalTimes_arraysize = newSize;
}

void RBsPacket::appendArrivalTimes(::omnetpp::simtime_t arrivalTimes)
{
    insertArrivalTimes(arrivalTimes_arraysize, arrivalTimes);
}

void RBsPacket::eraseArrivalTimes(size_t k)
{
    if (k >= arrivalTimes_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)arrivalTimes_arraysize, (unsigned long)k);
    size_t newSize = arrivalTimes_arraysize - 1;
    ::omnetpp::simtime_t *arrivalTimes2 = (newSize == 0) ? nullptr : new ::omnetpp::simtime_t[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        arrivalTimes2[i] = this->arrivalTimes[i];
    for (i = k; i < newSize; i++)
        arrivalTimes2[i] = this->arrivalTimes[i+1];
    delete [] this->arrivalTimes;
    this->arrivalTimes = arrivalTimes2;
    arrivalTimes_arraysize = newSize;
}

class RBsPacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_destinationUser,
        FIELD_usedRBs,
        FIELD_usedBytes,
        FIELD_arrivalTimes,
    };
  public:
    RBsPacketDescriptor();
    virtual ~RBsPacketDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(RBsPacketDescriptor)

RBsPacketDescriptor::RBsPacketDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(opportunisticcellularnetwork::RBsPacket)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

RBsPacketDescriptor::~RBsPacketDescriptor()
{
    delete[] propertyNames;
}

bool RBsPacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<RBsPacket *>(obj)!=nullptr;
}

const char **RBsPacketDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *RBsPacketDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int RBsPacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 4+base->getFieldCount() : 4;
}

unsigned int RBsPacketDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_destinationUser
        FD_ISEDITABLE,    // FIELD_usedRBs
        FD_ISEDITABLE,    // FIELD_usedBytes
        FD_ISARRAY | FD_ISEDITABLE | FD_ISRESIZABLE,    // FIELD_arrivalTimes
    };
    return (field >= 0 && field < 4) ? fieldTypeFlags[field] : 0;
}

const char *RBsPacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "destinationUser",
        "usedRBs",
        "usedBytes",
        "arrivalTimes",
    };
    return (field >= 0 && field < 4) ? fieldNames[field] : nullptr;
}

int RBsPacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "destinationUser") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "usedRBs") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "usedBytes") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "arrivalTimes") == 0) return baseIndex + 3;
    return base ? base->findField(fieldName) : -1;
}

const char *RBsPacketDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_destinationUser
        "int",    // FIELD_usedRBs
        "int",    // FIELD_usedBytes
        "omnetpp::simtime_t",    // FIELD_arrivalTimes
    };
    return (field >= 0 && field < 4) ? fieldTypeStrings[field] : nullptr;
}

const char **RBsPacketDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *RBsPacketDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int RBsPacketDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    RBsPacket *pp = omnetpp::fromAnyPtr<RBsPacket>(object); (void)pp;
    switch (field) {
        case FIELD_arrivalTimes: return pp->getArrivalTimesArraySize();
        default: return 0;
    }
}

void RBsPacketDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    RBsPacket *pp = omnetpp::fromAnyPtr<RBsPacket>(object); (void)pp;
    switch (field) {
        case FIELD_arrivalTimes: pp->setArrivalTimesArraySize(size); break;
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'RBsPacket'", field);
    }
}

const char *RBsPacketDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    RBsPacket *pp = omnetpp::fromAnyPtr<RBsPacket>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string RBsPacketDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    RBsPacket *pp = omnetpp::fromAnyPtr<RBsPacket>(object); (void)pp;
    switch (field) {
        case FIELD_destinationUser: return long2string(pp->getDestinationUser());
        case FIELD_usedRBs: return long2string(pp->getUsedRBs());
        case FIELD_usedBytes: return long2string(pp->getUsedBytes());
        case FIELD_arrivalTimes: return simtime2string(pp->getArrivalTimes(i));
        default: return "";
    }
}

void RBsPacketDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    RBsPacket *pp = omnetpp::fromAnyPtr<RBsPacket>(object); (void)pp;
    switch (field) {
        case FIELD_destinationUser: pp->setDestinationUser(string2long(value)); break;
        case FIELD_usedRBs: pp->setUsedRBs(string2long(value)); break;
        case FIELD_usedBytes: pp->setUsedBytes(string2long(value)); break;
        case FIELD_arrivalTimes: pp->setArrivalTimes(i,string2simtime(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RBsPacket'", field);
    }
}

omnetpp::cValue RBsPacketDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    RBsPacket *pp = omnetpp::fromAnyPtr<RBsPacket>(object); (void)pp;
    switch (field) {
        case FIELD_destinationUser: return pp->getDestinationUser();
        case FIELD_usedRBs: return pp->getUsedRBs();
        case FIELD_usedBytes: return pp->getUsedBytes();
        case FIELD_arrivalTimes: return pp->getArrivalTimes(i).dbl();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'RBsPacket' as cValue -- field index out of range?", field);
    }
}

void RBsPacketDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    RBsPacket *pp = omnetpp::fromAnyPtr<RBsPacket>(object); (void)pp;
    switch (field) {
        case FIELD_destinationUser: pp->setDestinationUser(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_usedRBs: pp->setUsedRBs(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_usedBytes: pp->setUsedBytes(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_arrivalTimes: pp->setArrivalTimes(i,value.doubleValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RBsPacket'", field);
    }
}

const char *RBsPacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr RBsPacketDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    RBsPacket *pp = omnetpp::fromAnyPtr<RBsPacket>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void RBsPacketDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    RBsPacket *pp = omnetpp::fromAnyPtr<RBsPacket>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RBsPacket'", field);
    }
}

}  // namespace opportunisticcellularnetwork

namespace omnetpp {

}  // namespace omnetpp

