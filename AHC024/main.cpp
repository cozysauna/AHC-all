#include <bits/stdc++.h>

using namespace std; 

bool debug = false;
const int N = 50;
const int M = 100;
const int DY[4] = {1, -1, 0, 0};
const int DX[4] = {0, 0, 1, -1};
vector<vector<int>> C(N, vector<int>(N));
vector<vector<bool>> origin_adjant_mat(M + 1, vector<bool>(M + 1));

inline double get_time() {
    return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

double start_time = get_time();

unsigned int randxor()
{
    static unsigned int x = 123456789, y = 362436069, z = 521288629, w = 88675123;
    unsigned int t;
    t = (x ^ (x << 11));
    x = y; y = z; z = w; 
    return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}

bool inside(int y, int x) {
    return 0 <= y && y < N && 0 <= x && x < N;
}

// 色ごとの隣接関係を行列で表す
vector<vector<bool>> get_adjant_mat(vector<vector<int>> C) {
    vector<vector<bool>> adjant_mat(M + 1, vector<bool>(M + 1));
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N - 1; j++) {
            int a = C[i][j], b = C[i][j + 1];
            if(a != b) {
                adjant_mat[a][b] = true;
                adjant_mat[b][a] = true;
            }
        }
    }
    for(int i = 0; i < N - 1; i++) {
        for(int j = 0; j < N; j++) {
            int a = C[i][j], b = C[i + 1][j];
            if(a != b) {
                adjant_mat[a][b] = true;
                adjant_mat[b][a] = true;
            }
        }
    }

    for(int i: {0, N - 1}) {
        for(int j = 0; j < N; j++) {
            int a = C[i][j], b = 0;
            if(a != b) {
                adjant_mat[a][b] = true;
                adjant_mat[b][a] = true;
            }
        }
    }

    for(int i = 0; i < N; i++) {
        for(int j: {0, N - 1}) {
            int a = C[i][j], b = 0;
            if(a != b) {
                adjant_mat[a][b] = true;
                adjant_mat[b][a] = true;
            }
        }
    }

    return adjant_mat;
}

bool is_valid_C(vector<vector<int>> C) {
    vector<vector<bool>> adjant_mat = get_adjant_mat(C);
    for(int i = 0; i < M + 1; i++) {
        for(int j = 0; j < M + 1; j++) {
            if(origin_adjant_mat[i][j] != adjant_mat[i][j]) {
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
            if(c != 0 && done_color[c]) return false;
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
                    if(inside(nx_y, nx_x)) {
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

bool row_delete(vector<vector<int>>& C, int r) {
    vector<vector<int>> new_C;
    for(int i = 0; i < N; i++) if(i != r) {
        new_C.push_back(C[i]);
    }
    new_C.push_back(vector<int>(N, 0));

    if(is_valid_C(new_C)) {
        C = new_C;
        return true;
    } else {
        return false;
    }
}

bool col_delete(vector<vector<int>>& C, int c) {
    vector<vector<int>> new_C;
    for(int i = 0; i < N; i++) {
        vector<int> add_C;
        for(int j = 0; j < N; j++) if(j != c) {
            add_C.push_back(C[i][j]);
        }
        add_C.push_back(0);
        new_C.push_back(add_C);
    }

    if(is_valid_C(new_C)) {
        C = new_C;
        return true;
    } else {
        return false;
    }

}

vector<tuple<int, int, int>> point_delete(vector<vector<int>>& C, int y, int x, int length, int d) {
    vector<tuple<int, int, int>> changes;
    if(C[y][x] == 0) {
        return changes;
    }
    // どの方向に詰めるか
    int dy = DY[d], dx = DX[d];

    bool not_break_flag = true;
    while(inside(y + dy, x + dx)) {
        changes.push_back(make_tuple(y, x, C[y][x]));
        C[y][x] = C[y + dy][x + dx];
        if(C[y][x] == 0) {
            not_break_flag = false;   
            break;
        }
        y += dy;
        x += dx;
    }

    if(not_break_flag) {
        changes.push_back(make_tuple(y, x, C[y][x]));
        C[y][x] = 0;
    }

    return changes;
}

void change_color(vector<vector<int>>& C) {
    for(int i = 0; i < 1000; i++) {
        int y = randxor() % N;
        int x = randxor() % N;
        if(C[y][x] != 0) {
        // if(true) {
            int d = randxor() % 4;
            int nx_y = y + DY[d];
            int nx_x = x + DX[d];

            int pre_color = C[y][x];
            if(
                0 <= nx_y && 
                0 <= nx_x && 
                nx_y < N &&
                nx_x < N) 
            {
                if(C[y][x] == C[nx_y][nx_x]) continue;
                C[y][x] = C[nx_y][nx_x];
            } else {
                C[y][x] = 0;
            }

            // if(C[y][x] == 0 || !is_valid_C(C)) {
            if(!is_valid_C(C)) {
                C[y][x] = pre_color;
            } else {
                // 一点更新完了
                break;
            }
        }
    }
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

bool can_remove_row(vector<vector<int>> C, int i) {
    for(int j = 0; j < N; j++) if(C[i][j] != 0) {
        return true;
    }
    return false;
}

bool can_remove_col(vector<vector<int>> C, int j) {
    for(int i = 0; i < N; i++) if(C[i][j] != 0) {
        return true;
    }
    return false;
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

bool is_change(int max_score, int tmp_score) {
    return max_score <= tmp_score;
}

vector<vector<int>> modified_C(vector<vector<int>> C) {
    vector<int> top(M + 1, M + 1);
    vector<int> bot(M + 1, 0);
    vector<int> lef(M + 1, M + 1);
    vector<int> rig(M + 1, 0);
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            int c = C[i][j];
            top[c] = min(top[c], i);
            bot[c] = max(bot[c], i);
            lef[c] = min(lef[c], j);
            rig[c] = max(rig[c], j);
        }
    }
    for(int c = 1; c < M + 1; c++) {
        vector<vector<int>> new_C(N, vector<int>(N));
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                new_C[i][j] = C[i][j];
            }
        }
        for(int i = top[c]; i <= bot[c]; i++) {
            for(int j = lef[c]; j <= rig[c]; j++) {
                new_C[i][j] = c;
            }
        }
        if(is_valid_C(new_C)) {
            C = new_C;
        }
    }
    return C;
}

void solve() {

    // Cの線が直線になるように変化させる
    C = modified_C(C);

    // 行と列の削除
    int remove_row_cnt = 0, remove_col_cnt = 0;
    while(true) {
        bool updated = false;
        for(int k = 0; k < N; k++) {
            // 行の削除
            if(can_remove_row(C, k)) {
                bool updated_row = row_delete(C, k);
                if(updated_row) {
                    updated = true;
                    remove_row_cnt++;
                }
            }

            // 列の削除
            if(can_remove_col(C, k)) {
                bool updated_col = col_delete(C, k);
                if(updated_col) {
                    updated = true;
                    remove_col_cnt++;
                }
            }
        }        

        if(!updated) {
            break;
        }
    }

    // 山登り
    int iter_cnt = 0;
    int cutoff_max_length = 15;
    int max_iter_cnt = 65500;
    // int max_iter_cnt = 205500;
    int max_score = get_score(C);
    while(get_time() - start_time < 1900) {
    // for(int iter = 0; iter < max_iter_cnt; iter++) {
        iter_cnt++;
        // if(iter_cnt % 1000 == 0) {
        //     cout << "iter_cnt: " << iter_cnt << " SCORE: " << get_score(C) << endl;
        // }

        int m = rand() % 3;
        int length = rand() % cutoff_max_length + 1;
        int y = rand() % (N - remove_row_cnt);
        int x = rand() % (N - remove_col_cnt);
        if(C[y][x] == 0) {
            continue;
        }

        if(m == 0) {
            // 行の一部削除（詰める方向はd = 0, 1)
            vector<tuple<int, int, int>> all_changes;

            if(can_remove_row(C, y)) {
                int d = rand() % 2;
                for(int j = x; j < min(N, x + length); j++) {
                    vector<tuple<int, int, int>> changes = point_delete(C, y, j, length, d);
                    for(auto change: changes) {
                        all_changes.push_back(change);
                    }
                }
            }

            // int tmp_score = get_score(C);
            // if(!is_valid_C(C) || !is_change(max_score, tmp_score)) {
            if(!is_valid_C(C)) {
                // rollback
                for(auto change: all_changes) {
                    int y = get<0>(change);
                    int x = get<1>(change);
                    int e = get<2>(change);
                    C[y][x] = e;
                }
            } 
            // else {
            //     max_score = tmp_score;
            // }


        } else if(m == 1) {
            // 列の削除(詰める報告 d = 2, 3)
            vector<tuple<int, int, int>> all_changes;
            if(can_remove_col(C, x)) {
                int d = rand() % 2 + 2;
                for(int i = y; i < min(N, y + length); i++) {
                    vector<tuple<int, int, int>> changes = point_delete(C, i, x, length, d);
                    for(auto change: changes) {
                        all_changes.push_back(change);
                    }
                }
            }

            // int tmp_score = get_score(C);
            // if(!is_valid_C(C) || !is_change(max_score, tmp_score)) {
            if(!is_valid_C(C)) {
                // rollback
                for(auto change: all_changes) {
                    int y = get<0>(change);
                    int x = get<1>(change);
                    int e = get<2>(change);
                    C[y][x] = e;
                }
            } 
            // else {
            //     max_score = tmp_score;
            // }

        } else {
            // 色の変換
            // TODO 真ん中に向かってコピーしたい、複数同時に変更する
            change_color(C);
            // change_color(C);
            // change_color(C);
        }
    }

    // std::cout << iter_cnt << endl;

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