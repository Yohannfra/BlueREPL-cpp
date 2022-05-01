#ifndef SIMPLE_BLE_CONTROLLER_HPP
#define SIMPLE_BLE_CONTROLLER_HPP

#include "simpleble/SimpleBLE.h"
#include "BleController.hpp"

class SimpleBLEController: public BleController {
public:
    SimpleBLEController();
    ~SimpleBLEController();

    int scan(std::uint32_t scan_time_second = 5, bool print_results = true) final;

    void printScannedPeripheral() const final;
    int connectByIndex(std::size_t index)  final;
    int connectByAddress(const std::string &device_address) final;
    int connectByName(const std::string &device_name) final;
    int disconnect() final;

    int print_peripheral_services() const final;
    int print_peripheral_infos() const final;

    bool isConnected() const final;

    std::vector<std::uint8_t> read(std::string const &service, std::string const &characteristic) final;
    /* SimpleBLE::ByteArray read( */
    /*     SimpleBLE::BluetoothUUID const &service, SimpleBLE::BluetoothUUID const &characteristic); */

private:
    void selectAdapter();
    std::string byte_array_to_string(SimpleBLE::ByteArray &bytes) const;
    int internal_connect();

    SimpleBLE::Adapter _adapter;
    std::vector<SimpleBLE::Peripheral> _scannedPeripherals;
    SimpleBLE::Peripheral _peripheral;
};

#endif
