#include "ElconChargerController.h"

// #include "BamocarMotorController.h"

ElconChargerController::ElconChargerController():Device() {
    commonName = "Elcon Charger";
    shortName = "Elcon";
}

void ElconChargerController::earlyInit()
{
    prefsHandler = new PrefHandler(ElconID);
}

void ElconChargerController::setup() {
    Logger::info("add device: DashboardDevice (id: %X, %X)", ElconID, this);


    Device::setup(); // run the parent class version of this function

    setAttachedCANBus(1);
    //Relevant BMS messages are 0x300 - 0x30F
    attachedCANBus->attach(this, 0x300, 0x7f0, false); // TODO: Need to update the bit masking
    tickHandler.attach(this, ElconTickInt);
}

/*For all multibyte integers the format is MSB first, LSB last
*/
void ElconChargerController::handleCanFrame(const CAN_message_t &frame) {
    if(Logger::isDebug()){
        Logger::debug("BMS Test id=%X len=%X data=%X,%X,%X,%X,%X,%X,%X,%X",
                      frame.id, frame.len, 
                      frame.buf[0], frame.buf[1], frame.buf[2], frame.buf[3],
                      frame.buf[4], frame.buf[5], frame.buf[6], frame.buf[7]);
    }
    
    switch(frame.id) {
        case 0x1806E5F4:
            MaxCellVolt_ = frame.buf[0];
            MaxCellVoltin_ = frame.buf[1];
            PackCCL_[0] = frame.buf[2];
            PackCCLin_[0] = frame.buf[3];
            customFlags_[0] = frame.buf[4];
        case 0x1806E7F4:
            MaxPackVolt_ = frame.buf[0];
            MaxPackVoltin_ = frame.buf[1];
            PackCCL_[1] = frame.buf[2];
            PackCCLin_[1] = frame.buf[3];
            customFlags_[1] = frame.buf[4];
        case 0x1806E9F4:
            MaxCellVolt2_ = frame.buf[0];
            MaxCellVolt2in_ = frame.buf[1];
            PackCCL_[2] = frame.buf[2];
            PackCCLin_[2] = frame.buf[3];
            customFlags_[2] = frame.buf[4];
    }
}

DeviceId ElconChargerController::getId() {
    return (ElconID);
}

DeviceType ElconChargerController::getType() {
    return (DEVICE_MISC);
}

void ElconChargerController::handleTick()
{

    var.len = 3;
    var.id = 0x203;
    
    var.buf[0] = 0x10;
    var.buf[1] = 0x10;
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
//testDevice test_device;
ElconChargerController dash_device;
