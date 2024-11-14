# Nome do executável
TARGET = ordenationSoftware

# Compilador
CXX = g++

# Flags do compilador (opcionalmente, você pode adicionar -O2 ou -O3 para otimizações)
CXXFLAGS = -std=c++17 -Wall -Wextra

# Arquivos fonte
SRC = ordenationSoftware.cpp

# Regra para criar o executável
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Limpeza dos arquivos gerados
clean:
	rm -f $(TARGET)
