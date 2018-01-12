//
// Generated file, do not edit! Created by nedtool 4.6 from Message.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "Message_m.h"

USING_NAMESPACE


// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




// Template rule for outputting std::vector<T> types
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

Register_Class(Message);

Message::Message(const char *name, int kind) : ::cMessage(name,kind)
{
    this->destination_var = 0;
    this->source_var = 0;
    this->currentRound_var = 0;
}

Message::Message(const Message& other) : ::cMessage(other)
{
    copy(other);
}

Message::~Message()
{
}

Message& Message::operator=(const Message& other)
{
    if (this==&other) return *this;
    ::cMessage::operator=(other);
    copy(other);
    return *this;
}

void Message::copy(const Message& other)
{
    this->destination_var = other.destination_var;
    this->source_var = other.source_var;
    this->currentRound_var = other.currentRound_var;
}

void Message::parsimPack(cCommBuffer *b)
{
    ::cMessage::parsimPack(b);
    doPacking(b,this->destination_var);
    doPacking(b,this->source_var);
    doPacking(b,this->currentRound_var);
}

void Message::parsimUnpack(cCommBuffer *b)
{
    ::cMessage::parsimUnpack(b);
    doUnpacking(b,this->destination_var);
    doUnpacking(b,this->source_var);
    doUnpacking(b,this->currentRound_var);
}

int Message::getDestination() const
{
    return destination_var;
}

void Message::setDestination(int destination)
{
    this->destination_var = destination;
}

int Message::getSource() const
{
    return source_var;
}

void Message::setSource(int source)
{
    this->source_var = source;
}

int Message::getCurrentRound() const
{
    return currentRound_var;
}

void Message::setCurrentRound(int currentRound)
{
    this->currentRound_var = currentRound;
}

class MessageDescriptor : public cClassDescriptor
{
  public:
    MessageDescriptor();
    virtual ~MessageDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(MessageDescriptor);

MessageDescriptor::MessageDescriptor() : cClassDescriptor("Message", "cMessage")
{
}

MessageDescriptor::~MessageDescriptor()
{
}

bool MessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Message *>(obj)!=NULL;
}

const char *MessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int MessageDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *MessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "destination",
        "source",
        "currentRound",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int MessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destination")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "source")==0) return base+1;
    if (fieldName[0]=='c' && strcmp(fieldName, "currentRound")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *MessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int MessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Message *pp = (Message *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string MessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Message *pp = (Message *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getDestination());
        case 1: return long2string(pp->getSource());
        case 2: return long2string(pp->getCurrentRound());
        default: return "";
    }
}

bool MessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Message *pp = (Message *)object; (void)pp;
    switch (field) {
        case 0: pp->setDestination(string2long(value)); return true;
        case 1: pp->setSource(string2long(value)); return true;
        case 2: pp->setCurrentRound(string2long(value)); return true;
        default: return false;
    }
}

const char *MessageDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *MessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Message *pp = (Message *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(TDMASchedule);

TDMASchedule::TDMASchedule(const char *name, int kind) : ::Message(name,kind)
{
    this->frame_var = 0;
}

TDMASchedule::TDMASchedule(const TDMASchedule& other) : ::Message(other)
{
    copy(other);
}

TDMASchedule::~TDMASchedule()
{
}

TDMASchedule& TDMASchedule::operator=(const TDMASchedule& other)
{
    if (this==&other) return *this;
    ::Message::operator=(other);
    copy(other);
    return *this;
}

void TDMASchedule::copy(const TDMASchedule& other)
{
    this->frame_var = other.frame_var;
}

void TDMASchedule::parsimPack(cCommBuffer *b)
{
    ::Message::parsimPack(b);
    doPacking(b,this->frame_var);
}

void TDMASchedule::parsimUnpack(cCommBuffer *b)
{
    ::Message::parsimUnpack(b);
    doUnpacking(b,this->frame_var);
}

int TDMASchedule::getFrame() const
{
    return frame_var;
}

void TDMASchedule::setFrame(int frame)
{
    this->frame_var = frame;
}

class TDMAScheduleDescriptor : public cClassDescriptor
{
  public:
    TDMAScheduleDescriptor();
    virtual ~TDMAScheduleDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(TDMAScheduleDescriptor);

TDMAScheduleDescriptor::TDMAScheduleDescriptor() : cClassDescriptor("TDMASchedule", "Message")
{
}

TDMAScheduleDescriptor::~TDMAScheduleDescriptor()
{
}

bool TDMAScheduleDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<TDMASchedule *>(obj)!=NULL;
}

const char *TDMAScheduleDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int TDMAScheduleDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int TDMAScheduleDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *TDMAScheduleDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "frame",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int TDMAScheduleDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "frame")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *TDMAScheduleDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *TDMAScheduleDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int TDMAScheduleDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    TDMASchedule *pp = (TDMASchedule *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string TDMAScheduleDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    TDMASchedule *pp = (TDMASchedule *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getFrame());
        default: return "";
    }
}

bool TDMAScheduleDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    TDMASchedule *pp = (TDMASchedule *)object; (void)pp;
    switch (field) {
        case 0: pp->setFrame(string2long(value)); return true;
        default: return false;
    }
}

const char *TDMAScheduleDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *TDMAScheduleDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    TDMASchedule *pp = (TDMASchedule *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(DataMessage);

DataMessage::DataMessage(const char *name, int kind) : ::Message(name,kind)
{
    this->data_var = 0;
}

DataMessage::DataMessage(const DataMessage& other) : ::Message(other)
{
    copy(other);
}

DataMessage::~DataMessage()
{
}

DataMessage& DataMessage::operator=(const DataMessage& other)
{
    if (this==&other) return *this;
    ::Message::operator=(other);
    copy(other);
    return *this;
}

void DataMessage::copy(const DataMessage& other)
{
    this->data_var = other.data_var;
}

void DataMessage::parsimPack(cCommBuffer *b)
{
    ::Message::parsimPack(b);
    doPacking(b,this->data_var);
}

void DataMessage::parsimUnpack(cCommBuffer *b)
{
    ::Message::parsimUnpack(b);
    doUnpacking(b,this->data_var);
}

double DataMessage::getData() const
{
    return data_var;
}

void DataMessage::setData(double data)
{
    this->data_var = data;
}

class DataMessageDescriptor : public cClassDescriptor
{
  public:
    DataMessageDescriptor();
    virtual ~DataMessageDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(DataMessageDescriptor);

DataMessageDescriptor::DataMessageDescriptor() : cClassDescriptor("DataMessage", "Message")
{
}

DataMessageDescriptor::~DataMessageDescriptor()
{
}

bool DataMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<DataMessage *>(obj)!=NULL;
}

const char *DataMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int DataMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int DataMessageDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *DataMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "data",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int DataMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='d' && strcmp(fieldName, "data")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *DataMessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "double",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *DataMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int DataMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    DataMessage *pp = (DataMessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string DataMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    DataMessage *pp = (DataMessage *)object; (void)pp;
    switch (field) {
        case 0: return double2string(pp->getData());
        default: return "";
    }
}

bool DataMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    DataMessage *pp = (DataMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setData(string2double(value)); return true;
        default: return false;
    }
}

const char *DataMessageDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *DataMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    DataMessage *pp = (DataMessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(DataToBSEncodeMsg);

DataToBSEncodeMsg::DataToBSEncodeMsg(const char *name, int kind) : ::Message(name,kind)
{
    source_arraysize = 0;
    this->source_var = 0;
    this->code_var = 0;
}

DataToBSEncodeMsg::DataToBSEncodeMsg(const DataToBSEncodeMsg& other) : ::Message(other)
{
    source_arraysize = 0;
    this->source_var = 0;
    copy(other);
}

DataToBSEncodeMsg::~DataToBSEncodeMsg()
{
    delete [] source_var;
}

DataToBSEncodeMsg& DataToBSEncodeMsg::operator=(const DataToBSEncodeMsg& other)
{
    if (this==&other) return *this;
    ::Message::operator=(other);
    copy(other);
    return *this;
}

void DataToBSEncodeMsg::copy(const DataToBSEncodeMsg& other)
{
    delete [] this->source_var;
    this->source_var = (other.source_arraysize==0) ? NULL : new int[other.source_arraysize];
    source_arraysize = other.source_arraysize;
    for (unsigned int i=0; i<source_arraysize; i++)
        this->source_var[i] = other.source_var[i];
    this->code_var = other.code_var;
}

void DataToBSEncodeMsg::parsimPack(cCommBuffer *b)
{
    ::Message::parsimPack(b);
    b->pack(source_arraysize);
    doPacking(b,this->source_var,source_arraysize);
    doPacking(b,this->code_var);
}

void DataToBSEncodeMsg::parsimUnpack(cCommBuffer *b)
{
    ::Message::parsimUnpack(b);
    delete [] this->source_var;
    b->unpack(source_arraysize);
    if (source_arraysize==0) {
        this->source_var = 0;
    } else {
        this->source_var = new int[source_arraysize];
        doUnpacking(b,this->source_var,source_arraysize);
    }
    doUnpacking(b,this->code_var);
}

void DataToBSEncodeMsg::setSourceArraySize(unsigned int size)
{
    int *source_var2 = (size==0) ? NULL : new int[size];
    unsigned int sz = source_arraysize < size ? source_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        source_var2[i] = this->source_var[i];
    for (unsigned int i=sz; i<size; i++)
        source_var2[i] = 0;
    source_arraysize = size;
    delete [] this->source_var;
    this->source_var = source_var2;
}

unsigned int DataToBSEncodeMsg::getSourceArraySize() const
{
    return source_arraysize;
}

int DataToBSEncodeMsg::getSource(unsigned int k) const
{
    if (k>=source_arraysize) throw cRuntimeError("Array of size %d indexed by %d", source_arraysize, k);
    return source_var[k];
}

void DataToBSEncodeMsg::setSource(unsigned int k, int source)
{
    if (k>=source_arraysize) throw cRuntimeError("Array of size %d indexed by %d", source_arraysize, k);
    this->source_var[k] = source;
}

const char * DataToBSEncodeMsg::getCode() const
{
    return code_var.c_str();
}

void DataToBSEncodeMsg::setCode(const char * code)
{
    this->code_var = code;
}

class DataToBSEncodeMsgDescriptor : public cClassDescriptor
{
  public:
    DataToBSEncodeMsgDescriptor();
    virtual ~DataToBSEncodeMsgDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(DataToBSEncodeMsgDescriptor);

DataToBSEncodeMsgDescriptor::DataToBSEncodeMsgDescriptor() : cClassDescriptor("DataToBSEncodeMsg", "Message")
{
}

DataToBSEncodeMsgDescriptor::~DataToBSEncodeMsgDescriptor()
{
}

bool DataToBSEncodeMsgDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<DataToBSEncodeMsg *>(obj)!=NULL;
}

const char *DataToBSEncodeMsgDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int DataToBSEncodeMsgDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int DataToBSEncodeMsgDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *DataToBSEncodeMsgDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "source",
        "code",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int DataToBSEncodeMsgDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "source")==0) return base+0;
    if (fieldName[0]=='c' && strcmp(fieldName, "code")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *DataToBSEncodeMsgDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "string",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *DataToBSEncodeMsgDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int DataToBSEncodeMsgDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    DataToBSEncodeMsg *pp = (DataToBSEncodeMsg *)object; (void)pp;
    switch (field) {
        case 0: return pp->getSourceArraySize();
        default: return 0;
    }
}

std::string DataToBSEncodeMsgDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    DataToBSEncodeMsg *pp = (DataToBSEncodeMsg *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSource(i));
        case 1: return oppstring2string(pp->getCode());
        default: return "";
    }
}

bool DataToBSEncodeMsgDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    DataToBSEncodeMsg *pp = (DataToBSEncodeMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setSource(i,string2long(value)); return true;
        case 1: pp->setCode((value)); return true;
        default: return false;
    }
}

const char *DataToBSEncodeMsgDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *DataToBSEncodeMsgDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    DataToBSEncodeMsg *pp = (DataToBSEncodeMsg *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(StatusInfo);

StatusInfo::StatusInfo(const char *name, int kind) : ::Message(name,kind)
{
    this->energy_var = 0;
}

StatusInfo::StatusInfo(const StatusInfo& other) : ::Message(other)
{
    copy(other);
}

StatusInfo::~StatusInfo()
{
}

StatusInfo& StatusInfo::operator=(const StatusInfo& other)
{
    if (this==&other) return *this;
    ::Message::operator=(other);
    copy(other);
    return *this;
}

void StatusInfo::copy(const StatusInfo& other)
{
    this->energy_var = other.energy_var;
}

void StatusInfo::parsimPack(cCommBuffer *b)
{
    ::Message::parsimPack(b);
    doPacking(b,this->energy_var);
}

void StatusInfo::parsimUnpack(cCommBuffer *b)
{
    ::Message::parsimUnpack(b);
    doUnpacking(b,this->energy_var);
}

double StatusInfo::getEnergy() const
{
    return energy_var;
}

void StatusInfo::setEnergy(double energy)
{
    this->energy_var = energy;
}

class StatusInfoDescriptor : public cClassDescriptor
{
  public:
    StatusInfoDescriptor();
    virtual ~StatusInfoDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(StatusInfoDescriptor);

StatusInfoDescriptor::StatusInfoDescriptor() : cClassDescriptor("StatusInfo", "Message")
{
}

StatusInfoDescriptor::~StatusInfoDescriptor()
{
}

bool StatusInfoDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<StatusInfo *>(obj)!=NULL;
}

const char *StatusInfoDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int StatusInfoDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int StatusInfoDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *StatusInfoDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "energy",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int StatusInfoDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='e' && strcmp(fieldName, "energy")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *StatusInfoDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "double",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *StatusInfoDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int StatusInfoDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    StatusInfo *pp = (StatusInfo *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string StatusInfoDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    StatusInfo *pp = (StatusInfo *)object; (void)pp;
    switch (field) {
        case 0: return double2string(pp->getEnergy());
        default: return "";
    }
}

bool StatusInfoDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    StatusInfo *pp = (StatusInfo *)object; (void)pp;
    switch (field) {
        case 0: pp->setEnergy(string2double(value)); return true;
        default: return false;
    }
}

const char *StatusInfoDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *StatusInfoDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    StatusInfo *pp = (StatusInfo *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(DataToBSMsg);

DataToBSMsg::DataToBSMsg(const char *name, int kind) : ::Message(name,kind)
{
    dataToBS_arraysize = 0;
    this->dataToBS_var = 0;
    this->size_var = 0;
}

DataToBSMsg::DataToBSMsg(const DataToBSMsg& other) : ::Message(other)
{
    dataToBS_arraysize = 0;
    this->dataToBS_var = 0;
    copy(other);
}

DataToBSMsg::~DataToBSMsg()
{
    for (unsigned int i=0; i<dataToBS_arraysize; i++)
        drop(&(this->dataToBS_var[i]));
    delete [] dataToBS_var;
}

DataToBSMsg& DataToBSMsg::operator=(const DataToBSMsg& other)
{
    if (this==&other) return *this;
    ::Message::operator=(other);
    copy(other);
    return *this;
}

void DataToBSMsg::copy(const DataToBSMsg& other)
{
    delete [] this->dataToBS_var;
    this->dataToBS_var = (other.dataToBS_arraysize==0) ? NULL : new DataMessage[other.dataToBS_arraysize];
    dataToBS_arraysize = other.dataToBS_arraysize;
    for (unsigned int i=0; i<dataToBS_arraysize; i++)
    {
        take(&(this->dataToBS_var[i]));
        this->dataToBS_var[i] = other.dataToBS_var[i];
        this->dataToBS_var[i].setName(other.dataToBS_var[i].getName());
    }
    this->size_var = other.size_var;
}

void DataToBSMsg::parsimPack(cCommBuffer *b)
{
    ::Message::parsimPack(b);
    b->pack(dataToBS_arraysize);
    doPacking(b,this->dataToBS_var,dataToBS_arraysize);
    doPacking(b,this->size_var);
}

void DataToBSMsg::parsimUnpack(cCommBuffer *b)
{
    ::Message::parsimUnpack(b);
    delete [] this->dataToBS_var;
    b->unpack(dataToBS_arraysize);
    if (dataToBS_arraysize==0) {
        this->dataToBS_var = 0;
    } else {
        this->dataToBS_var = new DataMessage[dataToBS_arraysize];
        doUnpacking(b,this->dataToBS_var,dataToBS_arraysize);
    }
    doUnpacking(b,this->size_var);
}

void DataToBSMsg::setDataToBSArraySize(unsigned int size)
{
    DataMessage *dataToBS_var2 = (size==0) ? NULL : new DataMessage[size];
    unsigned int sz = dataToBS_arraysize < size ? dataToBS_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        dataToBS_var2[i] = this->dataToBS_var[i];
    for (unsigned int i=sz; i<size; i++)
        take(&(dataToBS_var2[i]));
    dataToBS_arraysize = size;
    delete [] this->dataToBS_var;
    this->dataToBS_var = dataToBS_var2;
}

unsigned int DataToBSMsg::getDataToBSArraySize() const
{
    return dataToBS_arraysize;
}

DataMessage& DataToBSMsg::getDataToBS(unsigned int k)
{
    if (k>=dataToBS_arraysize) throw cRuntimeError("Array of size %d indexed by %d", dataToBS_arraysize, k);
    return dataToBS_var[k];
}

void DataToBSMsg::setDataToBS(unsigned int k, const DataMessage& dataToBS)
{
    if (k>=dataToBS_arraysize) throw cRuntimeError("Array of size %d indexed by %d", dataToBS_arraysize, k);
    this->dataToBS_var[k] = dataToBS;
}

int DataToBSMsg::getSize() const
{
    return size_var;
}

void DataToBSMsg::setSize(int size)
{
    this->size_var = size;
}

class DataToBSMsgDescriptor : public cClassDescriptor
{
  public:
    DataToBSMsgDescriptor();
    virtual ~DataToBSMsgDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(DataToBSMsgDescriptor);

DataToBSMsgDescriptor::DataToBSMsgDescriptor() : cClassDescriptor("DataToBSMsg", "Message")
{
}

DataToBSMsgDescriptor::~DataToBSMsgDescriptor()
{
}

bool DataToBSMsgDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<DataToBSMsg *>(obj)!=NULL;
}

const char *DataToBSMsgDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int DataToBSMsgDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int DataToBSMsgDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISCOMPOUND | FD_ISCOBJECT | FD_ISCOWNEDOBJECT,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *DataToBSMsgDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "dataToBS",
        "size",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int DataToBSMsgDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='d' && strcmp(fieldName, "dataToBS")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "size")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *DataToBSMsgDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "DataMessage",
        "int",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *DataToBSMsgDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int DataToBSMsgDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    DataToBSMsg *pp = (DataToBSMsg *)object; (void)pp;
    switch (field) {
        case 0: return pp->getDataToBSArraySize();
        default: return 0;
    }
}

std::string DataToBSMsgDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    DataToBSMsg *pp = (DataToBSMsg *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getDataToBS(i); return out.str();}
        case 1: return long2string(pp->getSize());
        default: return "";
    }
}

bool DataToBSMsgDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    DataToBSMsg *pp = (DataToBSMsg *)object; (void)pp;
    switch (field) {
        case 1: pp->setSize(string2long(value)); return true;
        default: return false;
    }
}

const char *DataToBSMsgDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0: return opp_typename(typeid(DataMessage));
        default: return NULL;
    };
}

void *DataToBSMsgDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    DataToBSMsg *pp = (DataToBSMsg *)object; (void)pp;
    switch (field) {
        case 0: return (void *)static_cast<cObject *>(&pp->getDataToBS(i)); break;
        default: return NULL;
    }
}


