enum class Status : int {
    FREE = 0,
    ALLOC,
};

class MemoryBlock {
private:
    Status status;
    int size;

    MemoryBlock *next;
    MemoryBlock *prev;

public: 
    MemoryBlock(int size, Status status, MemoryBlock *next = nullptr, MemoryBlock *prev = nullptr)
      : size((size + 7) & ~7), status(status), next(nullptr), prev(nullptr) {}

    void setSize(int size) { this->size = size; }
    void setStatus(Status status) { this->status = status; }
    void setNext(MemoryBlock *next) { this->next = next; }
    void setPrev(MemoryBlock *prev) { this->prev = prev; }

    int getSize() { return size; }
    Status getStatus() { return status; }
    MemoryBlock *getNext() { return next; }
    MemoryBlock *getPrev() { return prev; }

    void RemoveCurrent() {
        MemoryBlock *aux_next = getNext();
        MemoryBlock *aux_prev = getPrev();
        aux_prev->setNext(aux_next);
        aux_next->setPrev(aux_prev);
    }

    MemoryBlock *SplitBlock(int size) { 
        if (this->getSize() - size == 0) {
            this->setStatus(Status::ALLOC);
            return this;
        }
        MemoryBlock *aux = new MemoryBlock(this->getSize() - size, Status::FREE);
        this->setSize(size);
        this->setStatus(Status::ALLOC);
        MemoryBlock *next = this->getNext();
        this->setNext(aux);
        aux->setPrev(this);
        if (next != nullptr) {
            next->setPrev(aux);
        }
        aux->setNext(next);
        return this;
    }
    
    void joinNext(int size) {
        this->setSize(this->getSize() + size);
        this->getNext()->setSize(next->getSize() - size);
        if (this->getNext()->getSize() == 0) {
            this->getNext()->RemoveCurrent();
        }
    }
    void joinPrev(int size) {
        this->setSize(this->getSize() + size);
        this->getPrev()->setSize(prev->getSize() - size);
        if (this->getPrev()->getSize() == 0) {
            this->getPrev()->RemoveCurrent();
        }
    }

    void mergeNext() {
        this->setSize(this->getSize() + this->getNext()->getSize());
        this->getNext()->RemoveCurrent();
    }

    void mergePrev() {
        this->setSize(this->getSize() + this->getPrev()->getSize());
        this->getPrev()->RemoveCurrent();
    }
};
