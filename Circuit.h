#pragma once
#include <map>
#include <string>
#include <vector>

class CircuitCheckPoint;
class CheckPoint;
class CircuitUnion;

namespace Circuit
{
    //Ground* pGround_ = nullptr;

    //èâä˙âª
    void Initialize();

    std::vector<std::string>* GetCircuitNameArray();

    void GetCircuits(std::vector<Circuit::CircuitUnion>* circuitArray);

    void ResetCircuitModelHandle();
}

