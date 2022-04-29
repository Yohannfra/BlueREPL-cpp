#include "BleController.hpp"

#include "Repl.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

BleController::BleController()
{
    this->selectAdapter();
}

void BleController::selectAdapter()
{
    auto adapter_list = SimpleBLE::Adapter::get_adapters();

    if (adapter_list.size() == 0) {
        throw std::runtime_error("No adapter was found.");
    }

    if (adapter_list.size() == 1) {
        _adapter = adapter_list.at(0);
        std::cout << "Using " << _adapter.identifier() << " [" << _adapter.address() << "]" << std::endl;
    } else {
        std::cout << "Available adapters: \n";
        int i = 0;
        for (auto &adapter : adapter_list) {
            std::cout << "[" << i++ << "] " << adapter.identifier() << " [" << adapter.address() << "]"
                      << std::endl;
        }

        std::cout << "Which one to use ?" << std::endl;
        auto rep = Repl::get_line();
        if (rep.has_value()) {
            std::stringstream s;
            s << rep.value();
            std::size_t index;
            s >> index;
            if (index >= 0 && index < adapter_list.size()) {
                _adapter = adapter_list.at(index);
                std::cout << "Using adapter:" << std::endl;
                std::cout << _adapter.identifier() << " [" << _adapter.address() << "]" << std::endl;
            } else {
                throw std::invalid_argument("Invalid index, try again");
            }
        }
    }
}

std::string BleController::byte_array_to_string(SimpleBLE::ByteArray &bytes) const
{
    std::ostringstream oss;
    for (auto byte : bytes) {
        oss << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)((uint8_t)byte) << " ";
    }
    return oss.str();
}

void BleController::printScannedPeripheral() const
{
    std::size_t index = 0;
    for (const auto &per : _scannedPeripherals) {
        std::string connectable_string = per.is_connectable() ? "Connectable" : "Non-Connectable";
        std::string peripheral_string = per.identifier() + " [" + per.address() + "]";

        std::cout << "[" << index << "] " << peripheral_string << " " << connectable_string << "\n";
        std::map<uint16_t, SimpleBLE::ByteArray> manufacturer_data = per.manufacturer_data();
        for (auto &[manufacturer_id, data] : manufacturer_data) {
            std::cout << "\tManufacturer ID:\t" << manufacturer_id << "\n";
            std::cout << "\tManufacturer data:\t" << byte_array_to_string(data) << "\n" << std::endl;
        }
        index += 1;
    }
}

int BleController::scan(std::uint32_t scan_time_second, bool print_results)
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

int BleController::internal_connect()
{
    std::cout << "Connecting to " << _peripheral.identifier() << " [" << _peripheral.address() << "]" << std::endl;
    _peripheral.connect();

    if (!_peripheral.is_connected()) {
        std::cerr << "Connection failed" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Connected!" << std::endl;
    return EXIT_FAILURE;
}

int BleController::connectByIndex(std::size_t index)
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

int BleController::connectByAddress(const std::string &device_address)
{
    if (_scannedPeripherals.empty()) {
        if (this->scan(5, false) == EXIT_FAILURE) {
            return EXIT_FAILURE;
        }
    }

    auto res = std::find_if(_scannedPeripherals.begin(), _scannedPeripherals.end(),
        [&](const auto &per) { return per.address() == device_address; });
    if (res != _scannedPeripherals.end()) {
        _peripheral = *res.base();
        return this->internal_connect();
    }
    std::cout << device_address << " Not found" << std::endl;
    return EXIT_FAILURE;
}

int BleController::connectByName(const std::string &device_name)
{
    if (_scannedPeripherals.empty()) {
        if (this->scan(5, false) == EXIT_FAILURE) {
            return EXIT_FAILURE;
        }
    }

    auto res = std::find_if(_scannedPeripherals.begin(), _scannedPeripherals.end(),
        [&](const auto &per) { return per.identifier() == device_name; });
    if (res != _scannedPeripherals.end()) {
        _peripheral = *res.base();
        return this->internal_connect();
    }
    std::cout << device_name << " Not found" << std::endl;
    return EXIT_FAILURE;
}

int BleController::disconnect()
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

int BleController::print_peripheral_services() const
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

bool BleController::isConnected() const
{
    return (_peripheral.initialized() && _peripheral.is_connected());
}

int BleController::print_peripheral_infos() const
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

BleController::~BleController()
{
    if (this->isConnected()) {
        this->disconnect();
    }
}
