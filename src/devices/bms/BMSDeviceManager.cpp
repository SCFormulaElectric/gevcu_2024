#include "BMSDeviceManager.h"

BMSDeviceManager::BMSDeviceManager() : Device() {
    commonName = "Battery management system";
    shortName = "BMS";
}

void BMSDeviceManager::earlyInit()
{
    prefsHandler = new PrefHandler(BMSID);
}

void BMSDeviceManager::setup() {
    tickHandler.detach(this);
    Logger::info("add device: DashboardDevice (id: %X, %X)", BMSID, this);

    Device::setup(); // run the parent class version of this function

    setAttachedCANBus(1);

    //Relevant BMS messages are 0x300 - 0x30F
    attachedCANBus->attach(this, 0x300, 0x7f0, false); //TODO:: Masking is wrong
    tickHandler.attach(this, BMSTickInt);
}

// CAN Message Documentation https://www.hpevs.com/Site/images/orion%20byte%20structure/orion_byte_structure_9-17-13.pdf

/*For all multibyte integers the format is MSB first, LSB last
*/
void BMSDeviceManager::handleCanFrame(const CAN_message_t &frame) {
    if(Logger::isDebug()){
        Logger::debug("BMS Test id=%X len=%X data=%X,%X,%X,%X,%X,%X,%X,%X",
                      frame.id, frame.len, 
                      frame.buf[0], frame.buf[1], frame.buf[2], frame.buf[3],
                      frame.buf[4], frame.buf[5], frame.buf[6], frame.buf[7]);
    }

    switch(frame.id) {
        case 0x030:
            RollingCounter_ = frame.buf[0];
            Checksum_[0] = frame.buf[1];
        case 0x031:
            PackOpenVolt_ = frame.buf[0];
            PackOpenVoltin_ = frame.buf[1];
            PackInstVolt_ = frame.buf[2];
            PackInstVoltin_ = frame.buf[3];
            Checksum_[1] = frame.buf[4];
        case 0x032:
            PackDCLKW_ = frame.buf[0];
            PackDCLKWin_ = frame.buf[1];
            PackCCLKW_ = frame.buf[2];
            PackCCLKWin_ = frame.buf[3];
            CurrentLimit_ = frame.buf[4];
            Checksum_[2] = frame.buf[5];
        case 0x033:
            HighTemp_ = frame.buf[0];
            HighTherm_ = frame.buf[1];
            LowTemp_ = frame.buf[2];
            LowTherm_ = frame.buf[3];
            AverageTemp_ = frame.buf[4];
            Checksum_[3] = frame.buf[5];
        case 0x034:
            HighCellVolt_ = frame.buf[0];
            HighCellID_ = frame.buf[1];
            LowCellVolt_ = frame.buf[2];
            LowCellID_ = frame.buf[3];
            AverageCellVolt_ = frame.buf[4];
            Checksum_[4] = frame.buf[5];
        case 0x035:
            DTC_Flags_ = {frame.buf[0],frame.buf[1],frame.buf[2],frame.buf[3]};
            customFlag_ = frame.buf[4];
            Checksum_[5] = frame.buf[5];
        case 0x036:
            RelayState_ = frame.buf[0];
            PackSOC_ = frame.buf[1];
            PackResist_ = frame.buf[2];
            PackResistin_ = frame.buf[3];
            PackAmpHr_ = frame.buf[4];
            Checksum_[6] = frame.buf[5];
    }
    
}

DeviceId BMSDeviceManager::getId() {
    return (BMSID);
}

DeviceType BMSDeviceManager::getType() {
    return (DEVICE_MISC);
}

void BMSDeviceManager::handleTick()
{
    Device::handleTick();

    var.len = 3;
    var.id = 0x203;
    
    int speed = 0;
    int temp = 0;
    var.buf[0] = speed;
    var.buf[1] = temp;
    var.buf[2] = 0x10;
    attachedCANBus->sendFrame(var);
    
    // var.buf[0] = 0x51;
    // // 0x04 to DISABLE
    // //var.buf[1] = 0x04;
    // // 0x00 to ENABLE
    // var.buf[1] = 0x00;
    // var.buf[2] = 0x00;
    // attachedCANBus->sendFrame(var);
    
    // // send 5% speed
    // var.buf[0] = 0x31;
    // var.buf[1] = 0x66;
    // var.buf[2] = 0x06;
    // attachedCANBus->sendFrame(var);
}
BMSDeviceManager bms_device;
