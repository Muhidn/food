#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FOODS 5
#define MAX_USERS 10

typedef struct {
    char name[50];
    float price;
} FoodItem;

typedef struct {
    FoodItem food;
    int quantity;
} Order;

typedef struct {
    Order orders[MAX_FOODS];
    int front;
    int rear;
} OrderQueue;

typedef struct {
    char username[50];
    char password[50];
    int registered;
} User;

typedef struct {
    OrderQueue orders;
    User* users;
    int totalUsers;
} Admin;

void initializeQueue(OrderQueue* queue);
int isQueueEmpty(OrderQueue* queue);
int isQueueFull(OrderQueue* queue);
void enqueue(OrderQueue* queue, FoodItem food, int quantity);
void dequeue(OrderQueue* queue);
void displayQueue(OrderQueue* queue);
void registerUser(User* users, int* totalUsers);
int loginUser(User* users, int totalUsers);
void addOrder(OrderQueue* queue, FoodItem* menu, int loggedIn);
void showMenu(FoodItem* menu);
void clearScreen();

int main() {
    FoodItem menu[MAX_FOODS] = {{"Burger", 5.99}, {"Pizza", 8.99}, {"Fries", 2.99}, {"Salad", 4.99}, {"Ice Cream", 3.99}};
    OrderQueue orderQueue;
    initializeQueue(&orderQueue);

    User users[MAX_USERS];
    int totalUsers = 0;

    Admin admin;
    admin.orders = orderQueue;
    admin.users = users;
    admin.totalUsers = totalUsers;

    int loggedIn = 0; // Flag to track user login status

    int choice;
    do {
        printf("\n--- Order Management System ---\n");
        printf("1. Register\n");
        printf("2. Login\n");
        if (loggedIn) {
            printf("3. Add Order\n");
            printf("6. Exit\n");
        } else {
            printf("6. Exit\n");
        }
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerUser(users, &totalUsers);
                break;
            case 2:
                loggedIn = loginUser(users, totalUsers);
                break;
            case 3:
                if (loggedIn) {
                    addOrder(&orderQueue, menu, loggedIn);
                    printf("Order added successfully.\n");
                    printf("Thank you for your order!\n");
                    printf("Exiting the program.\n");
                    choice = 6;
                } else {
                    printf("Please login first.\n");
                }
                break;
            case 6:
                printf("Exiting the program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 6);

    return 0;
}

void initializeQueue(OrderQueue* queue) {
    queue->front = -1;
    queue->rear = -1;
}

int isQueueEmpty(OrderQueue* queue) {
    return (queue->front == -1 && queue->rear == -1);
}

int isQueueFull(OrderQueue* queue) {
    return ((queue->rear + 1) % MAX_FOODS == queue->front);
}

void enqueue(OrderQueue* queue, FoodItem food, int quantity) {
    if (isQueueFull(queue)) {
        printf("Order queue is full. Cannot add more orders.\n");
        return;
    }

    Order order;
    order.food = food;
    order.quantity = quantity;

    if (isQueueEmpty(queue)) {
        queue->front = 0;
        queue->rear = 0;
    } else {
        queue->rear = (queue->rear + 1) % MAX_FOODS;
    }

    queue->orders[queue->rear] = order;
}

void dequeue(OrderQueue* queue) {
    if (isQueueEmpty(queue)) {
        printf("Order queue is empty. No orders to dequeue.\n");
        return;
    }

    if (queue->front == queue->rear) {
        queue->front = -1;
        queue->rear = -1;
    } else {
        queue->front = (queue->front + 1) % MAX_FOODS;
    }
}

void displayQueue(OrderQueue* queue) {
    if (isQueueEmpty(queue)) {
        printf("Order queue is empty.\n");
        return;
    }

    printf("Current Orders in the Queue:\n");
    int i;
    int orderCount = (queue->rear - queue->front + MAX_FOODS) % MAX_FOODS + 1;
    for (i = 0; i < orderCount; i++) {
        int index = (queue->front + i) % MAX_FOODS;
        Order order = queue->orders[index];
        printf("%d. %s (Quantity: %d)\n", i + 1, order.food.name, order.quantity);
    }
}

void registerUser(User* users, int* totalUsers) {
    if (*totalUsers == MAX_USERS) {
        printf("Maximum number of users reached. Cannot register more users.\n");
        return;
    }

    printf("Enter username: ");
    scanf("%s", users[*totalUsers].username);
    printf("Enter password: ");
    scanf("%s", users[*totalUsers].password);
    users[*totalUsers].registered = 1;

    (*totalUsers)++;
}

int loginUser(User* users, int totalUsers) {
    char username[50];
    char password[50];

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    int i;
    for (i = 0; i < totalUsers; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            printf("Login successful!\n");
            return 1;
        }
    }

    printf("Invalid username or password. Login failed.\n");
    return 0;
}

void addOrder(OrderQueue* queue, FoodItem* menu, int loggedIn) {
    int choice;
    int quantity;

    do {
        showMenu(menu);
        printf("Enter food choice (1-%d): ", MAX_FOODS);
        scanf("%d", &choice);

        if (choice < 1 || choice > MAX_FOODS) {
            printf("Invalid food choice. Please try again.\n");
        }
    } while (choice < 1 || choice > MAX_FOODS);

    printf("Enter quantity: ");
    scanf("%d", &quantity);

    enqueue(queue, menu[choice - 1], quantity);
}

void showMenu(FoodItem* menu) {
    printf("\n--- Menu ---\n");
    int i;
    for (i = 0; i < MAX_FOODS; i++) {
        printf("%d. %s ($%.2f)\n", i + 1, menu[i].name, menu[i].price);
    }
}

void clearScreen() {
    system("clear");
}
