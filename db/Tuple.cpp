#include <db/Tuple.h>

using namespace db;

//
// Tuple
//


// TODO pa1.1: implement
Tuple::Tuple(const TupleDesc &td, RecordId *rid)
        : tupleDesc(td), recordId(rid) {
    fields.resize(td.numFields(), nullptr);
}

const TupleDesc &Tuple::getTupleDesc() const {
    // TODO pa1.1: implement
    return tupleDesc;
}

const RecordId *Tuple::getRecordId() const {
    // TODO pa1.1: implement
    return recordId;
}

void Tuple::setRecordId(const RecordId *id) {
    // TODO pa1.1: implement
    recordId = id;
}

const Field &Tuple::getField(int i) const {
    // TODO pa1.1: implement
    if (i >= 0 && i < fields.size()) {
        return *fields[i];
    }
    throw std::out_of_range("Index out of bounds");
}

void Tuple::setField(int i, const Field *f) {
    // TODO pa1.1: implement
    if (i >= 0 && i < fields.size()) {
        fields[i] = f;
    } else {
        throw std::out_of_range("Index out of bounds");
    }
}

Tuple::iterator Tuple::begin() const {
    // TODO pa1.1: implement
    return fields.begin();

}

Tuple::iterator Tuple::end() const {
    // TODO pa1.1: implement
    return fields.end();
}

std::string Tuple::to_string() const {
    // TODO pa1.1: implement
    std::string result = "{";
    for (const auto& fieldPtr : fields) {
        if (fieldPtr) {
            result += fieldPtr->to_string() + ", ";
        } else {
            result += "NULL, ";
        }
    }
    if (result.length() > 1) {
        result.pop_back();  // Remove the trailing ", "
        result.pop_back();
    }
    result += "}";
    return result;
}
