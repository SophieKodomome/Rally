#include <Windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <libpq-fe.h>

// Define the PostgreConnect class
class PostgreConnect
{
private:
    PGconn *conn; // PostgreSQL connection object
public:
    PostgreConnect(/* args */);
    ~PostgreConnect();
    void connect(const char *conninfo); // Method to establish connection

    PGconn* getConnection() const {
        return conn;
    }

    bool isConnected() const {
        return conn != nullptr;
    }
};

PostgreConnect::PostgreConnect(/* args */) : conn(nullptr)
{
}

PostgreConnect::~PostgreConnect()
{
    if (conn) {
        PQfinish(conn); // Close the connection
    }
}

void PostgreConnect::connect(const char *conninfo)
{
    // Establish a connection to the database
    conn = PQconnectdb(conninfo);

    // Check if the connection was successful
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection to database failed: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn); // Close the connection
        conn = nullptr;
    } else {
        std::cout << "Connection to database successful." << std::endl;
    }
}

// Define the Speciale class
class Speciale
{
private:
    int id;
    std::string name;

public:
    Speciale();
    ~Speciale(){};

    Speciale& addId(int i){
        id = i;
        return *this;
    }

    Speciale& addName(const std::string& n){
        name = n;
        return *this;
    }

    int getId() const{
        return id;
    }

    std::string getName() const{
        return name;
    }

    static std::vector<Speciale> getListSpecial(PostgreConnect& pgConn) {
        std::vector<Speciale> specials;

        // Check if the connection is established
        if (!pgConn.isConnected()) {
            std::cerr << "Not connected to database." << std::endl;
            return specials;
        }

        // Execute a query to select data from the table
        PGresult *res = PQexec(pgConn.getConnection(), "SELECT * FROM speciale");

        // Check if the query was successful
        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            std::cerr << "Failed to execute query: " << PQerrorMessage(pgConn.getConnection()) << std::endl;
            PQclear(res);
            return specials;
        }

        // Process the result set and populate the vector
        int numRows = PQntuples(res);
        for (int i = 0; i < numRows; ++i) {
            Speciale speciale;
            speciale.addId(std::stoi(PQgetvalue(res, i, 0))).addName(PQgetvalue(res, i, 1));
            specials.push_back(speciale);
        }

        // Clear the result
        PQclear(res);

        return specials;
    }
};

Speciale::Speciale(){}

// Window procedure function
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            // Create controls
            CreateWindow("STATIC", "Name:", WS_VISIBLE | WS_CHILD, 50, 50, 100, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 150, 50, 200, 20, hwnd, NULL, NULL, NULL);

            CreateWindow("STATIC", "Email:", WS_VISIBLE | WS_CHILD, 50, 100, 100, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 150, 100, 200, 20, hwnd, NULL, NULL, NULL);

            CreateWindow("BUTTON", "Submit", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 200, 150, 100, 30, hwnd, NULL, NULL, NULL);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char *conninfo = "dbname=rally user=rally password=rally hostaddr=127.0.0.1 port=5432";

    // Create an instance of PostgreConnect
    PostgreConnect pgConn;

    // Connect to the database
    pgConn.connect(conninfo);

    // Register window class
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "MyWindowClass";
    RegisterClass(&wc);

    // Create window
    HWND hwnd = CreateWindow("MyWindowClass", "Simple Form", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 250, NULL, NULL, hInstance, NULL);
    if (!hwnd) return -1;

    // Show and update window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
