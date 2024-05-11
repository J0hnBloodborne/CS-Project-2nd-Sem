#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>

class Button {
private:
    sf::Text text;
    sf::RectangleShape shape;

public:
    Button(const std::string& buttonText, const sf::Font& font, unsigned int fontSize, const sf::Vector2f& position, const sf::Color& textColor, const sf::Color& buttonColor) {
        text.setString(buttonText);
        text.setFont(font);
        text.setCharacterSize(fontSize);
        text.setFillColor(textColor);
        text.setPosition(position.x + 35, position.y + 5); // Adjust text position for centering in the button

        shape.setSize(sf::Vector2f(90, 90)); // Standard button size
        shape.setFillColor(buttonColor);
        shape.setPosition(position);
        shape.setOutlineThickness(2); // Add outline
        shape.setOutlineColor(sf::Color::Black); // Outline color
    }
    Button(const std::string& buttonText, const sf::Font& font, unsigned int fontSize, const sf::Vector2f& position, const sf::Color& textColor, const sf::Color& buttonColor, int width, int height) {
        text.setString(buttonText);
        text.setFont(font);
        text.setCharacterSize(fontSize);
        text.setFillColor(textColor);
        text.setPosition(position.x + 35, position.y + 5); // Adjust text position for centering in the button

        shape.setSize(sf::Vector2f(width, height)); // Standard button size
        shape.setFillColor(buttonColor);
        shape.setPosition(position);
        shape.setOutlineThickness(2); // Add outline
        shape.setOutlineColor(sf::Color::Black); // Outline color
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }

    bool contains(const sf::Vector2f& point) const {
        return shape.getGlobalBounds().contains(point);
    }

    std::string getText() const {
        return text.getString();
    }
};

class VendingMachineSlot {
private:
    std::string name;
    int quantity;
    double price;

public:
    VendingMachineSlot() {
        name = "Empty";
        quantity = 0;
        price = 0.0;
    }

    VendingMachineSlot(std::string name, int quantity, double price) {
        this->name = name;
        this->quantity = quantity;
        this->price = price;
    }

    std::string getName() const {
        return name;
    }

    int getQuantity() const {
        return quantity;
    }

    double getPrice() const {
        return price;
    }

    void setName(const std::string& name) {
        this->name = name;
    }

    void setQuantity(int quantity) {
        this->quantity = quantity;
    }

    void setPrice(double price) {
        this->price = price;
    }

    void operator+=(int x) {
        quantity += x;
    }

    void operator-=(int x) {
        quantity -= x;
    }
};

std::vector<VendingMachineSlot> slot;
std::string selectedItemId = "";

void readFromFile() {
    std::ifstream readFile("Vending Machine Data.txt");
    if (!readFile.is_open()) {
        std::cerr << "Error reading file." << std::endl << "Exiting...";
        exit(1);
    }

    std::string n;
    int q;
    double p;
    while (readFile >> n >> p >> q) {
        slot.emplace_back(n, p, q);
    }

    readFile.close();
}

void writeToFile() {
    std::ofstream writeFile;
    writeFile.open("Vending Machine Data.txt");
    for (int i = 0; i < slot.size(); i++) {
        writeFile << slot[i].getName() << std::endl << slot[i].getPrice() << std::endl << slot[i].getQuantity() << std::endl;
    }

    writeFile.close();
    slot.clear();
}

void UserMode() {
    sf::RenderWindow window(sf::VideoMode(1024, 1000), "Vending Machine");

    sf::Font font;
    if (!font.loadFromFile("game_over.ttf")) {
        std::cerr << "Error loading font." << std::endl;
        return;
    }

    sf::SoundBuffer clickBuffer;
    if (!clickBuffer.loadFromFile("blipSelect.wav")) {
        std::cerr << "Failed to load button click sound." << std::endl;
    }

    sf::SoundBuffer purchaseBuffer;
    if (!purchaseBuffer.loadFromFile("purchase.wav")) {
        std::cerr << "Failed to load purchase sound." << std::endl;
    }

    sf::Sound clickSound(clickBuffer);
    sf::Sound purchaseSound(purchaseBuffer);

    sf::Text title("VENDING MACHINE", font, 72); // Larger title
    title.setPosition(400, 10);
    title.setFillColor(sf::Color::Green);

    sf::RectangleShape itemGrid(sf::Vector2f(600, 600));
    itemGrid.setPosition(50, 120);
    itemGrid.setFillColor(sf::Color(100, 100, 100));

    const int rows = 3;
    const int cols = 3;
    std::vector<Button> itemButtons;
    int itemWidth = itemGrid.getSize().x / cols;
    int itemHeight = itemGrid.getSize().y / rows;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(40, 255); // Range for RGB colors
    for (int i = 0; i < slot.size(); i++) {
        sf::Color bgColor(distribution(generator), distribution(generator), distribution(generator));
        itemButtons.emplace_back(std::to_string(i) + "   " + slot[i].getName(), font, 36, sf::Vector2f(60 + (i % cols) * itemWidth, 130 + (i / cols) * itemHeight), sf::Color::Black, bgColor,160,90);
    }

    sf::Text prompt("Select item ID:", font, 48); // Larger prompt
    prompt.setPosition(700, 200);
    prompt.setFillColor(sf::Color::Yellow);

    sf::RectangleShape numpadGrid(sf::Vector2f(300, 400));
    numpadGrid.setPosition(700, 300);
    numpadGrid.setFillColor(sf::Color(150, 150, 150));

    std::vector<Button> numpadButtons;
    int buttonWidth = numpadGrid.getSize().x / 3;
    int buttonHeight = numpadGrid.getSize().y / 4;
    for (int i = 1; i <= 9; i++) {
        numpadButtons.emplace_back(std::to_string(i), font, 48, sf::Vector2f(700 + (i - 1) % 3 * buttonWidth, 300 + (i - 1) / 3 * buttonHeight), sf::Color::Black, sf::Color::White); // Larger numpad buttons
    }

    Button button0("0", font, 48, sf::Vector2f(700 + buttonWidth, 300 + 3 * buttonHeight), sf::Color::Black, sf::Color::White); // Larger 0 button
    Button buttonX("X", font, 48, sf::Vector2f(800 + buttonWidth, 300 + 3 * buttonHeight), sf::Color::White, sf::Color::Red);
    Button buttonQ("?", font, 48, sf::Vector2f(900 + buttonWidth, 300 + 3 * buttonHeight), sf::Color::Black, sf::Color::Yellow);
    Button purchaseButton("PURCHASE", font, 48, sf::Vector2f(750, 800), sf::Color::White, sf::Color::Green, 180, 90);

    sf::Text collectionTray("COLLECTION TRAY", font, 36); // Collection tray title
    collectionTray.setPosition(400, 900);
    collectionTray.setFillColor(sf::Color::White);

    sf::RectangleShape trayShape(sf::Vector2f(600, 80)); // Collection tray shape
    trayShape.setPosition(50, 920);
    trayShape.setFillColor(sf::Color(200, 200, 200));

    sf::RectangleShape selectedItemShape(sf::Vector2f(110, 40)); // Selected item ID shape
    selectedItemShape.setPosition(830, 220);
    selectedItemShape.setFillColor(sf::Color(30,30,30));
    selectedItemShape.setOutlineThickness(2);
    selectedItemShape.setOutlineColor(sf::Color::Black);
    sf::Font selectedItemFont;
    if (!selectedItemFont.loadFromFile("digital-7.ttf")) {
        std::cerr << "Error loading selected item font." << std::endl;
        return;
    }
    sf::RectangleShape popUpWindow(sf::Vector2f(400, 100)); // Pop-up window shape
    popUpWindow.setPosition(700, 50);
    popUpWindow.setFillColor(sf::Color(30, 30, 30));
    popUpWindow.setOutlineThickness(2);
    popUpWindow.setOutlineColor(sf::Color::Black);

    sf::Text popUpMessage("", selectedItemFont, 35); // Pop-up message text
    popUpMessage.setPosition(710, 55);
    popUpMessage.setFillColor(sf::Color::Blue);
    sf::Text selectedId("", selectedItemFont, 48); // Larger selected ID text
    selectedId.setPosition(830, 210);
    selectedId.setFillColor(sf::Color::Blue);
    sf::Text selectedItemText("", selectedItemFont, 24); // Selected item ID text
    selectedItemText.setPosition(850, 110);
    selectedItemText.setFillColor(sf::Color::Black);
    
    selectedItemText.setFont(selectedItemFont);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    for (auto& button : itemButtons) {
                        if (button.contains(sf::Vector2f(mousePos))) {
                            selectedItemId = button.getText();
                            selectedId.setString(selectedItemId);
                            clickSound.play();
                            break;
                        }
                    }

                    for (auto& button : numpadButtons) {
                        if (button.contains(sf::Vector2f(mousePos))) {
                            selectedItemId += button.getText();
                            selectedId.setString(selectedItemId);
                            clickSound.play();
                            break;
                        }
                    }

                    if (button0.contains(sf::Vector2f(mousePos))) {
                        selectedItemId += "0";
                        selectedId.setString(selectedItemId);
                        clickSound.play();
                    }
                    if (buttonX.contains(sf::Vector2f(mousePos))) {
                        if (!selectedItemId.empty()) {
                            selectedItemId.pop_back();
                            selectedId.setString(selectedItemId);
                            clickSound.play();
                        }
                    }
                    if (buttonQ.contains(sf::Vector2f(mousePos))) {
                        
                    }

                    if (purchaseButton.contains(sf::Vector2f(mousePos))) {
                        int id = std::stoi(selectedItemId);
                        if (id > 0 && id <= slot.size()) {
                            std::string purchasedItem = slot[id - 1].getName();
                            std::cout << "You selected item: " << purchasedItem << std::endl;

                            slot[id - 1] -= 1;
                            if (slot[id - 1].getQuantity() == 0) {
                                slot.erase(slot.begin() + id - 1);
                                itemButtons.clear(); // Clear item buttons
                                for (int i = 0; i < slot.size(); i++) { // Recreate item buttons
                                    sf::Color bgColor(distribution(generator), distribution(generator), distribution(generator));
                                    itemButtons.emplace_back(std::to_string(i) + "   " + slot[i].getName(), font, 36, sf::Vector2f(60 + (i % cols) * itemWidth, 130 + (i / cols) * itemHeight), sf::Color::Black, bgColor);
                                }
                            }
                            writeToFile();
                            readFromFile(); // Update list after purchase
                            selectedItemId.clear();
                            selectedId.setString(selectedItemId);

                            // Display pop-up window for purchase
                            popUpMessage.setString("You purchased:\n\n" + purchasedItem);
                            purchaseSound.play();
                        }
                        else {
                            popUpMessage.setString("Invalid item ID");
                            popUpMessage.setPosition(710, 55); // Reset message position for pop-up
                        }

                        // Show pop-up window
                        popUpWindow.setPosition(700, 50);
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9) {
                    selectedItemId += (event.key.code - sf::Keyboard::Num0) + '0';
                    selectedId.setString(selectedItemId);
                    clickSound.play();
                }
                else if (event.key.code == sf::Keyboard::Backspace) {
                    if (!selectedItemId.empty()) {
                        selectedItemId.pop_back();
                        selectedId.setString(selectedItemId);
                    }
            
                }
            }
        }

        window.clear(sf::Color(0, 0, 0));

        // Draw UI elements
        window.draw(title);
        window.draw(itemGrid);
        window.draw(prompt);
        for (auto& button : numpadButtons)
            button.draw(window);
        button0.draw(window);
        buttonX.draw(window);
        purchaseButton.draw(window);
        for (auto& button : itemButtons)
            button.draw(window);
        window.draw(collectionTray);
        window.draw(trayShape);
        window.draw(selectedItemShape);
        window.draw(selectedItemText);
        window.draw(selectedId);

        // Draw pop-up window if necessary
        if (popUpWindow.getPosition().y < 1000)
            window.draw(popUpWindow);
        window.draw(popUpMessage);

        window.display();
    }
}

int main() {
    readFromFile();
    UserMode();
    return 0;
}
