#include <db/BufferPool.h>
#include <db/Database.h>

using namespace db;

// TODO pa1.3: implement
BufferPool::BufferPool(int numPages) {
    maxPages = numPages;
    currentPageCount = 0;
}

Page *BufferPool::getPage(const TransactionId &tid, PageId *pid) {
    // TODO pa1.3: implement
    if (pageBuffer.find(pid) != pageBuffer.end()) {
        return pageBuffer[pid];
    } else {
        if (currentPageCount >= maxPages) {
            evictPage();
        }

        DbFile* dbFile = Database::getCatalog().getDatabaseFile(pid->getTableId());
        Page* fetchedPage = dbFile->readPage(*pid);

        pageBuffer[pid] = fetchedPage;
        currentPageCount++;

        return fetchedPage;
    }
}

void BufferPool::evictPage() {
    // do nothing for now
}
