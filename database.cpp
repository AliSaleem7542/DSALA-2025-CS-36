#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

class Row
{

public:
    vector<string> values;

    Row(vector<string> vals)
    {
        values = vals;
    }
};
class Column
{
public:
    string name;
    string type;
    unsigned int constraints;

    Column(string n, string t, unsigned int c)
    {
        name = n;
        type = t;
        constraints = c;
    }
};

class Table
{
public:
    string tablename;
    vector<Column> columns;
    vector<Row *> rows;

    Table(string name)
    {
        tablename = name;
    }

    ~Table()
    {
        for (Row *r : rows)
        {
            delete r;
        }
        rows.clear();
    }
    void insertrow(vector<string> vals)
    {
        if (vals.size() != columns.size())
        {
            cout << "columns count mismatch!\n";
            return;
        }

        // constraints check
        for (int i = 0; i < columns.size(); i++)
        {

            if ((columns[i].constraints & 2) && vals[i].empty())
            {
                cout << "Error: Not Null constraint violated on column " << columns[i].name << "\n";
                return;
            }

            if ((columns[i].constraints & 1) || (columns[i].constraints & 4))
            {
                for (Row *r : rows)
                {
                    if (r->values[i] == vals[i])
                    {
                        cout << "Error: Unique/Primary Key violation on column " << columns[i].name << "\n";
                        return;
                    }
                }
            }
        }

        Row *r = new Row(vals);
        rows.push_back(r);
        cout << "Record inserted successfully.\n";
    }

    void selectall()
    {
        for (Column c : columns)
            cout << c.name << "\t";
        cout << "\n";
        for (Row *r : rows)
        {
            for (string val : r->values)
                cout << val << "\t";
            cout << "\n";
        }
    }

    void savetable(string filename)
    {
        ofstream fout(filename);
        fout << "TABLE " << tablename << "\n";
        for (Column c : columns)
        {
            fout << c.name << " " << c.type << " " << c.constraints << "\n";
        }
        fout << "DATA\n";
        for (Row *r : rows)
        {
            for (string val : r->values)
                fout << val << " ";
            fout << "\n";
        }
        fout.close();
        cout << "Table saved to " << filename << "\n";
    }

    void loadtable(string filename)
    {
        ifstream fin(filename);
        if (!fin)
        {
            cout << "File not found!\n";
            return;
        }
        string line;
        rows.clear();
        columns.clear();

        getline(fin, line); 
        tablename = line.substr(6);

        while (getline(fin, line))
        {
            if (line == "DATA")
                break;
            stringstream ss(line);
            string name, type;
            unsigned int cons;
            ss >> name >> type >> cons;
            columns.push_back(Column(name, type, cons));
        }

        while (getline(fin, line))
        {
            if (line.empty())
                continue;
            stringstream ss(line);
            vector<string> vals;
            string val;
            while (ss >> val)
                vals.push_back(val);
            Row *r = new Row(vals);
            rows.push_back(r);
        }

        fin.close();
        cout << "Table loaded from " << filename << "\n";
    }
};

int main()
{
    Table *mytable = nullptr;
    char buffer[256];

    while (true)
    {
        cout << "\nEnter command (CREATE/INSERT/SELECT/SAVE/LOAD/EXIT): ";
        cin.getline(buffer, 256);
        string cmd(buffer);

        if (cmd.substr(0,6) == "CREATE" || cmd.substr(0,6) == "create")
        {
            cout << "Enter table name: ";
            string tname;
            cin >> tname;
            mytable = new Table(tname);
            int colcount;
            cout << "Number of columns: ";
            cin >> colcount;
            for (int i = 0; i < colcount; i++)
            {
                string name, type;
                unsigned int cons;
                cout << "Column " << i + 1 << " name: ";
                cin >> name;
                cout << "Type (int/string): ";
                cin >> type;
                cout << "Constraints (bitwise): ";
                cin >> cons;
                mytable->columns.push_back(Column(name, type, cons));
            }
            cin.ignore();
            cout << "Table created successfully.\n";
        }
        else if (cmd.substr(0, 6) == "INSERT" || cmd.substr(0, 6) == "insert")
        {
            if (!mytable)
            {
                cout << "Create table first!\n";
                continue;
            }
            vector<string> vals;
            for (Column c : mytable->columns)
            {
                string val;
                cout << "Enter value for " << c.name << ": ";
                cin >> val;
                vals.push_back(val);
            }
            cin.ignore();
            mytable->insertrow(vals);
        }
        else if (cmd.substr(0, 6) == "SELECT" || cmd.substr(0, 6) == "select")
        {
            if (!mytable)
            {
                cout << "Create table first!\n";
                continue;
            }
            mytable->selectall();
        }
        else if (cmd.substr(0, 4) == "SAVE" || cmd.substr(0, 4) == "save")
        {
            if (!mytable)
            {
                cout << "Create table first!\n";
                continue;
            }
            string fname;
            cout << "Enter filename: ";
            cin >> fname;
            cin.ignore();
            mytable->savetable(fname);
        }
        else if (cmd.substr(0, 4) == "LOAD" || cmd.substr(0, 4) == "load")
        {
            string fname;
            cout << "Enter filename: ";
            cin >> fname;
            cin.ignore();
            if (mytable)
                delete mytable;
            mytable = new Table("");
            mytable->loadtable(fname);
        }
        else if (cmd.substr(0, 4) == "EXIT" || cmd.substr(0, 4) == "exit")
        {
            break;
        }
        else
            cout << "Unknown command!\n";
    }

    if (mytable)
        delete mytable;
    return 0;
}
