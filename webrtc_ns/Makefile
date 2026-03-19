CXX = g++
CXXFLAGS = -std=c++14 -Wall -O2 -I./include/webrtc
LDFLAGS = -lm

SOURCES = src/ns/noise_suppressor.cc \
          src/ns/noise_estimator.cc \
          src/ns/speech_probability_estimator.cc \
          src/ns/wiener_filter.cc \
          src/ns/ns_fft.cc \
          src/ns/suppression_params.cc \
          src/ns/quantile_noise_estimator.cc \
          src/ns/signal_model_estimator.cc \
          src/main.cpp

OBJECTS = $(SOURCES:.cc=.o)
OBJECTS := $(OBJECTS:.cpp=.o)

TARGET = webrtc_ns_example

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean