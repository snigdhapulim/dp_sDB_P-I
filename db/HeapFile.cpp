#include <db/HeapFile.h>
#include <db/TupleDesc.h>
#include <db/Page.h>
#include <db/PageId.h>
#include <db/HeapPage.h>
#include <stdexcept>
#include <sys/stat.h>
#include <fcntl.h>

using namespace db;

//
// HeapFile
//

// TODO pa1.5: implement
HeapFile::HeapFile(const char *fname, const TupleDesc &td) : td(td) {
    filename = std::string(fname);
    file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!file) {
        // Handle file opening error
        throw std::runtime_error("Failed to open the heap file.");
    }
}

int HeapFile::getId() const {
    // TODO pa1.5: implement
    std::hash<std::string> hasher;
    return hasher(filename);

}

const TupleDesc &HeapFile::getTupleDesc() const {
    // TODO pa1.5: implement
    return td;
}

Page *HeapFile::readPage(const PageId &pid) {
    // TODO pa1.5: implement
    std::ifstream file(filename, std::ios::in | std::ios::binary);

    if (!file) {
        throw std::runtime_error("Unable to open the heap file.");
    }

    // Calculate the offset into the file for the desired page
    int offset = pid.pageNumber() * Database::getBufferPool().getPageSize();
    file.seekg(offset);

    // Read the page data into a buffer
    uint8_t *data = new uint8_t[Database::getBufferPool().getPageSize()];
    file.read(reinterpret_cast<char *>(data), Database::getBufferPool().getPageSize());

    if (file.fail()) {
        delete[] data;  // Clean up the buffer if the read failed
        throw std::runtime_error("Failed to read the page from the heap file.");
    }

    file.close();

    // Create a HeapPage object using the read data and return it
    HeapPage *page = new HeapPage(static_cast<const HeapPageId&>(pid), data);
    delete[] data;  // The data is copied by the HeapPage constructor, so we can delete our buffer

    return page;
}

int HeapFile::getNumPages() {
    // TODO pa1.5: implement
    file.open(filename, std::ios::in | std::ios::binary);  // Open the file in binary mode.
    if(!file) {
        throw std::runtime_error("Unable to open the file.");
    }
    file.seekg(0, std::ios::end);
    int numPages = file.tellg() / Database::getBufferPool().getPageSize();
    file.close();  // Close the file after getting the number of pages.
    return numPages;
}

HeapFileIterator HeapFile::begin() const {
    // TODO pa1.5: implement
}

HeapFileIterator HeapFile::end() const {
    // TODO pa1.5: implement
}

//
// HeapFileIterator
//

// TODO pa1.5: implement
HeapFileIterator::HeapFileIterator(/* TODO pa1.5: add parameters */) {
}

bool HeapFileIterator::operator!=(const HeapFileIterator &other) const {
    // TODO pa1.5: implement
}

Tuple &HeapFileIterator::operator*() const {
    // TODO pa1.5: implement
}

HeapFileIterator &HeapFileIterator::operator++() {
    // TODO pa1.5: implement
}