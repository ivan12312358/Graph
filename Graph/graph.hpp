#include <map>
#include <vector>
#include <array>
#include <iostream>
#include <iomanip>

//TODO	GetVcolor()
//		IsSameColors()

namespace graph
{
	template <typename, typename = void>
	constexpr bool is_edge_data = false;
	
	template <typename T>
	constexpr bool is_edge_data<T, std::void_t<decltype(std::get<2>(std::declval<T&>()))>> = true;

	enum class Color
	{
		colorless,
		blue,
		red
	};

	std::ostream& operator << (std::ostream& out, Color color)
	{
		switch(color)
		{
			case Color::red:
				std::cout << 'r';
				break;
			case Color::blue:
				std::cout << 'b';
				break;
			case Color::colorless:
				std::cout << "colorless";
				break;
			default:
				break;
		}

		return out;
	}

	void SwitchColor(Color& color)
	{
		color = color == Color::blue ? Color::red : 
									   Color::blue;
	}

	template <typename EL = int, typename VL = int>
	class Graph final
	{
		std::map<int, int> vertices;
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
				AddEdge(std::get<0>(*it));
				AddEdge(std::get<1>(*it));

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
			static int offset = (vertices.size() % 2) == 0 ? 0 : 2;

			for (size_t it = vertices.size(); it < Table.size(); it += 2)
			{
				if (Table[it][0] 	 			== edge.first && 
					Table[(it ^ 1) + offset][0] == edge.second)
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
				std::cout << i << " \t";

			std::cout << std::endl;

			for (size_t j = 0; j < 3; ++j)
			{
				for (size_t i = 0; i < Table.size(); ++i)
					std::cout << Table[i][j] << " \t";

				std::cout << std::endl;
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

			std::cout << std::endl;
		}

		void PrintEdges() const
		{
			int offset = (vertices.size() % 2) == 0 ? 0 : 2;

			if (!Edata.empty())
			{
				for (size_t it = vertices.size(), eit = 0; it < Table.size(); it += 2, eit++)
					std::cout << "({"  << Table[it][0] << ", " << Table[(it ^ 1) + offset][0] 
							  << "}, " << Edata[eit] << ") ";
			}
			else 
			{
				for (size_t it = vertices.size(); it < Table.size(); it += 2)
					std::cout << "{"  << Table[it][0] << ", " << Table[(it ^ 1) + offset][0] << "} ";
			}

			std::cout << std::endl;
		}

		void BFS();

		bool IsBipartite()
		{
			if (Table.empty())
				return 0;

			std::vector<Color> colors(vertices.size());
			int edge{};

			for (size_t it = 0, itt = vertices.size(); it != itt; ++it)
			{
				edge = Table[it][1];

				if (!Painted(colors, Table[edge][0]))
					if (PaintGraph(colors, edge) == 0)
						return 0;
			}

			auto cit = colors.begin();

			for (auto&& vit = vertices.begin(), vitt = vertices.end(); vit != vitt; ++vit, ++cit)
				std::cout << vit->first << ' ' << *cit << ' ';

			std::cout << std::endl;

			return 1;
		}

	private:
		bool Painted(std::vector<Color>& colors, int vert)
		{
			if (int it = GetVIndex(vert); it != -1)
				return colors[it] != Color::colorless;
	
			return 0;
		}

		bool PaintGraph(std::vector<Color>& colors, int edge)
		{
			static int offset = (vertices.size() % 2) == 0 ? 0 : 2;

			Color vcolor = Color::blue, nxtcolor;
			int nxtedge{}, fstedge = Table[edge][2];

			std::vector<int> prevedge;
			prevedge.reserve(Table.size() - vertices.size());

			while (true)
			{
				prevedge.push_back(edge);

				if (!Painted(colors, Table[edge][0]))
				{
					Paint(colors, Table[edge][0], vcolor);
					SwitchColor(vcolor);
				}
				
				if ((edge ^ 1) + offset < Table.size())
				{
					nxtedge = (edge ^ 1) + offset;

					if (Painted(colors, Table[nxtedge][0]))
					{
						if (Table[edge][2] == fstedge || Table[edge][1] == fstedge)
							return GetVColor(Table[edge][0]) != vcolor;
					}
					else if (Table[nxtedge][1] < vertices.size())
						Paint(colors, Table[nxtedge][0], vcolor);
					else 
					{
						edge = Table[nxtedge][1];
						continue;
					}
				}

				edge   = GetPrevEdge(prevedge);
				vcolor = colors[GetVIndex(Table[edge][0])];
				SwitchColor(vcolor);
			}
		}

		int GetPrevEdge(std::vector<int>& prevedge)
		{
			int edge{};

			while (!prevedge.empty())
			{
				edge = prevedge.back();

				if (Table[edge][1] < vertices.size())
					prevedge.pop_back();
				else return Table[edge][1];
			}

			return edge;
		}

		void Paint(std::vector<Color>& colors, int vert, Color color) { colors[GetVIndex(vert)] = color; }

		Color GetVColor(std::vector<Color>& colors, int vert) { return colors[GetVIndex(vert)]; }

		void AddEdge (int vert) noexcept
		{
			int index = GetVIndex(vert);

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
