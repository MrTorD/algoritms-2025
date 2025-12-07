#include "BTree.h"

#include <queue>

BTree::BTree(int level)
{
    this->level = level * 2;
    root = new Node;
    root->sons = std::vector<Node*>(this->level, nullptr);
    root->records = std::vector<std::pair<int, std::string>>(this->level - 1, { BTree::INF, ""});
}

void BTree::insertRecord(std::vector<Record>& records, Record& record)
{
    int index = 0;
    auto it = std::lower_bound(records.begin(), records.end(), record,
        [](const std::pair<int, std::string>& a, const std::pair<int, std::string>& b)
        { 
            return a.first < b.first; 
        }
    );
    index = std::distance(records.begin(), it);

    for (int i = records.size() - 2; i >= index; i--)
    {
        records[i + 1] = records[i];
    }
    records[index] = record;
}

std::vector<Record> BTree::copyRecordsPartly(const std::vector<Record>& records, int from, int to)
{
    std::vector<Record> newRecords(level - 1, { INF, ""});
    for (int i = 0; i < to - from + 1; i++)
    {
        newRecords[i] = records[from + i];
    }
    return newRecords;
}

std::vector<BTree::Node*> BTree::copySonsPartly(const std::vector<BTree::Node*>& sons, int from, int to)
{
    std::vector<BTree::Node*> newSons(level, nullptr);
    for (int i = 0; i < to - from + 1; i++)
    {
        newSons[i] = sons[from + i];
    }
    return newSons;
}

void BTree::insert(int key, std::string value)
{
    if (BTree::find(key))
    {
        std::cout << "Ключ " << key << " уже существует. Он не будет добавлен\n";
        return;
    }
    Record record{ key, value };
    auto result = BTree::recursiveInsert(root, record);
    if (result.first.first != INF)
    {
        Node* newRoot = new Node;
        newRoot->sons = std::vector<Node*>(level, nullptr);
        newRoot->records = std::vector<Record>(level - 1, { INF, ""});
        newRoot->sons[0] = root;
        newRoot->sons[1] = result.second;
        newRoot->records[0] = result.first;
        root = newRoot;
    }
}

std::pair<Record, BTree::Node*> BTree::recursiveInsert(Node* node, Record& record)
{
    if (node->sons[0] == nullptr) //Вставка в лист
    {
        if (node->records.back().first == INF) //Лист не переполнен
        {
            insertRecord(node->records, record);
            return { { INF, "" }, nullptr };
        }
        else // Лист переполнен
        {
            std::vector<Record> extraRecords = node->records;
            extraRecords.push_back({ INF, ""});
            insertRecord(extraRecords, record);
            auto middle = extraRecords[level / 2];
            auto newRightChild = new Node;
            newRightChild->sons = std::vector<Node*>(level, nullptr);
            node->records = copyRecordsPartly(extraRecords, 0, level / 2 - 1);
            newRightChild->records = copyRecordsPartly(extraRecords, level / 2 + 1, level - 1);
            return { middle, newRightChild };
        }
    }
    else //Вставка не в лист
    {
        int index = 0;
        while (index < level - 1 && node->records[index].first != INF && node->records[index].first < record.first)
        {
            index++;
        }
        
        auto result = recursiveInsert(node->sons[index], record);
        Record middle = result.first;
        if (middle.first == INF)
            return { { INF, "" }, nullptr};
        Node* rightChild = result.second;

        if (node->records.back().first == INF)
        {
            insertRecord(node->records, middle);

            for (int i = node->sons.size() - 2; i > index; i--) 
            {
                node->sons[i + 1] = node->sons[i];
            }
            node->sons[index + 1] = rightChild;

            return { { INF, "" }, nullptr};
        }
        else 
        {
            std::vector<Record> extraRecords = node->records;
            std::vector<Node*> extraSons = node->sons;
            
            extraRecords.push_back({ INF, ""});
            extraSons.push_back(nullptr);

            insertRecord(extraRecords, middle);
            for (int i = level - 1; i > index; i--)
            {
                extraSons[i + 1] = extraSons[i];
            }
            extraSons[index + 1] = rightChild;

            Record middle = extraRecords[level / 2];

            Node* newRightChild = new Node;
            newRightChild->sons = std::vector<Node*>(level, nullptr);
            newRightChild->records = std::vector<Record>(level - 1, { INF, ""});

            node->records = copyRecordsPartly(extraRecords, 0, level / 2 - 1);
            newRightChild->records = copyRecordsPartly(extraRecords, level / 2 + 1, level - 1);

            node->sons = copySonsPartly(extraSons, 0, level / 2);
            newRightChild->sons = copySonsPartly(extraSons, level / 2 + 1, level);

            return { middle, newRightChild };
        }
    }
}

void BTree::printStructure(std::ostream& out)
{
    recursivePrintStructure(out, root, 0);
}

void BTree::printValues(std::ostream& out)
{
    recursivePrintValues(out, root);
}

void BTree::recursivePrintStructure(std::ostream& out, BTree::Node* node, int level)
{
    for (int i = 0; i < level; i++)
    {
        out << "-";
    }

    out << "[";
    for (auto record : node->records)
    {
        if (record.first == INF)
            break;
        out << record.first << " ";
    }
    out << "]\n";

    for (auto son : node->sons)
    {
        if (son == nullptr)
            break;
        recursivePrintStructure(out, son, level + 1);
    }
}

void BTree::recursivePrintValues(std::ostream& out, Node* node)
{
    if (node == nullptr)
        return;

    int i = 0;
    while (i < level - 1 && node->records[i].first != INF)
    {
        if (node->sons[i] != nullptr)
            recursivePrintValues(out, node->sons[i]);

        out << node->records[i].first << " " << node->records[i].second << "\n";

        i++; 
    }

    if (node->sons[i] != nullptr)
        recursivePrintValues(out, node->sons[i]);
}

bool BTree::find(int value)
{
    return BTree::recursiveFind(root, value);
}

bool BTree::recursiveFind(BTree::Node* node, int key)
{
    if (node == nullptr)
        return false;

    int index = 0;
    while (index < level - 1 && node->records[index].first != INF && node->records[index].first < key)
    {
        index++;
    }

    if (index < level - 1 && node->records[index].first == key)
    {
        std::cout << "Значение по ключу: " << node->records[index].second << "\n";
        return true;
    }

    if (node->sons[index] != nullptr)
        return recursiveFind(node->sons[index], key);
    else
        return false;
}
