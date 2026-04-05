#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <windows.h>
#define _CRT_SECURE_NO_WARNINGS

using namespace std;



enum class Status {
	Pending,
	Processing,
	Confirmed,
	Preparing,
	Delivering,
	Paid,
	Cancelled
};
enum class DeliveryType {
	Delivery,
	Pickup
};

enum class PaymentType {
	None,
	Card,
	Cash
};

struct Product {
	string name;
	double price; // грн
	int quantity;

	Product(string n = "", double p = 0.0, int q = 1);
};

class Order {
private:
	vector<Product> products;
	string deliveryDate;
	DeliveryType deliveryType;
	Status status;
	PaymentType paymentType;

	string phoneNumber;
	string receiverName;
	string receiverPhone;
	bool otherPerson;

public:
	Order();

	void addProduct(const Product& p);
	void removeProduct(int);
	void changeQuantity(const string& name, int newQuantity);

	void setDeliveryDate(const string& date);
	void setDeliveryType(DeliveryType type);

	double calculateProductsTotal() const;
	double calculateTotalWithDelivery() const;

	void showOrder() const;
	void showFullFinalOrder() const;

	void updateStatusAutomatically();
	void finalizeOrder();

	string getStatusText() const;
	string getStatusProgress() const;

	void loadFromFile(const string& filename);
	void selectFromCatalog(const vector<Product>& catalog);
	void editMenu();
	void cancelOrder();
	void saveToFile(const string&)const;
	void saveReport(const string& filename) const;
};

vector<Product> loadCatalogFromFile(const string& filename);
bool isFileEmpty(const string& );



