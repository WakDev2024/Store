#include <iostream>
#include <string>
#include <fstream> // for reading/writing files
#include <type_traits> //for using templates
#include <iomanip> // for input/output manipulation in terminal

struct user {
    int id;
    char type;
    std::string username;
    std::string email;
    std::string password;
    struct user* next;
};

struct product {
    int id;
    std::string title;
    std::string desc;
    std::string cat;
    float price;
    int stock;
    struct product* next;
};

struct card_info {
    std::string first_name;
    std::string last_name;
    std::string RIB;  //16 nbr
    std::string expir_date; // MM/YY
    int cvv;
    double balance;
};

void print_option(const char* option, const char* option_symbol, const char* color = "\033[36m") {
    std::cout << color ;
    std::cout << std::left << std::setw(25) << std::setfill('.')  << option << option_symbol << std::endl << "\033[0m" ;
}

template <typename T>
void generate_elem(T* elem, const char* elem_line) {
    const char* ptr1 = elem_line;
    const char* ptr2 = ptr1;
    std::string id, price, stock;
    char type;

    char sep = '|';
    int sep_count = 0;

    while(*ptr1 != '\0') {
        if(*ptr1 == sep) {
            sep_count++;
        }
        ptr1++;
    }
    ptr1 = elem_line;

    for(int i=0;i<=sep_count;i++) {
        while(*ptr2 != sep && *ptr2 != '\0') {
            ptr2++;
        }

        while(ptr1 < ptr2) {
            if(i==0) id.insert(id.length(),1,*ptr1);
            if constexpr (std::is_same_v<T,product>) {
                if(i==1) elem->title.insert(elem->title.length(),1,*ptr1);
                if(i==2) elem->desc.insert(elem->desc.length(),1,*ptr1);
                if(i==3) elem->cat.insert(elem->cat.length(),1,*ptr1);
                if(i==4) price.insert(price.length(),1,*ptr1);
                if(i==5) stock.insert(stock.length(),1,*ptr1);
            } 
            else {
                if(i==1) elem->type = *ptr1;
                if(i==2) elem->username.insert(elem->username.length(),1,*ptr1);
                if(i==3) elem->email.insert(elem->email.length(),1,*ptr1);
                if(i==4) elem->password.insert(elem->password.length(),1,*ptr1);  
            }
            ptr1++;
        }
        ptr1++;
        ptr2++; 
    }
    
    elem->id = std::stoi(id);
    if constexpr (std::is_same_v<T,product>) {
        elem->price = std::stof(price);
        elem->stock = std::stoi(stock);
    }

}

template <typename T>
void integrate_elem(T** head, std::string elem_line) {
    T* elem = new T;
    generate_elem(elem, elem_line.c_str());
    elem->next = NULL;
   
    if(!*head) {
        *head = elem;
        return;
    }
    
    T* temp = *head;
    while(temp->next) {
        temp = temp->next;
    }

    temp->next = elem;
}

template <typename T>
void uploadElements(T** head,std::string file_name) {
    std::string elem_line;
    std::fstream file;
    file.open(file_name,std::ios::in);
    if(file.is_open()) {
        while(getline(file,elem_line)) {
            integrate_elem(head,elem_line);
        }
    }
    else {
        std::cout << "Error while opening " << file_name << " for reading !!!" << std::endl;
    }
}

int login(user* head) {
    std::string email, password;
    std::cout << "<> Login" << std::endl;
    //email
    std::cout << "E-mail : " << std::endl;
    std::cout << ">> ";
    std::cin >> email;
    std::cin.ignore();
    //password
    std::cout << "Password : " << std::endl;
    std::cout << ">> ";
    getline(std::cin,password);

    while(head) {
        if(email == head->email && password == head->password) {
            std::cout << "Logged In Successfully " << std::endl;
            if(head->type == 'A') return 1;
            else return 2;
        }

        head = head->next;
    }
    std::cout << "Your credentials are not valid !!!" << std::endl;
    return 0;
}

template <typename X>
void add_element(X** head, int is_admin = -1) {
    X *new_e = new X;
    int elem_id = 0;
    if constexpr (std::is_same_v <X,product>) {
        std::cout << "<> Add Product " << std::endl;
        //title
        std::cout << "Title : " << std::endl;
        std::cout << ">> ";
        getline(std::cin,new_e->title);
        //description
        std::cout << "Description : " << std::endl;
        std::cout << ">> ";
        getline(std::cin, new_e->desc);
        //cat
        std::cout << "Category : " << std::endl;
        std::cout << ">> ";
        getline(std::cin,new_e->cat);
        //price
        std::cout << "Price : " << std::endl;
        std::cout << ">> ";
        std::cin >> new_e->price;
        //stock
        std::cout << "Stock : " << std::endl;
        std::cout << ">> ";
        std::cin >> new_e->stock;
        //next
        new_e->id = elem_id;
        new_e->next = NULL;
    }
    else {
        std::cout << "<> Sign Up" << std::endl;
        //id
        new_e->id = elem_id;
        //type
        if(is_admin) new_e->type = 'A';
        else new_e->type = 'C';
        //username
        std::cout << "Username : ";
        getline(std::cin,new_e->username);

        //email
        std::cout << "E-mail : ";
        std::cin >> new_e->email;
        std::cin.ignore();

        //password
        std::cout << "Password : ";
        getline(std::cin,new_e->password);
        //next
        new_e->next = NULL;

    }
    
    if(!*head) {
        *head = new_e;
        return;
    }

    X* temp = *head;
    while(temp->next) {
        elem_id++;
        temp = temp->next;
    }
    
    new_e->id = ++elem_id;
    temp->next = new_e;
    
}

template <typename X>
void modify_element(X** head) {
    if(!*head) {
        if constexpr (std::is_same_v<X,product>)  
            std::cout << "There is no product to modify" << std::endl;
        else    
            std::cout << "There is no user to modify" << std::endl;
            return;
    }
    int id;
    if constexpr (std::is_same_v<X,product>)
        std::cout << "Enter the ID of Product : " << std::endl << ">> ";
    else    
        std::cout << "Enter the ID of user : " << std::endl << ">> ";

    std::cin >> id;

    X* temp = *head;
    char decision;

    while(temp) {
        if(temp->id == id) {
            if constexpr(std::is_same_v<X,product>) {
                //Title
                std::cout << "Do you want to modify the title ? (y,n)"<<std::endl << ">> ";
                std::cin >> decision;
                if(decision == 'y' || decision == 'Y') {
                    std::cout << "Enter the new title : "<<std::endl << ">> ";
                    std::cin >> temp->title;
                } 
                //Description
                std::cout << "Do you want to modify the description ? (y,n)"<<std::endl << ">> ";
                std::cin >> decision;
                if(decision == 'y' || decision == 'Y') {
                    std::cout << "Enter the new description : "<<std::endl << ">> ";
                    std::cin >> temp->desc;
                } 
                //category
                std::cout << "Do you want to modify the category ? (y,n)"<<std::endl << ">> ";
                std::cin >> decision;
                if(decision == 'y' || decision == 'Y') {
                    std::cout << "Enter the new category : "<<std::endl << ">> ";
                    std::cin >> temp->cat;
                } 
                //price
                std::cout << "Do you want to modify the price ? (y,n)"<<std::endl << ">> ";
                std::cin >> decision;
                if(decision == 'y' || decision == 'Y') {
                    std::cout << "Enter the new price : "<<std::endl << ">> ";
                    std::cin >> temp->price;
                } 
                //stock
                std::cout << "Do you want to modify the stock ? (y,n)"<<std::endl << ">> ";
                std::cin >> decision;
                if(decision == 'y' || decision == 'Y') {
                    std::cout << "Enter the new stock : "<<std::endl << ">> ";
                    std::cin >> temp->stock;
                } 
            }
            else {
                //type
                std::cout << "Do you want to modify the type ? (y,n)"<<std::endl << ">> ";
                std::cin >> decision;
                if(decision == 'y' || decision == 'Y') {
                    std::cout << "Enter the new type : "<<std::endl << ">> ";
                    std::cin >> temp->type;
                } 
                //username
                std::cout << "Do you want to modify the username ? (y,n)"<<std::endl << ">> ";
                std::cin >> decision;
                if(decision == 'y' || decision == 'Y') {
                    std::cout << "Enter the new username : "<<std::endl << ">> ";
                    std::cin >> temp->username;
                } 
                //email
                std::cout << "Do you want to modify the email ? (y,n)"<<std::endl << ">> ";
                std::cin >> decision;
                if(decision == 'y' || decision == 'Y') {
                    std::cout << "Enter the new email : "<<std::endl << ">> ";
                    std::cin >> temp->email;
                } 
                //password
                std::cout << "Do you want to modify the password ? (y,n)"<<std::endl << ">> ";
                std::cin >> decision;
                if(decision == 'y' || decision == 'Y') {
                    std::cout << "Enter the new password : "<<std::endl << ">> ";
                    std::cin >> temp->password;
                } 
            }
                       
        }
        temp = temp->next;
    }
    std::cout << "ID is invalid";

}

template <typename Z>
void delete_element(Z**head) {
    int id;
    if constexpr (std::is_same_v<Z,product>) 
        std::cout << "Enter the id of product : " << std::endl << ">> ";
    else
        std::cout << "Enter the id of user : " << std::endl << ">> ";
    
    std::cin >> id;
    if(!*head) {
        if constexpr (std::is_same_v<Z,product>) 
            std::cout << "Can't delete product, store is empty " << std::endl;
        else    
            std::cout << "There is no users to delete " << std::endl;
        return;
    }
    
    Z* temp = *head;
    
    if((*head)->id == id) {
        *head = (*head)->next;
        delete temp;
        if constexpr (std::is_same_v<Z,product>) 
            std::cout << "Product deleted successfuly"<< std::endl;
        else
            std::cout << "User deleted successfuly"<< std::endl;
        return;
    }


    while(temp->next && temp->next->id != id) {
        temp = temp->next;
    }
        
    if(temp->next && (temp->next->id == id)) {
        temp->next = temp->next->next;
        if constexpr (std::is_same_v<Z,product>)
            std::cout << "Product deleted successfuly"<< std::endl;
        else 
            std::cout << "User deleted successfuly"<< std::endl;
    }
    else {
        if constexpr (std::is_same_v<Z,product>)
            std::cout << "Can't delete product, id is invalid " << std::endl;
        else 
            std::cout << "Can't delete user, id is invalid" << std::endl;
    }
}

template <typename X>
void view_elements(X *head, bool is_admin = 0) {
    int elem_count = 0;
    if(!head) {
        if constexpr (std::is_same_v<X,product>) 
            std::cout << "Store doesn't have any product"<< std::endl;
        else 
            std::cout << "There is no accounts to display"<< std::endl;
            return;
    }
    while(head) {
        if constexpr (std::is_same_v<X,product>) {
            std::cout << std::endl << "<> Product " << ++elem_count << std::endl;
            if(is_admin) std::cout << "\tID : " << head->id << std::endl;
            std::cout << "\tTitle : " << head->title << std::endl;
            std::cout << "\tDescription : " << head->desc << std::endl;
            std::cout << "\tCategory : " << head->cat << std::endl;
            std::cout << "\tPrice : " << head->price << std::endl;
            std::cout << "\tStock : " << head->stock << std::endl;
        }
        else {
            std::cout << std::endl << "<> Account " << ++elem_count << std::endl;
            std::cout << "\tID : " << head->id << std::endl;
            std::cout << "\tType : " << head->type << std::endl;
            std::cout << "\tusername : " << head->username << std::endl;
            std::cout << "\temail : " << head->email << std::endl;
            std::cout << "\tpassword : " << head->password << std::endl;
            
        }
        head = head->next;
    }
}
template <typename X>
void view_element(X *head, bool is_admin = 0) {
    
        if constexpr (std::is_same_v<X,product>) {
            std::cout << std::endl << "<> Product " << std::endl;
            if(is_admin) std::cout << "\tID : " << head->id << std::endl;
            std::cout << "\tTitle : " << head->title << std::endl;
            std::cout << "\tDescription : " << head->desc << std::endl;
            std::cout << "\tCategory : " << head->cat << std::endl;
            std::cout << "\tPrice : " << head->price << std::endl;
            std::cout << "\tStock : " << head->stock << std::endl;
        }
        else {
            std::cout << std::endl << "<> Account " << std::endl;
            std::cout << "\tID : " << head->id << std::endl;
            std::cout << "\tType : " << head->type << std::endl;
            std::cout << "\tusername : " << head->username << std::endl;
            std::cout << "\temail : " << head->email << std::endl;
            std::cout << "\tpassword : " << head->password << std::endl;
            
        }  
}

template <typename X>
void search_element(X* head) {
    int elems_found = 0;
    std::string search_kW;
    if constexpr (std::is_same_v <X,product>)
        std::cout << "Enter a keyword of product ( title, description, category ) : " << std::endl << ">> "; 
    else 
        std::cout << "Enter a keyword of user ( username, email ) : " << std::endl << ">> "; 

    std::cin >> search_kW;

    while(head) {
        if constexpr (std::is_same_v <X,product> ) {
            if(head->title == search_kW || head->desc == search_kW || head->cat == search_kW) {
                view_element(head);
                elems_found++;
            }     
        }
        else {
            if(head->username == search_kW || head->email == search_kW) {
                view_element(head,1);
                elems_found++;
            }     
        }
        head = head->next;   
    }
    if(elems_found <=1 ) 
        std::cout << elems_found << " element found." << std::endl;
    else
        std::cout << elems_found << " elements found." << std::endl;

}

template <typename Y>
void save_changes(Y** elem_head,std::string file_name) {
    std::fstream elem_file;
    elem_file.open(file_name,std::ios::out);
    if(elem_file.is_open()) {
        if(!*elem_head) {
            elem_file << "";
            return;
        }
        Y* temp;
        
        while(*elem_head) {
            temp = *elem_head;
            if constexpr (std::is_same_v<Y,product>) {
                elem_file << (*elem_head)->id << "|" << (*elem_head)->title << "|" << (*elem_head)->desc << "|" << (*elem_head)->cat << "|" << (*elem_head)->price << "|" << (*elem_head)->stock << std::endl;   
            }
            else {
                elem_file << (*elem_head)->id << "|" << (*elem_head)->type << "|" << (*elem_head)->username << "|" << (*elem_head)->email << "|" << (*elem_head)->password << std::endl;             
            }
            *elem_head = (*elem_head)->next;
            delete temp;
        } 
    }
    else {
        std::cout << "Error while opening products.txt for reading"<< std::endl; 
    }
}

card_info insert_card_info() {
    card_info card;
    std::cout << "<> Card Info "<< std::endl;
    std::cout << "Enter First Name : " << std::endl << ">> ";
    std::cin >> card.first_name;
    std::cout << "Enter Last Name : " << std::endl << ">> ";
    std::cin >> card.last_name;
    std::cout << "Enter RIB : " << std::endl << ">> ";
    std::cin >> card.RIB;
    return card;
}

bool is_card_valid(card_info& user_card) { // the card that is provided by user
    std::ifstream file("DB/creditCard.txt");
    std::string line;
    
    if (!file.is_open()) {
        std::cout << "Error while opening creditCard.txt for reading !!!" << std::endl;
        return false;
    }
    
    while(std::getline(file,line)) {

        const char* c_line = line.c_str();
        const char* ptr = c_line;
        card_info card_line; // the card info that exists in file
        std::string cvv, balance;
        int sep_count = 0;   

        while(*ptr != '\0') {
            if(*ptr == '|') {
                ptr++;
                sep_count++;
                continue;
            }
            switch(sep_count) {
                case 0 :    card_line.first_name += *ptr; break;    
                case 1 :    card_line.last_name += *ptr; break;    
                case 2 :    card_line.RIB += *ptr; break;    
                case 3 :    card_line.expir_date += *ptr; break;    
                case 4 :    cvv += *ptr; break;    
                case 5 :    balance += *ptr; break;    
            }
            ptr++;
        }
    
        if( user_card.first_name == card_line.first_name &&
            user_card.last_name == card_line.last_name &&
            user_card.RIB == card_line.RIB ) 
        {
            user_card.cvv = std::stoi(cvv); // insert additional info into user's card
            user_card.balance = std::stof(balance);

            return true; //card is valid
        }
    }
    file.close();
    return false;

}

void buy_product(product* prod_head, card_info& card) {
    std::string category, title;
    int items_nbr;

    std::cout << "<> Buy Product" << std::endl;
    std::cout << "Enter Product category : "<< std::endl << ">> "; 
    std::cin >> category;
    std::cout << "Enter Product Title : " << std::endl << ">> ";
    std::cin >> title;
    
    while(prod_head) {
        if(prod_head->cat == category && prod_head->title == title) {
            if(prod_head->stock == 0) {
                std::cout << "The stock is out for this product";
                return;
            }
            do {            
                std::cout << "How many item you want (MAX " << prod_head->stock << ") : ";
                std::cin >> items_nbr;
            } while(items_nbr > prod_head->stock || items_nbr <= 0);

            if(card.balance <items_nbr*prod_head->price) {          // verify if balance is enough
                std::cout << "You don't have enough money for"<< items_nbr <<" item(s)\n";
                return;
            }
            card.balance -= items_nbr*prod_head->price;   // control balance - n*price
            prod_head->stock -= items_nbr;                 //control the stock of product
            std::cout << "Product purchased seccusfuly" << std::endl;
            // store product in a file 
            std::fstream file;
            file.open("DB/purchased_prods.txt",std::ios::app);
            if(file.is_open()) {
                file << prod_head->id << "|" << prod_head->title << "|" << prod_head->cat << "|" << std::fixed << std::setprecision(2) << prod_head->price << "\n"; 
            }
            else {
                std::cout << "Error while opening a file !!!" << std::endl;
                file.close();
                return;            
            }
            file.close();
            return;
        }
        prod_head = prod_head->next;
    }
    std::cout << "This product does not exist\n";
}

void update_card_info(card_info card) {
    std::fstream file;
    file.open("DB/creditCard.txt",std::ios::out);
    if(file.is_open()) {
        file << card.first_name << "|" << card.last_name << "|" << card.RIB << "|" << card.expir_date << "|" << card.cvv << "|" << std::fixed << std::setprecision(2) <<  card.balance << "\n";
    }
    else {
        std::cout << "Error while opening a file for writing !!!" << std::endl;
    }
    file.close();
}


int main() {
    char operation;
    product* products_head = NULL;
    user* users_head = NULL;
    card_info card{}; // create a card and initialize it's values to default
    int is_logged = 0;
    bool is_admin = 0;
    std::string storeName = "Wakrimi Fashion";
    uploadElements(&users_head,"DB/loginReg.txt");
    uploadElements(&products_head,"DB/products.txt");
    std::cout << std::endl << "Welcome to " + storeName + " store" << std::endl << std::endl;
    while(1) {
        if(is_logged) {        
            std::cout << "Products Management : " << std::endl;
            print_option("Add Credit Card", "(C)");
            print_option("View Products", "(V)");
            print_option("Find Products", "(F)");
            print_option("Buy Product", "(B)");

            if(is_admin) {
                print_option("Add Product", "(A)");
                print_option("Modify Product", "(M)");
                print_option("Delete Product", "(D)");

                std::cout << "Users Management : " << std::endl;
                print_option("Add Account", "(P)");
                print_option("View Accounts", "(W)");
                print_option("Edit Account", "(E)");
                print_option("Search for Account", "(Z)");
                print_option("Remove Account", "(R)");
            }
            print_option("Log Out", "(O)");
        }
        else {
            print_option("Sign Up", "(S)");
            print_option("Log In", "(L)");
        }
        print_option("Quit", "(Q)");
        std::cout << std::endl << ">> ";
        while(!(std::cin >> operation)) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << ">> "; 
        }
        std::cin.ignore();
        switch (operation) {
            case 'O' :
                    if(is_logged) is_logged = 0;
                    else std::cout << "Access denied" << std::endl; 
                    break;
            case 'S' : 
                    if(!is_logged) add_element(&users_head,is_admin);
                    else std::cout << "Access denied" << std::endl; 
                    break;
            case 'L' : 
                    if(!is_logged) {
                        is_logged = login(users_head);
                        if(is_logged == 1) is_admin = 1;
                        else is_admin = 0;
                    }
                    else std::cout << "Access denied" << std::endl; 
                    break;
            case 'A' :
                    if(is_logged && is_admin) add_element(&products_head);
                    else std::cout << "Access denied" << std::endl; 
                    break;
            case 'P' :
                    if(is_logged && is_admin) add_element(&users_head);
                    else std::cout << "Access denied" << std::endl; 
                    break;
            case 'M' : 
                    if(is_logged && is_admin) modify_element(&products_head);
                    else std::cout << "Access denied" << std::endl;
                    break;
            case 'E' : 
                    if(is_logged && is_admin) modify_element(&users_head);
                    else std::cout << "Access denied" << std::endl;
                    break;
            case 'D' :
                    if(is_logged && is_admin) delete_element(&products_head);
                    else std::cout << "Access denied" << std::endl; 
                    break;
            case 'R' :
                    if(is_logged && is_admin) delete_element(&users_head);
                    else std::cout << "Access denied" << std::endl; 
                    break;
            case 'V' : 
                    if(is_logged) view_elements(products_head,is_admin);
                    else std::cout << "Access denied" << std::endl; 
                    break;
            case 'W' : 
                    if(is_logged && is_admin) view_elements(users_head);
                    else std::cout << "Access denied" << std::endl; 
                    break;
            case 'F' : 
                    if(is_logged) search_element(products_head);
                    break;
            case 'C' : 
                    if(is_logged) {
                        int inc=0;
                        do {
                            if(inc) 
                                std::cout << "Sorry, your card info are incorrect" << std::endl;
                            card = insert_card_info();
                            inc = 1;
                        }
                        while(!is_card_valid(card));
                        std::cout << "<> Credit card is inserted successfuly\n";
                    }
                    break;
            case 'B' :
                    if(is_logged) {
                        if(card.RIB.empty()) {
                            std::cout << "You haven't added a credit card\n";
                            break;
                        }
                        buy_product(products_head,card);
                        update_card_info(card);
                    }
                    break;

            case 'Z' : 
                    if(is_logged && is_admin) search_element(users_head);
                    break;

            case 'Q' : 
                    std::cout << std::endl << "<> Quit" << std::endl << std::endl;
                    save_changes(&products_head,"DB/products.txt");
                    save_changes(&users_head,"DB/loginReg.txt");
                    exit(0);
                    break;
        }
    }
    return 0;
}