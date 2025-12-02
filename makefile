CXX = g++
CXXFLAGS = -fPIC -Wall -Wextra -std=c++11
LDFLAGS = -shared
RM = rm -rf

BUILD_DIR = build
CIPHERS = Atbash Hill Polybius CodeWord
LIBS = $(CIPHERS:%=$(BUILD_DIR)/lib%.so)

all: $(LIBS) $(BUILD_DIR)/crypto_app

# Правило для библиотек
$(BUILD_DIR)/lib%.so: $(BUILD_DIR)/%.o $(BUILD_DIR)/utils.o | $(BUILD_DIR)
	$(CXX) $(LDFLAGS) -o $@ $^

# Правило для объектных файлов шифров
$(BUILD_DIR)/%.o: ciphers/%_crypt.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Основная программа
$(BUILD_DIR)/crypto_app: main/main.cpp $(BUILD_DIR)/utils.o | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ -ldl -Wl,-rpath,'$$ORIGIN' -Wl,-rpath,./build

$(BUILD_DIR)/utils.o: main/utils.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR):
	mkdir -p $@

clean:
	$(RM) $(BUILD_DIR)
	$(RM) Password.txt

.PHONY: all clean
