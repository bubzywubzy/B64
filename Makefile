CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

TARGET = b64

SRC = src/main.cpp src/base64.cpp

INSTALL_DIR = $(HOME)/.local/bin

$(TARGET):
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

install: $(TARGET)
	mkdir -p $(INSTALL_DIR)
	cp $(TARGET) $(INSTALL_DIR)/$(TARGET)
	chmod +x $(INSTALL_DIR)/$(TARGET)

uninstall:
	rm -f $(INSTALL_DIR)/$(TARGET)

clean:
	rm -f $(TARGET)