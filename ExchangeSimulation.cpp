#include <iostream>
#include <vector>
#include <memory>
#include <time.h>
#include "TradingInstruments.h"
#include "DoublyLinkedList.h"

using namespace std;

class Client
{
private:
	double _balance = 0;
	DoublyLinkedList<TradingInstrument> _positions;
	static Client* _client;
	Client()
	{
		_positions = *(new DoublyLinkedList<TradingInstrument>());
	}
public:
	static Client& GetClient()
	{
		if (_client == nullptr)
		{
			_client = new Client();
		}
		return *_client;
	}
	void operator=(const Client&) = delete;
	DoublyLinkedList<TradingInstrument>& GetPositions() { return _positions; }
	int Buy(TradingInstrument asset)
	{
		if (asset.IsBuyable())
		{
			_positions.Add(asset);
			_balance -= asset.GetPriceToSell();
			auto tmp = asset.GetGlass().GetGlassToSell().begin();
			
			asset.GetGlass().GetGlassToSell().erase(tmp); 
			return 0;
		}
		return -1;
	}
	void TopUpBalance(int amount)
	{
		_balance += amount;
	}
	int WithdrawMoney(int amount)
	{
		return (amount > _balance) ? -1 : 0;
	}
	int GetLiquidCost()
	{
		int summ = 0;
		for (auto& i : _positions)
		{
			summ += i.data.GetAveragePrice();
		}
		return summ;
	}
};

Client* Client::_client = nullptr;

ostream& operator<<(std::ostream& out, TradingInstrument& ti)
{
	out << ti.GetName();
	return out;
}

int main()
{
	
	srand(time(NULL));
	TradingInstrument* trad = new TradingInstrument("example");
	Stock* sberStock = new Stock("sberbank");
	Stock* appleStock = new Stock("apple");
	Client::GetClient();
	Client::GetClient().Buy(*trad);
	Client::GetClient().Buy(*sberStock);
	Client::GetClient().Buy(*appleStock);
	cout << Client::GetClient().GetLiquidCost() << endl;
	Client::GetClient().GetPositions().Print();
	return 0;
}

