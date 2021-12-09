#include <iostream>
#include <bits/stdc++.h>

#define inf 100000000
#define maxFlow  110000

using namespace std;

class graph
{
    // bitul 1 orientat sau neorientat, bitul 2 cost muchie
    enum input {orientat = 1, neorientat = 2, cu_cost = 4, fillLat = 8};
    struct muchie
    {
        int from, to, cost;
        muchie(int _from, int _to, int _cost = 1):
            from(_from), to(_to), cost(_cost)
            {

            }
    };
    input tipGraf;
    int n,m;
    int* extraArgs = nullptr;

    vector<int>* la = nullptr;
    vector<int>* lat = nullptr;
    vector<pair<int,int>>* lac = nullptr;

    vector<int> topo;

    ifstream f;
    ofstream g;
public:
    void clearData();
    void readData(string nume_fisier, input _tip_graf, int _extra_args_count = 0);
    void addEdge(int from, int to, int cost = -1);
    //////////////////////////////////////////////////////////////////////////////////
    void bfs();
    //////////////////////////////////////////////////////////////////////////////////
    void dfs();
    void dfsTopo(int from, int dist[]);
    //////////////////////////////////////////////////////////////////////////////////
    void ctc();
    void dfsCtc(int nod, vector<vector<int>>& cc, int data[]);
    //////////////////////////////////////////////////////////////////////////////////
    void sortaret();
    //////////////////////////////////////////////////////////////////////////////////
    void critice();
    void dfsCritice(int from, int parent, vector<pair<int,int>>& mcrit, int lowLink[], int ids[], int id);
    //////////////////////////////////////////////////////////////////////////////////
    void biconex();
    void dfsBiconexe(int from, stack<pair<int,int>>& st, vector<vector<int>>& cb, int lowlink[], int ids[], int id);
    //////////////////////////////////////////////////////////////////////////////////
    int findParent(int x, int parents[]);
    void unite(int x, int y, int parents[], int sizes[]);
    void apm();
    pair<int,int> getParentAndHeight(int x, int parents[]);
    //////////////////////////////////////////////////////////////////////////////////
    void disjoint();
    //////////////////////////////////////////////////////////////////////////////////
    void dijkstra();
    //////////////////////////////////////////////////////////////////////////////////
    void bellmanford();
    //////////////////////////////////////////////////////////////////////////////////
    void royfloyd();
    //////////////////////////////////////////////////////////////////////////////////
    void darb();
    int bfs_farthest();
    int dist_to_node(int node);
    //////////////////////////////////////////////////////////////////////////////////
    void maxflow();
    void getPath(int parent[], int** fluxLeft);
};

void graph::clearData()
{
    if (la != nullptr)
    {
        delete[] la;
        la = nullptr;
    }
    if (lat != nullptr)
    {
        delete[] lat;
        lat = nullptr;
    }
    if (lac != nullptr)
    {
        delete[] lac;
        lac = nullptr;
    }
    if (extraArgs != nullptr)
    {
        delete[] extraArgs;
        extraArgs = nullptr;
    }
}

void graph::readData (string nume_fisier, input _tip_graf, int _extra_args_count)
{
    tipGraf = _tip_graf;

    f.open(nume_fisier + ".in");
    g.open(nume_fisier + ".out");

    f >> n >> m;

    if (_extra_args_count)
    {
        extraArgs = new int[_extra_args_count];
        for (int i = 0; i < _extra_args_count; i++)
            f >> extraArgs[i];
    }
    int x,y,c;

    if (_tip_graf & input::orientat)
    {
        if (_tip_graf & input::cu_cost)
        {
            lac = new vector<pair<int,int>>[n+1];
            for (int i = 1; i <= m; i++)
            {
                f >> x >> y >> c;
                lac[x].push_back({y,c});
            }
        }
        else
        {
            if (_tip_graf & input::fillLat)
            {
                la = new vector<int>[n+1];
                lat = new vector<int>[n+1];
                for (int i = 1; i <= m; i++)
                {
                    f >> x >> y;
                    // cout << x << ' ' << y << '\n';
                    la[x].push_back(y);
                    lat[y].push_back(x);
                }
            }
            else
            {
                la = new vector<int>[n+1];
                for (int i = 1; i <= m; i++)
                {
                    f >> x >> y;
                    la[x].push_back(y);
                }
            }
        }
    }
    else if (_tip_graf & input::neorientat)
    {
        if (_tip_graf & input::cu_cost)
        {
            lac = new vector<pair<int,int>>[n+1];
            for (int i = 1; i <= m; i++)
            {
                f >> x >> y >> c;
                lac[x].push_back({y,c});
                lac[y].push_back({x,c});
            }
        }
        else
        {
            la = new vector<int>[n+1];
            for (int i = 1; i <= m; i++)
            {
                f >> x >> y;
                la[x].push_back(y);
                la[y].push_back(x);
            }
        }
    }

    f.close();
}

void graph::addEdge(int from, int to, int cost)
{
    if (tipGraf & input::orientat)
    {
        if (tipGraf & input::cu_cost)
        {
            lac[from].push_back({to, cost});
        }
        else
        {
            if (tipGraf & input::fillLat)
            {
                la[from].push_back(to);
                lat[to].push_back(from);
            }
            else
            {
                la[from].push_back(to);
            }
        }
    }
    else if (tipGraf & input::neorientat)
    {
        if (tipGraf & input::cu_cost)
        {
            lac[from].push_back({to,cost});
            lac[to].push_back({from, cost});
        }
        else
        {
            la[from].push_back(to);
            la[to].push_back(from);
        }
    }
}

void graph::bfs()
{
    // readData("bfs", input::orientat, 1);

    int start = extraArgs[0];
    int* dist = new int[n+1];

    queue<int> q;
    q.push(start);
    dist[start] = 0;

    while (q.size())
    {
        for (auto to: la[q.front()])
        {
            if (dist[to] == -1)
            {
                q.push(to);
                dist[to] = dist[q.front()] + 1;
            }
        }
        q.pop();
    }

    for (int i = 1; i <= n; i++)
        g << dist[i] << ' ';

    delete[] dist;
    clearData();
}

void graph::dfsTopo(int from, int dist[])
{
    dist[from] = 1;
    for (auto to: la[from])
    {
        if (!dist[to])
            dfsTopo(to, dist);
    }
    topo.push_back(from);
}

void graph::dfs()
{
    // readData("dfs", input::neorientat);

    int* dist = new int[n+1];

    int cc = 0;
    for (int i = 1; i <= n; i++)
    {
        if (!dist[i])
        {
            dfsTopo(i, dist);
            cc++;
        }
    }

    g << cc;

    delete[] dist;
    clearData();
}

void graph::dfsCtc(int nod, vector<vector<int>>& cc, int dist[])
{
    dist[nod] = 1;
    cc[cc.size() - 1].push_back(nod);
    for (auto to: lat[nod])
    {
        if (!dist[to])
            dfsCtc(to, cc, dist);
    }
}

void graph::ctc()
{
    // readData("ctc", (input)(input::orientat | input::fillLat));

    int* dist = new int[n+1];

    for (int i = 1; i <= n; i++)
    {
        if (!dist[i])
            dfsTopo(i, dist);
    }
    reverse(topo.begin(), topo.end());

    memset(dist, 0, sizeof(int) * (n+1));
    vector<vector<int>> cc;
    for (auto nod: topo)
    {
        if (!dist[nod])
        {
            cc.push_back(vector<int>());
            dfsCtc(nod, cc, dist);
        }
    }

    g << cc.size() << '\n';
    for (auto& row: cc)
    {
        for (auto el: row)
            g << el << ' ';
        g << '\n';
    }

    delete[] dist;
    clearData();
}

void graph::sortaret()
{
    // readData("sortaret", (input)(input::orientat));

    int* dist = new int[n+1];

    for (int i = 1; i <= n; i++)
    {
        if (!dist[i])
            dfsTopo(i, dist);
    }
    reverse(topo.begin(), topo.end());

    for (auto el: topo)
        g << el << ' ';

    delete[] dist;
    clearData();
}

void graph::dfsCritice(int from, int parent, vector<pair<int,int>>& mcrit, int lowLink[], int ids[], int id)
{
    lowLink[from] = id;
    ids[from] = id;
    id ++;

    for (auto to: la[from])
    {
        if (ids[to] == -1)
        {
            dfsCritice(to, from, mcrit, lowLink, ids, id);
            lowLink[from] = min(lowLink[from], lowLink[to]);
        }
        else if (to != parent)
            lowLink[from] = min(lowLink[from], ids[to]);
    }
    if (lowLink[from] == ids[from] && parent != 0)
        mcrit.push_back({from, parent});
}

void graph::critice()
{
    // readData("critice", (input)(input::neorientat));

    int* ids = new int[n+1];
    for (int i = 1; i <= n; i++)
        ids[i] = -1;
    int* lowLink = new int[n+1];
    for (int i = 1; i <= n; i++)
        lowLink[i] = 0;

    vector<pair<int, int>> mcrit;
    for (int i = 1; i <= n; i++)
    {
        if (ids[i] == -1)
        {
            dfsCritice(i, 0, mcrit, lowLink, ids, 0);
        }
    }

    for (auto& per: mcrit)
        g << per.first << ' ' << per.second << '\n';

    delete[] ids;
    delete[] lowLink;
}

void graph::dfsBiconexe(int from, stack<pair<int,int>>& st, vector<vector<int>>& cb, int lowLink[], int ids[], int id)
{
    lowLink[from] = ids[from] = id++;

    for (auto to: la[from])
    {
        if (ids[to] != -1)
            lowLink[from] = min(lowLink[from], ids[to]);
        else
        {
            st.push({from, to});
            dfsBiconexe(to, st, cb, lowLink, ids, id);

            lowLink[from] = min(lowLink[from], lowLink[to]);

            if (lowLink[to] >= ids[from])
            {
                vector<int> comp;
                while (st.top().first != from)
                {
                    comp.push_back(st.top().second);
                    st.pop();
                }
                comp.push_back(st.top().first);
                comp.push_back(st.top().second);
                st.pop();
                cb.push_back(comp);
            }
        }
    }
}

void graph::biconex()
{
    // readData("biconex", (input)(input::neorientat));

    int* ids = new int[n+1];
    for (int i = 1; i <= n; i++)
        ids[i] = -1;
    int* lowLink = new int[n+1];
    for (int i = 1; i <= n; i++)
        lowLink[i] = 0;

    vector<vector<int>> cb;
    stack<pair<int,int>> st;
    for (int i = 1; i <= n; i++)
    {
        if (ids[i] == -1)
            dfsBiconexe(i, st, cb, lowLink, ids, 0);
    }

    g << cb.size() << '\n';
    for (auto& row: cb)
    {
        for (auto el: row)
            g << el << ' ';
        g << '\n';
    }

    delete[] ids;
    delete[] lowLink;
}

pair<int,int> graph::getParentAndHeight(int x, int parents[])
{
    vector<int> met;
    int h = 0;
    while (parents[x] != 0)
    {
        h++;
        met.push_back(x);
        x = parents[x];
    }
    for (auto el: met)
        parents[el] = x;
    return {x, h};
}

int graph::findParent(int x, int parents[])
{
    vector<int> met;
    while (parents[x] != 0)
    {
        met.push_back(x);
        x = parents[x];
    }
    for (int nod: met)
        parents[nod] = x;
    return x;
}

void graph::unite(int x, int y, int parents[], int sizes[])
{
    int x_parent = findParent(x, parents);
    int y_parent = findParent(y, parents);
    if (x_parent == y_parent)
        return;
    if (sizes[x_parent] > sizes[y_parent])
    {
        parents[y_parent] = x_parent;
        sizes[x_parent] += sizes[y_parent];
    }
    else
    {
        parents[x_parent] = y_parent;
        sizes[y_parent] += sizes[x_parent];
    }
}

void graph::apm()
{
    // readData("apm", (input)(input::orientat | input::cu_cost));

    int* parents = new int[n+1];
    for (int i = 1; i <= n; i++)
        parents[i] = 0;
    int* sizes = new int[n+1];
    for (int i = 1; i <= n; i++)
        sizes[i] = 1;

    vector<muchie> muchii;
    for (int i = 1; i <= n; i++)
    {
        for (auto per: lac[i])
            muchii.push_back({i, per.first, per.second});
    }

    sort(muchii.begin(), muchii.end(), [](muchie& m1, muchie& m2){ return (m1.cost < m2.cost);});

    vector<muchie> sol;
    int s = 0;
    for (auto& m: muchii)
    {
        int p1 = findParent(m.from, parents);
        int p2 = findParent(m.to, parents);

        if (p1 == p2)
            continue;

        unite(m.from, m.to, parents, sizes);
        sol.push_back(m);
        s += m.cost;
    }

    g << s << '\n';
    g << sol.size() << '\n';
    for (auto& m: sol)
        g << m.from << ' ' << m.to << '\n';

    delete[] parents;
    delete[] sizes;
}

void graph::disjoint()
{
    ifstream f("disjoint.in");
    ofstream g("disjoint.out");
    f >> n >> m;

    int* parents = new int[n+1];
    for (int i = 1; i <= n; i++)
        parents[i] = 0;
    int* sizes = new int[n+1];
    for (int i = 1; i <= n; i++)
        sizes[i] = 1;

    for (int i = 1; i <= m; i++)
    {
        int op, from, to;
        f >> op >> from >> to;
        int parinte_from = findParent(from, parents);
        int parinte_to = findParent(to, parents);
        if (op == 1)
        {
            unite(from, to, parents, sizes);
        }
        else
        {
            if (parinte_from == parinte_to)
                g << "DA\n";
            else
                g << "NU\n";
        }
    }

    delete[] parents;
    delete[] sizes;
}

void graph::dijkstra()
{
    int max_dist = 1e9;
    // readData("dijkstra", (input)(input::orientat | input::cu_cost));

    int* dist = new int[n+1];
    for (int i = 1; i <= n; i++)
        dist[i] = max_dist;
    int* viz = new int[n+1];
    for (int i = 1; i <= n; i++)
        viz[i] = 0;

    priority_queue<pair<int, int>> q; // cost negativ si nod
    q.push({0, 1});
    dist[1] = 0;

    while (q.size())
    {
        int top = q.top().second;
        q.pop();

        if (viz[top])
            continue;
        viz[top] = 1;

        for (auto& per: lac[top])
        {
            if (dist[per.first] > dist[top] + per.second)
            {
                dist[per.first] = dist[top] + per.second;
                q.push({-dist[per.first], per.first});
            }
        }
    }

    for (int i = 2; i <= n; i++)
    {
        if (dist[i] == max_dist)
        {
            dist[i] = 0;
        }
        g << dist[i] << ' ';
    }

    delete[] dist;
}

void graph::bellmanford()
{
    int max_dist = 1e9;
    // readData("bellmanford", (input)(input::orientat | input::cu_cost));

    int* dist = new int[n+1];
    for (int i = 1; i <= n; i++)
        dist[i] = max_dist;
    int* viz = new int[n+1];
    for (int i = 1; i <= n; i++)
        viz[i] = 0;

    queue<int> q; // cost negativ si nod
    q.push(1);
    dist[1] = 0;

    while (q.size())
    {
        int from = q.front();
        q.pop();

        viz[from] ++;
        if (viz[from] >= n)
        {
            g << "Ciclu negativ!";
            return;
        }

        for (auto& per: lac[from])
        {
            if (dist[per.first] > dist[from] + per.second)
            {
                dist[per.first] = dist[from] + per.second;
                q.push(per.first);
            }
        }
    }

    for (int i = 2; i <= n; i++)
    {
        if (dist[i] == max_dist)
        {
            dist[i] = 0;
        }
        g << dist[i] << ' ';
    }

    delete[] dist;
}

void graph::royfloyd()
{
    ifstream in("royfloyd.in");
    ofstream out("royfloyd.out");
    in >> n;
    int minDist[100][100];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            in >> minDist[i][j];
            if (minDist[i][j] == 0 && i != j)
                minDist[i][j] = inf;
        }

    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                //minDist[i][j] = min(min(minDist[i][j], minDist[i][k] + dist[k][j]), dist[i][k] + minDist[k][j]);
                if (minDist[i][k] + minDist[k][j] < minDist[i][j])
                    minDist[i][j] = minDist[i][k] + minDist[k][j];

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (minDist[i][j] == inf)
            {
                minDist[i][j] = 0;
            }
            out << minDist[i][j] << ' ';
        }
        out << '\n';
    }
}

int graph::bfs_farthest()
{
    int* viz = new int[n+1];
    queue<int> q;
    q.push(1);
    viz[1] = 1;

    while (q.size())
    {
        int from = q.front();
        for (auto& to: la[from])
        {
            if (!viz[to])
            {
                viz[to] = 1;
                q.push(to);
            }
        }

        if (q.size() == 1)
        {
            delete[] viz;
            return q.front();
        }
        q.pop();
    }
    delete[] viz;
    return -1;
}

int graph::dist_to_node(int node)
{
    int* dist = new int[n+1];
    memset(dist, -1, sizeof(int) * (n+1));
    queue<int> q;
    q.push(node);
    dist[node] = 0;

    while (q.size())
    {
        int from = q.front();
        for (auto& to: la[from])
        {
            if (dist[to] == -1)
            {
                dist[to] = 1 + dist[from];
                q.push(to);
            }
        }

        if (q.size() == 1)
        {
            int d = dist[q.front()];
            delete[] dist;
            return d;
        }
        q.pop();
    }
    delete[] dist;
    return -1;
}

void graph::darb()
{
    ifstream in("darb.in");
    ofstream out("darb.out");
    in >> n;
    la = new vector<int>[n+1];

    int from, to;
    while (in >> from >> to)
    {
        la[from].push_back(to);
        la[to].push_back(from);
    }

    int farthest = bfs_farthest();
    out << dist_to_node(farthest) + 1;
}

void graph::getPath(int parent[], int** fluxLeft)
{
    memset(parent, 0, sizeof(int) * (n+1));
    vector<int> q;
    parent[1] = -1;
    q.push_back(1);

    for (int i = 0; i < q.size() ; i++)
    {
        if (q[i] == n)
        {
            continue;
        }
        for (auto to: la[q[i]])
        {
            if (fluxLeft[q[i]][to] <= 0 || parent[to] != 0)
                continue;

            parent[to] = q[i];
            q.push_back(to);
        }
    }
}

void graph::maxflow()
{
    ifstream in("maxflow.in");
    ofstream out("maxflow.out");

    in >> n >> m;
    int s = 1, f = n;
    la = new vector<int>[n+1];
    int* parent = new int[n+1];
    int flux = 0;
    int** fluxLeft = new int*[n+1];
    for (int i = 0; i < n+1; i++)
    {
        fluxLeft[i] = new int[n+1];
        for (int j = 0; j < n+1; j++)
            fluxLeft[i][j] = 0;
    }

    for (int i = 1; i <= m ; ++i)
    {
        int x,y,c;
        in >> x >> y >> c;
        la[x].push_back(y);
        la[y].push_back(x);

        fluxLeft[x][y] = c;
    }

    getPath(parent, fluxLeft);
    while (parent[f] != 0)
    {
        for (int i = -1; i < (int)la[f].size(); i++)
        {
            if (i != -1)
            {
                parent[f] = la[f][i];
            }

            int minn = maxFlow;
            int nod = f;
            while (parent[nod] != -1)
            {
                if (fluxLeft[parent[nod]][nod] < minn)
                    minn = fluxLeft[parent[nod]][nod];
                if (minn <= 0)
                    break;
                nod = parent[nod];
            }

            if (minn <= 0)
                continue;

            flux += minn;
            nod = f;
            while (parent[nod] != -1)
            {
                fluxLeft[parent[nod]][nod] -= minn;
                fluxLeft[nod][parent[nod]] += minn;
                nod = parent[nod];
            }
        }

        getPath(parent, fluxLeft);
    }

    out << flux;
}

int main()
{
    graph g;

    g.maxflow();

    return 0;
}
