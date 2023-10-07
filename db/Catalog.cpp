#include <db/Catalog.h>
#include <db/TupleDesc.h>
#include <stdexcept>

using namespace db;

void Catalog::addTable(DbFile *file, const std::string &name, const std::string &pkeyField) {
    // TODO pa1.2: implement
    int tableId = file->getId();
    Table table(file, name, pkeyField);

    auto it = idToTableMap.find(tableId);
    if(it != idToTableMap.end()) {
        std::string existingTableName = it->second.name;
        nameToIdMap.erase(existingTableName);
    }

    nameToIdMap[name] = tableId;
    if(it != idToTableMap.end()) {
        std::string existingTableName = it->second.name;
        nameToIdMap.erase(existingTableName);
        idToTableMap.erase(it);
    }
    idToTableMap.emplace(tableId, table);
}

int Catalog::getTableId(const std::string &name) const {
    // TODO pa1.2: implement
    auto it = nameToIdMap.find(name);
    if (it == nameToIdMap.end()) {
        throw std::invalid_argument("Table name not found");
    }
    return it->second;
}

const TupleDesc &Catalog::getTupleDesc(int tableid) const {
    // TODO pa1.2: implement
    auto it = idToTableMap.find(tableid);
    if (it == idToTableMap.end()) {
        throw std::runtime_error("Table ID not found");
    }
    return it->second.file->getTupleDesc();
}

DbFile *Catalog::getDatabaseFile(int tableid) const {
    // TODO pa1.2: implement
    auto it = idToTableMap.find(tableid);
    if (it == idToTableMap.end()) {
        throw std::runtime_error("Table ID not found");
    }
    return it->second.file;
}

std::string Catalog::getPrimaryKey(int tableid) const {
    // TODO pa1.2: implement
    auto it = idToTableMap.find(tableid);
    if (it == idToTableMap.end()) {
        throw std::runtime_error("Table ID not found");
    }
    return it->second.pkeyField;
}

std::string Catalog::getTableName(int id) const {
    // TODO pa1.2: implement
    auto it = idToTableMap.find(id);
    if (it == idToTableMap.end()) {
        throw std::runtime_error("Table ID not found");
    }
    return it->second.name;
}

void Catalog::clear() {
    // TODO pa1.2: implement
    nameToIdMap.clear();
    idToTableMap.clear();
}
