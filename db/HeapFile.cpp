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
HeapFile::HeapFile(const char *fname, const TupleDesc &tdesc)
        : td(tdesc), filename(fname), mutableNumPages(0), isNumPagesSet(false) {

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
    if (!isNumPagesSet) {
        file.open(filename, std::ios::in | std::ios::binary);
        if (!file) {
            throw std::runtime_error("Unable to open the file.");
        }
        file.seekg(0, std::ios::end);
        mutableNumPages = file.tellg() / Database::getBufferPool().getPageSize();
        file.close();  // Close the file after getting the number of pages.
        isNumPagesSet = true;
    }
    return mutableNumPages;
}

HeapFileIterator HeapFile::begin() const {
    // TODO pa1.5: implement
    return HeapFileIterator(const_cast<HeapFile&>(*this), 0, 0);
}

HeapFileIterator HeapFile::end() const {
    // TODO pa1.5: implement
    return HeapFileIterator(const_cast<HeapFile&>(*this), mutableNumPages, 0);
}

//
// HeapFileIterator
//

// TODO pa1.5: implement
HeapFileIterator::HeapFileIterator(HeapFile &file, int startPageId, int startTupleIndex)
        : heapFile(file), currentPageId(startPageId), tupleIter(startTupleIndex) {

    if (numPagesCache == -1) {
        numPagesCache = heapFile.getNumPages();
    }

    currentPage = dynamic_cast<HeapPage*>(heapFile.readPage(HeapPageId(heapFile.getId(), currentPageId)));
}

bool HeapFileIterator::operator!=(const HeapFileIterator &other) const {
    // TODO pa1.5: implement
    return currentPageId != other.currentPageId ||
           (currentPageId == other.currentPageId && currentTupleIndex != other.currentTupleIndex);
}

Tuple &HeapFileIterator::operator*() const {
    // TODO pa1.5: implement
    if (!currentPage) {
        throw std::runtime_error("Iterator not initialized or dereferencing end iterator");
    }
    HeapPageIterator pageIter(0, currentPage);
    for(int i = 0; i < tupleIter; ++i) {
        ++pageIter;
    }
    return *pageIter;
}

HeapFileIterator &HeapFileIterator::operator++() {
    // TODO pa1.5: implement
    if (!currentPage) {
        throw std::runtime_error("Incrementing past the end of the iterator");
    }

    do {
        ++tupleIter;
    } while(tupleIter < currentPage->getNumTuples() && !currentPage->isSlotUsed(tupleIter)); // Find the next used slot

    if (tupleIter >= currentPage->getNumTuples()) {  // If end of the current page
        if (currentPageId >= heapFile.getNumPages() - 1) {
            currentPage = nullptr;
            tupleIter = -1; // Indicate end
            return *this;
        }

        currentPageId++;
        tupleIter = 0;
        currentPage = dynamic_cast<HeapPage*>(heapFile.readPage(HeapPageId(heapFile.getId(), currentPageId)));

        while(tupleIter < currentPage->getNumTuples() && !currentPage->isSlotUsed(tupleIter)) {
            ++tupleIter;
        }
    }
    return *this;
}