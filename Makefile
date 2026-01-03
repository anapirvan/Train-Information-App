all:
    g++ server.cpp login.c mersul_trenurilor.c status_plecari-sosiri.c update_plecare-sosire.c -o server $(pkg-config --cflags --libs libxml-2.0) -pthread
    gcc client.c -o client
clean:
    rm -f server client