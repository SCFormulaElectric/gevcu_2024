#ifndef ELCONMANAGE_H_
#define ELCONMANAGE_H_

#include <Arduino.h>
#include <vector>
#include "../Device.h"
#include "../../DeviceManager.h"
#include "../../CanHandler.h"


#define ElconID 0x269
#define ElconTickInt 1000000

class ElconChargerController: public Device, CanObserver{
public:
    ElconChargerController(); //called nearly immediately to initialize your own variables
    void setup(); //called only if the device is actually enabled
    void earlyInit(); //called early and whether or not the device is enabled. Just used to setup configuration
    void handleTick(); 
    void handleCanFrame(const CAN_message_t &frame);
    DeviceId getId();
    DeviceType getType();
private:
    CAN_message_t var;
    int MaxPackVolt_ = 0;
    int MaxPackVoltin_ = 0;
    std::vector<int> customFlags_ = std::vector<int>(3,0);
    int MaxCellVolt_ = 0;
    int MaxCellVoltin_ = 0;
    int MaxCellVolt2_ = 0;
    int MaxCellVolt2in_ = 0;
    std::vector<int> PackCCL_ = std::vector<int>(3,0);
    std::vector<int> PackCCLin_ = std::vector<int>(3,0);
};
#endif
