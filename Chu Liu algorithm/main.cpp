#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <vector>

using namespace std;

struct Edge {
    int begin;
    int end;
    int weight;
};


bool used[1000];
vector<int> component;
vector<int> order;
vector <short int> gt[1000];

void dfs_g (int v, const vector<Edge> &e) {
    used[v] = 1;
    for (int i = 0; i < e.size(); ++i)
        if (!used[e[i].end] && e[i].begin == v)
            dfs_g(e[i].end, e);

    order.push_back(v);
}

void dfs_gt (int v) {
    used[v] = 1;
    component.push_back (v);

    for (size_t i = 0; i < gt[v].size(); ++i)
        if (!used[gt[v][i]])
            dfs_gt (gt[v][i]);
}

/*
    Finding condensation graph
*/
void cond (int* nc, const vector<Edge> &e, int &component_count, int n) {

    for (int i = 0; i < 1000; ++i)
        gt[i].clear();

    for (int i = 0; i < e.size(); ++i)
        gt[e[i].end].push_back (e[i].begin);

    memset(used, 0, sizeof(used));
    order.clear();

    for (int i = 0; i < n; ++i)
        if (!used[i])
            dfs_g (i, e);

    memset(used, 0, sizeof(used));
    for (int i = 0; i < n; ++i) {
        int v = order[n - 1 - i];
        if (!used[v]) {
            dfs_gt (v);

            for (int i = 0; i < component.size(); ++i)
                nc[component[i]] = component_count;

            component.clear();
            ++component_count;
        }
    }
}

/*
    Check on the reach of all the vertices of a given vertex
*/
void dfs (int root, const vector<Edge> &zero_edges, int &count) {
    used[root] = 1;
    count++;
    for (int i = 0; i < zero_edges.size(); ++i)
        if (!used[zero_edges[i].end] && zero_edges[i].begin == root)
            dfs(zero_edges[i].end, zero_edges, count);
}

long long findMST(vector<Edge> &edges, int n, int root) {
    long long res = 0;

    int *min_edge = (int*) malloc(n * sizeof(int));
    for (size_t i = 0; i < n; i++)
      min_edge[i] = INT_MAX;

    for (int i = 0; i < edges.size(); ++i)
        min_edge[edges[i].end] = min(edges[i].weight, min_edge[edges[i].end]);

    for (int i = 0; i < n; ++i)
        res += (long long) (min_edge[i]);

    res -= (long long) (min_edge[root]);

    vector<Edge> zero_edges;
    for (int i = 0; i < edges.size(); ++i)
        if (edges[i].weight == min_edge[edges[i].end]) {
            Edge e = edges[i];
            e.weight -= min_edge[edges[i].end];
            zero_edges.push_back(e);
        }

    int count = 0;
    memset (used, 0, sizeof(used));
    dfs (root, zero_edges, count);

    if (count == n)
        return res;

    int components_count = 0;
    int *new_components = (int*) malloc(n * sizeof(int));
    cond(new_components, zero_edges, components_count, n);

    vector<Edge> new_edges;

    for (int i = 0; i < edges.size(); ++i) {
        if (new_components[edges[i].begin] != new_components[edges[i].end]) {
            Edge e;
            e.begin = new_components[edges[i].begin];
            e.end = new_components[edges[i].end];
            e.weight = edges[i].weight - min_edge[edges[i].end];
            new_edges.push_back(e);
        }
    }

    edges = new_edges;
    root = new_components[root];

    free (new_components);
    free (min_edge);

    res += findMST(edges, components_count, root);

    return res;
}

int main() {
    // freopen("chinese.in", "r", stdin);
    // freopen("chinese.out", "w", stdout);

    int n; //Count of vertices
    int m; //Count of edges

    scanf("%d%d", &n, &m);
    vector <Edge> edge (m);
    // Edge *edge = (Edge*) malloc((m + 1) * sizeof(Edge));

    for (int i = 0; i < m; ++i) {
        scanf("%d%d%d", &edge[i].begin, &edge[i].end, &edge[i].weight);
        edge[i].begin--;
        edge[i].end--;
    }

    int count = 0;
    dfs(0, edge, count);
    if (count != n) {
        printf("NO\n");
        return 0;
    }

    printf("YES\n%lld\n", findMST(edge, n, 0));

    return 0;
}
