#include "MockController.hpp"

MockController::MockController()
{
}

MockController::~MockController()
{
}

void MockController::printScannedPeripheral()
{
    std::cout << "Mock []" << std::endl;
}

int MockController::scan(std::uint32_t scan_time_second, bool print_results)
{
    std::cout << "Mock scanning for " << scan_time_second << "s ..." << std::endl;

    (void)print_results;

    return EXIT_SUCCESS;
}

int MockController::connectByIndex(std::size_t index)
{
    std::cout << "Mock connecting to index: " << index << std::endl;
    return EXIT_SUCCESS;
}

int MockController::connectByAddress(const std::string &device_address)
{
    std::cout << "Mock connecting to device address: " << device_address << std::endl;
    return EXIT_SUCCESS;
}

int MockController::connectByName(const std::string &device_name)
{
    std::cout << "Mock connecting to device name: " << device_name << std::endl;
    return EXIT_SUCCESS;
}

int MockController::disconnect()
{
    std::cout << "Mock disconnecting" << std::endl;
    return EXIT_SUCCESS;
}

int MockController::print_peripheral_services()
{
    std::cout << "Mock services []" << std::endl;
    return EXIT_SUCCESS;
}

bool MockController::isConnected()
{
    std::cout << "Mock isConnected" << std::endl;
    return true;
}

int MockController::print_peripheral_infos()
{
    std::cout << "Mock print peripheral infos" << std::endl;
    return EXIT_SUCCESS;
}

ByteArray MockController::read(
    std::string const &service, std::string const &characteristic)
{
    std::cout << "Mock reading service " << service << " characteristic "
              << characteristic << std::endl;
    return "";
}

int MockController::write(BluetoothUUID const &service,
    BluetoothUUID const &characteristic, const ByteArray &payload)
{
    std::cout << "Mock writing service " << service << " characteristic "
              << characteristic << " with value " << payload << std::endl;
    return EXIT_SUCCESS;
}

int MockController::notify(BluetoothUUID const &service,
    BluetoothUUID const &characteristic, std::function<void(ByteArray payload)> callback)
{
    std::cout << "Mock notify service " << service << " characteristic " << characteristic
              << std::endl;
    (void)callback;
    return EXIT_SUCCESS;
}

int MockController::indicate(BluetoothUUID const &service,
    BluetoothUUID const &characteristic, std::function<void(ByteArray payload)> callback)
{
    std::cout << "Mock indicate service " << service << " characteristic "
              << characteristic << std::endl;
    (void)callback;
    return EXIT_SUCCESS;
}

int MockController::unsubscribe(
    BluetoothUUID const &service, BluetoothUUID const &characteristic)
{
    std::cout << "Mock unsubscribe service " << service << " characteristic "
              << characteristic << std::endl;
    return EXIT_SUCCESS;
}
