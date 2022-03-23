#include <iostream>
#include <vector>
#include <memory>
#include <time.h>
#include "TradingInstruments.h"
#include "RBTree.h"

using namespace std;




class Client
{
private:
	double _balance = 0;
	vector<TradingInstrument> _positions;
	static Client* _client;
	Client()
	{
		
	}
public:
	static Client GetClient()
	{
		if (_client == nullptr)
		{
			_client = new Client();
		}
		return *_client;
	}
	void operator=(const Client&) = delete;
	void Buy(TradingInstrument purchasedAsset, int count)
	{
		if (purchasedAsset.GetPrice() <= _balance && purchasedAsset.GetOrderBySellCount() > count)
		{
			_balance -= purchasedAsset.GetPrice();
			purchasedAsset.SetOrderBySellCount(count);
		}
	}
	void Buy(TradingInstrument purchasedAsset, double desiredPrice, int count)
	{
		for (auto order: purchasedAsset.GetListOfOrders())
		{
			//логика такая: ищем заявки с меньшей ценой, чем хотим купить и после этого удаляем эти заявки из вектора, вычитаем
			//их стоимость с баланса и так пока не закончатся подходящие заявки или пока не выполним эту заявку.
		}
	}
};

Client* Client::_client = nullptr;

int main()
{
	
	srand(time(NULL));
	auto rb = new RBTree<Order>();
	
	
	return 0;
}

