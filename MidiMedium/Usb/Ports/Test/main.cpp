
#include "UsbMidiInputPortListProvider.h"
#include "UsbMidiOutputPortListProvider.h"
#include "UsbMidiPortNotifier.h"
#include <iostream>
#include <unistd.h> // sleep()

using namespace midi;

int main()
{
    InputPortListProvider  inputPortListProvider;
    OutputPortListProvider outputPortListProvider;
    PortNotifier<InputPortListProvider> inputPortListChangeHandler(inputPortListProvider);
    PortNotifier<OutputPortListProvider> outputPortListChangeHandler(outputPortListProvider);

    if(!inputPortListProvider.init())
    {
        std::cerr << "could not initialize inputPortListProvider" << std::endl;
        return -1;
    }
    if(!outputPortListProvider.init())
    {
        std::cerr << "could not initialize outputPortListProvider" << std::endl;
        return -1;
    }

    inputPortListChangeHandler.registerNewPortCb([](rtmidiadapt::PortIndex i, const rtmidiadapt::DeviceOnUsbPort& devOnUsbPort){
        std::cout << "New input " << i << " : " << devOnPort << std::endl;
    });
    inputPortListChangeHandler.registerRemovedPortCb([](const rtmidiadapt::DeviceOnUsbPort& devOnUsbPort){
        std::cout << "Removed input " << devOnPort << std::endl;
    });
    outputPortListChangeHandler.registerNewPortCb([](rtmidiadapt::PortIndex i, const rtmidiadapt::DeviceOnUsbPort& devOnUsbPort){
        std::cout << "New output " << i << " : " << devOnPort << std::endl;
    });
    outputPortListChangeHandler.registerRemovedPortCb([](const rtmidiadapt::DeviceOnUsbPort& devOnUsbPort){
        std::cout << "Removed output " << devOnPort << std::endl;
    });

    while(true)
    {
        inputPortListChangeHandler.update();
        outputPortListChangeHandler.update();
        sleep(1);
    }
    return 0;
}