#pragma once

#include "list.h"
#include "structure_iterator.h"
#include "array.h"

namespace structures
{
	/// <summary> Zoznam implementovany polom. </summary>
	/// <typeparam name = "T"> Typ dat ukladanych v zozname. </typepram>
	template<typename T>
	class ArrayList : public List<T>
	{
	public:
		/// <summary> Konstruktor. </summary>
		ArrayList();

		/// <summary> Kopirovaci konstruktor. </summary>
		/// <param name = "other"> ArrayList, z ktoreho sa prevezmu vlastnosti. </param>
		ArrayList(ArrayList<T>& other);

		/// <summary> Destruktor. </summary>
		~ArrayList();

		/// <summary> Vrati pocet prvkov v zozname. </summary>
		/// <returns> Pocet prvkov v zozname. </returns>
		size_t size() override;

		/// <summary> Priradenie struktury. </summary>
		/// <param name = "other"> Struktura, z ktorej ma prebrat vlastnosti. </param>
		/// <returns> Adresa, na ktorej sa struktura nachadza. </returns>
		Structure& assign(Structure& other) override;

		/// <summary> Porovnanie struktur. </summary>
        /// <param name="other">Struktura, s ktorou sa ma tato struktura porovnat. </param>
        /// <returns>True ak su struktury zhodne typom aj obsahom. </returns>
		bool equals(Structure& other);

		/// <summary> Vrati adresou prvok na indexe. </summary>
		/// <param name = "index"> Index prvku. </param>
		/// <returns> Adresa prvku na danom indexe. </returns>
		/// <exception cref="std::out_of_range"> Vyhodena, ak index nepatri do zoznamu. </exception>
		T& at(int index) override;

		/// <summary> Prida prvok do zoznamu. </summary>
		/// <param name = "data"> Pridavany prvok. </param>
		void add(const T& data) override;

		/// <summary> Vlozi prvok do zoznamu na dany index. </summary>
		/// <param name = "data"> Pridavany prvok. </param>
		/// <param name = "index"> Index prvku. </param>
		/// <exception cref="std::out_of_range"> Vyhodena, ak index nepatri do zoznamu. </exception>
		/// <remarks> Ak je ako index zadana hodnota poctu prvkov (teda prvy neplatny index), metoda insert sa sprava ako metoda add. </remarks>
		void insert(const T& data, int index) override;

		/// <summary> Odstrani prvy vyskyt prvku zo zoznamu. </summary>
		/// <param name = "data"> Odstranovany prvok. </param>
		/// <returns> true, ak sa podarilo prvok zo zoznamu odobrat, false inak. </returns>
		bool tryRemove(const T& data) override;

		/// <summary> Odstrani zo zoznamu prvok na danom indexe. </summary>
		/// <param name = "index"> Index prvku. </param>
		/// <returns> Odstraneny prvok. </returns>
		/// <exception cref="std::out_of_range"> Vyhodena, ak index nepatri do zoznamu. </exception>
		T removeAt(int index) override;

		/// <summary> Vrati index prveho vyskytu prvku v zozname. </summary>
		/// <param name = "data"> Prvok, ktoreho index sa hlada. </param>
		/// <returns> Index prveho vyskytu prvku v zozname, ak sa prvok v zozname nenachadza, vrati -1. </returns>
		int getIndexOf(const T& data) override;

		/// <summary> Vymaze zoznam. </summary>
		void clear() override;

		/// <summary> Vrati skutocny iterator na zaciatok struktury </summary>
		/// <returns> Iterator na zaciatok struktury. </returns>
		/// <remarks> Zabezpecuje polymorfizmus. </remarks>
		Iterator<T>* getBeginIterator() override;

		/// <summary> Vrati skutocny iterator na koniec struktury </summary>
		/// <returns> Iterator na koniec struktury. </returns>
		/// <remarks> Zabezpecuje polymorfizmus. </remarks>
		Iterator<T>* getEndIterator() override;

	private:
		/// <summary> Pole s datami. </summary>
		Array<T>* array_;
		/// <summary> Pocet prvkov v zozname. </summary>
		size_t size_;

	private:
		/// <summary> Rozsiri kapacitu zoznamu. </summary>
		void enlarge();

		/// <summary> Iterator pre ArrayList. </summary>
		class ArrayListIterator : public Iterator<T>
		{
		public:
			/// <summary> Konstruktor. </summary>
			/// <param name = "arrayList"> Zoznam, cez ktory iteruje. </param>
			/// <param name = "position"> Pozicia, na ktorej zacina. </param>
			ArrayListIterator(ArrayList<T>* arrayList, int position);

			/// <summary> Destruktor. </summary>
			~ArrayListIterator();

			/// <summary> Operator priradenia. Priradi do seba hodnotu druheho iteratora. </summary>
			/// <param name = "other"> Druhy iterator. </param>
			/// <returns> Vrati seba po priradeni. </returns>
			Iterator<T>& operator= (Iterator<T>& other) override;

			/// <summary> Porovna sa s druhym iteratorom na nerovnost. </summary>
			/// <param name = "other"> Druhy iterator. </param>
			/// <returns> True, ak sa iteratory nerovnaju, false inak. </returns>
			bool operator!=(Iterator<T>& other) override;

			/// <summary> Vrati data, na ktore aktualne ukazuje iterator. </summary>
			/// <returns> Data, na ktore aktualne ukazuje iterator. </returns>
			T operator*() override;

			/// <summary> Posunie iterator na dalsi prvok v strukture. </summary>
			/// <returns> Iterator na dalsi prvok v strukture. </returns>
			/// <remarks> Zvycajne vrati seba. Ak vrati iny iterator, povodny bude automaticky zruseny. </remarks>
			Iterator<T>& operator++() override;

		private:
			/// <summary> Zoznam, cez ktory iteruje. </summary>
			ArrayList<T>* arrayList_;
			/// <summary> Aktualna pozicia v zozname. </summary>
			int position_;
		};
	};

	template<typename T>
	inline ArrayList<T>::ArrayList() :
		List<T>(),
		array_(new Array<T>(4)),
		size_(0)
	{
	}

	template<typename T>
	inline ArrayList<T>::ArrayList(ArrayList<T>& other) :
		List<T>(),
		array_(new Array<T>(*other.array_)),
		size_(other.size_)
	{
	}

	template<typename T>
	inline ArrayList<T>::~ArrayList()
	{
		delete array_;
		array_ = nullptr;
		size_ = 0;
	}

	template<typename T>
	inline size_t ArrayList<T>::size()
	{
		return size_;
	}

	template<typename T>
	inline Structure& ArrayList<T>::assign(Structure& other)
	{
		if (this != &other) {
			ArrayList<T> otherList = dynamic_cast<ArrayList<T>&>(other);
			size_ = otherList.size_;
			delete array_;
			array_ = new Array<T>(*otherList.array_);
		}
		return *this;
	}

	template<typename T>
	inline bool ArrayList<T>::equals(Structure& other)
	{
		ArrayList<T> otherList = dynamic_cast<ArrayList<T>&>(other);

		if (size_ != otherList.size_)
			return false;

		for (size_t i = 0; i < otherList.size_; i++)
		{
			if ((*array_)[i] != (*otherList.array_)[i])
			{
				return false;
			}
		}
		return true;
	}

	template<typename T>
	inline T& ArrayList<T>::at(int index)
	{
		Utils::rangeCheckExcept(index, size_, "Invalid index");
		return (*array_)[index];
	}

	template<typename T>
	inline void ArrayList<T>::add(const T& data)
	{
		if (array_->size() == size_) {
			enlarge();
		}
		(*array_)[static_cast<int>(size_++)] = data;
		
	}

	template<typename T>
	inline void ArrayList<T>::insert(const T& data, int index)
	{
		if (index == static_cast<int>(size_)) {
			add(data);
		}
		else {
			Utils::rangeCheckExcept(index, size_, "Invalid index");
			if (array_->size() < size_ + 1) {
				enlarge();
			}
			Array<T>::copy(*array_, static_cast<int>(index), *array_, static_cast<int>(index + 1), static_cast<int>(size_ - index));
			(*array_)[index] = data;
			size_++;
		}
	}

	template<typename T>
	inline bool ArrayList<T>::tryRemove(const T& data)
	{
		int index = getIndexOf(data);
		if (index == -1) {
			return false;
		} else {
			removeAt(index);
			return true;
		}
	}

	template<typename T>
	inline T ArrayList<T>::removeAt(int index)
	{
		Utils::rangeCheckExcept(index, size_, "Invalid index");
		T deleted = (*this)[index];
		Array<T>::copy(*array_, static_cast<int>(index+1), *array_, static_cast<int>(index), static_cast<int>(size_ - index - 1));
		size_--;
		return deleted;
	}

	template<typename T>
	inline int ArrayList<T>::getIndexOf(const T& data)
	{
		for (size_t index = 0; index < static_cast<int>(size_); index++)
		{
			if ((*this)[index] == data) {
				return index;
			}
		}

		return -1;
	}

	template<typename T>
	inline void ArrayList<T>::clear()
	{
		size_ = 0;
	}

	template<typename T>
	inline Iterator<T>* ArrayList<T>::getBeginIterator()
	{
		return new ArrayListIterator(this,0);
	}

	template<typename T>
	inline Iterator<T>* ArrayList<T>::getEndIterator() //prvy neplatny prvok size_, inak by to bolo size_
	{
		return new ArrayListIterator(this, static_cast<int>(size_));
	}

	template<typename T>
	inline void ArrayList<T>::enlarge()
	{
		Array<T>* newArray = new Array<T>(size_ * 2);
		Array<T>::copy(*array_,0,*newArray,0, static_cast<int>(size_));
		delete array_;
		array_ = newArray;
	}

	template<typename T>
	inline ArrayList<T>::ArrayListIterator::ArrayListIterator(ArrayList<T>* arrayList, int position) :
		arrayList_(arrayList),
		position_(position)
	{
	}

	template<typename T>
	inline ArrayList<T>::ArrayListIterator::~ArrayListIterator()
	{
		arrayList_ = nullptr;
		position_ = 0;
	}

	template<typename T>
	inline Iterator<T>& ArrayList<T>::ArrayListIterator::operator=(Iterator<T>& other)
	{
		position_ = dynamic_cast<const ArrayListIterator&>(other).position_;
		return *this;
	}

	template<typename T>
	inline bool ArrayList<T>::ArrayListIterator::operator!=(Iterator<T>& other)
	{
		return position_ != dynamic_cast<const ArrayListIterator&>(other).position_;
	}

	template<typename T>
	inline T ArrayList<T>::ArrayListIterator::operator*()
	{
		return (*arrayList_)[position_];
	}

	template<typename T>
	inline Iterator<T>& ArrayList<T>::ArrayListIterator::operator++()
	{
		position_++;
		return *this;

	}

}