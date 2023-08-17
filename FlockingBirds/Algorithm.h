#pragma once

#include "Bird.h"
#include "CSVWriter.h"
#include <vector>

class Algorithm
{
  public:
    virtual ~Algorithm() = default;
    virtual void update() = 0;
    virtual void fillCSV(CSVEntry& entry) = 0;
};