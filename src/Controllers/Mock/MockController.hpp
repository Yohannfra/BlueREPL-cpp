#ifndef MOCKCONTROLLER_HPP
#define MOCKCONTROLLER_HPP

#include "BleController.hpp"

class MockController : public BleController {
public:
    MockController();
    ~MockController();

    int scan(std::uint32_t scan_time_second = 5, bool print_results = true) final;

    void printScannedPeripheral() final;
    int connectByIndex(std::size_t index) final;
    int connectByAddress(const std::string &device_address) final;
    int connectByName(const std::string &device_name) final;
    int disconnect() final;

    int print_peripheral_services() final;
    int print_peripheral_infos() final;

    bool isConnected() final;

    ByteArray read(std::string const &service, std::string const &characteristic) final;
    int write(BluetoothUUID const &service, BluetoothUUID const &characteristic,
        const ByteArray &payload) final;

    int notify(BluetoothUUID const &service, BluetoothUUID const &characteristic,
        std::function<void(ByteArray payload)> callback) final;

    int indicate(BluetoothUUID const &service, BluetoothUUID const &characteristic,
        std::function<void(ByteArray payload)> callback) final;

    int unsubscribe(
        BluetoothUUID const &service, BluetoothUUID const &characteristic) final;

private:
};

#endif
