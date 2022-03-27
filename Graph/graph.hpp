#include <map>
#include <vector>
#include <array>
#include <iostream>
#include <iomanip>
#include <deque>

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
		std::vector<Color> colors;
		int offset;

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
			offset = (v_count % 2) == 0 ? 0 : 2;

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
			for (size_t it = vertices.size(); it < Table.size(); it += 2)
			{
				if (Table[it][0] 	 			== edge.first && 
					Table[GetAdjacentEdge(it)][0] == edge.second)
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
			if (!Edata.empty())
			{
				for (size_t it = vertices.size(), eit = 0; it < Table.size(); it += 2, eit++)
					std::cout << "({"  << Table[it][0] << ", " << Table[GetAdjacentEdge(it)][0] 
							  << "}, " << Edata[eit] << ") ";
			}
			else 
			{
				for (size_t it = vertices.size(); it < Table.size(); it += 2)
					std::cout << "{"  << Table[it][0] << ", " << Table[GetAdjacentEdge(it)][0] << "} ";
			}

			std::cout << std::endl;
		}

		int Distance(int vstart, int vend) const
		{
			if (GetVIndex(vstart) == -1 || GetVIndex(vend) == -1)
				return -1;

			std::vector<int> dist(vertices.size(), -1);
			dist[GetVIndex(vstart)] = 0;

			std::deque<int> verts;
			verts.push_back(vstart);

			std::vector<int> adjacentv;
			int vcurr{};

			while (!verts.empty())
			{
				vcurr = verts.front();
				verts.pop_front();

				adjacentv = FindAdjacentVerts(vcurr);

				for (auto&& vert: adjacentv)
				{
					if (dist[GetVIndex(vert)] == -1)
					{
						verts.push_back(vert);
						dist[GetVIndex(vert)] = dist[GetVIndex(vcurr)] + 1;
					}
				}
			}

			return dist[GetVIndex(vend)];
		}

		std::vector<int> FindAdjacentVerts(int vert) const
		{
			std::vector<int> adjacentv;

			int curredge = Table[GetVIndex(vert)][1];

			while (curredge >= vertices.size())
			{
				adjacentv.push_back(Table[GetAdjacentEdge(curredge)][0]);
				curredge = Table[curredge][1];
			}

			return adjacentv;
		}

		bool IsBipartite()
		{
			if (Table.empty())
				return 0;

			colors.resize(vertices.size());
			int edge{};

			for (size_t it = 0, itt = vertices.size(); it != itt; ++it)
			{
				edge = Table[it][1];

				if (!Painted(Table[edge][0]))
					if (PaintGraph(edge) == 0)
						return 0;
			}
			return 1;
		}

		void Coloration()
		{
			auto cit = colors.begin();
			for (auto&& vit = vertices.begin(), vitt = vertices.end(); vit != vitt; ++vit, ++cit)
				std::cout << vit->first << ' ' << *cit << ' ';
		}

	private:
		bool Painted(int vert) const
		{
			if (int it = GetVIndex(vert); it != -1)
				return colors[it] != Color::colorless;
	
			return 0;
		}

		bool PaintGraph(int edge)
		{
			Color vcolor = Color::blue;
			int nxtedge{}, fstedge = Table[edge][2];

			std::vector<int> prevedge;
			prevedge.reserve(Table.size() - vertices.size());

			while (true)
			{
				prevedge.push_back(edge);

				if (!Painted(Table[edge][0]))
				{
					Paint(Table[edge][0], vcolor);
					SwitchColor(vcolor);
				}

				nxtedge = GetAdjacentEdge(edge);
				
				if (nxtedge < Table.size())
				{
					if (Painted(Table[nxtedge][0]))
					{
						if (GetVColor(Table[nxtedge][0]) != vcolor)
							return 0;
						else if (Table[edge][1] == fstedge)
							return GetVColor(Table[edge][0]) != vcolor;
					}
					else if (Table[nxtedge][1] < vertices.size())
						Paint(Table[nxtedge][0], vcolor);
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

		int GetAdjacentEdge(int edge) const
		{
			if (edge % 2 == 0)
				return (edge ^ 1) - offset;
			else
				return (edge ^ 1) + offset;
		}

		int GetPrevEdge(std::vector<int>& prevedge) const
		{
			int edge{};

			while (!prevedge.empty())
			{
				edge = prevedge.back();
				prevedge.pop_back();

				if (Table[edge][1] >= vertices.size())
					return Table[edge][1];
			}

			return edge;
		}

		void Paint(int vert, Color color) { colors[GetVIndex(vert)] = color; }

		Color GetVColor(int vert) const { return colors[GetVIndex(vert)]; }

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
