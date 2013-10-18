_Task SortMapper : public Mapper {
    void main();
  public:
    // Writes items to the queue in sorted order.
    SortMapper(const string& filename, int queue_len, int buffer_size, uSemaphore* signal);
    virtual ~SortMapper();
};
