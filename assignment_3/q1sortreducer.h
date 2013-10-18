_Task SortReducer : public Reducer {
    void main();
  public:
    SortReducer(int id, int num_reducers, uSemaphore* signal, const vector<Mapper*>& mappers);
    virtual ~SortReducer();
};
