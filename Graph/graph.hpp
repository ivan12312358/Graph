#include <unordered_map>
#include <vector>
#include <array>
#include <iostream>
#include <iomanip>

namespace graph
{
	template <typename, typename = void>
	constexpr bool is_edge_data = false;
	
	template <typename T>
	constexpr bool is_edge_data<T, std::void_t< decltype(std::get<2>(std::declval<T&>()))>> = true;

	enum class color
	{
		red,
		blue
	};

	template <typename EL = int, typename VL = color>
	class Graph final
	{
		std::unordered_map<int, int> vertices;
		std::vector<std::array<int, 3>> Table;
		std::vector<EL> Edata;
		std::vector<VL> Vdata;

	public:
		Graph () = default;
	   ~Graph () = default;

		template <typename Iter, typename = std::void_t<decltype(*std::declval<Iter&>()),
														decltype(++std::declval<Iter&>()),
														decltype(std::get<1>(*std::declval<Iter&>()))>>
		Graph (Iter begin, Iter end)
		{
			size_t v_count = 0;

			for (auto it = begin; it != end; ++it)
			{
				if (vertices.insert({std::get<0>(*it), v_count}).second)
					v_count++;

				if (vertices.insert({std::get<1>(*it), v_count}).second)
					v_count++;
			}

			Table.resize(v_count);

			if constexpr (is_edge_data<typename std::iterator_traits<Iter>::value_type>)
				Edata.reserve(std::distance(begin, end));

			for (auto it = begin; it != end; ++it)
			{
				AddEdge(std::get<0>(*it), GetVIndex(std::get<0>(*it)));
				AddEdge(std::get<1>(*it), GetVIndex(std::get<1>(*it)));

				if constexpr (is_edge_data<typename std::iterator_traits<Iter>::value_type>)
					Edata.push_back(std::get<2>(*it));
			}
		}

		Graph (std::initializer_list<std::pair<int, int>> edges) 	  noexcept: Graph(edges.begin(), edges.end()) {}
		Graph (std::initializer_list<std::tuple<int, int, EL>> edges) noexcept: Graph(edges.begin(), edges.end()) {}

		void FillData(int vert, const VL& vdata) noexcept 
		{
			if (Vdata.empty())
				Vdata.resize(vertices.size());

			if (int it = GetVIndex(vert); it != -1)
				Vdata[it] = vdata;
		}

		void FillData(const std::tuple<int, int, EL>& edge)
		{
			FillData({std::get<0>(edge), std::get<1>(edge)}, std::get<2>(edge));
		}

		void FillData(const std::pair<int, int>& edge, const EL& edata) noexcept
		{
			if (Edata.empty())
				Edata.resize((Table.size() - vertices.size()) / 2);

			if (int it = GetEIndex(edge); it != -1)
				Edata[it] = edata;
		}

		int GetEIndex(const std::pair<int, int>& edge) const 
		{
			for (size_t it = vertices.size(); it < Table.size(); it += 2)
			{
				if (Table[it][0] 	 == edge.first && 
					Table[it ^ 1][0] == edge.second)
					return it - vertices.size();
			}

			return -1;
		}

		int GetVIndex(int vert) const
		{ 
			auto it = vertices.find(vert);

			if (it != vertices.cend())
				return it->second;

			return -1;
		}

		void Dump() const
		{
			PrintVertices();
			PrintEdges();
			PrintTable();
		}

		void PrintTable() const
		{
			for (size_t i = 0; i < Table.size(); ++i)
				std::cout << i << std::setw(6);

			std::cout << '\n';

			for (size_t j = 0; j < 3; ++j)
			{
				for (size_t i = 0; i < Table.size(); ++i)
					std::cout << Table[i][j] << std::setw(6);

				std::cout << '\n';
			}
		}

		void PrintVertices() const
		{
			if (!Vdata.empty())
			{
				for (auto&& it: vertices)
					std::cout << "(" << it.first << ", " << Vdata[it.second] << ") ";
			}
			else
			{
				for (auto&& it: vertices)
					std::cout << it.first << ' ';
			}

			std::cout << '\n';
		}

		void PrintEdges() const
		{
			if (!Edata.empty())
			{
				for (size_t it = vertices.size(), eit = 0; it < Table.size(); it += 2, eit++)
					std::cout << "({"  << Table[it][0] << ", " << Table[it ^ 1][0] 
							  << "}, " << Edata[eit] << ") ";
			}
			else 
			{
				for (size_t it = vertices.size(); it < Table.size(); ++it)
					std::cout << "{"  << Table[it][0] << ", " << Table[it ^ 1][0] << "} ";
			}

			std::cout << '\n';
		}

		// breadth search
		void BFS();
		// depth search
		void DFS();

	private:
		void AddEdge (int vert, int index) noexcept
		{
			if (Table[index][2] == 0)
			{
				Table.push_back({vert, index, index});
				Table[index][1] = Table[index][2] = Table.size() - 1;
			}
			else
			{
				Table.push_back({vert, index, Table[index][2]});					
				Table[index][2] = Table[Table[index][2]][1] = Table.size() - 1;
			}
		}
	};
}
