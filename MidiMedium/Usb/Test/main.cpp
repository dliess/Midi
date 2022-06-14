#include "UsbMidiIn.h"
#include "UsbMidiOut.h"
#include <iostream>
#include <unistd.h> // sleep()

using namespace midi;

int main()
{
   UsbMidiIn usbMidiIn{};
   UsbMidiOut usbMidiOut{};

   usbMidiOut.openVirtualPort("MyTestPort");
   std::cout << "Device Name: " << usbMidiOut.getDeviceName() << "\n";
   std::cout << "Device Port Name: " << usbMidiOut.getDevicePortName() << "\n";
   std::cout << "Host Connector Port Name: " << usbMidiOut.getHostConnectorPortName() << "\n";
   while(true)
   {
      sleep(1);
   }
   return 0;
}