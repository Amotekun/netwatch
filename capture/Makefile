CXX := c++
CXXFLAGS := -std=c++17 -Wall -Wextra $(shell pkg-config --cflags rdkafka++)
LDFLAGS := -lpcap $(shell pkg-config --libs rdkafka++)

SRC := src/main.cpp
OUT := build/capture

.PHONY: all clean

all: $(OUT)

$(OUT): $(SRC)
	@mkdir -p $(dir $(OUT))
	$(CXX) $(CXXFLAGS) $(SRC) $(LDFLAGS) -o $(OUT)

clean:
	rm -f $(OUT)
