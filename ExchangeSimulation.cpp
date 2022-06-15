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
		//cout << asset.GetPriceToSell() << " || " << (_balance * asset.GetLongLeverage());
		if ((asset.GetPriceToSell() > (_balance * asset.GetLongLeverage())) || (!asset.IsBuyable())) { return -1; } //проверка что можем купить
		asset.GetGlass().GetGlassToSell().erase(asset.GetGlass().GetGlassToSell().begin()); //удаляем заявку на продажу (мы ее покупаем, т.е. заявка удовлетворена нами)
		_balance -= asset.GetPriceToSell(); //списываем средства с счета клиента
		_positions.Add(asset); //добавляем актив в наш список
		return 0;
	}
	int Sell(TradingInstrument asset)
	{
		if (!asset.IsSellable() || !(_positions.FoundElem(asset) != nullptr)) { return -1; } //проверка что можем продать
		asset.GetGlass().GetGlassToBuy().pop_back(); //удаляем заявку на покупку
		_balance += asset.GetPriceToBuy(); //добавляем деньги на счет
		_positions.Delete(asset); //удаляем актив из списка
		return 0;
		
	}
	void TopUpBalance(int amount) //пополнение баланса
	{
		_balance += amount;
	}
	int WithdrawMoney(int amount) //вывод средств
	{
		if (amount > _balance) { return -1; }
		_balance -= amount;
		return 0;
	}
	int GetLiquidCost() //получение ликвидной стоимости портфеля
	{
		int summ = 0;
		for (auto& i : _positions)
		{
			summ += i.data.GetAveragePrice() * i.counter;
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
	Client::GetClient().TopUpBalance(5500);
	Client::GetClient().Buy(*trad);
	for (int i = 0; i < 5; i++)
		Client::GetClient().Buy(*sberStock);
	Client::GetClient().Buy(*appleStock);
	Client::GetClient().Sell(*trad);
	for (int i = 0; i < 7; i++)
		Client::GetClient().Sell(*sberStock);
	
	cout << Client::GetClient().GetLiquidCost() << endl;
	Client::GetClient().GetPositions().Print();
	return 0;
}


