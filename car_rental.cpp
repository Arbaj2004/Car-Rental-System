#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

// Base class
class Vehicle {
private:
    std::string vehicleId;
    std::string brand;
    std::string model;
    double basePricePerDay;
    bool isAvailable;

public:
    Vehicle(const std::string& vehicleId, const std::string& brand, const std::string& model, double basePricePerDay)
        : vehicleId(vehicleId), brand(brand), model(model), basePricePerDay(basePricePerDay), isAvailable(true) {}

    virtual ~Vehicle() = default; // Virtual destructor for polymorphic base class

    std::string getVehicleId() const {
        return vehicleId;
    }

    std::string getBrand() const {
        return brand;
    }

    std::string getModel() const {
        return model;
    }

    virtual double calculatePrice(int rentalDays) const {
        return basePricePerDay * rentalDays;
    }

    bool available() const {
        return isAvailable;
    }

    void rent() {
        isAvailable = false;
    }

    void returnVehicle() {
        isAvailable = true;
    }
};

// Derived class
class Car : public Vehicle {
public:
    Car(const std::string& vehicleId, const std::string& brand, const std::string& model, double basePricePerDay)
        : Vehicle(vehicleId, brand, model, basePricePerDay) {}

    double calculatePrice(int rentalDays) const override {
        // Custom price calculation if needed
        return Vehicle::calculatePrice(rentalDays);
    }
};

class Customer {
private:
    std::string customerId;
    std::string name;

public:
    Customer(const std::string& customerId, const std::string& name)
        : customerId(customerId), name(name) {}

    std::string getCustomerId() const {
        return customerId;
    }

    std::string getName() const {
        return name;
    }
};

class Rental {
private:
    Vehicle* vehicle;
    Customer* customer;
    int days;

public:
    Rental(Vehicle* vehicle, Customer* customer, int days)
        : vehicle(vehicle), customer(customer), days(days) {}

    Vehicle* getVehicle() const {
        return vehicle;
    }

    Customer* getCustomer() const {
        return customer;
    }

    int getDays() const {
        return days;
    }
};

class CarRentalSystem {
private:
    std::vector<Vehicle*> vehicles;
    std::vector<Customer> customers;
    std::vector<Rental> rentals;

public:
    ~CarRentalSystem() {
        for (auto vehicle : vehicles) {
            delete vehicle;
        }
    }

    void addVehicle(Vehicle* vehicle) {
        vehicles.push_back(vehicle);
    }

    void addCustomer(const Customer& customer) {
        customers.push_back(customer);
    }

    void rentVehicle(Vehicle* vehicle, Customer* customer, int days) {
        if (vehicle->available()) {
            vehicle->rent();
            rentals.emplace_back(vehicle, customer, days);
        } else {
            std::cout << "Vehicle is not available for rent." << std::endl;
        }
    }

    void returnVehicle(Vehicle* vehicle) {
        vehicle->returnVehicle();
        for (auto it = rentals.begin(); it != rentals.end(); ++it) {
            if (it->getVehicle()->getVehicleId() == vehicle->getVehicleId()) {
                rentals.erase(it);
                break;
            }
        }
    }

    void menu() {
        while (true) {
            std::cout << "===== Car Rental System =====" << std::endl;
            std::cout << "1. Rent a Vehicle" << std::endl;
            std::cout << "2. Return a Vehicle" << std::endl;
            std::cout << "3. Exit" << std::endl;
            std::cout << "Enter your choice: ";

            int choice;
            std::cin >> choice;
            std::cin.ignore(); // Consume newline

            if (choice == 1) {
                std::cout << "\n== Rent a Vehicle ==\n" << std::endl;
                std::cout << "Enter your name: ";
                std::string customerName;
                std::cin.ignore();
                std::getline(std::cin, customerName);

                std::cout << "\nAvailable Vehicles:" << std::endl;
                for (const auto& vehicle : vehicles) {
                    if (vehicle->available()) {
                        std::cout << vehicle->getVehicleId() << " - " << vehicle->getBrand() << " " << vehicle->getModel() << std::endl;
                    }
                }

                std::cout << "\nEnter the vehicle ID you want to rent: ";
                std::string vehicleId;
                std::getline(std::cin, vehicleId);

                std::cout << "Enter the number of days for rental: ";
                int rentalDays;
                std::cin >> rentalDays;
                std::cin.ignore(); // Consume newline

                Customer newCustomer("CUS" + std::to_string(customers.size() + 1), customerName);
                addCustomer(newCustomer);

                Vehicle* selectedVehicle = nullptr;
                for (auto& vehicle : vehicles) {
                    if (vehicle->getVehicleId() == vehicleId && vehicle->available()) {
                        selectedVehicle = vehicle;
                        break;
                    }
                }

                if (selectedVehicle != nullptr) {
                    double totalPrice = selectedVehicle->calculatePrice(rentalDays);
                    std::cout << "\n== Rental Information ==\n" << std::endl;
                    std::cout << "Customer ID: " << newCustomer.getCustomerId() << std::endl;
                    std::cout << "Customer Name: " << newCustomer.getName() << std::endl;
                    std::cout << "Vehicle: " << selectedVehicle->getBrand() << " " << selectedVehicle->getModel() << std::endl;
                    std::cout << "Rental Days: " << rentalDays << std::endl;
                    std::cout << "Total Price: $" << std::fixed << std::setprecision(2) << totalPrice << std::endl;

                    std::cout << "\nConfirm rental (Y/N): ";
                    std::string confirm;
                    std::cin >> confirm;
                    std::cin.ignore(); // Consume newline

                    if (confirm == "Y" || confirm == "y") {
                        rentVehicle(selectedVehicle, &customers.back(), rentalDays);
                        std::cout << "\nVehicle rented successfully." << std::endl;
                    } else {
                        std::cout << "\nRental canceled." << std::endl;
                    }
                } else {
                    std::cout << "\nInvalid vehicle selection or vehicle not available for rent." << std::endl;
                }
            } else if (choice == 2) {
                std::cout << "\n== Return a Vehicle ==\n" << std::endl;
                std::cout << "Enter the vehicle ID you want to return: ";
                std::string vehicleId;
                std::getline(std::cin, vehicleId);

                Vehicle* vehicleToReturn = nullptr;
                for (auto& vehicle : vehicles) {
                    if (vehicle->getVehicleId() == vehicleId && !vehicle->available()) {
                        vehicleToReturn = vehicle;
                        break;
                    }
                }

                if (vehicleToReturn != nullptr) {
                    Customer* customer = nullptr;
                    for (const auto& rental : rentals) {
                        if (rental.getVehicle()->getVehicleId() == vehicleToReturn->getVehicleId()) {
                            customer = rental.getCustomer();
                            break;
                        }
                    }

                    if (customer != nullptr) {
                        returnVehicle(vehicleToReturn);
                        std::cout << "Vehicle returned successfully by " << customer->getName() << std::endl;
                    } else {
                        std::cout << "Vehicle was not rented or rental information is missing." << std::endl;
                    }
                } else {
                    std::cout << "Invalid vehicle ID or vehicle is not rented." << std::endl;
                }
            } else if (choice == 3) {
                break;
            } else {
                std::cout << "Invalid choice. Please enter a valid option." << std::endl;
            }
        }

        std::cout << "\nThank you for using the Car Rental System!" << std::endl;
    }
};

int main() {
    CarRentalSystem rentalSystem;

    Vehicle* car1 = new Car("C001", "Toyota", "Camry", 60.0);
    Vehicle* car2 = new Car("C002", "Honda", "Accord", 70.0);
    Vehicle* car3 = new Car("C003", "Mahindra", "Thar", 150.0);
    rentalSystem.addVehicle(car1);
    rentalSystem.addVehicle(car2);
    rentalSystem.addVehicle(car3);

    rentalSystem.menu();

    return 0;
}
