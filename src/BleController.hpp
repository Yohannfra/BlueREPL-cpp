#ifndef BLECONTROLLER_HPP
#define BLECONTROLLER_HPP

#include "simpleble/SimpleBLE.h"

class BleController {
public:
    BleController();
    ~BleController();

    int scan(std::uint32_t scan_time_second = 5, bool print_results = true);

    void printScannedPeripheral() const;
    int connectToScannedPeripheral(std::size_t index);
    int connectByIndex(std::size_t index);
    int connectByAddress(const std::string &device_address);
    int connectByName(const std::string &device_name);
    int disconnect();

    int print_peripheral_services() const;
    int print_peripheral_infos() const;

    bool isConnected() const;

private:
    void selectAdapter();
    std::string byte_array_to_string(SimpleBLE::ByteArray &bytes) const;
    int internal_connect();

    SimpleBLE::Adapter _adapter;
    std::vector<SimpleBLE::Peripheral> _scannedPeripherals;
    SimpleBLE::Peripheral _peripheral;
};

#endif
