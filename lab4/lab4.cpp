/*
27. Информация о некотором изделии задана с помощью
ориентированного графа. Вершина графа соответствует отдельной
операции, а дуги показывают очередность выполнения операций
(каждая операция может начаться не ранее окончания
предшествующих операций). Продолжительность каждой операции
известна. Имеется один станок для выполнения каждого типа
операций. Если станок освобождается, то производится  та  из
готовых к выполнению операций, для которой максимальный путь
от соответствующей вершины до конца имеет наибольшую  длину.
Определить время изготовления изделия. Выдать общий график
выполнения операций и график загрузки каждого станка (12).
*/

#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <stack>


using AdjacencyMatrix = std::vector<std::vector<int>>;
using OperationsInfo = std::map<int, std::pair<int, int>>;

const int NO_EDGE = 0;
const int INF = INT_MAX;

enum VertexState
{
	White, //Непосещенные
	Gray, //В процессе обработки
	Black //Полностью обработанные
};

struct Graph
{
	AdjacencyMatrix matrix;
	std::vector<int> maxWays;
	std::vector<int> time;
	std::vector<std::string> types;
	std::map<std::string, std::vector<std::pair<int, int>>> machineStatuses;
};

std::ostream& operator << (std::ostream& out, std::stack<int> stack)
{
	while (stack.size() != 0)
	{
		out << stack.top() + 1 << " ";
		stack.pop();
	}
	return out;
}

std::ostream& PrintOperations(std::ostream& out, OperationsInfo operations, std::vector<std::string> types)
{
	out << "График выполнения операций\n  N        Тип                Время\n";
	for (auto operation : operations)
	{
		std::string margin(20 - types[operation.first].length(), ' ');
		out << "  " << operation.first << "      " << types[operation.first] << margin << operation.second.first << " " << operation.second.second << "\n";
	}
	std::cout << "\n";
	return out;
}

std::ostream& PrintMachines(std::ostream& out, std::map<std::string, std::vector<std::pair<int, int>>>& machineStatuses)
{
	out << "Загрузка станков:\n";
	for (const auto& machine : machineStatuses)
	{
		out << "  " << machine.first << ":";
		bool droppedFirst = false;
		for (const auto& operation : machine.second)
		{
			if (!droppedFirst)
			{
				droppedFirst = true;
				continue;
			}
			out << " [" << operation.first << " " << operation.second << "],";
		}
		out << "\n";
	}
	out << "\n";
	return out;
}

std::ostream& operator << (std::ostream& out, std::vector<int> vector)
{
	for (auto elm : vector)
	{
		out << elm << " ";
	}
	return out;
}

std::ostream& operator << (std::ostream& out, Graph graph)
{
	int size = graph.matrix.size();

	out << "Продолжительности:  " << graph.time << "\n";

	out << "Типы: ";
	for (auto type : graph.types)
	{
		out << type << " ";
	}
	out << "\n";

	out << "Максимальные пути: " << graph.maxWays << "\n";

	out << "Матрица смежности:\n";
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			out << graph.matrix[i][j] << " ";
		}
		out << "\n";
	}
	return out;
}

Graph CreateGraph(std::ifstream& operations, std::ifstream& edges)
{
	Graph graph;
	std::string type;
	int time;
	while (operations >> type >> time)
	{
		graph.types.push_back(type);
		if (!graph.machineStatuses.count(type))
			graph.machineStatuses[type] = std::vector<std::pair<int, int>>{ {0, 0} };
		graph.time.push_back(time);
	}
	graph.matrix = AdjacencyMatrix(graph.types.size(), std::vector<int>(graph.types.size(), NO_EDGE));

	int from;
	int to;
	while (edges >> from >> to) 
	{
		if (from > graph.matrix.size() || to > graph.matrix.size() || from < 1 || to < 1)
			std::cout << "Некорректное ребро: из " << from << " в " << to << "\n";
		else
			graph.matrix[from - 1][to - 1] = 1; ///СДВИГ!!!
	}

	graph.maxWays = std::vector<int>(graph.types.size(), 0);
	return graph;
}

Graph SortTopologically(Graph& graph, std::vector<int>& order)
{
	Graph sortedGraph = graph;

	int size = graph.types.size();
	for (int i = 0; i < size; i++)
	{
		sortedGraph.time[i] = graph.time[order[size - 1 - i]];
		sortedGraph.types[i] = graph.types[order[size - 1 - i]];

		for (int j = 0; j < size; j++)
		{
			sortedGraph.matrix[i][j] = graph.matrix[order[size - 1 - i]][order[size - 1 - j]];
		}
	}

	for (int i = size - 1; i >= 0; i--)
	{
		int childrenMaxWay = 0;
		for (int j = size - 1; j >= 0; j--)
		{
			if (sortedGraph.matrix[i][j] != NO_EDGE)
				childrenMaxWay = (std::max)(childrenMaxWay, sortedGraph.maxWays[j]);
		}
		sortedGraph.maxWays[i] = sortedGraph.time[i] + childrenMaxWay;
	}
	return sortedGraph;
}

bool dfs(int i, std::vector<int>& topologicalIndexes, std::stack<int>& cycleStack, std::vector<VertexState>& vertexStates, AdjacencyMatrix& adjMatrix)
//Возвращает true, если нет циклов, false, если найден цикл
{
	vertexStates[i] = Gray;

	for (int j = 0; j < adjMatrix[i].size(); j++)
	{
		bool notProcessedEdge = adjMatrix[i][j] != NO_EDGE && vertexStates[j] != Black;
		if (notProcessedEdge)
		{
			bool cycleFound = vertexStates[j] == Gray || !dfs(j, topologicalIndexes, cycleStack, vertexStates, adjMatrix);
			if (cycleFound)
			{
				cycleStack.push(i);
				return false;
			}
		}
	}
	vertexStates[i] = Black;
	topologicalIndexes.push_back(i);
	return true;
}

bool GetTopolicalOrder(AdjacencyMatrix& adjMatrix, std::vector<int>& topologicalOrder, std::stack<int>& cycle)
{
	std::vector<VertexState> vertexStates(adjMatrix.size(), White);

	for (int i = 0; i < vertexStates.size(); i++)
	{
		if (vertexStates[i] == Black)
			continue;
		if (!dfs(i, topologicalOrder, cycle, vertexStates, adjMatrix))
			return false;
	};
	return true;
}

void UpdateAvalible(AdjacencyMatrix& matrix, OperationsInfo& avalible, OperationsInfo& completed)
{
	for (int i = 0; i < matrix.size(); i++)
	{
		bool hasUncompletedParents = false;

		if (avalible.count(i) || completed.count(i)) 
			continue;

		for (int j = 0; j < matrix.size(); j++)
		{
			if (matrix[j][i] && !completed.count(j))
			{
				hasUncompletedParents = true;
				break;
			}
		}

		if (!hasUncompletedParents)
			avalible[i] = { INF, INF };
	}
}

void UpdateCompleted(AdjacencyMatrix&, int time, OperationsInfo& avalible, OperationsInfo& completed)
{
	for (auto it = avalible.begin(); it != avalible.end(); ) {
		if (time >= it->second.second) {
			completed[it->first] = it->second;
			it = avalible.erase(it);
		}
		else
			++it;
	}
}

int FindPriorityOperation(Graph graph, std::string type, OperationsInfo& avalible, OperationsInfo& completed)
{
	int maxPriorityOperation = -INF;
	int priority = -INF;
	for (const auto& operation : avalible)
	{
		if (type == graph.types[operation.first] && graph.maxWays[operation.first] > priority)
		{
			maxPriorityOperation = operation.first;
			priority = graph.maxWays[operation.first];
		}
	}
	return maxPriorityOperation;
}

int HandleMachines(Graph& graph, int& time, OperationsInfo& avalible, OperationsInfo& completed)
{
	int newTime = INF;
	for (auto& machine : graph.machineStatuses)
	{
		if (machine.second.back().second <= time) //Время завершения последней операции на станке
		{
			int operation = FindPriorityOperation(graph, machine.first, avalible, completed);
			if (operation != -INF)
			{
				avalible[operation] = { time, time + graph.time[operation] };
				machine.second.push_back({ time, time + graph.time[operation] });
				newTime = (std::min)(newTime, time + graph.time[operation]);
			}
		}
	}
	return newTime != INF ? newTime : time + 1;
}

int StartSimulation(Graph& graph)
{
	int time = 0;

	OperationsInfo completed;
	OperationsInfo avalible;

	while (completed.size() != graph.matrix.size())
	{
		UpdateCompleted(graph.matrix, time, avalible, completed);
		UpdateAvalible(graph.matrix, avalible, completed);
		time = HandleMachines(graph, time, avalible, completed);
	}
	PrintOperations(std::cout, completed, graph.types);
	PrintMachines(std::cout, graph.machineStatuses);

	return time - 1;
}

int main(int argc, char* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	if (argc != 3)
	{
		std::cout << "Передайте в качестве параметров файл операций и файл ребер";
		return 1;
	}
	std::ifstream operations(argv[1]);
	std::ifstream edges(argv[2]);

	Graph graph = CreateGraph(operations, edges);
	std::cout << "Начальный граф:\n" << graph << "\n";
	std::stack<int> cycle;
	std::vector<int> topologicalOrder;

	if (!GetTopolicalOrder(graph.matrix, topologicalOrder, cycle))
	{
		std::cout << "Найден цикл: " << cycle << "\n";
		return 1;
	}

	graph = SortTopologically(graph, topologicalOrder);

	std::cout << "Отсортированный граф:\n" << graph << "\n";

	int totalTime = StartSimulation(graph);

	std::cout << "Итоговое время: " << totalTime << "\n";
}
