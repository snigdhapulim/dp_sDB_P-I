#include <db/TupleDesc.h>
#include <functional> // for std::hash

using namespace db;

//
// TDItem
//

bool TDItem::operator==(const TDItem &other) const {
    // TODO pa1.1: implement
    return fieldType == other.fieldType && fieldName == other.fieldName;
}

std::size_t std::hash<TDItem>::operator()(const TDItem &r) const {
    // TODO pa1.1: implement
    std::hash<std::string> stringHasher;
    return r.fieldType + 31 * stringHasher(r.fieldName);
}

//
// TupleDesc
//

// TODO pa1.1: implement
TupleDesc::TupleDesc(const std::vector<Types::Type> &types) {
    for (const auto &type : types) {
        items.emplace_back(type, "");
    }
}

// TODO pa1.1: implement
TupleDesc::TupleDesc(const std::vector<Types::Type> &types, const std::vector<std::string> &names) {
    if (types.size() != names.size()) {
        throw std::invalid_argument("Mismatch between types and names lengths");
    }
    for (size_t i = 0; i < types.size(); ++i) {
        items.emplace_back(types[i], names[i]);
    }
}

size_t TupleDesc::numFields() const {
    // TODO pa1.1: implement
    return items.size();
}

std::string TupleDesc::getFieldName(size_t i) const {
    // TODO pa1.1: implement
    if (i >= items.size()) throw std::out_of_range("Index out of range");
    return items[i].fieldName;
}

Types::Type TupleDesc::getFieldType(size_t i) const {
    // TODO pa1.1: implement
    if (i >= items.size()) throw std::out_of_range("Index out of range");
    return items[i].fieldType;
}

int TupleDesc::fieldNameToIndex(const std::string &fieldName) const {
    // TODO pa1.1: implement
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i].fieldName == fieldName) return static_cast<int>(i);
    }
    throw std::invalid_argument ("The "+fieldName +" does not exist"); // not found
}

size_t TupleDesc::getSize() const {
    // TODO pa1.1: implement
    size_t size = 0;
    for (const auto &item : items) {
        size += Types::getLen(item.fieldType);
    }
    return size;
}

TupleDesc TupleDesc::merge(const TupleDesc &td1, const TupleDesc &td2) {
    // TODO pa1.1: implement
    std::vector<Types::Type> mergedTypes;
    std::vector<std::string> mergedNames;

    for (const auto &item : td1.items) {
        mergedTypes.push_back(item.fieldType);
        mergedNames.push_back(item.fieldName);
    }

    for (const auto &item : td2.items) {
        mergedTypes.push_back(item.fieldType);
        mergedNames.push_back(item.fieldName);
    }

    return TupleDesc(mergedTypes, mergedNames);
}

std::string TupleDesc::to_string() const {
    // TODO pa1.1: implement
    std::string result;
    for (const auto &item : items) {
        if (!result.empty()) result += ", ";
        result += item.to_string();
    }
    return result;
}

bool TupleDesc::operator==(const TupleDesc &other) const {
    // TODO pa1.1: implement
    return items == other.items;
}

TupleDesc::iterator TupleDesc::begin() const {
    // TODO pa1.1: implement
    return items.begin();
}

TupleDesc::iterator TupleDesc::end() const {
    // TODO pa1.1: implement
    return items.end();
}

std::size_t std::hash<db::TupleDesc>::operator()(const db::TupleDesc &td) const {
    // TODO pa1.1: implement
    std::size_t h = 0;
    for (const auto &item : td.items) {
        h = h * 31 + std::hash<TDItem>()(item);
    }
    return h;
}


