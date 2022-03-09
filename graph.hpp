#include <unordered_map>
#include <vector>
#include <array>
#include <iostream>

namespace graph
{
	enum class color
	{
		red,
		blue
	};

	template <typename EL = int, typename VL = color>
	class Graph 
	{
		std::unordered_map<int, int> vertices;
		std::vector<std::array<int, 3>> Table;
		std::vector<EL> Edata;
		std::vector<VL> Vdata;

	public:
		Graph () = default;
	   ~Graph () = default;
		Graph (std::initializer_list<std::pair<int, int>> vertex) noexcept 
		{
			size_t v_count = 0;

			for (auto&& it: vertex)
			{
				if (vertices.insert({it.first, v_count}).second)
					v_count++;

				if (vertices.insert({it.second, v_count}).second)
					v_count++;
			}

			Table.resize(v_count);

			for (auto&& it: vertex)
			{
				AddEdge(it.first,  GetVIndex(it.first));
				AddEdge(it.second, GetVIndex(it.second));
			}
		}

		void FillData(int vert, VL vdata) noexcept 
		{
			if (Vdata.empty())
				Vdata.resize(vertices.size());

			int it = GetVIndex(vert);
			if (it != -1) Vdata[it] = vdata;
		}

		void FillData(std::pair<int, int> edge, EL edata) noexcept 
		{
			//reserve(Table,size() - vert.size())
			if (Edata.empty())
				Edata.resize(Table.size() - vertices.size());

			int it = GetEIndex(edge);
			if (it != -1) Edata[it] = edata;
		}
		// breadth search
		void BFS();
		// depth search
		void DFS();

		int GetEIndex(const std::pair<int, int>& edge) const 
		{
			for (size_t it = vertices.size(); it < Table.size(); ++it)
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

		void Dump()
		{
			PrintTable();
			PrintVertices();
			PrintEdges();
		}

		void PrintTable()
		{
			for (size_t i = 0; i < Table.size(); ++i)
				std::cout << i << '\t';

			std::cout << '\n';

			for (size_t j = 0; j < 3; ++j)
			{
				for (size_t i = 0; i < Table.size(); ++i)
					std::cout << Table[i][j] << '\t';

				std::cout << '\n';
			}
		}

		void PrintVertices()
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

		void PrintEdges()
		{
			if (!Edata.empty())
			{
				for (size_t it = vertices.size(); it < Table.size(); ++it)
					std::cout << "({"  << Table[it][0] << ", " << Table[it ^ 1][0] 
							  << "}, " << Edata[it - vertices.size()] << ") ";
			}
			else 
			{
				for (size_t it = vertices.size(); it < Table.size(); ++it)
					std::cout << "{"  << Table[it][0] << ", " << Table[it ^ 1][0] << "} ";
			}

			std::cout << '\n';
		}

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
