#ifndef __FAULTS_MANAGER__
#define __FAULTS_MANAGER__

#include "pico/stdlib.h"
#include <stdio.h>
#include <array>

#define FAULT_OVER_IMD_YELLOW           1
#define FAULT_OVER_IMR_YELLOW           2
#define FAULT_OVER_IMC_YELLOW           3
#define FAULT_OVER_IMD_RED              4
#define FAULT_OVER_IMR_RED              5
#define FAULT_OVER_IMC_RED              6
#define FAULT_OVER_TEMPERATURE_BLACK    7
#define FAULT_OVER_TEMPERATURE_RED      8
#define FAULT_OVER_TEMPERATURE_YELLOW   9
#define FAULT_OVER_VOLTAGE_BLACK        10
#define FAULT_OVER_VOLTAGE_RED          11
#define FAULT_OVER_VOLTAGE_YELLOW       12


#define FAULTS_NUMBER                   12

// Definition of fault severity levels
enum SeverityLevel
{
    N1, // Alarm (no performance changes)
    N2, // Performance reduction
    N3  // Safety level (shuts down the system in safe mode)
};

// Structure to represent a fault
struct Fault
{
    uint8_t id;
    SeverityLevel severity;
    const char* description;
};

// Predefined list of faults
constexpr std::array<Fault, FAULTS_NUMBER> predefinedFaults =
{{
    {1,     N1, "N1 FAULT_OVER_IMD_YELLOW"              },
    {2,     N1, "N1 FAULT_OVER_IMR_YELLOW"              },
    {3,     N1, "N1 FAULT_OVER_IMC_YELLOW"              },
    {4,     N2, "N2 FAULT_OVER_IMD_RED"                 },
    {5,     N2, "N2 FAULT_OVER_IMR_RED"                 },
    {6,     N2, "N2 FAULT_OVER_IMC_RED"                 },
    {7,     N3, "N3 FAULT_OVER_TEMPERATURE_BLACK"       },
    {8,     N2, "N2 FAULT_OVER_TEMPERATURE_RED"         },
    {9,     N1, "N1 FAULT_OVER_TEMPERATURE_YELLOW"      },
    {10,    N3, "N3 FAULT_OVER_VOLTAGE_BLACK"           },
    {11,    N2, "N2 FAULT_OVER_VOLTAGE_RED"             },
    {12,    N1, "N1 FAULT_OVER_VOLTAGE_YELLOW"          }
}};

// Structure for fault state
struct FaultState
{
    const Fault* fault;
    bool active;
    bool acknowledged;
};

class c_faultsManager
{
public:
    // Constructor
    c_faultsManager();

    // Method to trigger a fault
    void triggerFault(uint8_t id);

    // Method to acknowledge a fault
    void acknowledgeFault(uint8_t id);

    // Method to get the list of faults
    const std::array<FaultState, predefinedFaults.size()>& getFaults() const;

    // Method to check if there are active faults
    bool hasActiveFaults() const;

    // Method to handle faults based on their severity
    void handleFaults();

    // Method to print active faults
    void printActiveFaults() const;

private:
    // List of fault states
    std::array<FaultState, predefinedFaults.size()> faultsStates;

    // Method to handle N1 level faults
    void handleN1Fault(const Fault& fault);

    // Method to handle N2 level faults
    void handleN2Fault(const Fault& fault);

    // Method to handle N3 level faults
    void handleN3Fault(const Fault& fault);
};

#endif
