#ifndef BLECONTROLLER_HPP
#define BLECONTROLLER_HPP

#include "utils.hpp"

#include <string>
#include <vector>

class BleController {
public:
    virtual int scan(std::uint32_t scan_time_second = 5, bool print_results = true) = 0;
    virtual void printScannedPeripheral() = 0;
    virtual int connectByIndex(std::size_t index) = 0;
    virtual int connectByAddress(const std::string &device_address) = 0;
    virtual int connectByName(const std::string &device_name) = 0;
    virtual int disconnect() = 0;
    virtual int print_peripheral_services() = 0;
    virtual int print_peripheral_infos() = 0;
    virtual bool isConnected() = 0;
    virtual ByteArray read(BluetoothUUID const &service, BluetoothUUID const &characteristic) = 0;
    virtual int write(
        BluetoothUUID const &service, BluetoothUUID const &characteristic, const ByteArray &payload) = 0;
    virtual int notify(BluetoothUUID const &service, BluetoothUUID const &characteristic,
        std::function<void(ByteArray payload)> callback) = 0;
    virtual int indicate(BluetoothUUID const &service, BluetoothUUID const &characteristic,
        std::function<void(ByteArray payload)> callback) = 0;
    virtual int unsubscribe(BluetoothUUID const &service, BluetoothUUID const &characteristic) = 0;
};

#endif
