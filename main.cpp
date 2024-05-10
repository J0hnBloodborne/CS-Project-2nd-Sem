#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

class Button
{
private:
    sf::RectangleShape shape;
    sf::Text text;

public:
    Button(sf::Font& font, const std::string& buttonText, unsigned int textSize, const sf::Vector2f& size, const sf::Vector2f& position)
    {
        shape.setSize(size);
        shape.setPosition(position);
        shape.setFillColor(sf::Color::White);
        shape.setOutlineThickness(2);
        shape.setOutlineColor(sf::Color::Black);

        text.setString(buttonText);
        text.setFont(font);
        text.setCharacterSize(textSize);
        text.setFillColor(sf::Color::Black);

        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(position.x + size.x / 2.0f, position.y + size.y / 2.0f);
    }

    bool contains(const sf::Vector2f& point) const
    {
        return shape.getGlobalBounds().contains(point);
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(shape);
        window.draw(text);
    }
};

class VendingMachineSlot
{
private:
    std::string name;
    int quantity;
    double price;

public:
    VendingMachineSlot()
    {
        name = "Empty";
        quantity = 0;
        price = 0.0;
    }

    VendingMachineSlot(std::string name, int quantity, double price)
    {
        this->name = name;
        this->quantity = quantity;
        this->price = price;
    }

    std::string getName() const
    {
        return name;
    }

    int getQuantity() const
    {
        return quantity;
    }

    double getPrice() const
    {
        return price;
    }

    void setName(const std::string& name)
    {
        this->name = name;
    }

    void setQuantity(int quantity)
    {
        this->quantity = quantity;
    }

    void setPrice(double price)
    {
        this->price = price;
    }

    void operator+=(int x)
    {
        quantity += x;
    }

    void operator-=(int x)
    {
        quantity -= x;
    }
};

std::vector<VendingMachineSlot> slot;
std::string selectedItemId = "";

void readFromFile()
{
    std::ifstream readFile("Vending Machine Data.txt");
    if (!readFile.is_open())
    {
        std::cerr << "Error reading file." << std::endl << "Exiting...";
        exit(1);
    }

    slot.clear();

    std::string n;
    int q;
    double p;
    while (readFile >> n >> p >> q)
    {
        VendingMachineSlot temp;
        temp.setName(n);
        temp.setPrice(p);
        temp.setQuantity(q);
        slot.push_back(temp);
    }

    readFile.close();
}

void writeToFile()
{
    std::ofstream writeFile;
    writeFile.open("Vending Machine Data.txt");
    for (int i = 0; i < slot.size(); i++)
    {
        writeFile << slot[i].getName() << std::endl << slot[i].getPrice() << std::endl << slot[i].getQuantity() << std::endl;
    }

    writeFile.close();
}

void UserMode()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Vending Machine");

    sf::Font font;
    if (!font.loadFromFile("game_over.ttf")) {
        std::cerr << "Error loading font." << std::endl;
        return;
    }

    readFromFile();

    sf::Text title("VENDING MACHINE", font, 48);
    title.setPosition(200, 50);
    title.setFillColor(sf::Color::Green);

    sf::RectangleShape itemGrid(sf::Vector2f(400, 400));
    itemGrid.setPosition(50, 100);
    itemGrid.setFillColor(sf::Color(100, 100, 100));

    const int rows = 3;
    const int cols = 3;
    std::vector<sf::Text> itemTexts;
    int itemWidth = itemGrid.getSize().x / cols;
    int itemHeight = itemGrid.getSize().y / rows;
    for (int i = 0; i < slot.size(); i++)
    {
        std::string itemInfo = std::to_string(i + 1) + " " + slot[i].getName() + " ($" + std::to_string(slot[i].getPrice()) + ")";
        sf::Text item(itemInfo, font, 24);
        item.setPosition(50 + (i % cols) * itemWidth, 100 + (i / cols) * itemHeight);
        item.setFillColor(sf::Color::White);
        itemTexts.push_back(item);
    }

    sf::Text prompt("Select item ID:", font, 30);
    prompt.setPosition(100, 520);
    prompt.setFillColor(sf::Color::Yellow);

    sf::Text selectedId("", font, 30);
    selectedId.setPosition(350, 520);
    selectedId.setFillColor(sf::Color::Red);

    sf::RectangleShape numpadGrid(sf::Vector2f(300, 300));
    numpadGrid.setPosition(500, 100);
    numpadGrid.setFillColor(sf::Color(150, 150, 150));

    std::vector<Button> numpadButtons;
    int buttonSize = 80;
    int buttonPadding = 10;
    for (int i = 1; i <= 9; i++)
    {
        int col = (i - 1) % 3;
        int row = (i - 1) / 3;
        sf::Vector2f buttonPos(500 + col * (buttonSize + buttonPadding), 100 + row * (buttonSize + buttonPadding));
        Button button(font, std::to_string(i), 24, sf::Vector2f(buttonSize, buttonSize), buttonPos);
        numpadButtons.push_back(button);
    }

    sf::Vector2f zeroButtonPos(500 + buttonSize + buttonPadding, 100 + 3 * (buttonSize + buttonPadding));
    Button button0(font, "0", 24, sf::Vector2f(buttonSize, buttonSize), zeroButtonPos);

    sf::Vector2f purchaseButtonPos(600, 500);
    Button purchaseButton(font, "PURCHASE", 30, sf::Vector2f(150, 50), purchaseButtonPos);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    for (int i = 0; i < numpadButtons.size(); i++)
                    {
                        if (numpadButtons[i].contains(sf::Vector2f(mousePos)))
                        {
                            selectedItemId += std::to_string(i + 1);
                            selectedId.setString(selectedItemId);
                            break;
                        }
                    }

                    if (button0.contains(sf::Vector2f(mousePos)))
                    {
                        selectedItemId += "0";
                        selectedId.setString(selectedItemId);
                    }

                    if (purchaseButton.contains(sf::Vector2f(mousePos)))
                    {
                        int id = std::stoi(selectedItemId);
                        if (id > 0 && id <= slot.size())
                        {
                            std::cout << "You selected item: " << slot[id - 1].getName() << std::endl;
                            slot[id - 1] -= 1;
                            if (slot[id - 1].getQuantity() == 0)
                            {
                                slot.erase(slot.begin() + id - 1);
                            }
                            writeToFile();
                            readFromFile(); // Update list after purchase
                            selectedItemId.clear();
                            selectedId.setString(selectedItemId);
                        }
                        else
                        {
                            std::cout << "Invalid item ID." << std::endl;
                        }
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9)
                {
                    selectedItemId += (event.key.code - sf::Keyboard::Num0) + '0';
                    selectedId.setString(selectedItemId);
                }
                else if (event.key.code == sf::Keyboard::Backspace)
                {
                    if (!selectedItemId.empty())
                    {
                        selectedItemId.pop_back();
                        selectedId.setString(selectedItemId);
                    }
                }
                else if (event.key.code == sf::Keyboard::Return)
                {
                    int id = std::stoi(selectedItemId);
                    if (id > 0 && id <= slot.size())
                    {
                        std::cout << "You selected item: " << slot[id - 1].getName() << std::endl;
                        slot[id - 1] -= 1;
                        if (slot[id - 1].getQuantity() == 0)
                        {
                            slot.erase(slot.begin() + id - 1);
                        }
                        writeToFile();
                        readFromFile(); // Update list after purchase
                        selectedItemId.clear();
                        selectedId.setString(selectedItemId);
                    }
                    else
                    {
                        std::cout << "Invalid item ID." << std::endl;
                    }
                }
            }
        }

        window.clear(sf::Color(30, 30, 30));

        window.draw(title);
        window.draw(itemGrid);
        for (const auto& item : itemTexts)
            window.draw(item);
        window.draw(prompt);
        window.draw(selectedId);
        window.draw(numpadGrid);
        for (auto& button : numpadButtons)
            button.draw(window);
        button0.draw(window);
        purchaseButton.draw(window);

        window.display();
    }
}

int main()
{
    UserMode();

    return 0;
}

