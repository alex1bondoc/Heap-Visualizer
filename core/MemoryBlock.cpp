enum class Status {
  FREE = 0,
  Allocated = 1,
};

class MemoryBlock {
private:
  Status status;
  int size;

  MemoryBlock *next;
  MemoryBlock *prev;

public:
  MemoryBlock(int size, Status status)
      : size((size + 7) & ~7), Status(status), next(nullptr), prev(nullptr) {}
  void SetSize(int size) { this->size = size; }
  void SetStatus(Status status) { this->status = status; }
  void SetNext(MemoryBlock *next) { this->next = next; }
  void SetPrev(MemoryBlock *prev) { this->prev = prev; }

  int GetSize() { return size; }
  Status GetStatus() { return status; }
  MemoryBlock *GetNext() { return next; }
  MemoryBlock *GetPrev() { return prev; }
};
