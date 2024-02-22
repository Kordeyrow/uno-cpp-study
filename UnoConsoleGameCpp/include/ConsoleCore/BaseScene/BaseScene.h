#pragma once
#include <iostream>

class BaseScene : public std::enable_shared_from_this<BaseScene> {
public:
    virtual auto Init() -> void = 0;
    virtual auto Run() -> std::shared_ptr<BaseScene> = 0;
};