#include <Windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <libpq-fe.h>
#include <chrono>

#define IDC_NEXT_BUTTON 1001
#define WM_SELECTED_OPTION (WM_USER + 1)

// Define the PostgreConnect class
class PostgreConnect
{
private:
    PGconn *conn; // PostgreSQL connection object
public:
    PostgreConnect(/* args */);
    ~PostgreConnect();
    void connect(const char *conninfo); // Method to establish connection

    PGconn *getConnection() const
    {
        return conn;
    }

    bool isConnected() const
    {
        return conn != nullptr;
    }
};

PostgreConnect::PostgreConnect(/* args */) : conn(nullptr)
{
}

PostgreConnect::~PostgreConnect()
{
    if (conn)
    {
        PQfinish(conn); // Close the connection
    }
}

void PostgreConnect::connect(const char *conninfo)
{
    // Establish a connection to the database
    conn = PQconnectdb(conninfo);

    // Check if the connection was successful
    if (PQstatus(conn) != CONNECTION_OK)
    {
        std::cerr << "Connection to database failed: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn); // Close the connection
        conn = nullptr;
    }
    else
    {
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
    Speciale &addId(int i)
    {
        id = i;
        return *this;
    }

    Speciale &addName(const std::string &n)
    {
        name = n;
        return *this;
    }

    int getId() const
    {
        return id;
    }

    std::string getName() const
    {
        return name;
    }

    static std::vector<Speciale> getListSpecial(PostgreConnect &pgConn)
    {
        std::vector<Speciale> specials;

        // Check if the connection is established
        if (!pgConn.isConnected())
        {
            std::cerr << "Not connected to database." << std::endl;
            return specials;
        }

        // Execute a query to select data from the table
        PGresult *res = PQexec(pgConn.getConnection(), "SELECT * FROM speciale");

        // Check if the query was successful
        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            std::cerr << "Failed to execute query: " << PQerrorMessage(pgConn.getConnection()) << std::endl;
            PQclear(res);
            return specials;
        }

        // Process the result set and populate the vector
        int numRows = PQntuples(res);
        for (int i = 0; i < numRows; ++i)
        {
            Speciale speciale;
            speciale.addId(std::stoi(PQgetvalue(res, i, 0))).addName(PQgetvalue(res, i, 1));
            specials.push_back(speciale);
        }

        // Clear the result
        PQclear(res);

        return specials;
    }
};
class Racer
{
private:
    std::string name;
    int id;
    int idGroup;
    std::string nameGroup;
    std::chrono::system_clock::time_point time;

public:
    Racer &addName(const std::string &n)
    {
        name = n;
        return *this;
    }

    Racer &addId(int i)
    {
        id = i;
        return *this;
    }

    Racer &addNameGroup(const std::string &ng)
    {
        nameGroup = ng;
        return *this;
    }

    Racer &addIdGroup(int ig)
    {
        idGroup = ig;
        return *this;
    }

    Racer &addTime(const std::chrono::system_clock::time_point &t)
    {
        time = t;
        return *this;
    }

    std::string getName() const
    {
        return name;
    }

    int getId() const
    {
        return id;
    }

    std::string getNameGroup() const
    {
        return nameGroup;
    }

    int getIdGroup() const
    {
        return idGroup;
    }

    std::chrono::system_clock::time_point getTime() const
    {
        return time;
    }

    static std::vector<Racer> getListRacer(PostgreConnect &pgConn)
    {
        std::vector<Racer> racers;

        // Check if the connection is established
        if (!pgConn.isConnected())
        {
            std::cerr << "Not connected to database." << std::endl;
            return racers;
        }

        // Execute a query to select data from the table
        PGresult *res = PQexec(pgConn.getConnection(), "SELECT * FROM v_racers");

        // Check if the query was successful
        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            std::cerr << "Failed to execute query: " << PQerrorMessage(pgConn.getConnection()) << std::endl;
            PQclear(res);
            return racers;
        }

        // Process the result set and populate the vector
        int numRows = PQntuples(res);
        for (int i = 0; i < numRows; ++i)
        {
            Racer racer;
            racer
                .addName(PQgetvalue(res, i, 0))
                .addId(std::stoi(PQgetvalue(res, i, 1)))
                .addIdGroup(std::stoi(PQgetvalue(res, i, 2)))
                .addNameGroup(PQgetvalue(res, i, 0));
            racers.push_back(racer);
        }

        // Clear the result
        PQclear(res);

        return racers;
    }
};

HWND hwndSecondWindow = NULL;

struct RaceStruct
{
    std::string selectedSpecial;
    std::string selectedNumberRacer;
    std::vector<Racer> racers;
    std::vector<Racer> excludedRacers;
};
void ShowSecondWindow(HWND hwndParent, const std::string &selectedSpecial, const std::string &selectedNumberRacer, const std::vector<Racer> &racers, const Racer &excludedRacer);

// In the second window's window procedure
LRESULT CALLBACK SecondWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    std::string selectedRacer;
    static HWND comboBoxRacers;

    RaceStruct *raceStruct = reinterpret_cast<RaceStruct *>(reinterpret_cast<CREATESTRUCT *>(lParam)->lpCreateParams);

    int comboBoxRacersHeight = 20;
    switch (msg)
    {
    case WM_CREATE:
    {
        // Retrieve the data passed from ShowSecondWindow

        comboBoxRacersHeight = comboBoxRacersHeight * (raceStruct->racers.size());

        if (raceStruct != nullptr)
        {
            // Use raceStruct->selectedSpecial, raceStruct->selectedNumberRacer, and raceStruct->racers here
            std::string labelSpecial = "Type de Speciale: " + raceStruct->selectedSpecial;
            CreateWindow("STATIC", labelSpecial.c_str(), WS_VISIBLE | WS_CHILD, 0, 0, 200, 20, hwnd, NULL, NULL, NULL);
            std::string labelNumberRacer = "Nombre de concurent: " + raceStruct->selectedNumberRacer;
            CreateWindow("STATIC", labelNumberRacer.c_str(), WS_VISIBLE | WS_CHILD, 0, 20, 200, 20, hwnd, NULL, NULL, NULL);

            CreateWindow("STATIC", "Coureur", WS_VISIBLE | WS_CHILD, 50, 60, 100, comboBoxRacersHeight, hwnd, NULL, NULL, NULL);
            comboBoxRacers = CreateWindow("COMBOBOX", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 150, 60, 200, comboBoxRacersHeight, hwnd, NULL, NULL, NULL);

            for (const auto &racer : raceStruct->racers)
            {
                SendMessage(comboBoxRacers, CB_ADDSTRING, 0, (LPARAM)racer.getName().c_str());
            }
            int inputHeight = 80 + comboBoxRacersHeight;
            CreateWindow("STATIC", "Minute", WS_VISIBLE | WS_CHILD, 50, inputHeight, 100, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 150, inputHeight, 200, 20, hwnd, NULL, NULL, NULL);

            inputHeight = inputHeight + 20;
            CreateWindow("STATIC", "Sec", WS_VISIBLE | WS_CHILD, 50, inputHeight, 100, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 150, inputHeight, 200, 20, hwnd, NULL, NULL, NULL);

            inputHeight = inputHeight + 20;
            CreateWindow("STATIC", "Millisec", WS_VISIBLE | WS_CHILD, 50, inputHeight, 100, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 150, inputHeight, 200, 20, hwnd, NULL, NULL, NULL);

            CreateWindow("BUTTON", "Suivant", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 200, 400, 100, 40, hwnd, (HMENU)IDC_NEXT_BUTTON, NULL, NULL);
        }
        break;
    }
    case WM_COMMAND:
    {
        // Handle commands
        switch (LOWORD(wParam))
        {
        case IDC_NEXT_BUTTON:
            // Capture the selected special and racer before creating the second window
            int indexSpecial = SendMessage(comboBoxRacers, CB_GETCURSEL, 0, 0);
            if (indexSpecial != CB_ERR)
            {
                TCHAR itemBuffer[256];
                SendMessage(comboBoxRacers, CB_GETLBTEXT, indexSpecial, reinterpret_cast<LPARAM>(itemBuffer));
                itemBuffer[255] = '\0'; // Ensure null-termination
                selectedRacer = itemBuffer;
            }
            // Show the second window with the captured data
            int selectNumberRacer = stoi(raceStruct->selectedNumberRacer);
            selectNumberRacer--;

            std::vector<Racer> racers;
            Racer excludedRacers;

            for (size_t i = 0; i < raceStruct->racers.size(); i++)
            {
                if(raceStruct->racers[i].getName()!=selectedRacer)
                {
                    racers.push_back(raceStruct->racers[i]);
                }
                else
                {
                    excludedRacers
                        .addId(raceStruct->racers[i].getId())
                        .addName(raceStruct->racers[i].getName())
                        .addIdGroup(raceStruct->racers[i].getIdGroup())
                        .addNameGroup(raceStruct->racers[i].getNameGroup());
                }
                
            }
            
            ShowSecondWindow(hwnd, raceStruct->selectedSpecial, std::to_string(selectNumberRacer),racers,excludedRacers);
            break;
        }
    }
        // Handle other messages
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// Function to create and show the second window
void ShowSecondWindow(HWND hwndParent, const std::string &selectedSpecial, const std::string &selectedNumberRacer, const std::vector<Racer> &racers,const Racer &excludedRacer)
{
    // Register window class for the second window
    WNDCLASS wc = {0};
    wc.lpfnWndProc = SecondWndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "SecondWindowClass";
    RegisterClass(&wc);

    RaceStruct *raceStruct = new RaceStruct;
    raceStruct->selectedSpecial = selectedSpecial;
    raceStruct->selectedNumberRacer = selectedNumberRacer;
    raceStruct->racers = racers;
    if(excludedRacer.getId()<0){
        raceStruct->excludedRacers =racers;
    }

    // Create the second window
    std::string windowText = "Rally Noble (form_2)";
    hwndSecondWindow = CreateWindow("SecondWindowClass", windowText.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 600, hwndParent, NULL, GetModuleHandle(NULL), raceStruct);

    // Show and update the second window
    ShowWindow(hwndSecondWindow, SW_SHOW);
    UpdateWindow(hwndSecondWindow);

    delete raceStruct;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, std::vector<Speciale> specials, std::vector<Racer> racers)
{
    std::string selectedSpecial;
    std::string selectedNumberRacer;
    static HWND comboBoxSpecial;
    static HWND comboBoxRacer;

    switch (msg)
    {
    case WM_CREATE:
    {
        // Create controls
        int comboBoxSpecialeHeight = 20;
        comboBoxSpecialeHeight = comboBoxSpecialeHeight * (specials.size() + 1);

        int comboBoxRacerHeight = 20;
        comboBoxRacerHeight = comboBoxRacerHeight * (racers.size());

        std::string racerCount;

        CreateWindow("STATIC", "Type de Speciale", WS_VISIBLE | WS_CHILD, 50, 50, 100, comboBoxSpecialeHeight, hwnd, NULL, NULL, NULL);
        comboBoxSpecial = CreateWindow("COMBOBOX", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 150, 50, 200, comboBoxSpecialeHeight, hwnd, NULL, NULL, NULL);

        // Populate the combo box with options from the list
        for (const auto &special : specials)
        {
            SendMessage(comboBoxSpecial, CB_ADDSTRING, 0, (LPARAM)special.getName().c_str());
        }

        CreateWindow("STATIC", "Nombre de concurent", WS_VISIBLE | WS_CHILD, 50, 200, 100, comboBoxRacerHeight, hwnd, NULL, NULL, NULL);
        comboBoxRacer = CreateWindow("COMBOBOX", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 150, 200, 200, comboBoxRacerHeight, hwnd, NULL, NULL, NULL);

        for (int i = 1; i <= racers.size(); ++i)
        {
            racerCount = std::to_string(i);
            SendMessage(comboBoxRacer, CB_ADDSTRING, 0, (LPARAM)racerCount.c_str());
        }

        CreateWindow("BUTTON", "Suivant", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 200, 400, 100, 40, hwnd, (HMENU)IDC_NEXT_BUTTON, NULL, NULL);
        break;
    }
    case WM_COMMAND:
    {
        // Handle commands
        switch (LOWORD(wParam))
        {
        case IDC_NEXT_BUTTON:
            // Capture the selected special and racer before creating the second window
            int indexSpecial = SendMessage(comboBoxSpecial, CB_GETCURSEL, 0, 0);
            if (indexSpecial != CB_ERR)
            {
                TCHAR itemBuffer[256];
                SendMessage(comboBoxSpecial, CB_GETLBTEXT, indexSpecial, reinterpret_cast<LPARAM>(itemBuffer));
                itemBuffer[255] = '\0'; // Ensure null-termination
                selectedSpecial = itemBuffer;
            }
            int indexRacer = SendMessage(comboBoxRacer, CB_GETCURSEL, 0, 0);
            if (indexRacer != CB_ERR)
            {
                TCHAR itemBuffer[256];
                SendMessage(comboBoxRacer, CB_GETLBTEXT, indexRacer, reinterpret_cast<LPARAM>(itemBuffer));
                itemBuffer[255] = '\0'; // Ensure null-termination
                selectedNumberRacer = itemBuffer;
            }

            Racer excluderRacer;
            excluderRacer.addId(-1);
            // Show the second window with the captured data
            ShowSecondWindow(hwnd, selectedSpecial, selectedNumberRacer, racers,excluderRacer);
            break;
        }
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

// Define a static PostgreConnect object outside the WinMain function
static PostgreConnect pgConn;

// WinMain function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Establish the database connection only once
    const char *conninfo = "dbname=rally user=rally password=rally hostaddr=127.0.0.1 port=5432";
    pgConn.connect(conninfo);

    // Define the window class and register it
    WNDCLASS wc = {0};
    wc.lpfnWndProc = [](HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        static std::vector<Speciale> specials;
        static std::vector<Racer> racers;
        // Retrieve the list of specials only once
        if (specials.empty() && pgConn.isConnected())
        {
            specials = Speciale::getListSpecial(pgConn);
        }
        if (racers.empty() && pgConn.isConnected())
        {
            racers = Racer::getListRacer(pgConn);
        }
        // Call the window procedure with the list of specials
        return WndProc(hwnd, msg, wParam, lParam, specials, racers);
    };
    wc.hInstance = hInstance;
    wc.lpszClassName = "MyWindowClass";
    RegisterClass(&wc);

    // Create window
    HWND hwnd = CreateWindow("MyWindowClass", "Rally Noble", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 600, NULL, NULL, hInstance, NULL);
    if (!hwnd)
        return -1;

    // Show and update window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
