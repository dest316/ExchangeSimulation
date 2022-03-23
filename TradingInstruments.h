#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Order
{
	double price;
	bool type;

};

class TradingInstrument
{
protected:
	string _name;
	double _price;
	int _orderByBuyCount;
	int _orderBySellCount;
	double _longLeverage;
	double _shortLeverage;
	vector<Order> _listOfOrders;

public:
	vector<Order> GetListOfOrders() { return _listOfOrders; }
	double GetPrice() { return _price; }
	void SetOrderBySellCount(int count) { _orderBySellCount -= count; }
	double GetOrderBySellCount() { return _orderBySellCount; }
	
	

	
};

