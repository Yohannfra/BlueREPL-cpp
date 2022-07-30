#include "SimpleBleController.hpp"

#include "Repl/LineReader.hpp"

#include <exception>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

SimpleBLEController::SimpleBLEController()
{
    this->selectAdapter();
}

void SimpleBLEController::selectAdapter()
{
    auto adapter_list = SimpleBLE::Adapter::get_adapters();

    if (adapter_list.size() == 0) {
        throw std::runtime_error("No adapter was found.");
    }

    if (adapter_list.size() == 1) {
        _adapter = adapter_list.at(0);
        std::cout << "Using " << _adapter.identifier() << " [" << _adapter.address()
                  << "]" << std::endl;
    } else {
        std::cout << "Available adapters: \n";
        int i = 0;
        for (auto &adapter : adapter_list) {
            std::cout << "[" << i++ << "] " << adapter.identifier() << " ["
                      << adapter.address() << "]" << std::endl;
        }

        std::cout << "Which one to use ?" << std::endl;
        auto rep = LineReader::get();
        if (rep.has_value()) {
            std::stringstream s;
            s << rep.value();
            std::size_t index;
            s >> index;
            if (index < adapter_list.size()) {
                _adapter = adapter_list.at(index);
                std::cout << "Using adapter:" << std::endl;
                std::cout << _adapter.identifier() << " [" << _adapter.address() << "]"
                          << std::endl;
            } else {
                throw std::invalid_argument("Invalid index, try again");
            }
        }
    }
}

std::string SimpleBLEController::byte_array_to_string(const ByteArray &bytes) const
{
    std::ostringstream oss;
    for (auto byte : bytes) {
        oss << std::hex << std::setfill('0') << std::setw(2)
            << (std::uint32_t)((std::uint8_t)byte) << " ";
    }
    return oss.str();
}

void SimpleBLEController::printScannedPeripheral()
{
    std::size_t index = 0;
    for (auto &per : _scannedPeripherals) {
        std::string connectable_string =
            per.is_connectable() ? "Connectable" : "Non-Connectable";
        std::string peripheral_string = per.identifier() + " [" + per.address() + "]";

        std::cout << "[" << index << "] " << peripheral_string << " "
                  << connectable_string << "\n";
        std::map<uint16_t, ByteArray> manufacturer_data = per.manufacturer_data();
        for (const auto &[manufacturer_id, data] : manufacturer_data) {
            std::cout << "\tManufacturer ID:\t" << manufacturer_id << "\n";
            std::cout << "\tManufacturer data:\t" << byte_array_to_string(data) << "\n"
                      << std::endl;
        }
        index += 1;
    }
}

int SimpleBLEController::scan(std::uint32_t scan_time_second, bool print_results)
{
    std::cout << "Scanning for " << scan_time_second << " seconds..." << std::endl;
    _adapter.scan_for(scan_time_second * 1000);
    _scannedPeripherals = _adapter.scan_get_results();
    std::cout << "Scan complete, found " << _scannedPeripherals.size() << " devices:"
              << "\n";

    if (_scannedPeripherals.empty()) {
        std::cerr << "No device found" << std::endl;
        return EXIT_FAILURE;
    }
    if (print_results) {
        this->printScannedPeripheral();
    }
    return EXIT_SUCCESS;
}

int SimpleBLEController::internal_connect()
{
    std::cout << "Connecting to " << _peripheral.identifier() << " ["
              << _peripheral.address() << "]" << std::endl;
    _peripheral.connect();

    if (!_peripheral.is_connected()) {
        std::cerr << "Connection failed" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Connected!" << std::endl;
    return EXIT_SUCCESS;
}

int SimpleBLEController::connectByIndex(std::size_t index)
{
    if (_scannedPeripherals.empty()) {
        std::cerr << "No device to connect to, please scan first" << std::endl;
        return EXIT_FAILURE;
    }

    if (index >= _scannedPeripherals.size()) {
        std::cerr << "No device to connect to at index " << index << std::endl;
        return EXIT_FAILURE;
    }

    _peripheral = _scannedPeripherals[index];
    return this->internal_connect();
}

int SimpleBLEController::connectByAddress(const std::string &device_address)
{
    if (_scannedPeripherals.empty()) {
        if (this->scan(5, false) == EXIT_FAILURE) {
            return EXIT_FAILURE;
        }
    }

    auto res = std::find_if(_scannedPeripherals.begin(), _scannedPeripherals.end(),
        [&](auto &per) { return per.address() == device_address; });
    if (res != _scannedPeripherals.end()) {
        _peripheral = *res.base();
        return this->internal_connect();
    }
    std::cerr << device_address << " Not found" << std::endl;
    return EXIT_FAILURE;
}

int SimpleBLEController::connectByName(const std::string &device_name)
{
    if (_scannedPeripherals.empty()) {
        if (this->scan(5, false) == EXIT_FAILURE) {
            return EXIT_FAILURE;
        }
    }

    auto res = std::find_if(_scannedPeripherals.begin(), _scannedPeripherals.end(),
        [&](auto &per) { return per.identifier() == device_name; });
    if (res != _scannedPeripherals.end()) {
        _peripheral = *res.base();
        return this->internal_connect();
    }
    std::cerr << device_name << " Not found" << std::endl;
    return EXIT_FAILURE;
}

int SimpleBLEController::disconnect()
{
    if (!this->isConnected()) {
        std::cerr << "No device connected" << std::endl;
        return EXIT_FAILURE;
    }
    _peripheral.disconnect();

    if (_peripheral.is_connected()) {
        std::cerr << "Disconnection failed" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Disconnected!" << std::endl;
    return EXIT_SUCCESS;
}

int SimpleBLEController::print_peripheral_services()
{
    if (!this->isConnected()) {
        std::cerr << "No device connected" << std::endl;
        return EXIT_FAILURE;
    }

    auto services = _peripheral.services();

    std::cout << "---------------------------------------\n";
    for (auto &service : services) {
        std::cout << "Service: " << service.uuid << std::endl;
        for (auto characteristic : service.characteristics) {
            std::cout << "  Characteristic: " << characteristic << std::endl;
        }
    }
    std::cout << "---------------------------------------" << std::endl;
    return EXIT_SUCCESS;
}

bool SimpleBLEController::isConnected()
{
    return (_peripheral.initialized() && _peripheral.is_connected());
}

int SimpleBLEController::print_peripheral_infos()
{
    if (!this->isConnected()) {
        std::cerr << "No device connected" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "---------------------------------------\n";
    std::cout << "Address: " << _peripheral.address() << std::endl;
    std::cout << "Identifier: " << _peripheral.identifier() << std::endl;
    std::cout << "rssi: " << _peripheral.rssi() << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    return EXIT_SUCCESS;
}

ByteArray SimpleBLEController::read(
    std::string const &service, std::string const &characteristic)
{
    ByteArray read_bytes;

    if (!this->isConnected()) {
        std::cerr << "No device connected" << std::endl;
        return read_bytes;
    }

    return _peripheral.read(service, characteristic);
}

int SimpleBLEController::write(BluetoothUUID const &service,
    BluetoothUUID const &characteristic, const ByteArray &payload)
{
    if (!this->isConnected()) {
        std::cerr << "No device connected" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        _peripheral.write_command(service, characteristic, payload);
        return EXIT_SUCCESS;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}

int SimpleBLEController::notify(BluetoothUUID const &service,
    BluetoothUUID const &characteristic, std::function<void(ByteArray payload)> callback)
{
    if (!this->isConnected()) {
        std::cerr << "No device connected" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        _peripheral.notify(service, characteristic, callback);
        return EXIT_SUCCESS;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}

int SimpleBLEController::indicate(BluetoothUUID const &service,
    BluetoothUUID const &characteristic, std::function<void(ByteArray payload)> callback)
{
    if (!this->isConnected()) {
        std::cerr << "No device connected" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        _peripheral.indicate(service, characteristic, callback);
        return EXIT_SUCCESS;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}

int SimpleBLEController::unsubscribe(
    BluetoothUUID const &service, BluetoothUUID const &characteristic)
{
    if (!this->isConnected()) {
        std::cerr << "No device connected" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        _peripheral.unsubscribe(service, characteristic);
        std::cout << "Unsubscribed" << std::endl;
        return EXIT_SUCCESS;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}

SimpleBLEController::~SimpleBLEController()
{
    if (this->isConnected()) {
        this->disconnect();
    }
}
