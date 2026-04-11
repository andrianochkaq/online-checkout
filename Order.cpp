#include "Order.h"

// --- Product ---
Product::Product(string n, double p, int q) : name(n), price(p), quantity(q) {}

// --- Order ---
Order::Order()
    : status(Status::Pending),
    deliveryType(DeliveryType::Delivery),
    paymentType(PaymentType::None),
    otherPerson(false) {}

// --- ДОДАННЯ ТОВАРУ ---
void Order::addProduct(const Product& p) {
    auto it = find_if(products.begin(), products.end(),
        [&](const Product& prod) { return prod.name == p.name; });

    if (it != products.end()) it->quantity += p.quantity;
    else products.push_back(p);

    updateStatusAutomatically();
}



// --- ВИДАЛЕННЯ ---
void Order::removeProduct(int index) {

    if (index > 0 && index <= (int)products.size()) {

        products.erase(products.begin() + (index - 1));
        cout << "Товар номер " << index << " видалено.\n";
    }
    else {
        cout << "Помилка: товару з таким номером не існує!\n";
    }
    }
// --- ЗМІНА КІЛЬКОСТІ ---
void Order::changeQuantity(const string& name, int newQuantity) {
    for (auto& p : products) {
        if (p.name == name) {
            p.quantity = newQuantity;
            break;
        }
    }
    updateStatusAutomatically();
}

// --- НАЛАШТУВАННЯ ДОСТАВКИ ---
void Order::setDeliveryDate(const string& date) {
    deliveryDate = date;
    updateStatusAutomatically();
}

void Order::setDeliveryType(DeliveryType type) {
    deliveryType = type;
    updateStatusAutomatically();
}

// --- СКАСУВАННЯ ---
void Order::cancelOrder() {
    status = Status::Cancelled;
}

// --- РОЗРАХУНКИ ---

double Order::calculateProductsTotal() const {
    double total = 0;
    for (const auto& p : products)
        total += p.price * p.quantity;
    return total;
}

double Order::calculateTotalWithDelivery() const {
    double total = calculateProductsTotal();
    if (deliveryType == DeliveryType::Delivery && total < 500)
        total += 50;
    return total;
}
// --- ВИВІД ---

string Order::getStatusText() const {
    switch (status) {
    case Status::Pending: return " Створено";
    case Status::Processing: return " Зв'язуємось";
    case Status::Confirmed: return " Прийнято";
    case Status::Preparing: return " Готується";
    case Status::Delivering: return " В дорозі";
    case Status::Paid: return " Оплачено";
    case Status::Cancelled: return " Скасовано";
    default: return "Невідомо";
    }
}

string Order::getStatusProgress() const {
    switch (status) {
    case Status::Pending:     return "[●○○○○]";
    case Status::Processing:  return "[●●○○○]";
    case Status::Confirmed:   return "[●●●○○]";
    case Status::Preparing:   return "[●●●●○]";
    case Status::Delivering:  return "[●●●●●]";
    case Status::Paid:        return "[✔✔✔✔✔]";
    case Status::Cancelled:   return "[XXXXX]";
    default:                  return "[-----]";
    }
}
void Order::showOrder() const {
  

    if (products.empty()) {
        cout << "Нема товарів\n";
    }
    else {
        double productsTotal = calculateProductsTotal();
        for (const auto& p : products) {
            cout << p.name << " - " << p.quantity << " шт - "
                << fixed << setprecision(2) << p.price << " грн\n";
        }

        cout << "--------------------------\n";
        cout << "Сума товарів: " << productsTotal << " грн\n";


        if (deliveryType == DeliveryType::Delivery) {
            if (productsTotal < 500) {
                cout << "Доставка кур'єром: 50.00 грн\n";
            }
            else {
                cout << "Доставка кур'єром: Безкоштовно (акція від 500 грн)\n";
            }
        }
        else {
            cout << "Доставка: Самовивіз (0 грн)\n";
        }

        cout << "РАЗОМ ДО ОПЛАТИ: " << calculateTotalWithDelivery() << " грн\n";
    }

    cout << "Статус: " << getStatusText()
        << " " << getStatusProgress() << "\n";}


void Order::showFullFinalOrder() const {
    showOrder();

    cout << "\n--- ДОДАТКОВО ---\n";

    if (paymentType == PaymentType::Card)
        cout << "Оплата: Картка (оплачено)\n";
    else
        cout << "Оплата: Готівка (очікується)\n";

    if (!phoneNumber.empty())
        cout << "Передзвонити: " << phoneNumber << "\n";

    if (deliveryType == DeliveryType::Delivery) {
        if (otherPerson) {
            cout << "Отримувач: " << receiverName << "\n";
            cout << "Телефон: " << receiverPhone << "\n";
        }
        else {
            cout << "Отримувач: замовник\n";
        }
    }
}



void Order::updateStatusAutomatically() {
    if (status == Status::Cancelled) return;

    if (products.empty()) {
        status = Status::Pending;
        return;
    }

    if (deliveryDate.empty()) {
        status = Status::Processing;
        return;
    }

 
    if (status == Status::Paid) return;

   
    switch (status) {
    case Status::Pending:
    case Status::Processing:
        status = Status::Confirmed;
        break;

    case Status::Confirmed:
        status = Status::Preparing;
        break;

    case Status::Preparing:
        status = Status::Delivering;
        break;

    case Status::Delivering:
     
        break;

    default:
        break;
    }
}


void Order::finalizeOrder() {

    {

    // ❌ якщо кошик пустий — просто вихід
    if (products.empty()) {
        cout << "\nНема товарів!\n";
        return;
    }

    // питання про відсутність товару
    cout << "\nЯкщо товару не буде:\n";
    cout << "1. Передзвонити\n2. Замінити автоматично\nВибір: ";

    int ch;
    cin >> ch;
    cin.ignore();

    if (ch == 1) {
        cout << "Номер: ";
        getline(cin, phoneNumber); // зберігаємо номер
    }
    else {
        phoneNumber = "REPLACE"; // маркер для заміни
    }

    // --- Доставка ---

    if (deliveryType == DeliveryType::Delivery) {
        cout << "\nХто отримує?\n1. Я\n2. Інша людина\nВибір: ";
        int ch;
        cin >> ch;
        cin.ignore();

        if (ch == 2) {
            otherPerson = true;
            cout << "Ім'я: ";
            getline(cin, receiverName);
            cout << "Телефон: ";
            getline(cin, receiverPhone);
        }
    }

    cout << "\nОплата:\n1. Картка\n2. Готівка\nВибір: ";
    int pay;
    cin >> pay;

    if (pay == 1) {
        paymentType = PaymentType::Card;
        status = Status::Paid;
    }
    else {
        paymentType = PaymentType::Cash;
        status = Status::Confirmed;
    }

    cout << "\n Замовлення оформлено!\n";
    showFullFinalOrder();
}




// --- ЗАВАНТАЖЕННЯ ---
void Order::loadFromFile(const string& filename) {
    ifstream file(filename);

    if (!file) {
        cerr << "Не відкривається файл для запису !\n";
        return;
    }

    int type, st, n;
    file >> deliveryDate >> type >> st >> n;

    deliveryType = static_cast<DeliveryType>(type);
    status = static_cast<Status>(st);

    products.clear();
    file.ignore();

    for (int i = 0; i < n; ++i) {
        string name;
        double price;
        int quantity;

        getline(file, name);
        file >> price >> quantity;
        file.ignore();

        products.emplace_back(name, price, quantity);
    }

    file.close();

    cout << "Замовлення завантажено з " << filename << "\n";
}


// --- ВИБІР З КАТАЛОГУ ---
void Order::selectFromCatalog(const vector<Product>& catalog) {
    int choice;

    do {
        cout << "\n--- КАТАЛОГ ПРОДУКТІВ ---\n";
        for (size_t i = 0; i < catalog.size(); ++i)
            cout << i + 1 << ". " << catalog[i].name << " - " << catalog[i].price << " грн\n";

        cout << "0. кінець\nВибір: ";
        cin >> choice;
        cin.ignore();

        if (choice > 0 && choice <= (int)catalog.size()) {
            int quantity;
            cout << "Кількість: ";
            cin >> quantity;
            cin.ignore();

            addProduct(Product(catalog[choice - 1].name,
                catalog[choice - 1].price,
                quantity));
        }

    } while (choice != 0);
}

// --- МЕНЮ ---
void Order::editMenu() {
    int choice;

    do {
        showOrder();

        cout << "\n--- МЕНЮ ---\n";
        cout << "1. додати товар\n";
        cout << "2. видалити товар\n";
        cout << "3. змінити кількість\n";
        cout << "4. змінити час доставки\n";
        cout << "5. змінити тип доставки\n";
        cout << "6. скасувати замовлення\n";
        cout << "7. Змінити отримувача (тільки для кур'єра)\n";
        cout << "0. вихід\n";
        cout << "Вибір: ";

        cin >> choice;
        cin.ignore(10000, '\n');

        if (choice == 1) {
            string name;
            double price;
            int quantity;

            cout << "Товар: ";
            getline(cin, name);
            cout << "Ціна: ";
            cin >> price;
            cout << "Кількість: ";
            cin >> quantity;
            cin.ignore();

            addProduct(Product(name, price, quantity));
        }
        else if (choice == 2) {
            int index;
            cout << "Введіть номер товару для видалення (зі списку вище): ";

            if (cin >> index) {
                removeProduct(index);
            }
            else 
                cout << "Помилка: введіть число!\n";
            
        }
        else if (choice == 3) {
            string name;
            int quantity;

            cout << "Товар: ";
            getline(cin, name);
            cout << "Кількість: ";
            cin >> quantity;
            cin.ignore();

            changeQuantity(name, quantity);
        }
        else if (choice == 4) {
            string date;
            cout << "Час доставки (YYYY-MM-DD): ";
            getline(cin, date);
            setDeliveryDate(date);
        }
        else if (choice == 5) {
            int t;
            cout << "Тип доставки (0-Кур'єр, 1-Самовивіз): ";
            cin >> t;
            cin.ignore();

            setDeliveryType(static_cast<DeliveryType>(t));
        }
        else if (choice == 6) {
            cancelOrder();
        }
        else if (choice == 7) { 
            if (deliveryType == DeliveryType::Delivery) {
                cout << "1. Я особисто\n2. Інша людина\nВибір: ";
                int ch; cin >> ch; cin.ignore();
                if (ch == 2) {
                    otherPerson = true;
                    cout << "Ім'я: "; getline(cin, receiverName);
                    cout << "Телефон: "; getline(cin, receiverPhone);
                }
                else {
                    otherPerson = false;
                }
            }
            else {
                cout << "Цей пункт доступний лише для доставки кур'єром!\n";
            }
        }

    } while (choice != 0);
}

// --- КАТАЛОГ ---
vector<Product> loadCatalogFromFile(const string& filename) {
    vector<Product> catalog;
    ifstream file(filename);

    if (!file) {
        cerr << "Не відкривається каталог з товарами!\n";
        return catalog;
    }

    string name;
    double price;

    while (file >> name >> price)
        catalog.emplace_back(name, price);

    file.close();
    return catalog;
}

void Order::saveToFile(const string& filename) const {
    ofstream file(filename);

    if (!file) {
        cerr << "Помилка відкриття файлу для запису!\n";
        return;
    }

    // Використовуємо ту саму логіку, що й у showFullFinalOrder
    file << "===== ЗАМОВЛЕННЯ =====\n";

    if (products.empty()) {
        file << "Нема товарів\n";
    }
    else {
        double productsTotal = calculateProductsTotal();
        for (const auto& p : products) {
            file << p.name << " - " << p.quantity << " шт - "
                << fixed << setprecision(2) << p.price << " грн\n";
        }

        file << "--------------------------\n";
        file << "Сума товарів: " << productsTotal << " грн\n";

     
        if (deliveryType == DeliveryType::Delivery) {
            if (productsTotal < 500) {
                file << "Доставка кур'єром: 50.00 грн\n";
            }
            else {
                file << "Доставка кур'єром: Безкоштовно (акція від 500 грн)\n";
            }
        }
        else {
            file << "Доставка: Самовивіз (0 грн)\n";
        }

        file << "РАЗОМ ДО ОПЛАТИ: " << calculateTotalWithDelivery() << " грн\n";
    }

    file << "Статус: " << getStatusText()
        << " " << getStatusProgress() << "\n";

    file << "\n--- ДОДАТКОВО ---\n";

    if (paymentType == PaymentType::Card)
        file << "Оплата: Картка (оплачено)\n";
    else
        file << "Оплата: Готівка (очікується)\n";

    file << "Якщо товару не буде: ";

if (phoneNumber == "REPLACE") {
    file << "Замінити (ціна може бути змінена)\n";
}
else if (!phoneNumber.empty()) {
    file << "Передзвонити: " << phoneNumber << "\n";
}
else {
    file << "Не вказано\n";
}
    if (deliveryType == DeliveryType::Delivery) {
        if (otherPerson) {
            file << "Отримувач: " << receiverName << "\n";
            file << "Телефон: " << receiverPhone << "\n";
        }
        else {
            file << "Отримувач: замовник\n";
        }
    }

    file.close();
    cout << "Фінальний звіт збережено у файл: " << filename << "\n";
}

void Order::saveReport(const string& filename) const {
    ofstream file(filename);
    if (!file) return;

    // Зберігаємо тільки чисті дані через пробіл
    file << (deliveryDate.empty() ? "no_date" : deliveryDate) << " "
        << static_cast<int>(deliveryType) << " "
        << static_cast<int>(status) << " "
        << products.size() << "\n";

    for (const auto& p : products) {
        file << p.name << "\n" << p.price << " " << p.quantity << "\n";
    }
    file.close();
}

bool isFileEmpty(const string& filename) {
    ifstream file(filename);
    return file.peek() == ifstream::traits_type::eof();
}