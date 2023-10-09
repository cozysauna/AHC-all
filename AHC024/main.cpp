#include <bits/stdc++.h>

using namespace std; 

bool debug = false;
const int N = 50;
const int M = 100;
const int DY[4] = {1, -1, 0, 0};
const int DX[4] = {0, 0, 1, -1};
vector<vector<int>> C(N, vector<int>(N));
vector<vector<int>> origin_adjant_mat(M + 1);

inline double get_time() {
    return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

double start_time = get_time();
int iter_cnt = 0;

unsigned int randxor()
{
    static unsigned int x = 123456789, y = 362436069, z = 521288629, w = 88675123;
    unsigned int t;
    t = (x ^ (x << 11));
    x = y; y = z; z = w; 
    return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}

// 色ごとの隣接関係を行列で表す
vector<vector<int>> get_adjant_mat(vector<vector<int>> C) {
    vector<set<int>> neighbor(M + 1);
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N - 1; j++) {
            int a = C[i][j], b = C[i][j + 1];
            if(a != b) {
                neighbor[a].insert(b);
                neighbor[b].insert(a);
            }
        }
    }
    for(int i = 0; i < N - 1; i++) {
        for(int j = 0; j < N; j++) {
            int a = C[i][j], b = C[i + 1][j];
            if(a != b) {
                neighbor[a].insert(b);
                neighbor[b].insert(a);
            }
        }
    }

    for(int i: {0, N - 1}) {
        for(int j = 0; j < N; j++) {
            int a = C[i][j], b = 0;
            if(a != b) {
                neighbor[a].insert(b);
                neighbor[b].insert(a);
            }
        }
    }

    for(int i = 0; i < N; i++) {
        for(int j: {0, N - 1}) {
            int a = C[i][j], b = 0;
            if(a != b) {
                neighbor[a].insert(b);
                neighbor[b].insert(a);
            }
        }
    }

    vector<vector<int>> ret_neighbor(M + 1);
    for(int i = 0; i < M + 1; i++) {
        for(auto it = neighbor[i].begin(); it != neighbor[i].end(); it++) {
            ret_neighbor[i].push_back(*it);
        }
    }


    return ret_neighbor;
}

bool is_valid_C(vector<vector<int>> C) {
    vector<vector<int>> adjant_mat = get_adjant_mat(C);
    for(int i = 0; i < M + 1; i++) {
        if(adjant_mat[i].size() != origin_adjant_mat[i].size()) {
            return false;
        }

        for(int j = 0; j < adjant_mat[i].size(); j++) {
            if(adjant_mat[i][j] != origin_adjant_mat[i][j]) {
                return false;
            }
        }
    }

    vector<vector<bool>> done(N, vector<bool>(N));
    vector<bool> done_color(M + 1);
    for(int y = 0; y < N; y++) {
        for(int x = 0; x < N; x++) {
            if(done[y][x]) continue;
            done[y][x] = true;
            int c = C[y][x];
            if(done_color[c]) return false;
            done_color[c] = true;
            queue<pair<int, int>> que;
            que.push(make_pair(y, x));
            bool getout = false;
            while(!que.empty()) {
                int now_y = que.front().first;
                int now_x = que.front().second;
                que.pop();
                for(int k = 0; k < 4; k++) {
                    int nx_y = now_y + DY[k];
                    int nx_x = now_x + DX[k];
                    if(0 <= nx_y && nx_y < N && 0 <= nx_x && nx_x < N){
                        if(!done[nx_y][nx_x] && C[nx_y][nx_x] == c) {
                            que.push(make_pair(nx_y, nx_x));
                            done[nx_y][nx_x] = true;
                        }
                    } else {
                        getout = true;
                    }
                }
            }
            if(c == 0 && !getout) {
                return false;
            }
        }
    }

    return true;
}

vector<vector<int>> row_delete(vector<vector<int>> C, int r) {
    vector<vector<int>> new_C;
    for(int i = 0; i < N; i++) if(i != r) {
        new_C.push_back(C[i]);
    }
    new_C.push_back(vector<int>(N, 0));
    return new_C;
}

vector<vector<int>> col_delete(vector<vector<int>> C, int c) {
    vector<vector<int>> new_C;
    for(int i = 0; i < N; i++) {
        vector<int> add_C;
        for(int j = 0; j < N; j++) if(j != c) {
            add_C.push_back(C[i][j]);
        }
        add_C.push_back(0);
        new_C.push_back(add_C);
    }
    return new_C;
}

vector<vector<int>> change_color(vector<vector<int>> C) {
    for(int i = 0; i < 1000; i++) {
        int y = randxor() % N;
        int x = randxor() % N;
        if(C[y][x] != 0) {
            int d = randxor() % 4;
            int nx_y = y + DY[d];
            int nx_x = x + DX[d];

            int pre_color = C[y][x];
            if(
                0 <= nx_y && 
                0 <= nx_x && 
                nx_y < N &&
                nx_x < N) {
                C[y][x] = C[nx_y][nx_x];
            } else {
                C[y][x] = 0;
            }

            if(C[y][x] == 0 || !is_valid_C(C)) {
                C[y][x] = pre_color;
            }
        }
    }
    return C;
}

void init() {
    // input
    int n, m; cin >> n >> m;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            cin >> C[i][j];
        }
    }
    // 前処理
    origin_adjant_mat = get_adjant_mat(C);
}

int get_score(vector<vector<int>> C) {
    int score = 1;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            if(C[i][j] == 0) {
                score++;
            }
        }
    }
    return score;
}

void solve() {

    // 行と列の削除

    int row_remove_cnt = 0, col_remove_cnt = 0;
    while(get_time() - start_time < 1900) {
        iter_cnt++;
        bool updated = false;
        for(int k = 0; k < N; k++) {
            // 行の削除
            if(k + row_remove_cnt < N) {
                vector<vector<int>> new_C = row_delete(C, k);
                if(is_valid_C(new_C)) {
                    C = new_C;
                    updated = true;
                    row_remove_cnt++;
                }
            }

            // 列の削除
            if(k + col_remove_cnt < N) {
                vector<vector<int>> new_C = col_delete(C, k);
                if(is_valid_C(new_C)) {
                    C = new_C;
                    updated = true;
                    col_remove_cnt++;
                }
            }
        }        

        if(!updated) {
            C = change_color(C);
        }
    }
}

void output() {

    if(debug) {
        ofstream writing_file;
        string file_name = "out";
        writing_file.open(file_name, ios::out);
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                writing_file << C[i][j];
                if(j + 1 != N) {
                    writing_file << " ";
                }
            }
            writing_file << endl;
        }
        writing_file.close();

        cout << "SCORE : " << get_score(C) << endl;
        cout << "iter_cnt : "  << iter_cnt << endl;

    } else {
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                cout << C[i][j];
                if(j + 1 != N) {
                    cout << " ";
                }
            }
            cout << endl;
        }
    }
}

int main() {
    init();
    solve();
    output();
    return 0;
}