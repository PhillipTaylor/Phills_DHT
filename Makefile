
BUILD_DIR=./build
SRC_DIR=./src
BIN_DIR=./bin

application_components: clean
	gcc -Wall -g -c -o $(BUILD_DIR)/dht.o $(SRC_DIR)/dht.c

test_suite : application_components
	gcc -Wall -g -o $(BIN_DIR)/unit_test_dht $(BUILD_DIR)/dht.o -L$(BUILD_DIR) $(SRC_DIR)/test/unit_tests/01_dht.c

all : test_suite

clean:
	rm -rf $(BIN_DIR)
	rm -rf $(BUILD_DIR)
	mkdir $(BIN_DIR)
	mkdir $(BUILD_DIR)

default : all
