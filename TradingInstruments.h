#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Glass
{
private:
	vector<int> glassToBuy;
	vector<int> glassToSell;
	void Sort()
	{	
		for (int i = 1; i < glassToBuy.size(); i++)
			for (int j = i; j > 0 && glassToBuy[j - 1] > glassToBuy[j]; j--)
				swap(glassToBuy[j - 1], glassToBuy[j]);
		for (int i = 1; i < glassToSell.size(); i++)
			for (int j = i; j > 0 && glassToSell[j - 1] > glassToSell[j]; j--)
				swap(glassToSell[j - 1], glassToSell[j]);
	}
public:
	Glass() //потом можно будет добавить регулирование объема стакана, пока константа = 100 в обе стороны
	{
		for (size_t i = 0; i < 100; i++)
		{
			auto a = rand() % 100;
			auto b = 100 + (rand() % 100);
			glassToBuy.push_back(a);
			glassToSell.push_back(b);
			Sort();
		}
	}
	vector<int>& GetGlassToBuy() { return glassToBuy; }
	vector<int>& GetGlassToSell() { return glassToSell; }
	void Update()
	{
		auto update = (rand() % 11) - 5; //считаем, куда изменится цена: > 0 - рост, < 0 - падение
		if (update > 0)
		{
			for (int i = 0; i < update; i++)
			{
				if (!glassToSell.empty())
					glassToSell.erase(glassToSell.begin()); //удаляем заявки на продажу => цена растет
				
			}
			if (!glassToSell.empty())
			{
				auto maxBuy = glassToBuy.empty() ? 0 : glassToBuy[glassToBuy.size() - 1]; 
				for (int i = 0; i < update; i++)
				{
					glassToBuy.push_back(maxBuy + (rand() % (glassToSell[0] - maxBuy))); //создаем заявки на покупки в пределах [max цена на покупку; min цена на продажу]
				}
			}

		}
	}
};

class Order
{
private:
	double price;
	bool type;
public:
	Order() = default;
	Order(double price, bool type)
	{
		this->price = price;
		this->type = type;
	}
	double GetPrice() {return price; }
};

class TradingInstrument
{
protected:
	string name;
	double priceToBuy;
	double priceToSell;
	double longLeverage;
	double shortLeverage;
	Glass* glass;


public:
	TradingInstrument(string name)
	{
		this->name = name;
		glass = new Glass();
		priceToBuy = glass->GetGlassToBuy()[glass->GetGlassToBuy().size() - 1];
		priceToSell = glass->GetGlassToSell()[0];
		longLeverage = 1.0; //пока захардкодил 1
	}
	TradingInstrument() = default;
	double GetLongLeverage() { return longLeverage; }
	double GetPriceToBuy() { return priceToBuy; }
	double GetPriceToSell() { return priceToSell; }
	double GetAveragePrice() { return (priceToBuy + priceToSell) / 2; }
	string GetName() { return name; }
	Glass& GetGlass() { return (*glass); }
	bool IsBuyable() { return (!glass->GetGlassToSell().empty()); }
	bool IsSellable() { return (!glass->GetGlassToBuy().empty()); }
	bool operator==(const TradingInstrument& other) { return this->name == other.name; }
	void Update() //этот метод будет отвечать за обновление цены актива
	{
		glass->Update();
	}
};



class Stock : public TradingInstrument
{
public:
	Stock(string name) : TradingInstrument(name) {}
	Stock() = default;
};



