#include <gtest/gtest.h>
#include "Order.h"

// ===============================
// TEST FIXTURE
// ===============================
// Це базовий клас для всіх тестів.
// Він створює новий Order перед кожним тестом,
// щоб тести не впливали один на одного.
class OrderTest : public ::testing::Test {
protected:
    Order order;

    void SetUp() override {
        order = Order(); // новий чистий об'єкт перед кожним тестом
    }
};

// ===============================
// 1. ADD PRODUCT
// ===============================
// Перевіряємо: чи однакові товари об'єднуються (по name)
TEST_F(OrderTest, AddProduct_MergesSameName) {
    order.addProduct(Product("Apple", 10.0, 2));
    order.addProduct(Product("Apple", 10.0, 3));

    // 2 + 3 = 5 шт * 10 = 50
    EXPECT_EQ(order.calculateProductsTotal(), 50.0);
}

// ===============================
// 2. ADD DIFFERENT PRODUCTS
// ===============================
// Перевіряємо суму різних товарів
TEST_F(OrderTest, AddDifferentProducts) {
    order.addProduct(Product("Apple", 10.0, 2));
    order.addProduct(Product("Banana", 5.0, 4));

    // 20 + 20 = 40
    EXPECT_EQ(order.calculateProductsTotal(), 40.0);
}

// ===============================
// 3. REMOVE PRODUCT
// ===============================
// Перевіряємо видалення товару по індексу
TEST_F(OrderTest, RemoveProduct) {
    order.addProduct(Product("Apple", 10.0, 2));
    order.addProduct(Product("Banana", 5.0, 4));

    order.removeProduct(1); // видаляємо Apple

    // залишився тільки Banana: 5 * 4 = 20
    EXPECT_EQ(order.calculateProductsTotal(), 20.0);
}

// ===============================
// 4. CHANGE QUANTITY
// ===============================
// Перевіряємо зміну кількості товару
TEST_F(OrderTest, ChangeQuantity) {
    order.addProduct(Product("Apple", 10.0, 2));

    order.changeQuantity("Apple", 5);

    // 10 * 5 = 50
    EXPECT_EQ(order.calculateProductsTotal(), 50.0);
}

// ===============================
// 5. DELIVERY UNDER 500
// ===============================
// Якщо сума < 500 → доставка +50
TEST_F(OrderTest, DeliveryCost_Under500) {
    order.setDeliveryType(DeliveryType::Delivery);

    order.addProduct(Product("Apple", 100.0, 4)); // 400

    // 400 + 50 доставка = 450
    EXPECT_EQ(order.calculateTotalWithDelivery(), 450.0);
}

// ===============================
// 6. FREE DELIVERY OVER 500
// ===============================
// Якщо >= 500 → доставка безкоштовна
TEST_F(OrderTest, DeliveryFreeOver500) {
    order.setDeliveryType(DeliveryType::Delivery);

    order.addProduct(Product("Apple", 200.0, 3)); // 600

    EXPECT_EQ(order.calculateTotalWithDelivery(), 600.0);
}

// ===============================
// 7. PICKUP DELIVERY
// ===============================
// Самовивіз = без доставки
TEST_F(OrderTest, PickupNoDeliveryCost) {
    order.setDeliveryType(DeliveryType::Pickup);

    order.addProduct(Product("Apple", 100.0, 2));

    EXPECT_EQ(order.calculateTotalWithDelivery(), 200.0);
}

// ===============================
// 8. EMPTY ORDER
// ===============================
// Перевірка порожнього замовлення
TEST_F(OrderTest, EmptyOrder) {
    EXPECT_EQ(order.calculateProductsTotal(), 0);
}

// ===============================
// 9. CANCEL ORDER
// ===============================
// Перевіряємо скасування замовлення
TEST_F(OrderTest, CancelOrder) {
    order.addProduct(Product("Apple", 10.0, 1));

    order.cancelOrder();

    // статус має бути Cancelled
    EXPECT_EQ(order.getStatusText(), " Скасовано");
}

// ===============================
// 10. STATUS AUTO UPDATE
// ===============================
// Перевіряємо, що статус змінюється після додавання товару
TEST_F(OrderTest, StatusChangesWhenProductsAdded) {
    order.addProduct(Product("Apple", 10.0, 1));

    // статус НЕ повинен бути "Створено"
    EXPECT_NE(order.getStatusText(), " Створено");
}