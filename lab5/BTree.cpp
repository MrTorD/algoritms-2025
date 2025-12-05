#include "BTree.h"

#include <queue>

BTree::BTree(int level)
{
    this->level = level;
    root = new Node;
    root->sons = std::vector<Node*>(level, nullptr);
    root->values = std::vector<int>(level - 1, BTree::INF);
}

std::vector<int> BTree::insertValue(std::vector<int> values, int value)
{
    int index = 0;
    while (index < values.size() && values[index] < value)
    {
        index++;
    }

    for (int i = values.size() - 2; i >= index; i--)
    {
        values[i + 1] = values[i];
    }

    values[index] = value;

    return values;
}

std::vector<int> BTree::copyValuesPart(std::vector<int> values, int from, int to)
{
    std::vector<int> newValues(level - 1, BTree::INF);
    for (int i = 0; i < to - from + 1; i++)
    {
        newValues[i] = values[from + i];
    }
    return newValues;
}

std::vector<BTree::Node*> BTree::copySonsPart(std::vector<BTree::Node*> sons, int from, int to)
{
    std::vector<BTree::Node*> newSons(level, nullptr);
    for (int i = 0; i < to - from + 1; i++)
    {
        newSons[i] = sons[from + i];
    }
    return newSons;
}

void BTree::insert(int value)
{
    if (BTree::find(value))
    {
        std::cout << "Ключ " << value << " уже существует. Он не будет добавлен\n";
        return;
    }
    auto result = BTree::recursiveInsert(root, value);
    if (result.first != INF)
    {
        Node* newRoot = new Node;
        newRoot->sons = std::vector<Node*>(level, nullptr);
        newRoot->values = std::vector<int>(level - 1, INF);
        newRoot->sons[0] = root;
        newRoot->sons[1] = result.second;
        newRoot->values[0] = result.first;
        root = newRoot;
    }
}

std::pair<int, BTree::Node*> BTree::recursiveInsert(Node* node, int value)
{
    if (node->sons[0] == nullptr) //Вставка в лист
    {
        if (node->values.back() == INF) //Лист не переполнен
        {
            node->values = insertValue(node->values, value);
            return { INF, nullptr };
        }
        else // Лист переполнен
        {
            std::vector<int> extraValues = node->values;
            extraValues.push_back(INF);
            extraValues = insertValue(extraValues, value);
            int middle = extraValues[level / 2];
            Node* newRightChild = new Node;
            newRightChild->sons = std::vector<Node*>(level, nullptr);
            node->values = copyValuesPart(extraValues, 0, level / 2 - 1);
            newRightChild->values = copyValuesPart(extraValues, level / 2 + 1, level - 1);
            return { middle, newRightChild };
        }
    }
    else //Вставка не в лист
    {
        int index = 0;
        while (index < level - 1 && node->values[index] != INF && node->values[index] < value)
        {
            index++;
        }
        
        auto result = recursiveInsert(node->sons[index], value);
        int middle = result.first;
        if (middle == INF)
            return { INF, nullptr };
        Node* rightChild = result.second;

        if (node->values.back() == INF)
        {
            node->values = insertValue(node->values, middle);

            for (int i = node->sons.size() - 2; i > index; i--) 
            {
                node->sons[i + 1] = node->sons[i];
            }
            node->sons[index + 1] = rightChild;

            return { INF, nullptr };
        }
        else 
        {
            std::vector<int> extraValues = node->values;
            std::vector<Node*> extraSons = node->sons;
            
            extraValues.push_back(INF);
            extraSons.push_back(nullptr);

            extraValues = insertValue(extraValues, middle);
            for (int i = level - 1; i > index; i--)
            {
                extraSons[i + 1] = extraSons[i];
            }
            extraSons[index + 1] = rightChild;

            int middle = extraValues[level / 2];

            Node* newRightChild = new Node;
            newRightChild->sons = std::vector<Node*>(level, nullptr);
            newRightChild->values = std::vector<int>(level - 1, INF);

            node->values = copyValuesPart(extraValues, 0, level / 2 - 1);
            newRightChild->values = copyValuesPart(extraValues, level / 2 + 1, level - 1);

            node->sons = copySonsPart(extraSons, 0, level / 2);
            newRightChild->sons = copySonsPart(extraSons, level / 2 + 1, level);

            return { middle, newRightChild };
        }
    }
}

void BTree::print(std::ostream& out)
{
    std::queue<Node*> q;
    q.push(root);

    while (!q.empty())
    {
        int size = q.size();

        for (int i = 0; i < size; i++)
        {
            Node* node = q.front();
            q.pop();

            out << "[";
            for (auto value : node->values)
            {
                if (value == INF)
                    break;
                out << value << " ";
            }
            out << "]";

            for (auto son : node->sons)
            {
                if (son == nullptr)
                    break;
                q.push(son);
            }
        }
        out << "\n";
    }
}

bool BTree::find(int value)
{
    return BTree::recursiveFind(root, value);
}

bool BTree::recursiveFind(BTree::Node* node, int value)
{
    if (node == nullptr)
        return false;

    int index = 0;
    while (index < level - 1 && node->values[index] != INF && node->values[index] < value)
    {
        index++;
    }

    if (index < level - 1 && node->values[index] == value)
    {
        return true;
    }

    if (node->sons[index] != nullptr)
        return recursiveFind(node->sons[index], value);
    else
        return false;
}
