#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK 1
//

Transaction::Transaction( std::string ticker_symbol, unsigned int day_date, unsigned int month_date, unsigned year_date, bool buy_sell_trans, unsigned int number_shares, double trans_amount ):

  symbol{ticker_symbol}, 
  day{day_date},
  month{month_date},
  year{year_date},
  trans_type{(buy_sell_trans)?("Buy"):("Sell")},
  shares{number_shares},
  amount{trans_amount},
  trans_id{assigned_trans_id}, 
  acb{},
  acb_per_share{},
  share_balance{},
  cgl{},
  p_next{nullptr}

{++assigned_trans_id;};

// Destructor
// TASK 1
//

Transaction::~Transaction(){
  delete p_next;
  p_next = nullptr;
}

// Overloaded < operator.
// TASK 2
//
bool Transaction::operator< (Transaction const &other){ 
   
    if (get_year() > other.get_year()){

      return false;

    } else if (get_year() < other.get_year()){

      return true;

    } else if (get_month() > other.get_month()){

      return false;

    }  else if (get_month() < other.get_month()){

      return true;

    } else if (get_day() > other.get_day()){

      return false;

    } else if (get_day() < other.get_day()){

      return true;

    } else if (get_year() == other.get_year() && get_month() == other.get_month() && get_day() == other.get_day()){ 
      //comparing transaction IDs
        if (get_trans_id() > other.get_trans_id()){
          return true;
        }
    }

  return false;
  
}

// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK 3
//

History::History():
p_head{nullptr}
{}
  
// Destructor
// TASK 3
//

History::~History(){

  delete p_head;
  p_head = nullptr;
}


// read_history(...): Read the transaction history from file.
// TASK 4
//

void History::read_history(){
  
   ece150::open_file();

      while(ece150::next_trans_entry()){
        std::string symbol = ece150::get_trans_symbol();
        unsigned int day = ece150::get_trans_day();
        unsigned int month = ece150::get_trans_month();
        unsigned int year = ece150::get_trans_year();
        unsigned int shares = ece150::get_trans_shares();
        bool type = ece150::get_trans_type();
        double amount = ece150::get_trans_amount();

        insert (new Transaction (symbol, day, month, year, type, shares, amount));
      }

   ece150::close_file(); 

}


// insert(...): Insert transaction into linked list.
// TASK 5
//

//Adds the passed Transaction instance to the end of the linked-list of Transactions. 
void History::insert( Transaction *p_new_trans ){

  if (p_head == nullptr){
    p_head = p_new_trans;
  } else {
    Transaction *temp_ptr = p_head;
   
    while (temp_ptr -> get_next() != nullptr){
      temp_ptr = temp_ptr -> get_next();
    }

    temp_ptr -> set_next(p_new_trans);
  }

}

// sort_by_date(): Sort the linked list by trade date.
// TASK 6
//


/*Sorts the linked list in ascending order of trade date (or transaction id). Uses the overloaded less-than
operator to define sort order.*/
void History::sort_by_date(){

  Transaction *new_head{p_head}; // head of sorted list
  Transaction *temp_ptr{nullptr}; 

  p_head = p_head -> get_next();
  new_head -> set_next(nullptr);

  Transaction *next{p_head}; //next node of the unsorted
  
  while (p_head != nullptr){
    next = next -> get_next();
    if (*p_head < *new_head){
      p_head -> set_next(new_head);
      new_head = p_head;
    } else {
      temp_ptr = new_head;
      bool sorted {false};

      while (temp_ptr -> get_next() != nullptr){
        if (*p_head < *(temp_ptr -> get_next())){
          p_head -> set_next(temp_ptr -> get_next());
          temp_ptr -> set_next(p_head);
          sorted = true;
          break;
        } 
        temp_ptr = temp_ptr -> get_next();
      }

      if (sorted == false){
        temp_ptr -> set_next(p_head);
        p_head -> set_next(nullptr);
      }

      

    }
    p_head = next;
  }

  p_head = new_head;
}

// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//

/*Walks through the linked-list and updates the following private member variables: acb, acb_per_share,
share_balance, and cgl for each Transaction. See section on Computing gains and losses using Adjusted Cost
Base for additional details on how these values are calculated.
*/
void History::update_acb_cgl(){
  
  double total_amount{0.0};
  unsigned int total_shares{0};
  Transaction *temp_ptr = p_head;
  Transaction *prev_ptr {nullptr};

  while (temp_ptr != nullptr){

   if (temp_ptr -> get_trans_type()){
      // if it is a buy transaction

      if (temp_ptr == p_head){
        temp_ptr -> set_acb(temp_ptr -> get_amount());
        temp_ptr -> set_share_balance(temp_ptr -> get_shares());
        temp_ptr -> set_acb_per_share((temp_ptr -> get_acb())/(temp_ptr -> get_share_balance()));

      } else {
        // update acb
        total_amount = (temp_ptr -> get_amount()) + (prev_ptr -> get_acb());
        temp_ptr -> set_acb(total_amount);

        // update share_balance
        total_shares = (temp_ptr -> get_shares() )+ (prev_ptr -> get_share_balance());
        temp_ptr -> set_share_balance(total_shares);

        // update acb_per_share
        temp_ptr -> set_acb_per_share((temp_ptr -> get_acb())/(temp_ptr -> get_share_balance()));
      }

    } else {
      // if it is a sell transaction

      if (temp_ptr == p_head){
        temp_ptr -> set_acb(-(temp_ptr ->get_shares()));
        temp_ptr -> set_share_balance (-(temp_ptr -> get_shares()));
        temp_ptr -> set_acb_per_share((temp_ptr -> get_acb())/(temp_ptr -> get_share_balance()));

      } else {
        // update acb
        total_amount = (prev_ptr -> get_acb()) - (temp_ptr -> get_shares()) * (prev_ptr -> get_acb_per_share());
        temp_ptr -> set_acb(total_amount);

        // update share_balance
          total_shares = (prev_ptr -> get_share_balance()) - (temp_ptr -> get_shares());
          temp_ptr -> set_share_balance (total_shares);

        // update acb_per_share
          temp_ptr -> set_acb_per_share((temp_ptr -> get_acb())/(temp_ptr -> get_share_balance()));

        // update cgl
          // we subtract the number of shares sold multiplied by the ACB/Share from the previous transaction. 
          temp_ptr -> set_cgl((temp_ptr -> get_amount()) - (temp_ptr -> get_shares()) * (prev_ptr -> get_acb_per_share()));
      }

    }

  prev_ptr = temp_ptr;
  temp_ptr = temp_ptr -> get_next();
    
  }

}

// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8

/* Computes the capital gains or capital losses for every transaction in the history of transactions and updates
the respective instances in the linked list. In addition, this function returns the total capital gains for the
specified year. See section on Computing gains and losses using Adjusted Cost Base for additional details on
how to the CGL is calculated */
double History::compute_cgl( unsigned int year ){
  // we subtract the number of shares sold multiplied by the ACB/Share from the previous transaction.
  Transaction *temp_ptr = p_head;
  Transaction *prev_ptr{nullptr};
  double total_cgl{0.0};

  while (temp_ptr != nullptr){

    if (year == temp_ptr -> get_year()){
      total_cgl += temp_ptr -> get_cgl();
    }

    temp_ptr = temp_ptr -> get_next();
  }
  //run through entire LL with while loop
  //  if year = current node.getyear()
  //  add node cgl to total cgl
  //  cureent node = next node

  //return total cgl
  
  return total_cgl; 
}


// print() Print the transaction history.
//TASK 9
//

//Prints to the console output the transaction history. 
void History::print(){

  std::cout << "========== BEGIN TRANSACTION HISTORY ============" << std::endl;

  Transaction *temp_ptr = p_head;

  while(temp_ptr != nullptr){
    temp_ptr -> print();
    temp_ptr = temp_ptr -> get_next();
  }
  
  std::cout << "========== END TRANSACTION HISTORY ============" << std::endl;
}

// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head;}
