#include "faultsManager.h"

// Constructor
c_faultsManager::c_faultsManager()
{
    for (size_t i = 0; i < predefinedFaults.size(); ++i)
    {
        faultsStates[i] = {&predefinedFaults[i], false, false};
    }
}

// Method to trigger a fault
void c_faultsManager::triggerFault(uint8_t id)
{
    for (FaultState& faultState : faultsStates)
    {
        if (faultState.fault->id == id)
        {
            faultState.active = true;
            faultState.acknowledged = false;
            handleFaults();
            return;
        }
    }
    printf("Fault %d not found.\n", id);
}

// Method to acknowledge a fault
void c_faultsManager::acknowledgeFault(uint8_t id)
{
    for (FaultState& faultState : faultsStates)
    {
        if (faultState.fault->id == id)
        {
            faultState.acknowledged = true;
            printf("Fault %d acknowledged.\n", id);
            return;
        }
    }
    printf("Fault %d not found.\n", id);
}

// Method to get the list of faults
const std::array<FaultState, predefinedFaults.size()>& c_faultsManager::getFaults() const
{
    return faultsStates;
}

// Method to check if there are active faults
bool c_faultsManager::hasActiveFaults() const
{
    for (const FaultState& faultState : faultsStates)
    {
        if (faultState.active && !faultState.acknowledged)
        {
            return true;
        }
    }
    return false;
}

// Method to handle faults based on their severity
void c_faultsManager::handleFaults()
{
    for (const FaultState& faultState : faultsStates)
    {
        if (faultState.active && !faultState.acknowledged)
        {
            switch (faultState.fault->severity)
            {
                case N1:
                    handleN1Fault(*faultState.fault);
                    break;
                case N2:
                    handleN2Fault(*faultState.fault);
                    break;
                case N3:
                    handleN3Fault(*faultState.fault);
                    break;
                default:
                    break;
            }
        }
    }
}

// Method to handle N1 level faults
void c_faultsManager::handleN1Fault(const Fault& fault)
{
    // Logic to handle N1 level faults
    //printf("N1 Fault triggered: %s\n", fault.description);
}

// Method to handle N2 level faults
void c_faultsManager::handleN2Fault(const Fault& fault)
{
    // Logic to handle N2 level faults
    //printf("N2 Fault triggered: %s\n", fault.description);
    // Add performance reduction logic here
}

// Method to handle N3 level faults
void c_faultsManager::handleN3Fault(const Fault& fault)
{
    // Logic to handle N3 level faults
    //printf("N3 Fault triggered: %s\n", fault.description);
    // Add system shutdown logic here
    // For example: put the system in safe mode and stop all critical operations
    // shutdownSystem();
}

// Method to print active faults
void c_faultsManager::printActiveFaults() const
{
    bool foundActiveFault = false;
    for (const FaultState& faultState : faultsStates)
    {
        if (faultState.active && !faultState.acknowledged)
        {
            printf("Active Fault: ID=%d, Severity=%d, Description=%s\n", faultState.fault->id, faultState.fault->severity, faultState.fault->description);
            foundActiveFault = true;
        }
    }
    if (!foundActiveFault)
    {
        printf("No active faults.\n");
    }
}

// System shutdown function (example)
void shutdownSystem()
{
    printf("System shutting down due to critical fault.\n");
    // Add code to shut down the system in safe mode
    // exit(1);
}
