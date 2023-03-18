class GarbageBag
{
private:
    int weight;

public:
    explicit GarbageBag(const int weight)
    {
        this->weight = weight;
    }

    auto get_weight()
    {
        return this->weight;
    }
};