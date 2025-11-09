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
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <stack>


using AdjacencyMatrix = std::vector<std::vector<int>>;

const int NO_EDGE = 0;
const int INF = INT_MAX;

enum VertexState
{
	White, //Непосещенные
	Gray, //В процессе обработки
	Black //Полностью обработанные
};

struct Operation 
{
	int index;
	int name;
	std::string type;
	int time;
	int maxWay;
	int start = INF;
	int end = INF;
};

struct Graph
{
	AdjacencyMatrix matrix;
	std::vector<Operation> operations;
	std::map<std::string, std::vector<std::pair<int, int>>> machineStatuses;
};

std::ostream& PrintCycle (std::ostream& out, std::stack<int> stack, std::vector<Operation> ops)
{
	while (stack.size() != 0)
	{
		int index = stack.top();
		auto op = std::find_if(ops.begin(), ops.end(), [index](const Operation& op)
			{
				return op.index == index;
			});
		out << op->name << " ";
		stack.pop();
	}
	out << "\n";
	return out;
}

std::ostream& PrintOperations(std::ostream& out, const std::map<int, Operation>& completed)
{

	std::map<int, Operation> nameSort;

	for (const auto& op : completed)
	{
		nameSort[op.second.name] = op.second;
	}

	out << std::left;
	out << "  График выполнения операций \n";
	out << "    N       Тип              Время\n";

	for (const auto& op : nameSort)
	{
		out << "    " << std::setw(5) << op.second.name << std::setw(20) << op.second.type << op.second.start << " " << op.second.end << "\n";
	}

	out << "\n";

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

Graph CreateGraph(std::ifstream& operations, std::ifstream& edges)
{
	Graph graph;

	std::string type;
	int name;
	int time;
	for (int i = 0; operations >> name >> type >> time; i++)
	{
		Operation op{ i, name, type, time, 0 };
		graph.operations.push_back(op);
		if (!graph.machineStatuses.count(type))
			graph.machineStatuses[type] = std::vector<std::pair<int, int>>{ {0, 0} };
	}

	graph.matrix = AdjacencyMatrix(graph.operations.size(), std::vector<int>(graph.operations.size(), NO_EDGE));

	int from;
	int to;
	while (edges >> from >> to) 
	{
		auto itFrom = std::find_if(graph.operations.begin(), graph.operations.end(),
			[from](const Operation& op)
			{
					return op.name == from; 
			});
		auto itTo = std::find_if(graph.operations.begin(), graph.operations.end(),
			[to](const Operation& op)
			{
				return op.name == to;
			});

		if (itFrom == graph.operations.end() || itTo == graph.operations.end())
		{
			std::cout << "Некорректное ребро из " << from << " в " << to << "\n";
		}
		else
			graph.matrix[itFrom->index][itTo->index] = 1;
	}

	return graph;
}

Graph SortTopologically(Graph& graph, std::vector<int>& order)
{
	Graph sortedGraph = graph;

	int size = graph.operations.size();
	for (int i = 0; i < size; i++)
	{
		sortedGraph.operations[i] = graph.operations[order[size - 1 - i]];
		sortedGraph.operations[i].index = i;

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
				childrenMaxWay = (std::max)(childrenMaxWay, sortedGraph.operations[j].maxWay);
		}
		sortedGraph.operations[i].maxWay = sortedGraph.operations[i].time + childrenMaxWay;
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

void UpdateAvalible(Graph& graph, std::map<int, Operation>& avalible, std::map<int, Operation>& completed, std::map<int, Operation>& inProgress)
{
	for (int i = 0; i < graph.matrix.size(); i++)
	{
		if (avalible.count(i) || completed.count(i) || inProgress.count(i))
			continue;

		bool hasUncompletedParents = false;

		for (int j = 0; j < graph.matrix.size(); j++)
		{
			if (graph.matrix[j][i] && !completed.count(j))
			{
				hasUncompletedParents = true;
				break;
			}
		}
		if (!hasUncompletedParents)
			avalible[i] = graph.operations[i];
	}
}

void UpdateCompleted(int time, std::map<int, Operation>& completed, std::map<int, Operation>& inProgress)
{
	for ( auto it = inProgress.begin(); it != inProgress.end(); )
	{
		if (time >= it->second.end)
		{
			completed[it->first] = it->second;
			it = inProgress.erase(it);
		}
		else
			++it;
	}
}

int FindNextOperationId(const Graph& graph, std::string type, std::map<int, Operation>& avalible)
{
	int maxPriorityOperationId = -INF;
	int priority = -INF;
	for (const auto& operationInfo : avalible)
	{
		if (type == operationInfo.second.type && operationInfo.second.maxWay > priority)
		{
			maxPriorityOperationId = operationInfo.first;
			priority = operationInfo.second.maxWay;
		}
	}
	return maxPriorityOperationId;
}

int HandleMachines(Graph& graph, int& time, std::map<int, Operation>& avalible, std::map<int, Operation>& inProgress)
{
	int newTime = INF;
	for (auto& machine : graph.machineStatuses)
	{
		if (machine.second.back().second <= time) //Время завершения последней операции на станке
		{
			int id = FindNextOperationId(graph, machine.first, avalible);
			if (id != -INF)
			{
				Operation op = avalible.at(id);
				avalible.erase(id);

				op.start = time;
				op.end = time + op.time;
				inProgress[id] = op;

				machine.second.push_back({ time, time + op.time });
				newTime = (std::min)(newTime, time + op.time);
			}
		}
	}
	return newTime != INF ? newTime : time + 1;
}

int StartSimulation(Graph& graph)
{
	int time = 0;

	std::map<int, Operation> completed;
	std::map<int, Operation> avalible;
	std::map<int, Operation> inProgress;

	int ITERATIONS = 0;

	while (completed.size() != graph.matrix.size())
	{
		UpdateCompleted(time, completed, inProgress);
		UpdateAvalible(graph, avalible, completed, inProgress);
		time = HandleMachines(graph, time, avalible, inProgress);
	}

	PrintOperations(std::cout, completed);
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
	std::stack<int> cycle;
	std::vector<int> topologicalOrder;

	if (!GetTopolicalOrder(graph.matrix, topologicalOrder, cycle))
	{
		std::cout << "Найден цикл: ";
		PrintCycle(std::cout, cycle, graph.operations);
		return 1;
	}

	graph = SortTopologically(graph, topologicalOrder);

	int totalTime = StartSimulation(graph);

	std::cout << "Итоговое время: " << totalTime << "\n";
}
