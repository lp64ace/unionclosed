#include <assert.h>
#include <stdlib.h>

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <ostream>
#include <set>
#include <vector>

template<class Tp> class Set {
	/**
	 * We do not care about the order of the elements altough we do not want to break it, so that we can easily enumerate them.
	 */
	std::vector<Tp> Elements_;
	/**
	 * This will take double the space we want to make this solution as small (in code) as possible, so instead of having hash values
	 * per element where I would have to define hashing functions for each element where we could only lookup for duplicates in case
	 * of collisions we use a std::set to see for duplicates before we add a new element.
	 */
	std::set<Tp> ElementsLookup_;

public:
	using const_iterator = typename std::vector<Tp>::const_iterator;

public:
	Set() = default;
	Set(const std::vector<Tp> &elements) {
		for (auto element : elements) {
			this->Add(element);
		}
	}
	Set(const std::set<Tp> &elements) {
		for (auto element : elements) {
			this->Add(element);
		}
	}
	Set(const std::initializer_list<Tp> &elements) {
		for (auto element : elements) {
			this->Add(element);
		}
	}
	Set(const Set<Tp> &elements) {
		for (auto element : elements) {
			this->Add(element);
		}
	}

	Set &operator=(const Set<Tp> &other) {
		this->Clear();
		for (auto element : other) {
			this->Add(element);
		}
		return *this;
	}
	Set &operator|=(const Set<Tp> &other) {
		for (auto element : other) {
			this->Add(element);
		}
		return *this;
	}
	Set &operator&=(const Set<Tp> &other) {
		std::vector<Tp> v_common;
		std::set<Tp> s_common;
		for (auto element : *this) {
			if (other.Has(element)) {
				v_common.push_back(element);
				s_common.insert(element);
			}
		}
		Elements_ = v_common;
		ElementsLookup_ = s_common;
		return *this;
	}

	bool operator<(const Set<Tp> &other) const {
		return ElementsLookup_ < other.ElementsLookup_;
	}
	bool operator>(const Set<Tp> &other) const {
		return ElementsLookup_ > other.ElementsLookup_;
	}
	bool operator<=(const Set<Tp> &other) const {
		return ElementsLookup_ <= other.ElementsLookup_;
	}
	bool operator>=(const Set<Tp> &other) const {
		return ElementsLookup_ >= other.ElementsLookup_;
	}

	bool operator==(const Set<Tp> &other) const {
		return ElementsLookup_ == other.ElementsLookup_;
	}
	bool operator!=(const Set<Tp> &other) const {
		return ElementsLookup_ != other.ElementsLookup_;
	}

	Set<Tp> &operator-=(const Set<Tp> &other) {
		for (const auto &element : other) {
			this->TryRemove(element);
		}
		return *this;
	}
	Set<Tp> &operator+=(const Set<Tp> &other) {
		for (auto element : other) {
			this->Add(element);
		}
		return *this;
	}

public:
	const_iterator begin() const {
		return Elements_.begin();
	}

	const_iterator end() const {
		return Elements_.end();
	}

	/**
	 * Altering elements is not so simple since altering an element would only update the element within the vector,
	 * and therefore the lookup table (set) would be rendered invalid.
	 *
	 * This is why we return a const reference to the element.
	 */
	const Tp &operator[](const size_t index) const {
		return Elements_[index];
	}

public:
	bool Has(const Tp &element) const {
		return ElementsLookup_.find(element) != ElementsLookup_.end();
	}

	void Clear() {
		Elements_.clear();
		ElementsLookup_.clear();
	}

	bool IsEmpty() const {
		return ElementsLookup_.empty();
	}
	bool IsSingleton() const {
		return ElementsLookup_.size() == 1;
	}

	const Tp &Get(const size_t index) const {
		return Elements_[index];
	}

	void Replace(const size_t index, const Tp &new_element) {
		ElementsLookup_.erase(Elements_[index]);
		ElementsLookup_.insert(Elements_[index] = new_elem);
	}

	void Add(const Tp &new_element) {
		if (ElementsLookup_.insert(new_element).second) {
			Elements_.push_back(new_element);
		}
	}

	bool TryRemove(const Tp &old_element) {
		std::set<Tp>::const_iterator itr = ElementsLookup_.find(old_element);
		if (itr != ElementsLookup_.end()) {
			/** If it exists in the lookup table, it HAS to exist in the vector too. */
			assert(std::find(Elements_.begin(), Elements_.end(), old_element) != Elements_.end());
			/** TODO: Move the last element of the vector to that position and then do a pop_back, since it would be cheaper. */
			Elements_.erase(std::find(Elements_.begin(), Elements_.end(), old_element));
			ElementsLookup_.erase(itr);
			return true;
		}
		return false;
	}

	void Remove(const Tp &old_element) {
		assert(ElementsLookup_.find(old_element) != ElementsLookup_.end());
		TryRemove(old_element);
	}

	size_t Count() const {
		assert(Elements_.size() == ElementsLookup_.size());
		return Elements_.size();
	}

public:
	template<class Tp> friend std::ostream &operator<<(std::ostream &stream, const Set<Tp> &set);
	template<class Tp> friend Set<Tp> operator-(const Set<Tp> &left, const Set<Tp> &right);
	template<class Tp> friend Set<Tp> operator+(const Set<Tp> &left, const Set<Tp> &right);
	template<class Tp> friend Set<Tp> operator|(const Set<Tp> &left, const Set<Tp> &right);
	template<class Tp> friend Set<Tp> operator&(const Set<Tp> &left, const Set<Tp> &right);
};

template<class Tp> Set<Tp> operator-(const Set<Tp> &left, const Set<Tp> &right) {
	Set<Tp> ret;
	for (auto element : left) {
		if (!right.Has(element)) {
			ret.Add(element);
		}
	}
	return ret;
}
template<class Tp> Set<Tp> operator+(const Set<Tp> &left, const Set<Tp> &right) {
	Set<Tp> ret;
	for (auto element : left) {
		ret.Add(element);
	}
	for (auto element : right) {
		ret.Add(element);
	}
	return ret;
}
template<class Tp> Set<Tp> operator|(const Set<Tp> &left, const Set<Tp> &right) {
	Set<Tp> ret;
	for (auto element : left) {
		ret.Add(element);
	}
	for (auto element : right) {
		ret.Add(element);
	}
	return ret;
}
template<class Tp> Set<Tp> operator&(const Set<Tp> &left, const Set<Tp> &right) {
	Set<Tp> ret;
	for (auto element : left) {
		if (ret.Has(element)) {
			ret.Add(element);
		}
	}
	return ret;
}

template<class Tp> std::ostream &operator<<(std::ostream &stream, const Set<Tp> &set) {
	stream << "{";
	for (size_t i = 0; i < set.Count(); i++) {
		stream << set.Get(i);
		if (i + 1 != set.Count()) {
			stream << ", ";
		}
	}
	stream << "}";

	return stream;
}

template<class Tp> bool IsSubset(const Set<Tp> &left, const Set<Tp> &right) {
	for (auto element : left) {
		if (!right.Has(element)) {
			return false;
		}
	}
	return true;
}

template<class Tp> bool IsSuperset(const Set<Tp> &left, const Set<Tp> &right) {
	for (auto element : right) {
		if (!left.Has(element)) {
			return false;
		}
	}
	return true;
}

int random(int min, int max) {
	const unsigned long long bytes = ((unsigned long long)log2(RAND_MAX)) >> 3;
	const unsigned long long bits = bytes * 8;

	unsigned int scaled = 0;
	for (size_t i = 0; i < sizeof(scaled); i += bytes) {
		const unsigned long long mask = ((1ull << bits) - 1);
		scaled = (scaled << bits) | (rand() & mask);
	}
	const float normalized = ((float)scaled) / UINT_MAX;
	const float r = min + normalized * (max - min + 1);

	return (int)r + 0.5;
}

Set<Set<int>> MakeRandomIndependentFamilyBase(size_t length, int length_min, int length_max) {
	Set<Set<int>> FamilyBase;

	int iter = 1;
	for (size_t index = 0; index < length; index++) {
		Set<int> Vec;

		int end = length_min + random(iter, iter + (length_max - length_min));
		for (int v = iter; v < end; v++) {
			Vec.Add(v);
		}
		iter = end - random(0, end - iter - 1);

		FamilyBase.Add(Vec);
	}

	return FamilyBase;
}

class Bitset {
	std::vector<bool> Bits_;

public:
	Bitset() = default;

	Bitset(size_t count) : Bits_(count) {
	}
	Bitset(const Bitset &other) : Bits_(other.Bits_) {
	}
	Bitset(const std::initializer_list<int> &bits) {
		for (auto bit : bits) {
			Bits_.push_back(bit);
		}
	}
	Bitset(const std::initializer_list<bool> &bits) {
		for (auto bit : bits) {
			Bits_.push_back(bit);
		}
	}

	size_t Count() const {
		return Bits_.size();
	}

	bool Has(size_t index) const {
		return (index < Bits_.size()) ? Bits_[index] : false;
	}

	Bitset &operator++() {
		bool carry = true;

		for (size_t index = 0; index < Bits_.size(); index++) {
			if (Bits_[index] == true) {
				carry = !(Bits_[index] = false);
			}
			else {
				carry = !(Bits_[index] = true);
				break;
			}
		}
		if (carry) {
			Bits_.push_back(true);
		}

		return *this;
	}
};

Set<Set<int>> MakeFamilyFromBase(const Set<Set<int>> &Base, float SkipFactor = 0.0f) {
	Set<Set<int>> Family;

	for (Bitset bitset; bitset.Count() <= Base.Count(); ++bitset) {
		if (random(0, SHRT_MAX) > (SkipFactor * SHRT_MAX)) {
			Set<int> Union;
			for (size_t index = 0; index < Base.Count(); index++) {
				if (bitset.Has(index)) {
					Union |= Base[index];
				}
			}
			Family.Add(Union);
		}
	}

	return Family;
}

Set<Set<int>> Sigma(const Set<Set<int>> &Family) {
	Set<Set<int>> Ret;

	for (size_t i = 0; i < Family.Count(); i++) {
		Set<int> UnionOfSubsets;

		const Set<int> &A = Family[i];
		for (size_t j = 0; j < Family.Count(); j++) {
			const Set<int> &B = Family[j];

			if (B.Count() < A.Count() && IsSubset(B, A)) {
				UnionOfSubsets |= B;
			}
		}

		/* A cannot be exressed as a union of subsets within the selected family. */
		if (UnionOfSubsets != A) {
			Ret.Add(A);
		}
	}

	return Ret;
}

Set<Set<int>> MakeD(size_t i, const Set<Set<int>> &C) {
	const Set<int> &Enforced = C[i];

	Set<Set<int>> D;

	for (Bitset bitset({1}); bitset.Count() < C.Count() - i; ++bitset) {
		Set<int> Union = Enforced;

		for (size_t index = 0; index + i + 1 < C.Count(); index++) {
			if (bitset.Has(index)) {
				Union &= C[index + i + 1];
			}
		}

		D.Add(Union);
	}

	return D;
}

Set<Set<int>> MakeBeta(const Set<Set<int>> &C) {
	Set<Set<int>> B;

	for (size_t index = 0; index < C.Count() - 1; index++) {
		Set<Set<int>> D = MakeD(index, C);

		B |= D;
	}

	return B;
}

Set<Set<int>> MakeAltEta(const Set<Set<int>>& H, const Set<Set<int>>& V) {
	Set<Set<int>> H_new;

	std::cout << "H " << H << " x V" << V << std::endl;

	for (auto h_ : H) {
		Set<int> h = h_;
		for (auto v : V) {
			if (!v.IsEmpty() && IsSubset(v, h)) {
				H_new.Add(v);
				
				h -= v;
			}
		}
		H_new.Add(h);
	}
	
	return H_new;
}

int main(void) {
	Set<Set<int>> FamilyBase = MakeRandomIndependentFamilyBase(8, 1, 3);
	std::cout << "Family Base " << FamilyBase << std::endl;
	Set<Set<int>> Family = MakeFamilyFromBase(FamilyBase, 0.0);
	/** Too verbose... here for debug purposes! */
	// std::cout << "Family " << Family << std::endl;
	Set<Set<int>> SF = Sigma(Family);
	std::cout << "Sigma(F) " << SF << std::endl;

	std::vector<Set<Set<int>>> C({SF});

	{
		std::cout << "Ypsilon Algorithm" << std::endl;

		std::cout << "C_" << 0 << " := " << C.back() << std::endl;

		while (!C.back().IsEmpty()) {
			const Set<Set<int>> &c = C.back(), b = MakeBeta(c);

			std::cout << "C_" << C.size() << " := " << b << std::endl;

			C.push_back(b);
		}

		Set<Set<int>> V = C[C.size() - 2];

		for (size_t index = C.size() - 2; index; index--) {
			const Set<Set<int>> &H = C[index - 1];
			Set<Set<int>> H_new;

			std::cout << "H " << H << " x V " << V << std::endl;

			V = MakeAltEta(H, V);

			std::cout << "Produced " << V << std::endl;
		}
		
		V = MakeAltEta(SF, V);
		
		std::cout << "Applying to Sigma(F) := " << V << std::endl;
	}

	return 0;
}
