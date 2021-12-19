#include <map>
#include <limits>

template<typename K, typename V>
class interval_map {
	std::map<K,V> m_map;

public:
	using key_type = K;
	using element_type = V;

    interval_map(V const& val) {
        m_map.insert(m_map.end(),std::make_pair(std::numeric_limits<K>::lowest(), val));
    }

	void assign(K const& keyBegin, K const& keyEnd, V const& val) {
		if (!(keyBegin < keyEnd))
			return;

		auto iEnd = m_map.lower_bound(keyEnd);
		if (iEnd == m_map.end() || keyEnd < iEnd->first)
		{
			--iEnd;
			if (!(iEnd->second == val))
				m_map.insert(iEnd, std::make_pair(keyEnd, iEnd->second));
			++iEnd;
		}
		else
		{
			if (iEnd->second == val)
				++iEnd;
		}

		auto iBegin = --m_map.upper_bound(keyBegin);
		if (iBegin->first < keyBegin)
		{
			if (!(iBegin->second == val))
				iBegin = m_map.insert(iBegin, std::make_pair(keyBegin, val));
		}
		else
		{
			if (iBegin != m_map.begin())
			{
				--iBegin;
				if (!(iBegin->second == val))
				{
					++iBegin;
					iBegin->second = val;
				}
			}
			else
				iBegin->second = val;
		}
		++iBegin;
		m_map.erase(iBegin, iEnd);
    }

    V const& operator[](K const& key) const {
        return (--m_map.upper_bound(key))->second;
    }
};