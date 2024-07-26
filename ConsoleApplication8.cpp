#include <iostream>
#include <cstdlib>
#include <stack>
#include <time.h> 

struct Tree 
{ 
    int key, size; // key: значение узла, size : размер поддерева с корнем в данном узле
    Tree* left_son;  // Указатель на левого потомка
    Tree* right_sibling;  // Указатель на правого "брата" (в контексте структуры "левый сын, правый брат")
    Tree* parent;   // Указатель на родительский узел


    // Конструктор узла дерева, инициализирующий узел с заданным ключом
    Tree(int k) : key(k), size(1), left_son(nullptr), right_sibling(nullptr), parent(nullptr) {}
};


// Определение класса для нашего двоичного дерева
class BinaryTree 
{
public:
    Tree* root;  // Корень дерева

    // Конструктор дерева, инициализирует корень как nullptr (пустое дерево)
    BinaryTree() : root(nullptr) {}

    // Функция для обновления размера узла
    void fix_size(Tree* node) 
    {
        if (!node) return;  // Если узел не существует, выходим из функции
        node->size = 1;    // Инициализируем размер как 1 (сам узел)
        // Добавляем размеры левого потомка и правого брата к размеру узла, если они существуют
        if (node->left_son) node->size += node->left_son->size;
        if (node->right_sibling) node->size += node->right_sibling->size;
    }


    // Функция левого поворота
    Tree* rotateLeft(Tree* node) 
    {
        if (!node || !node->right_sibling) return node; // Если узел или его правый брат не существует, возвращаем текущий узел

        Tree* rSibling = node->right_sibling;
        node->right_sibling = rSibling->left_son;  // Устанавливаем правого брата левым сыном правого брата текущего узла
        if (node->right_sibling) node->right_sibling->parent = node;  // Обновляем родителя правого брата, если он существует

        rSibling->left_son = node;  // Делаем текущий узел левым сыном его правого брата
        rSibling->parent = node->parent;  // Обновляем родителя нового корня поддерева
        node->parent = rSibling;  // Делаем новый корень поддерева родителем текущего узла

        fix_size(node);  // Обновляем размер текущего узла
        fix_size(rSibling);  // Обновляем размер нового корня поддерева
        return rSibling;  // Возвращаем новый корень поддерева
    }

    // Функция правого поворота
    Tree* rotateRight(Tree* node) 
    {
        if (!node || !node->left_son) return node; // Если узел или его левый сын не существует, возвращаем текущий узел

        Tree* lSon = node->left_son;
        node->left_son = lSon->right_sibling;  // Устанавливаем правого брата левого сына текущего узла своим левым сыном
        if (node->left_son) node->left_son->parent = node;  // Обновляем родителя нового левого сына, если он существует

        lSon->right_sibling = node;  // Делаем текущий узел правым братом его левого сына
        lSon->parent = node->parent;  // Обновляем родителя нового корня поддерева
        node->parent = lSon;  // Делаем новый корень поддерева родителем текущего узла

        fix_size(node);  // Обновляем размер текущего узла
        fix_size(lSon);  // Обновляем размер нового корня поддерева
        return lSon;  // Возвращаем новый корень поддерева
    }

    // Вставка нового узла с заданным ключом в дерево, начиная с узла node
    Tree* insert(Tree* node, int key) 
    {
        // Если узла не существует, создаем новый узел с ключом key
        if (node == nullptr) 
        {
            return new Tree(key);
        }
        // Рандомизированное решение о вставке нового узла в корень поддерева
        if (rand() % (node->size + 1) == 0) 
        {
            return insertRoot(node, key);
        }
        else 
        {
            // Стандартная вставка в двоичное дерево поиска
            if (key < node->key) 
            {
                node->left_son = insert(node->left_son, key);
                node->left_son->parent = node;
            }
            else {
                node->right_sibling = insert(node->right_sibling, key);
                node->right_sibling->parent = node;
            }
        }
        fix_size(node);  // Обновляем размер узла
        return node;   // Возвращаем обновленный узел
    }

    void insert(int key) 
    {
        root = insert(root, key);
    }

    // Вспомогательная функция для вставки нового узла в корень поддерева
    Tree* insertRoot(Tree* node, int key) 
    {
        // Если узла не существует, создаем и возвращаем новый узел
        if (node == nullptr) 
        {
            return new Tree(key);
        }
        // Если ключ меньше ключа узла, вставляем новый узел в левое поддерево и выполняем правый поворот
        if (key < node->key) 
        {
            node->left_son = insertRoot(node->left_son, key);
            return rotateRight(node);
        }
        // В противном случае вставляем в правое поддерево и выполняем левый поворот
        else 
        {
            node->right_sibling = insertRoot(node->right_sibling, key);
            return rotateLeft(node);
        }
    }


    // Функция поиска узла с заданным ключом
    Tree* find(Tree* node, int key) 
    {
        if (!node) return nullptr; // Если узел не существует, возвращаем nullptr

        if (node->key == key) 
        {
            return node; // Если ключ узла совпадает с искомым, возвращаем этот узел
        }
        else if (key < node->key) 
        {
            return find(node->left_son, key); // Ищем в левом поддереве
        }
        else 
        {
            return find(node->right_sibling, key); // Ищем среди правых братьев
        }
    }

    
    // Функция, возвращающая значение ключа узла, если такой узел найден, иначе -1
    int values(Tree* node, int key) 
    {
        if (!node) return -1; // Если узел не существует, возвращаем -1

        if (node->key == key) 
        {
            return node->key; // Если ключ узла совпадает с искомым, возвращаем его
        }
        else if (key < node->key) 
        {
            return values(node->left_son, key); // Ищем в левом поддереве
        }
        else 
        {
            return values(node->right_sibling, key); // Ищем среди правых братьев
        }
    }

    // Класс для итератора прямого обхода
    class iterator_pre
    {
        std::stack<Tree*> stack;  // Стек для хранения пути обхода

    public:
        // Конструктор, принимающий корневой узел дерева
        iterator_pre(Tree* root) 
        {
            if (root) stack.push(root);  // Начинаем с корня дерева, если он существует
        }

        // Проверка на наличие следующего элемента
        bool hasNext() 
        {
            return !stack.empty();  // Возвращает true, если стек не пуст
        }

        // Переход к следующему элементу
        Tree* next() 
        {
            if (stack.empty()) return nullptr;  // Если стек пуст, возвращаем nullptr

            // Возвращаем текущий узел и добавляем детей в стек для последующего обхода
            Tree* currentNode = stack.top();
            stack.pop();

            // Сначала добавляем правого брата, затем левого сына (поскольку стек LIFO)
            if (currentNode->right_sibling) stack.push(currentNode->right_sibling);
            if (currentNode->left_son) stack.push(currentNode->left_son);

            return currentNode;  // Возвращаем текущий узел
        }
    };



    // Класс для итератора обратного обхода
    class iterator_post
    {
        std::stack<Tree*> stack;  // Стек для хранения пути обхода

    public:
        // Конструктор, принимающий корневой узел дерева
        iterator_post(Tree* root)
        {
            pushAll(root);  // Инициализируем стек, добавив все узлы из самого левого поддерева
        }

        // Проверка на наличие следующего элемента
        bool hasNext()
        {
            return !stack.empty();  // Возвращает true, если стек не пуст
        }

        // Переход к следующему элементу
        Tree* next() 
        {
            while (!stack.empty()) 
            {
                Tree* node = stack.top(); // Берем узел из вершины стека
                stack.pop(); // Удаляем узел из стека

                if (node->right_sibling) 
                {
                    pushAll(node->right_sibling); // Если у узла есть правый брат, добавляем его и всех его левых потомков в стек
                }

                return node; // Возвращаем узел
            }
            return nullptr; // Если стек пуст, возвращаем nullptr
        }

    private:
        void pushAll(Tree* node) 
        {
            while (node) 
            {
                stack.push(node); // Добавляем узел в стек
                node = node->left_son; // Переходим к левому потомку, если он есть
            }
            // Здесь не нужно добавлять правых братьев, так как они будут добавлены в методе next()
        }

    };
    // Класс для итератора симметричного обхода
    class iterator_in
    {
        std::stack<Tree*> stack;  // Стек для хранения пути обхода

    public:
        // Конструктор, принимающий корневой узел дерева
        iterator_in(Tree* root)
        {
            pushLeft(root);  // Инициализируем стек, добавив все левые узлы начиная с корня
        }

        // Проверка на наличие следующего элемента
        bool hasNext()
        {
            return !stack.empty();  // Возвращает true, если есть следующий элемент
        }

        // Переход к следующему элементу
        Tree* next()
        {
            if (stack.empty()) return nullptr;  // Если стек пуст, возвращаем nullptr

            Tree* node = stack.top();  // Получаем следующий узел из стека
            stack.pop();  // Удаляем узел из стека

            // Если у узла есть правый брат, добавляем все его левые узлы
            if (node->right_sibling)
            {
                pushLeft(node->right_sibling);
            }

            return node;  // Возвращаем узел
        }

    private:
        // Добавление всех левых узлов в стек
        void pushLeft(Tree* node)
        {
            while (node)
            {
                stack.push(node);  // Добавляем узел в стек
                node = node->left_son;  // Перемещаемся к левому сыну
            }
        }
    };


    // Методы для получения итераторов
    iterator_pre begin_pre() { return iterator_pre(root); } // Возвращает итератор для начала прямого обхода
    iterator_post begin_post() { return iterator_post(root); } // Возвращает итератор для начала обратного обхода
    iterator_in begin_in() { return iterator_in(root); } // Возвращает итератор для начала симметричного обхода

    /*
    iterator_pre end_pre() { return iterator_pre(nullptr); } // Возвращает итератор для конца прямого обхода
    iterator_post end_post() { return iterator_post(nullptr); } // Возвращает итератор для конца обратного обхода
    iterator_in end_in() { return iterator_in(nullptr); } // Возвращает итератор для конца симметричного обхода
    */

    Tree* find(int key) 
    {
        return find(root, key);
    }

    int values(int key) 
    {
        return values(root, key);
    }

    // Метод для объединения текущего дерева (A) с другим деревом (B) в прямом порядке
    void unionWith(BinaryTree& B) 
    {
        for (auto it = B.begin_pre(); it.hasNext();) 
        {
            Tree* node = it.next();
            if (!find(node->key)) 
            { // Если ключ не найден в текущем дереве
                insert(node->key); // Вставляем узел в текущее дерево
            }
        }
    }
    
};

int main()
{
    srand(time(nullptr)); // Инициализация генератора случайных чисел
    BinaryTree treeA;
    BinaryTree treeB;

    const int numElements = 6;
    int elementsA[numElements]; // Массив для элементов, которые будут вставлены в дерево A
    int elementsB[numElements]; // Массив для элементов, которые будут вставлены в дерево B

    /// Заполнение массива уникальными элементами для Tree A
    std::cout << "Elements to be inserted in Tree A:" << std::endl;
    for (int i = 0; i < numElements; ) 
    {
        int element = rand() % 100;
        bool isUnique = true;
        for (int j = 0; j < i; ++j) 
        {
            if (elementsA[j] == element) 
            {
                isUnique = false;
                break;
            }
        }
        if (isUnique) 
        {
            elementsA[i] = element;
            std::cout << element << " ";
            ++i;
        }
    }
    std::cout << std::endl;

    // Заполнение массива уникальными элементами для Tree B
    std::cout << "Elements to be inserted in Tree B:" << std::endl;
    for (int i = 0; i < numElements; ) 
    {
        int element = rand() % 100;
        bool isUnique = true;
        for (int j = 0; j < i; ++j) 
        {
            if (elementsB[j] == element) 
            {
                isUnique = false;
                break;
            }
        }
        if (isUnique) 
        {
            elementsB[i] = element;
            std::cout << element << " ";
            ++i;
        }
    }
    std::cout << std::endl;

    // Вставка элементов в деревья treeA и treeB
    for (int i = 0; i < numElements; i++) 
    {
        treeA.insert(elementsA[i]);
    }
    for (int i = 0; i < numElements; i++) 
    {
        treeB.insert(elementsB[i]);
    }

    // Вывод элементов treeA в обратном порядке перед объединением
    std::cout << "Tree A (post-order) before union:" << std::endl;
    for (auto it = treeA.begin_post(); it.hasNext();) 
    {
        Tree* node = it.next();
        std::cout << node->key << " ";
    }
    std::cout << std::endl;

    // Вывод элементов treeB в симметричном порядке
    std::cout << "Tree B (in-order):" << std::endl;
    for (auto it = treeB.begin_in(); it.hasNext();) 
    {
        Tree* node = it.next();
        std::cout << node->key << " ";
    }
    std::cout << std::endl;

    // Объединяем treeA и treeB
    treeA.unionWith(treeB);

    // Вывод элементов treeA после объединения в прямом порядке
    std::cout << "Tree A (pre-order) after union with B:" << std::endl;
    for (auto it = treeA.begin_pre(); it.hasNext();) 
    {
        Tree* node = it.next();
        std::cout << node->key << " ";
    }
    std::cout << std::endl;
};