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
    Button(const std::string& buttonText, const sf::Font& font, unsigned int fontSize, const sf::Vector2f& position, const sf::Color& textColor, const sf::Color& buttonColor,const int width, const int height) {
        text.setString(buttonText);
        text.setFont(font);
        text.setCharacterSize(fontSize);
        text.setFillColor(textColor);
        text.setPosition(position.x + 40, position.y); // Adjust text position for centering in the button

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
        slot.emplace_back(n,p,q);
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
    std::cout << std::endl;
}

void UserMode() {
    sf::RenderWindow window(sf::VideoMode(1024,840), "Vending Machine");

    sf::Font font;
    if (!font.loadFromFile("game_over.ttf")) {
        std::cerr << "Error loading font." << std::endl;
        return;
    }

    sf::SoundBuffer clickBuffer;
    if (!clickBuffer.loadFromFile("click.wav")) {
        std::cerr << "Failed to load button click sound." << std::endl;
    }

    sf::Sound clickSound(clickBuffer);

    sf::Text title("VENDING MACHINE", font, 72); // Larger title
    title.setPosition(300, 50);
    title.setFillColor(sf::Color::Green);

    sf::RectangleShape itemGrid(sf::Vector2f(600, 600));
    itemGrid.setPosition(50, 100);
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
        itemButtons.emplace_back(std::to_string(i) + "   " + slot[i].getName(), font, 36, sf::Vector2f(1+ (i % cols) * itemWidth, 100 + (i / cols) * itemHeight), sf::Color::Black, bgColor,150,90);
    }

    sf::Text prompt("Select item ID:", font, 48); // Larger prompt
    prompt.setPosition(700, 200);
    prompt.setFillColor(sf::Color::Yellow);

    sf::Text selectedId("", font, 48); // Larger selected ID text
    selectedId.setPosition(850, 200);
    selectedId.setFillColor(sf::Color::Red);

    sf::RectangleShape numpadGrid(sf::Vector2f(300, 400));
    numpadGrid.setPosition(700, 200);
    numpadGrid.setFillColor(sf::Color(150, 150, 150));

    std::vector<Button> numpadButtons;
    int buttonWidth = numpadGrid.getSize().x / 3;
    int buttonHeight = numpadGrid.getSize().y / 4;
    for (int i = 1; i <= 9; i++) {
        numpadButtons.emplace_back(std::to_string(i), font, 48, sf::Vector2f(700 + (i - 1) % 3 * buttonWidth, 300 + (i - 1) / 3 * buttonHeight), sf::Color::Black, sf::Color::White); // Larger numpad buttons
    }

    Button button0("0", font, 48, sf::Vector2f(700 + buttonWidth, 300 + 3 * buttonHeight), sf::Color::Black, sf::Color::White); // Larger 0 button
    Button purchaseButton("PURCHASE", font, 48, sf::Vector2f(750, 700), sf::Color::White, sf::Color::Blue,180,90);

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

                    if (purchaseButton.contains(sf::Vector2f(mousePos))) {
                        int id = std::stoi(selectedItemId);
                        if (id > 0 && id <= slot.size()) {
                            std::cout << "You selected item: " << slot[id - 1].getName() << std::endl;
                            slot[id - 1] -= 1;
                            if (slot[id - 1].getQuantity() == 0) {
                                slot.erase(slot.begin() + id - 1);
                            }
                            writeToFile();
                            readFromFile(); // Update list after purchase
                            selectedItemId.clear();
                            selectedId.setString(selectedItemId);
                        }
                        else {
                            std::cout << "Invalid item ID." << std::endl;
                        }
                        clickSound.play();
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
                else if (event.key.code == sf::Keyboard::Return) {
                    int id = std::stoi(selectedItemId);
                    if (id > 0 && id <= slot.size()) {
                        std::cout << "You selected item: " << slot[id - 1].getName() << std::endl;
                        slot[id - 1] -= 1;
                        if (slot[id - 1].getQuantity() == 0) {
                            slot.erase(slot.begin() + id - 1);
                        }
                        writeToFile();
                        readFromFile(); // Update list after purchase
                        selectedItemId.clear();
                        selectedId.setString(selectedItemId);
                    }
                    else {
                        std::cout << "Invalid item ID." << std::endl;
                    }
                    clickSound.play();
                }
            }
        }

        window.clear(sf::Color(30, 30, 30));
        window.draw(numpadGrid);

        window.draw(title);
        window.draw(itemGrid);
 
        window.draw(prompt);
        window.draw(selectedId);
        for (auto& button : numpadButtons)
            button.draw(window);
        button0.draw(window);
        purchaseButton.draw(window);
        for (auto& button : itemButtons)
            button.draw(window);

        window.display();
    }
}

int main() {
    readFromFile();
    UserMode();
    return 0;
}
