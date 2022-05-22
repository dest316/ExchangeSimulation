#include <iostream>
#include <concepts>
#include <type_traits>

using namespace std;

template <typename T>
concept printable = requires(T t)
{
	(std::cout << t) -> std::is_same<T, std::ostream&>::value;
};

template <typename T> 
class Node
{
public:
	T data;
	Node* prev;
	Node* next;
	int counter;
	
};

template <typename T>
class DoublyLinkedList
{
private:
	Node<T>* head;
	Node<T>* tail;
public:
	
	using value_type = Node<T>;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using iterator_category = std::bidirectional_iterator_tag;

	/*using iterator = pointer;
	using const_iterator = const_pointer;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;*/

	friend class DoublyLinkedListBidirectionalIterator;
	//friend class ConstDoublyLinkedListBidirectionalIterator;

	class DoublyLinkedListBidirectionalIterator
	{
	private:
		pointer _value;
	public:
		DoublyLinkedListBidirectionalIterator(pointer value) : _value(value) {}
		DoublyLinkedListBidirectionalIterator(const DoublyLinkedListBidirectionalIterator& it) : _value(it._value) {}
		DoublyLinkedListBidirectionalIterator& operator++()
		{			
			_value = _value->next;
			return *this;
		}
		DoublyLinkedListBidirectionalIterator operator++(int)
		{
			auto tmp = this;
			_value = _value->next;
			return tmp;
		}
		DoublyLinkedListBidirectionalIterator& operator--()
		{
			_value = _value->prev;
			return *this;
		}
		DoublyLinkedListBidirectionalIterator operator--(int)
		{
			auto tmp = this;
			_value = _value->prev;
			return tmp;
		}
		reference operator*()
		{
			return *_value;
		}
		pointer operator->()
		{
			return _value;
		}
		bool operator==(const DoublyLinkedListBidirectionalIterator value) const
		{
			return _value == value._value;
		}
		auto operator<=>(const DoublyLinkedListBidirectionalIterator value) const
		{
			return value <=> value._value;
		}		
	};
	
	using iterator = DoublyLinkedListBidirectionalIterator;
	using const_iterator = const iterator;

	DoublyLinkedList()
	{
		head = nullptr;
		tail = nullptr;
	}
	Node<T>* FoundElem(T foundedData)
	{
		Node<T>* cur = this->head;		
		while (cur != nullptr)
		{
			if (cur->data == foundedData)
			{
				return cur;
			}
			else
			{
				cur = cur->next;
			}
		}
		return nullptr;
	}
	void Add(T newElem)
	{
		if (head == nullptr)
		{
			head = new Node<T>();
			head->data = newElem;
			head->prev = nullptr;
			head->next = nullptr;
			head->counter = 1;
			tail = head;
		}
		else
		{
			if (FoundElem(newElem) != nullptr)
			{
				FoundElem(newElem)->counter++;
			}
			else
			{
				Node<T>* cur = new Node<T>();
				cur->data = newElem;
				cur->prev = this->tail;
				cur->next = nullptr;
				cur->counter = 1;
				this->tail->next = cur;
				tail = tail->next;
			}
		}
	}
	void Print()
	{
		Node<T>* cur = head;
		while (cur != nullptr)
		{
			cout << cur->data << ":" << cur->counter << endl;
			cur = cur->next;
		}
	}
	void Delete(T deleteValue)
	{
		if (FoundElem(deleteValue) != nullptr)
		{
			if (FoundElem(deleteValue)->counter > 1)
			{
				FoundElem(deleteValue)->counter--;
			}
			else
			{
				Node<T>* deletedElem = FoundElem(deleteValue);
				Node<T>* prevElem = deletedElem->prev;
				Node<T>* nextElem = deletedElem->next;
				if (prevElem == nextElem)
				{
					this->head = nullptr;
					this->tail = nullptr;
				}
				else if (prevElem == nullptr)
				{
					nextElem->prev = nullptr;
					this->head = nextElem;	
					
				}
				else if (nextElem == nullptr)
				{
					prevElem->next = nullptr;
					this->tail = prevElem;
				}
				else
				{
					nextElem->prev = prevElem;
					prevElem->next = nextElem;					
				}
				delete deletedElem;				
			}
		}
	}
	int GetLength()
	{
		Node<T>* cur = head;
		int length = 0;
		while (cur != nullptr)
		{
			length += cur->counter;
			cur = cur->next;
		}
		return length;
	}
	int GetDifferenceLength()
	{
		Node<T>* cur = head;
		auto length = 0;
		while (cur != nullptr)
		{
			length++;
			cur = cur->next;
		}
		return length;
	}
	void Clear()
	{
		Node<T>* cur = head;
		while (cur != nullptr)
		{
			auto tmp = cur->next;
			Delete(cur->data);
			cur = tmp;
		} 
	}
	void swap(Node<T>* firstElem, Node<T>* secondElem)
	{
		T tmp = firstElem->data;
		firstElem->data = secondElem->data;
		secondElem->data = tmp;
	}
	~DoublyLinkedList()
	{
		Clear();
	}
	DoublyLinkedListBidirectionalIterator begin()
	{
		return DoublyLinkedListBidirectionalIterator(this->head);
	}
	DoublyLinkedListBidirectionalIterator end() 
	{
		return DoublyLinkedListBidirectionalIterator(this->tail->next); 
	}
	const_iterator cbegin()
	{
		return const_iterator(this->begin());
	}
	const_iterator cend()
	{
		return const_iterator(this->end());
	}
};




