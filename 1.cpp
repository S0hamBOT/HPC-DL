#include <iostream>
#include <queue>
#include <vector>
#include <omp.h>
using namespace std;

// -------- Node Structure --------
struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int val) {
        data = val;
        left = nullptr;
        right = nullptr;
    }
};

// -------- Parallel BFS --------
void parallelBFS(Node* root) {
    if (!root) return;

    queue<Node*> q;
    q.push(root);

    cout << "BFS Traversal:\n";

    while (!q.empty()) {

        int size = q.size();        // ✅ Snapshot level size
        vector<Node*> level;

        // Extract current level
        for (int i = 0; i < size; i++) {
            level.push_back(q.front());
            q.pop();
        }

        // ✅ FIX: Remove parallel for — critical makes it sequential anyway
        // Just use critical directly for thread-safe print + push
        for (int i = 0; i < (int)level.size(); i++) {
            #pragma omp critical
            {
                cout << level[i]->data << " ";
                if (level[i]->left)
                    q.push(level[i]->left);
                if (level[i]->right)
                    q.push(level[i]->right);
            }
        }
    }
    cout << endl;
}

// -------- Parallel DFS (Preorder using Tasks) --------
void parallelDFSUtil(Node* root) {
    if (!root) return;

    // ✅ FIX: Added braces to critical
    #pragma omp critical
    {
        cout << root->data << " ";
    }

    #pragma omp task
    parallelDFSUtil(root->left);

    #pragma omp task
    parallelDFSUtil(root->right);

    #pragma omp taskwait          // ✅ FIX: Wait for both tasks to finish
}

void parallelDFS(Node* root) {
    cout << "DFS (Preorder):\n";

    #pragma omp parallel
    {
        #pragma omp single
        {
            parallelDFSUtil(root);
        }
    }
    cout << endl;
}

// -------- Main Function --------
int main() {
    //        1
    //      /   \
    //     2     3
    //    / \   / \
    //   4   5 6   7

    Node* root            = new Node(1);
    root->left            = new Node(2);
    root->right           = new Node(3);
    root->left->left      = new Node(4);
    root->left->right     = new Node(5);
    root->right->left     = new Node(6);
    root->right->right    = new Node(7);

    double start, end;

    // ----- BFS -----
    start = omp_get_wtime();
    parallelBFS(root);
    end = omp_get_wtime();
    cout << "BFS Time: " << end - start << " sec\n\n";

    // ----- DFS -----
    start = omp_get_wtime();
    parallelDFS(root);
    end = omp_get_wtime();
    cout << "DFS Time: " << end - start << " sec\n";

    return 0;
}
