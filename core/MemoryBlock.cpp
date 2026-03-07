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
    MemoryBlock(int size, Status status)
      : size((size + 7) & ~7), status(status), next(nullptr), prev(nullptr) {}

  
    void SetSize(int size) { this->size = size; }
    void SetStatus(Status status) { this->status = status; }
    void SetNext(MemoryBlock *next) { this->next = next; }
    void SetPrev(MemoryBlock *prev) { this->prev = prev; }

    int GetSize() { return size; }
    Status GetStatus() { return status; }
    MemoryBlock *GetNext() { return next; }
    MemoryBlock *GetPrev() { return prev; }

    void RemoveCurrent() {
        MemoryBlock *aux_next = GetNext();
        MemoryBlock *aux_prev = GetPrev();
        aux_prev->SetNext(aux_next);
        aux_next->SetPrev(aux_prev);
    }

    MemoryBlock *SplitBlock(int size) { 
        MemoryBlock *aux = new MemoryBlock(this->GetSize() - size, Status::FREE);
        this->SetSize(this->GetSize() - size);
        this->SetStatus(Status::ALLOC);
        MemoryBlock *next = this->GetNext();
        this->SetNext(aux);
        aux->SetPrev(this);
        aux->SetNext(next);
        return this;
  }
};
