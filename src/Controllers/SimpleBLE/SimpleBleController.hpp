#ifndef SIMPLE_BLE_CONTROLLER_HPP
#define SIMPLE_BLE_CONTROLLER_HPP

#include "BleController.hpp"
#include "simpleble/SimpleBLE.h"

class SimpleBLEController : public BleController {
public:
    SimpleBLEController();
    ~SimpleBLEController();

    int scan(std::uint32_t scan_time_second = 5, bool print_results = true) final;

    void printScannedPeripheral() final;
    int connectByIndex(std::size_t index) final;
    int connectByAddress(const std::string &device_address) final;
    int connectByName(const std::string &device_name) final;
    int disconnect() final;

    int print_peripheral_services() final;
    int print_peripheral_infos() final;

    bool isConnected() final;

    SimpleBLE::ByteArray read(std::string const &service, std::string const &characteristic) final;
    int write(BluetoothUUID const &service, BluetoothUUID const &characteristic, const ByteArray &payload) final;

private:
    void selectAdapter();
    std::string byte_array_to_string(const SimpleBLE::ByteArray &bytes) const;
    int internal_connect();

    SimpleBLE::Adapter _adapter;
    std::vector<SimpleBLE::Peripheral> _scannedPeripherals;
    SimpleBLE::Peripheral _peripheral;
};

#endif
