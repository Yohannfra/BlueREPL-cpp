#ifndef BLECONTROLLER_HPP
#define BLECONTROLLER_HPP

#include <string>
#include <vector>

using BluetoothUUID = std::string;
using BluetoothAddress = std::string;
using ByteArray = std::string;

class BleController {
public:
    virtual int scan(std::uint32_t scan_time_second = 5, bool print_results = true) = 0;
    virtual void printScannedPeripheral() const = 0;
    virtual int connectByIndex(std::size_t index) = 0;
    virtual int connectByAddress(const std::string &device_address) = 0;
    virtual int connectByName(const std::string &device_name) = 0;
    virtual int disconnect() = 0;
    virtual int print_peripheral_services() const = 0;
    virtual int print_peripheral_infos() const = 0;
    virtual bool isConnected() const = 0;
    virtual std::vector<std::uint8_t> read(BluetoothUUID const &service, BluetoothUUID const &characteristic) = 0;
};

#endif
