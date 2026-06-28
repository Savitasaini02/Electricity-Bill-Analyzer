#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include<ctime>
#include<algorithm>
using namespace std;

class Customer
{
private:
    int C_id;
    string name;
    float prev_reading, curr_reading, units, total_bill;
    bool isPaid;

public:
    Customer()
    {
        C_id = 0;
        name = "";
        prev_reading = curr_reading = units = total_bill = 0;
        isPaid = false;
    }
    void setName(string n)
    {
        name = n;
    }

    void setPreviousReading(float p)
    {
        prev_reading = p;
    }

    void setCurrentReading(float c)
    {
        curr_reading = c;
    }
    float getPreviousReading() const
    {
        return prev_reading;
    }

    float getCurrentReading() const
    {
        return curr_reading;
    }
    int getID() const { return C_id; }
    string getName() const { return name; }
    float getUnits() const { return units; }
    float getBill() const { return total_bill; }
    bool getPaymentStatus() const { return isPaid; }

    void set_Data(int id, string n, float prev, float curr, float u, float b, bool paid)
    {
        C_id = id;
        name = n;
        prev_reading = prev;
        curr_reading = curr;
        units = u;
        total_bill = b;
        isPaid = paid;
    }

    void calculate_bill()
    {
        units = curr_reading - prev_reading;

        if (units <= 100)
            total_bill = units * 5;
        else if (units <= 200)
            total_bill = (100 * 5) + (units - 100) * 7;
        else
            total_bill = (100 * 5) + (100 * 7) + (units - 200) * 10;

        // High usage alert
        if (units > 300)
            cout << " High Usage Alert! For " << name << endl;
    }

    void pay_bill()
    {
        if (total_bill == 0)
        {
            cout << "Bill not generated yet!\n";
            return;
        }
        isPaid = true;
        cout << "Payment successful!\n";
    }

    void display()
    {
        cout << "\n---------------------------------\n";
        cout << "ID: " << C_id << endl;
        cout << "Name: " << name << endl;
        cout << "Units: " << units << endl;
        cout << "Bill: " << total_bill << endl;
        cout << "Status: " << (isPaid ? "Paid" : "Unpaid") << endl;
        cout << "---------------------------------\n";
    }

    
    void writeToFile(ofstream &out)
    {
        out << C_id << " " << name << " "
        << prev_reading << " " << curr_reading << " "
        << units << " " << total_bill << " "
        << isPaid << endl;
    }
    void generateCustomer_receipt();
    
};
void Customer  :: generateCustomer_receipt()
{
     if (total_bill == 0)
    {
        cout << "Bill not generated yet!\n";
        return;
    }
    // Display on screen
    cout << "\n========================================\n";
    cout << "        ELECTRICITY BILL RECEIPT\n";
    cout << "========================================\n\n";

    cout << "Customer ID       : " << C_id << endl;
    cout << "Customer Name     : " << name << endl;

    cout << "\n";
    cout << "Previous Reading  : " << prev_reading << endl;
    cout << "Current Reading   : " << curr_reading << endl;
    cout << "Units Consumed    : " << units << endl;

    cout << "\n";
    cout << "Total Bill        : Rs. " << total_bill << endl;
    cout << "Payment Status    : "
         << (isPaid ? "Paid" : "Pending") << endl;

    cout << "\n========================================\n";


    // Create file name
    string fileName = "Receipt_" + to_string(C_id) + ".txt";

    ofstream outFile(fileName);

    if (!outFile)
    {
        cout << "Error creating receipt file!\n";
        return;
    }

    // Get current date and time
    time_t now = time(0);
    char* dt = ctime(&now);

    // Write to file
    outFile << "====== ELECTRICITY BILL RECEIPT ======\n";
    outFile << "Date              : " << dt;
    outFile << "Customer ID       : " << C_id << endl;
    outFile << "Name              : " << name << endl;
    outFile << "Previous Reading  : " << prev_reading << endl;
    outFile << "Current Reading   : " << curr_reading << endl;
    outFile << "Units Consumed    : " << units << endl;
    outFile << "Total Bill        : Rs. " << total_bill << endl;
    outFile << "Payment Status    : "
            << (isPaid ? "Paid" : "Pending") << endl;
            outFile << "\n";
    outFile << "========================================\n";
    outFile << "   Thank you for using our service!\n";
    outFile << "========================================\n";

    outFile.close();

    cout << "Receipt saved successfully as "
         << fileName << endl;
}
class BillingSystem
{
private:
    vector<Customer> customers;

public:
    BillingSystem() {}

    bool login();
    void LoadFromFile();
    void update_file();
    bool isDuplicateID(int id);
    float getValidReading(string message);
    void add_customer();
    void update_customer();
    void calculate_bill();
    void pay_bill();
    void show_pending();
    void search_by_name();
    void sort_by_bill();
    void sort_by_name();
    void sort_by_units();
    void usage_analysis();
    void generate_receipt();
    void display_all();
    void delete_customer();
};
bool BillingSystem ::login()
{
    string user, pass;
    cout << "Enter Username: ";
    cin >> user;
    cout << "Enter Password: ";
    cin >> pass;

    if (user == "admin" && pass == "Admin@123")
    {
        cout << "Login successful!\n";
        return true;
    }
    cout << "Invalid credentials!\n";
    return false;
}
void BillingSystem ::LoadFromFile()
{
    customers.clear();

    ifstream file("bills.txt");
    if (!file)
    {
        ofstream newFile("bills.txt");
        newFile.close();
        return;
    }

    int id;
    string name;
    float p, c, u, b;
    bool paid;

    while (file >> id >> name >> p >> c >> u >> b >> paid)
    {
        Customer temp;
        temp.set_Data(id, name, p, c, u, b, paid);
        customers.push_back(temp);
    }
    file.close();
}

void BillingSystem ::update_file()
{
    ofstream file("bills.txt");
    for (Customer &c : customers) // range based for loop instead of for(int i = 0 ; i < customers.size(); i++)  &c is used the pass the original object insid the vector so that changes reflect in original obj in vector
    {
        c.writeToFile(file);
    }

    file.close();
}

bool BillingSystem ::isDuplicateID(int id)
{
    for (Customer &c : customers)
    {
        if (c.getID() == id)
            return true;
    }
    return false;
}
float BillingSystem ::getValidReading(string message)
{
    float reading;

    while (true)
    {
        cout << message;

        if (cin >> reading && reading > 0)
            return reading;

        cout << "Invalid reading. Try again.\n";

        cin.clear();
        cin.ignore(1000, '\n');
    }
}
void BillingSystem ::add_customer()
{
    int id;
    cout << "Enter Customer ID: ";
    if (!(cin >> id))
    {
        cout << "Invalid input! Please enter numbers only.\n";

        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }
    if (id <= 0)
    {
        cout << "Customer ID must be greater than 0 " << endl;
        return;
    }
    if (isDuplicateID(id))
    {
        cout << "Duplicate ID!\n";
        return;
    }

    string name;
    float prev, curr;

    cout << "Enter Name: ";
    cin.ignore(1000, '\n');
    getline(cin, name);
    if (name.empty())
    {
        cout << "Customer name cannot be empty.\n";
        return;
    }
    for (char ch : name)
    {
        if (!isalpha(ch) && ch != ' ')
        {
            cout << "Name should contain only alphabets.\n";
            return;
        }
    }
    prev = getValidReading("Enter previous reading  : ");
    curr = getValidReading("Enter current reading : ");
    if (curr < prev)
    {
        cout << "Current reading cannot be less than previous reading.\n";
        return;
    }
    Customer temp;
    temp.set_Data(id, name, prev, curr, 0, 0, false);

    customers.push_back(temp);

    update_file();
}
void BillingSystem ::update_customer()
{
    int id;

    cout << "Enter Customer ID: ";
    cin >> id;
    bool found = false;

    for (Customer &c : customers)
    {
        if (c.getID() == id)
        {
            cout << "Customer Found!\n";
            c.display();
            found = true;
            int choice;
            cout << "\nWhat do you want to update?\n";
            cout << "1. Name\n";
            cout << "2. Previous Reading\n";
            cout << "3. Current Reading\n";
            cin >> choice;
            switch (choice)
            {
            case 1:
            {
                string newName;
                cin.ignore();
                cout << "Enter New Name: ";
                getline(cin, newName);
                c.setName(newName);
                update_file();
                cout << "Name Updated Successfully.\n";
                break;
            }
            case 2:
            {
                float newReading = getValidReading("Enter New Previous Reading: ");
                ;
                cout << "Enter New Reading : ";
                cin >> newReading;
                if (newReading > c.getCurrentReading())
                {
                    cout << "Previous reading cannot be greater than current reading.\n";
                    break;
                }
                c.setPreviousReading(newReading);
                c.calculate_bill();
                update_file();
                cout << "Previous Reading updated successfully.\n ";
                break;
            }
            case 3:
            {
                float newReading = getValidReading("Enter New Current Reading: ");
                ;
                cout << "Enter New Reading : ";
                cin >> newReading;
                if (newReading < c.getPreviousReading())
                {
                    cout << "Current reading cannot be less than previous reading.\n";
                    break;
                }
                c.setCurrentReading(newReading);
                c.calculate_bill();
                update_file();
                cout << "current Reading updated successfully.\n ";
                break;
            }
            }
            return;
        }
        if (!found)
        {
            cout << "Customer not found.\n";
        }
    }
}
void BillingSystem ::delete_customer()
{
    int id;
    char choice;
    bool found = false;
    cout << "Enter customer ID  : ";
    cin >> id;
    for (int i = 0; i < customers.size(); i++)
    {
        if (customers[i].getID() == id)
        {
            found = true;
            customers[i].display();
            while (true)
            {
                cout << "Are you sure you want to delete this customer? (Y/N): ";
                cin >> choice;
                cin.ignore(1000, '\n');

                if (choice == 'Y' || choice == 'y')
                {
                    customers.erase(customers.begin() + i);
                    update_file();

                    cout << "Customer deleted successfully.\n";
                    return;
                }
                else if (choice == 'N' || choice == 'n')
                {
                    cout << "Deletion cancelled.\n";
                    return;
                }
                else
                {
                    cout << "Invalid choice! Please enter Y or N.\n";
                }
            }
        }
    }
    if (!found)
    {
        cout << "Customer not found" << endl;
    }
}
void BillingSystem ::calculate_bill()
{
    int id;
    cout << "Enter ID: ";
    cin >> id;

    for (Customer &c : customers)
    {
        if (c.getID() == id)
        {
            c.calculate_bill();
            c.display();
            update_file();
            return;
        }
    }
    cout << "Customer not found\n";
}

void BillingSystem ::pay_bill()
{
    int id;
    cout << "Enter ID: ";
    cin >> id;

    for (Customer &c : customers)
    {
        if (c.getID() == id)
        {
            c.pay_bill();
            update_file();
            return;
        }
    }
}
void BillingSystem ::show_pending()
{
    for (Customer &c : customers)
    {
        if (!c.getPaymentStatus())
            c.display();
    }
}
void BillingSystem ::search_by_name()
{
    string name;
    cout << "Enter name: ";
    cin.ignore();
    getline(cin,name);

    for (Customer &c : customers)
    {
        if (c.getName() == name)
            c.display();
    }
}
void BillingSystem ::sort_by_bill()
{
    sort(customers.begin(), customers.end(),[](const Customer& a , const  Customer& b){
        return a.getBill() > b.getBill();  //here we are using lambda function as 3rd parameter for sort() to tell slort how to comparer elements...if it returns true means a is placed before b otherwose oppsoite
    });

    cout << "\nSorted by bill (high to low):\n";
    for (Customer &c : customers)
        c.display();
}
void BillingSystem :: sort_by_name(){
    sort(customers.begin(),customers.end(),[](const Customer& a,const Customer& b){
        return a.getName() < b.getName();
    });

    cout<<"Sorted By Name : "<<endl;
    for (Customer &c : customers)
        c.display();
}
void BillingSystem::sort_by_units()
{
    sort(customers.begin(), customers.end(),
    [](const Customer& a, const Customer& b)
    {
        return a.getUnits() > b.getUnits();
    });

    cout << "\nSorted By Units Consumed (High to Low):\n";

    for (Customer& c : customers)
    {
        c.display();
    }
}
void BillingSystem ::usage_analysis()
{
    cout << "=============================================" << endl;
    cout << "             SYSTEM DASHBOARD" << endl;
    cout << "=============================================" << endl
         << endl;
    if (customers.empty())
    {
        cout << "No customers available." << endl;
        return;
    }

    int totalPaid = 0, totalPending = 0;
    float totalUnits = 0;
    float totalBillAmount = 0;
    float totalBillCollected = 0;
    float highestBill = customers[0].getBill();
    float lowestBill = customers[0].getBill();
    Customer highestCustomer = customers[0];
    Customer lowestCustomer = customers[0];
    int highUsageCustomers = 0;
    for (Customer &c : customers)
    {
        if (c.getPaymentStatus())
        {
            totalPaid++;
            totalBillCollected += c.getBill();
        }
        else
        {
            totalPending++;
        }
        totalUnits += c.getUnits();
        totalBillAmount += c.getBill();
        if (c.getBill() > highestBill)
        {
            highestBill = c.getBill();
            highestCustomer = c;
        }
        if (c.getBill() < lowestBill)
        {
            lowestBill = c.getBill();
            lowestCustomer = c;
        }
        if (c.getUnits() > 300)
        {
            highUsageCustomers++;
        }
    }
    float avg_units = (float)totalUnits / customers.size();
    float pendingAmount = totalBillAmount - totalBillCollected;

    cout << "Total Customers       : " << customers.size()<<endl;
    cout << "Paid Customers        : " << totalPaid << endl;
    cout << "Pending Customers     : " << totalPending << endl
         << endl;
    cout << "Total Units           :" << totalUnits << endl;
    cout << "Average Units         :" << avg_units << endl
         << endl;
    cout << "Total Bill Amount     :" << totalBillAmount << endl;
    cout << "Total Bill collected  :" << totalBillCollected << endl;
    cout << "Pending Amount        :" << pendingAmount << endl
         << endl;
    cout << "---------------------------------------" << endl;
    cout << "Highest Bill          : " << highestBill << endl;
    cout << "Customer ID           : " << highestCustomer.getID() << endl;
    cout << "Customer Name         : " << highestCustomer.getName() << endl
         << endl;
    cout << "Lowest Bill           : " << lowestBill << endl;
    cout << "Customer ID           : " << lowestCustomer.getID() << endl;
    cout << "Customer Name         : " << lowestCustomer.getName() << endl;

    cout << endl;
    cout << "High Usage Customers  : " << highUsageCustomers << endl;
    cout << "=========================================" << endl;
}
void BillingSystem ::generate_receipt()
{
    int id;
    cout << "Enter ID: ";
    cin >> id;

    for (Customer &c : customers)
    {
        if (c.getID() == id)
        {
            c.generateCustomer_receipt();
            return;
        }
    }
}
void BillingSystem ::display_all()
{
    for (Customer &c : customers)
        c.display();
}
int main()
{
    BillingSystem system;

    if (!system.login())
        return 0;

    system.LoadFromFile();

    int choice;

    while (1)
    {
        cout << "\n===== ELECTRICITY BILL SYSTEM =====\n";
        cout << "1. Add Customer\n";
        cout << "2. Calculate Bill\n";
        cout << "3. Pay Bill\n";
        cout << "4. View All\n";
        cout << "5. Pending Bills\n";
        cout << "6. Search by Name\n";
        cout << "7. Sort by Bill\n";
        cout << "8. Sort by Name\n";
        cout << "9. Sort by Units\n";
        cout << "10. Usage Analysis\n";
        cout << "11. Generate Receipt\n";
        cout << "12. Update customer\n";
        cout << "13. Delete Customer\n";
        cout << "14. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            system.add_customer();
            break;
        case 2:
            system.calculate_bill();
            break;
        case 3:
            system.pay_bill();
            break;
        case 4:
            system.display_all();
            break;
        case 5:
            system.show_pending();
            break;
        case 6:
            system.search_by_name();
            break;
        case 7:
            system.sort_by_bill();
            break;
        case 8:
            system.sort_by_name();
            break;
        case 9:
            system.sort_by_units();
            break;
        case 10:
            system.usage_analysis();
            break;
        case 11:
            system.generate_receipt();
            break;
        case 12:
            system.update_customer();
            break;
        case 13:
            system.delete_customer();
            break;
        case 14:
            exit(0);
            break;
        default:
            cout << "Invalid choice\n";
            break;
        }
    }
}