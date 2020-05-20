#include "UsbMidiPortNotifier.h"
#include <iostream>
#include <unistd.h> // sleep()

int main()
{
   midi::PortNotifiers::instance().init();
   midi::PortNotifiers::instance().inputs.registerNewPortCb([](rtmidiadapt::PortIndex portIndex,
                                                               const rtmidiadapt::DeviceOnUsbPort& devOnPort){
      std::cout << "ADDED Inputs: PortIndex: " << portIndex << " devOnPort: " << devOnPort << std::endl;
   }, { {}, {}, false } );

   midi::PortNotifiers::instance().outputs.registerNewPortCb([](rtmidiadapt::PortIndex portIndex,
                                                               const rtmidiadapt::DeviceOnUsbPort& devOnPort){
      std::cout << "ADDED Outputs: PortIndex: " << portIndex << " devOnPort: " << devOnPort << std::endl;
   }, { {}, {}, false } );

   midi::PortNotifiers::instance().inputs.registerRemovedPortCb([](const rtmidiadapt::DeviceOnUsbPort& devOnPort){
      std::cout << "REMOVED Inputs: devOnPort: " << devOnPort << std::endl;
   }, { {}, {}, false } );

   midi::PortNotifiers::instance().outputs.registerRemovedPortCb([](const rtmidiadapt::DeviceOnUsbPort& devOnPort){
      std::cout << "REMOVED Outputs: devOnPort: " << devOnPort << std::endl;
   }, { {}, {}, false } );

   while(true)
   {
      midi::PortNotifiers::instance().update();
      sleep(1);
   }
   return 0;
}