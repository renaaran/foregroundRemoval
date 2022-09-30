#pragma once

template<typename T>
class Algorithm {
public:
    virtual void execute() = 0;
    virtual T getResult() = 0;
    virtual ~Algorithm() = default;
};