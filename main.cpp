
#include <iostream>
#include <string>
#include <algorithm>
#include <dev/devs.hpp>
#include <thread>
#include <sstream>
#define PREFIX "PYCTRL "
#define ZOOMSPEED 255
using namespace std;

vector<string> kDevs;
shared_ptr<Device> dev;
int deviceIndex = 0;


/// call when detect device connected or disconnected
void onDevChanged(std::string dev_sn, bool in_out, void *param)
{
    cout << "Device sn: " << dev_sn << (in_out ? " Connected" : " DisConnected") << endl;

    auto it = std::find(kDevs.begin(), kDevs.end(), dev_sn);
    if (in_out)
    {
        if (it == kDevs.end())
        {
            kDevs.emplace_back(dev_sn);
        }
    }
    else
    {
        if (it != kDevs.end())
        {
            kDevs.erase(it);
        }
    }

    cout << "Device num: " << kDevs.size() << endl;
}

void handleCommand(const std::string& command) {
    std::istringstream stream(command);
    std::string action;

    if (!(stream >> action)) {
        std::cout << PREFIX << "ERROR Empty command" << std::endl;
        return;
    }

    // MOVE
    if (action == "MOVE") {
        double x, y;
        char comma1;

        // Try to put stream into variables
        if (!(stream >> x >> comma1 >> y) ||
            comma1 != ',') {
            std::cout << PREFIX << "ERROR Expected MOVE x,y" << std::endl;
            return;
            }

        // Move the camera
        try {
            if (dev->aiSetGimbalMotorAngleR(x, y)== RM_RET_OK) {
                std::cout << PREFIX << "MOVE: " << x << ", " << y << std::endl;
            }
        }
        catch (exception e) {
            cerr << PREFIX <<  e.what() << endl;
        }

    // SPEED
    } else if (action == "SPEED") {
        double x, y;
        char comma1;
        // Put stream into variable
        if (!(stream >> x >> comma1 >> y)){
            std::cout << PREFIX << "ERROR Missing SPEED argument" << std::endl;
            return;
        }


        if (dev->aiSetGimbalSpeedCtrlR(x, y) == RM_RET_OK) {
            std::cout << PREFIX << "SPEED: " << x << ", " << y << std::endl;
            return;
        }

    // ZOOM function
    }else if (action == "ZOOM") {
        double zoom;
        // Put stream into variable
        if (!(stream >> zoom)) {
            cout << PREFIX << "ERROR Missing ZOOM argument" << endl;
            return;
        }
        // Zoom in
        if(dev->cameraSetZoomWithSpeedAbsoluteR(zoom * 100.0f, ZOOMSPEED) == RM_RET_OK) {
            cout << PREFIX << "ZOOM: " << zoom << endl;
            return;
        }

    } else if (action == "QUIT") {
        std::cout << PREFIX << "BYE" << std::endl;

    } else {
        std::cout << PREFIX << "ERROR Unknown command: "
                  << action << std::endl;
    }
}


int main() {
    std::string command;

    kDevs.clear();
    Devices::get().setDevChangedCallback(onDevChanged, nullptr);

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    // auto dev_list = Devices::get().getDevList();

    dev = Devices::get().getDevBySn(kDevs[deviceIndex]);
    cout << "select the device: " << dev->devName().c_str() << endl;

    dev->cameraSetDevRunStatusR(Device::DevStatusRun);
    dev->cameraSetAiModeU(Device::AiWorkModeNone);

    cout << "-----------------------------" << endl;
    cout << "OBSBOT Tiny2 Lite is ready!" << endl;
    cout << "Ready to receive" << endl;
    // Signal to Python that the controller is ready.
    std::cout << PREFIX << "READY" << std::endl;

    while (std::getline(std::cin, command)) {
        if (command.empty()) continue;   // ignore blank lines
        handleCommand(command);
        if (command == "QUIT") break;    // or have handleCommand return a bool
    }

    // while (true) {
    //     for (int i =0; i < 3; i++) {
    //         switch (i) {
    //             case 0:
    //                 dev->cameraSetFovU(Device::FovType86);
    //                 break;
    //             case 1:
    //                 dev->cameraSetFovU(Device::FovType78);
    //                 break;
    //             case 2:
    //                 dev->cameraSetFovU(Device::FovType65);
    //                 break;
    //         }
    //         this_thread::sleep_for(chrono::milliseconds(2000));
    //     }
    // }
    return 0;
}
