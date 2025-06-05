#include <iostream>
#include <memory>
#include <algorithm>
#include <queue>


template<typename T>
class AVL
{
    class Node
    {
        T data; //ce tine nodul in el
        std::shared_ptr<Node> left; //ce are in stg vs in dreapta;
        std::shared_ptr<Node> right;
        int height = 1; //ca avem un nod
    public:
        explicit Node(T data_): data(data_), height(1), left(nullptr), right(nullptr)
        {
        } ///constructor  ptr un nod fara copii
        // friend int height(std::shared_ptr<Node> N);
        friend class AVL;
    };

    int height(std::shared_ptr<Node> N)
    {
        if (N != nullptr)
            return N->height;
        return 0;
    }

    ///    x N
    ///   /
    ///  y newROOT
    /// / \
    ///z    in caz ca e cv aici
    std::shared_ptr<Node> rightRotation(std::shared_ptr<Node> N)
    {
        std::shared_ptr<Node> newROOT = N->left;
        N->left = newROOT->right; ///in caz ca avem  arbore in dreapta, vine mai mic decat x deci in stanga
        newROOT->right = N;
        N->height = 1 + std::max(height(N->left), height(N->right));
        newROOT->height = 1 + std::max(height(newROOT->left), height(newROOT->right));
        return newROOT;
    }

    std::shared_ptr<Node> leftRotation(std::shared_ptr<Node> N)
    {
        std::shared_ptr<Node> newROOT = N->right;
        N->right = newROOT->left; //ptr ca orice e in dreapta lui N este mai mare decat N, deci merge in dreapta lui
        newROOT->left = N;
        N->height = 1 + std::max(height(N->left), height(N->right));
        newROOT->height = 1 + std::max(height(newROOT->left), height(newROOT->right));
        return newROOT;
    }

    std::shared_ptr<Node> Insert(std::shared_ptr<Node> N, T value) //rot  si valoarea pe care o inserez
    {
        if (N == nullptr)
        {
            return std::make_shared<Node>(value); //initializare
        } else
        {
            if (value < N->data)
                N->left = Insert(N->left, value); //incerc sa merg mai departe pe stanga
            else if (value > N->data)
                N->right = Insert(N->right, value);
            else if (value == N->data)
                return N;//nu inserez de 2 ori

            ///dupa ce insereaza  schimba inaltimea si apoi balanseaza
            N->height = 1 + std::max(height(N->left), height(N->right));
            int bf = height(N->left) - height(N->right); //balance factor,sa vad daca le rotesc
            ///poate fi doar -1, 0 , 1, daca e >1- trb sa fac right rotation, daca e < -1  trebuie sa fac  leftRotation
            if (bf > 1)
            {
                if (value < N->left->data) //avem LL-left left deci facem rightRotation
                    return rightRotation(N);
                else //AM LR---- aduc catre LL si appoi fac Rrot
                {
                    N->left = leftRotation(N->left);
                    return rightRotation(N);
                }
            }
            if (bf < -1)
            {
                if (value > N->right->data)
                    return leftRotation(N); // RR
                else
                {
                    N->right = rightRotation(N->right); // RL
                    return leftRotation(N);
                }
            }
        }

        return N;
    }

    ////----------CAUTARE------------
    bool search(std::shared_ptr<Node> N, T value)
    {
        if (N == nullptr)
            return false;
        if (value == N->data)
            return true;
        if (value < N->data)
            return search(N->left, value);
        return search(N->right, value);
    }

    /////////--------------STERGERE-------
    std::shared_ptr<Node> remove(std::shared_ptr<Node> N, T value)
    {
        if (N == nullptr)
            return nullptr;

        if (value < N->data)
        {
            N->left = remove(N->left, value);
        }
        else if (value > N->data)
        {
            N->right = remove(N->right, value);
        }
        else
        {
            if (N->left == nullptr && N->right == nullptr)//n-are copii deci sterg direct
                return nullptr;
            if (N->left == nullptr)
                return N->right;//cu un sg copil
            if (N->right == nullptr)
                return N->left;

            std::shared_ptr<Node> maximst = N->left;//gen pun cea mai mare val din subarbore ca sa stiu sigur ca toate celelate noduri sunt mai mici decat el
            while (maximst->right != nullptr)
                maximst = maximst->right;

            N->data = maximst->data;//copiez in ala pe care vr sa l sterg
            N->left = remove(N->left, maximst->data);//sterg pe cel mai din dr;
        }

        N->height = 1 + std::max(height(N->left), height(N->right));
        int bf = height(N->left) - height(N->right);

        if (bf > 1)
        {
            if (height(N->left->left) >= height(N->left->right))
                return rightRotation(N);
            else
            {
                N->left = leftRotation(N->left);
                return rightRotation(N);
            }
        }

        if (bf < -1)
        {
            if (height(N->right->right) >= height(N->right->left))
                return leftRotation(N);
            else
            {
                N->right = rightRotation(N->right);
                return leftRotation(N);
            }
        }

        return N;
    }


    std::shared_ptr<Node> ROOT = nullptr;

public:
    void insert(T value)
    {
        ROOT = Insert(ROOT, value);
    }

    ///---------AFISARE-----------
    void preorder(std::shared_ptr<Node> N)
    {
        //AFISARE ROOT-STNGA-DREAPTA
        if (N == nullptr)
            return;
        std::cout << N->data << " ";
        preorder(N->left);
        preorder(N->right);
    }

    void preOrder()
    {
        preorder(ROOT);
        std::cout << std::endl;
    }

    void inorder(std::shared_ptr<Node> N)
    {
        //stanga root  dr
        if (N == nullptr)
            return;
        inorder(N->left);
        std::cout << N->data << " ";
        inorder(N->right);
    }

    void inOrder()
    {
        inorder(ROOT);
        std::cout << std::endl;
    }

    void postorder(std::shared_ptr<Node> N)
    {
        //stanga dreapta, radacina
        if (N == nullptr)
            return;
        postorder(N->left);
        postorder(N->right);
        std::cout << N->data << " ";
    }

    void postOrder()
    {
        postorder(ROOT);
        std::cout << std::endl;
    }

    void levelOrder()
    {
        if (ROOT == nullptr)
            return;

        std::queue<std::shared_ptr<Node> > q;
        q.push(ROOT);

        while (!q.empty())
        {
            std::shared_ptr<Node> N = q.front();
            q.pop();

            std::cout << N->data << " ";

            if (N->left != nullptr)
                q.push(N->left);
            if (N->right != nullptr)
                q.push(N->right);
        }

        std::cout << std::endl;
    }

    bool search(T value)
    {
        return search(ROOT, value);
    }

    void remove(T value)
    {
        ROOT = remove(ROOT, value);
    }
};
int main() {
    AVL<int> avl;

    avl.insert(31);
    avl.insert(03);
    avl.insert(05);
    avl.insert(17);
    avl.insert(11);
    avl.insert(73);
    avl.insert(22);
    avl.insert(9);
    avl.insert(05);




    std::cout << "Stanga-root-dreapta(inorder): "; avl.inOrder();
    std::cout << "Root-stanga-dreapta(preorder): "; avl.preOrder();
    std::cout << "Stanga-dreapta-radacina(postorder): "; avl.postOrder();
    std::cout << "Pe nivele: "; avl.levelOrder();


    int x;
    std::cout<<std::endl<<"Cautare de numar. Numarul cautat e: \n";
    std::cin>>x;
    std::cout << "Status "<<x<<": " << (avl.search(x) ? "gasit" : "nu exista") << "\n";
    int y,z;
    std::cout<<"Stergere de noduri. 2 valori de sters: \n";
    std::cin>>y>>z;

    avl.remove(y);
    avl.remove(z);

    std::cout << "Dupa stergeri:\n";
    std::cout << "InOrder: "; avl.inOrder();
    std::cout << "LevelOrder: "; avl.levelOrder();

    return 0;
}

