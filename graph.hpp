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

			for (auto it = vertex.begin(), itt = vertex.end(); it != itt; ++it)
			{
				if (vertices.insert({it->first, v_count}).second)
					v_count++;

				if (vertices.insert({it->second, v_count}).second)
					v_count++;
			}

			Table.resize(v_count);

			for (auto it = vertex.begin(), itt = vertex.end(); it != itt; ++it)
			{
				AddEdge(it->first,  vertices.find(it->first)->second);
				AddEdge(it->second, vertices.find(it->second)->second);
			}
	
/* 			{
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
 */		
		}

		void FillData(int vert, VL vdata) noexcept {}
		void FillData(std::pair<int, int> edge, EL edata) noexcept {}
		// breadth search
		void BFS();
		// depth search
		void DFS();
		// 
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
