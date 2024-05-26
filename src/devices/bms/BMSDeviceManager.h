#ifndef BATTMANAGE_H_
#define BATTMANAGE_H_

#include <Arduino.h>
#include <vector>
#include "../../config.h"
#include "../Device.h"
#include "../../DeviceManager.h"
#include "BatteryManager.h"
#include "../../CanHandler.h"

#define BMSID 0x300
#define BMSTickInt 500000

class BMSDeviceManager: public Device, CanObserver {
public:
    BMSDeviceManager(); //called nearly immediately to initialize your own variables
    void setup(); //called only if the device is actually enabled
    void earlyInit(); //called early and whether or not the device is enabled. Just used to setup configuration
    void handleTick(); 
    void handleCanFrame(const CAN_message_t &frame);
    DeviceId getId();
    DeviceType getType();
private:
    CAN_message_t var;
    std::vector<int> Checksum_ = {0,0,0,0,0,0,0};
    int RollingCounter_ = 0;
    int PackOpenVolt_ = 0;
    int PackOpenVoltin_ = 0;
    int PackInstVolt_ = 0;
    int PackInstVoltin_ = 0;
    int PackDCLKW_ = 0;
    int PackDCLKWin_ = 0;
    int PackCCLKW_ = 0;
    int PackCCLKWin_ = 0;
    int CurrentLimit_ = 0;
    int HighTemp_ = 0;
    int HighTherm_ = 0;
    int LowTemp_ = 0;
    int LowTherm_ = 0;
    int AverageTemp_ = 0;
    int HighCellVolt_ = 0;
    int HighCellID_ = 0;
    int LowCellVolt_ = 0;
    int LowCellID_ = 0;
    int AverageCellVolt_ = 0;
    std::vector<int> DTC_Flags_ = {0,0,0,0};
    int customFlag_ = 0;
    int RelayState_ = 0;
    int PackSOC_ = 0;
    int PackResist_ = 0;
    int PackResistin_ = 0;
    int PackAmpHr_ = 0;
};
#endif
