#include "Order.h"


int main() {
    
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    Order order;
    vector<Product> catalog = loadCatalogFromFile("catalog.txt");
    string filename = "order.txt";

    int startChoice = 0;

    // --- ПЕРЕВІРКА ТА ЗАВАНТАЖЕННЯ ---
    if (!isFileEmpty(filename)) {
        cout << "Знайдено збережене замовлення.\n";
        cout << "1. Продовжити роботу зі старим замовленням\n";
        cout << "2. Створити нове (старе буде видалено)\n";
        cout << "Вибір: ";
        cin >> startChoice;
        cin.ignore(10000, '\n');

        if (startChoice == 1) {
            order.loadFromFile(filename);
        }
    }
    else {
        cout << "Вітаємо! Створюємо ваше перше замовлення.\n";
    }

    if (startChoice != 1) {
        cout << "\n--- Крок 1: Оберіть товари з каталогу ---\n";
        order.selectFromCatalog(catalog);

        cout << "\n--- Крок 2: Встановіть дату доставки (YYYY-MM-DD): ";
        string date;
        getline(cin, date);
        order.setDeliveryDate(date);

        cout << "\n--- Крок 3: Оберіть тип доставки (0 - Кур'єр, 1 - Самовивіз): ";
        int dtype;
        cin >> dtype;
        cin.ignore(10000, '\n');
        order.setDeliveryType(static_cast<DeliveryType>(dtype));

       
        cout << "\n--- Крок 4: Деталі отримання та оплати ---";
        order.finalizeOrder();
    }

    int mainChoice;
    do {
    
        cout << "\n========== ПОВНА ІНФОРМАЦІЯ ПРО ЗАМОВЛЕННЯ ==========\n";
        order.showFullFinalOrder();

        cout << "\n--- ГОЛОВНЕ МЕНЮ ---";
        cout << "\n1. Редагувати замовлення (товари, доставку, оплату)";
        cout << "\n2. Додати ще товари з каталогу";
        cout << "\n3. Скасувати замовлення";
        cout << "\n4. Зберегти фінальний чек (SAVE) та вийти";
        cout << "\n0. Вийти без збереження чека (зберегти лише стан)";
        cout << "\nВибір: ";

        if (!(cin >> mainChoice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore(10000, '\n');

        switch (mainChoice) {
        case 1:
          
            order.editMenu();
          
            break;

        case 2:
            order.selectFromCatalog(catalog);
            break;

        case 3:
            order.cancelOrder();
            cout << "Замовлення скасовано.\n";
            break;

        case 4:
            cout << "Генерація фінального звіту...\n";
            order.saveToFile("final_report.txt");
            mainChoice = 0; 
            break;

        case 0:
            cout << "Збереження прогресу...\n";
            break;

        default:
            cout << "Невірний вибір.\n";
        }

    } while (mainChoice != 0);

  
    order.saveReport(filename);
    cout << "До побачення!\n";

    return 0;
}