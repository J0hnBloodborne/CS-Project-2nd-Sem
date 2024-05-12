#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <random>

class Button {
private:
    sf::Text text;  // Text of button
    sf::RectangleShape shape; // Shape of Button

public:
    // Three overloaded paremeterized constructors
    Button(const std::string& buttonText, const sf::Font& font, unsigned int fontSize, const sf::Vector2f& position, const sf::Color& textColor, const sf::Color& buttonColor) {
        text.setString(buttonText); // Set button text
        text.setFont(font); // Set text font
        text.setCharacterSize(fontSize); // Set font size
        text.setFillColor(textColor); // Set text colo
        text.setPosition(position.x + 35, position.y + 5); // Adjust text position for centering in the button

        shape.setSize(sf::Vector2f(90, 90)); // Standard button size
        shape.setFillColor(buttonColor); // Set button color
        shape.setPosition(position); // Set position
        shape.setOutlineThickness(2); // Add outline
        shape.setOutlineColor(sf::Color::Black); // Set outline color
    }
    Button(const std::string& buttonText, const sf::Font& font, unsigned int fontSize, const sf::Vector2f& position, const sf::Color& textColor, const sf::Color& buttonColor, int width, int height) {
        text.setString(buttonText); 
        text.setFont(font); 
        text.setCharacterSize(fontSize);
        text.setFillColor(textColor);
        text.setPosition(position.x + 35, position.y + 5);

        shape.setSize(sf::Vector2f(width, height)); // Custom button size from different params
        shape.setFillColor(buttonColor);
        shape.setPosition(position);
        shape.setOutlineThickness(2);
        shape.setOutlineColor(sf::Color::Black);
    }
    Button(const std::string& buttonText, const sf::Font& font, unsigned int fontSize, const sf::Vector2f& position, const sf::Color& textColor, const sf::Color& buttonColor, int width, int height, int textPositionXOffset, int textPositionYOffset) {
        text.setString(buttonText);
        text.setFont(font);
        text.setCharacterSize(fontSize);
        text.setFillColor(textColor);
        text.setPosition(position.x + textPositionXOffset, position.y + textPositionYOffset); // Adjust text position according to offSet params
        shape.setSize(sf::Vector2f(width, height));
        shape.setFillColor(buttonColor);
        shape.setPosition(position);
        shape.setOutlineThickness(2);
        shape.setOutlineColor(sf::Color::White);  
    }
    void draw(sf::RenderWindow& window) { //Draw button
        window.draw(shape);
        window.draw(text);
    }

    bool contains(const sf::Vector2f& point) const {  // Return button bounds to check for button clicks
        return shape.getGlobalBounds().contains(point);
    }
    
    void setOutlineThinkness(unsigned int t) {  // For custom outlineThickness (used only once or twice)
        shape.setOutlineThickness(t);
    }
    std::string getText() const {  // Return button text
        return text.getString();
    }
};

class VendingMachineSlot {   // Class for storing item data
private: // Self explanatory
    std::string name;  
    int quantity;
    double price;

public: // Default and Parameterized Constructor
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

    // Setters and Getters
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

    //Overloaded operators for quickly adding quantity
    void operator+=(int x) {
        quantity += x;
    }

    void operator-=(int x) {
        quantity -= x;
    }
};

std::vector<VendingMachineSlot> slot;  // Global vector to store vending machine data
std::string selectedItemId = ""; // Global string for current input

void readFromFile() {  // Self explanatory
    std::ifstream readFile("Vending Machine Data.txt");
    if (!readFile.is_open()) {
        std::cerr << "Error reading file." << std::endl << "Exiting...";
        exit(1);
    }

    std::string n;
    int q;
    double p;
    while (readFile >> n >> p >> q) {
        slot.emplace_back(n, p, q); // emplace_back instead of push_back for directly passing constructor params
    }

    readFile.close();
}

void writeToFile() {  // Self explanatory
    std::ofstream writeFile;
    writeFile.open("Vending Machine Data.txt");
    for (int i = 0; i < slot.size(); i++) {
        writeFile << slot[i].getName() << std::endl << slot[i].getPrice() << std::endl << slot[i].getQuantity() << std::endl;
    }

    writeFile.close();
    slot.clear();
}
void UserMode() { // Main GUI Code
    sf::RenderWindow window(sf::VideoMode(1024, 1000), "Vending Machine");  // Render windpow of res : 1024x1000 and title "Vending Machone"
    
    sf::Texture background;  // Texture Object for Background
    if (!background.loadFromFile("Background.png")) {  // Load background texture
        std::cerr << "Error loading background." << std::endl;
    }
    sf::Sprite s(background); // Create sprite with parameterized constructor passing the texture as arg
    
    sf::Font font; // Font object for main font
    if (!font.loadFromFile("game_over.ttf")) {  // Load font
        std::cerr << "Error loading font." << std::endl;
        return;
    }

    sf::SoundBuffer clickBuffer; // SoundBuffer object for click sound
    if (!clickBuffer.loadFromFile("blipSelect.wav")) { // Load click sound
        std::cerr << "Failed to load button click sound." << std::endl;
    }

    sf::SoundBuffer purchaseBuffer; // SoundBuffer object for purchase sound 
    if (!purchaseBuffer.loadFromFile("purchase.wav")) { // Load purchase sound
        std::cerr << "Failed to load purchase sound." << std::endl;
    }
    
    sf::SoundBuffer backBuffer; // BackBuffer object for back sound
    if (!backBuffer.loadFromFile("backSound.wav")) { // Load back sound
        std::cerr << "Error loading back sound." << std::endl;
    }
    
    sf::SoundBuffer invalidBuffer; // SoundBuffer object for invalid sound
    if (!invalidBuffer.loadFromFile("invalid.wav")) { // Load invalid sound
        std::cerr << "Error loading invalid sound." << std::endl;
    }
    //Make Sound objects using parameterized constructors passing their respective sound buffers as args
    sf::Sound clickSound(clickBuffer); 
    sf::Sound purchaseSound(purchaseBuffer);
    sf::Sound backSound(backBuffer);
    sf::Sound invalidSound(invalidBuffer);

    sf::Text title("VENDING MACHINE", font, 84); // Title text
    title.setPosition(400, 2);
    title.setFillColor(sf::Color::White);
    
    // Item grid shape and texture
    sf::Texture T_itemGrid; 
    sf::RectangleShape itemGrid(sf::Vector2f(600, 752));
    itemGrid.setPosition(50, 110);
    itemGrid.setFillColor(sf::Color(100, 100, 100));
    T_itemGrid.loadFromFile("itemgrid.png");
    itemGrid.setTexture(&T_itemGrid);

    // Initialize items
    const int rows = 3;
    const int cols = 3;
    std::vector<Button> itemButtons;
    int itemWidth = itemGrid.getSize().x / cols;
    int itemHeight = itemGrid.getSize().y / rows;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(40, 255); // Range for RGB colors
    for (int i = 0; i < slot.size(); i++) {
        sf::Color bgColor(distribution(generator), distribution(generator), distribution(generator));
        itemButtons.emplace_back(std::to_string(i+1) + " -  " + slot[i].getName(), font, 36, sf::Vector2f(100 + (i % cols) * itemWidth, 150 + (i / cols) * itemHeight), sf::Color::White, bgColor, 50, 50, 0, 90);
    }

    // Numpad Shape
    sf::RectangleShape numpadGrid(sf::Vector2f(300, 400));
    numpadGrid.setPosition(700, 300);
    numpadGrid.setFillColor(sf::Color(150, 150, 150));

    // Initialize Numpad (1-9)
    std::vector<Button> numpadButtons;
    int buttonWidth = numpadGrid.getSize().x / 3;
    int buttonHeight = numpadGrid.getSize().y / 4;
    for (int i = 1; i <= 9; i++) {
        numpadButtons.emplace_back(std::to_string(i), font, 48, sf::Vector2f(700 + (i - 1) % 3 * buttonWidth, 300 + (i - 1) / 3 * buttonHeight), sf::Color::Black, sf::Color::White);
    }

    // Initialize other buttons
    Button button0("0", font, 48, sf::Vector2f(700 + buttonWidth, 300 + 3 * buttonHeight), sf::Color::Black, sf::Color::White); // Larger 0 button
    Button buttonX("X", font, 48, sf::Vector2f(800 + buttonWidth, 300 + 3 * buttonHeight), sf::Color::White, sf::Color::Red);
    Button purchaseButton("PURCHASE", font, 48, sf::Vector2f(750, 800), sf::Color::White, sf::Color::Green, 180, 90);

    // Different digital font for selected item window and pipup window
    sf::Font selectedItemFont;
    if (!selectedItemFont.loadFromFile("digital-7.ttf")) {
        std::cerr << "Error loading selected item font." << std::endl;
        return;
    }

    // Selected item window
    sf::RectangleShape selectedItemShape(sf::Vector2f(110, 40)); // Selected item ID shape
    selectedItemShape.setPosition(830, 240);
    selectedItemShape.setFillColor(sf::Color(30, 30, 30));
    selectedItemShape.setOutlineThickness(5);
    selectedItemShape.setOutlineColor(sf::Color(100,100,100));
    // Selected item id text
    sf::Text selectedId("", selectedItemFont, 48);
    selectedId.setPosition(830, 230);
    selectedId.setFillColor(sf::Color::Blue);
    
    // Pop up / status window
    sf::RectangleShape popUpWindow(sf::Vector2f(330, 100));
    popUpWindow.setPosition(680, 120);
    popUpWindow.setFillColor(sf::Color(30, 30, 30));
    popUpWindow.setOutlineThickness(5);
    popUpWindow.setOutlineColor(sf::Color(100,100,100));
    // Popup window text with digital font
    sf::Text popUpMessage("", selectedItemFont, 35);
    popUpMessage.setPosition(710, 125);
    popUpMessage.setFillColor(sf::Color::Blue);
    
    // Main GUI Loop
    while (window.isOpen()) {
        sf::Event event;  // Event Object
        while (window.pollEvent(event)) { // Check for events
            if (event.type == sf::Event::Closed)  // If window closed exit
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {   // Check for left click
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    for (auto& button : itemButtons) {  // Check if any button was clicked by comparing mouse positon and button bounds
                        if (button.contains(sf::Vector2f(mousePos))) {
                            selectedItemId = button.getText();
                            selectedId.setString(selectedItemId);
                            clickSound.play(); // Play click sound
                            break;
                        }
                    }

                    for (auto& button : numpadButtons) {  // Same for numpad
                        if (button.contains(sf::Vector2f(mousePos))) {
                            selectedItemId += button.getText(); // Concatenate clicked button to selectedItemId
                            selectedId.setString(selectedItemId);
                            clickSound.play(); // Play click sound
                            break;
                        }
                    }
                  
                    if (button0.contains(sf::Vector2f(mousePos))) { // Same as numpad
                        selectedItemId += "0";
                        selectedId.setString(selectedItemId);
                        clickSound.play();
                    }
                    if (buttonX.contains(sf::Vector2f(mousePos))) { // Same as numpad
                        if (!selectedItemId.empty()) {   // Remove last element in selectedI, sort of a backspace button
                            selectedItemId.pop_back();
                            selectedId.setString(selectedItemId);
                            backSound.play(); // Play back sound
                        }
                    }
                    if (purchaseButton.contains(sf::Vector2f(mousePos))) { // Check for purchase bytton
                        int id = std::stoi(selectedItemId);
                        if (id > 0 && id <= slot.size()) {
                            std::string purchasedItem = slot[id - 1].getName();
                            std::cout << "You selected item: " << purchasedItem << std::endl; // Cout in console

                            slot[id - 1] -= 1; // Decrement quantity
                            if (slot[id - 1].getQuantity() == 0) { // If item exhausted move items and update file
                                slot.erase(slot.begin() + id - 1);
                                itemButtons.clear(); // Clear item buttons
                                for (int i = 0; i < slot.size(); i++) { // Recreate item buttons
                                    sf::Color bgColor(distribution(generator), distribution(generator), distribution(generator));
                                    itemButtons.emplace_back(std::to_string(i) + "   " + slot[i].getName(), font, 36, sf::Vector2f(100 + (i % cols) * itemWidth, 150 + (i / cols) * itemHeight), sf::Color::White, bgColor,50,50,0,90);
                                }
                            }
                            writeToFile();
                            readFromFile(); // Update list after purchase
                            selectedItemId.clear();
                            selectedId.setString(selectedItemId);

                            popUpMessage.setString("You purchased:\n\n" + purchasedItem); // Display pop-up window for purchase
                            purchaseSound.play(); // Play purchase sound
                        }
                        else {
                            popUpMessage.setString("Invalid item ID");
                            popUpMessage.setPosition(700, 125); // Reset message position for pop-up
                            invalidSound.play();
                        }

                        // Show pop-up window
                        popUpWindow.setPosition(680, 120); // Reset pop up window position
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed) {  // Check for key presses
                if (event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9) { // Enter nums into selected id using keyboard buttons
                    selectedItemId += (event.key.code - sf::Keyboard::Num0) + '0';
                    selectedId.setString(selectedItemId);
                    clickSound.play(); // Play click sound 
                }
                else if (event.key.code == sf::Keyboard::Backspace) { // Use backspace key instead of button
                    if (!selectedItemId.empty()) {
                        selectedItemId.pop_back();
                        selectedId.setString(selectedItemId);
                        backSound.play(); // Play back sound
                    }
                }
            }
        }

        window.clear(sf::Color(0, 0, 0)); // Clear for next frame
        // Main drawinf sequence
        window.draw(s); // Draw background texture
        window.draw(title); // Draw title
        window.draw(itemGrid); // Draw itemGrid
        for (auto& button : numpadButtons) // Draw numpad (1-9)
            button.draw(window);
        button0.draw(window); // Draw 0
        buttonX.draw(window); // Draw X
        purchaseButton.draw(window); // Draw Purchase Button
        for (auto& button : itemButtons) // Draw items
            button.draw(window);
        window.draw(selectedItemShape); // Draw selected item window
        window.draw(selectedId); // Draw selected id
        if (popUpWindow.getPosition().y < 1000) // Draw pop-up window if purchase button clicked
            window.draw(popUpWindow);
        window.draw(popUpMessage); // Draw pop up message

        window.display(); // Display
    }
}

void MaintenanceMode() {
    // Maintenance mode implementation
    std::string productName;
    double productPrice;
    int productStock;
    //Take item information inputs
    std::cout << "Enter product name: ";
    std::cin >> productName;
    std::cout << "Enter product price: ";
    std::cin >> productPrice;
    std::cout << "Enter product stock: ";
    std::cin >> productStock;

    // Add the new item to the slot vector
    slot.emplace_back(productName, productPrice, productStock);
    writeToFile(); // Update the file
}

int main() {
    readFromFile();
    int choice;
    bool exitRequested = false;
    while (!exitRequested) {
        std::cout << "Press 0 to exit, any other number for user mode, or 177 for maintenance mode: ";
        std::cin >> choice;

        switch (choice) {
        case 0:
            // Exit code
            exitRequested = true;
            break;
        case 177:
            MaintenanceMode();
            // After maintenance mode, reload data from file
            readFromFile();
            break;
        default:
            // Enter User Mode
            UserMode();
            break;
        }
    }

    return 0;
}