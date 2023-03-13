#include <bits/stdc++.h>
#include <windows.h>
#define fastio cin.tie(NULL);cout.tie(NULL);ios::sync_with_stdio(false)
using namespace std;


void gotoxy(short x, short y)
{
	COORD Pos = { static_cast<SHORT>(x - 1), static_cast<SHORT>(y - 1) };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

struct Random {
	mt19937 rd;
	Random() : rd((unsigned)chrono::steady_clock::now().time_since_epoch().count()) {}
	Random(int seed) : rd(seed) {}
	short GetInt(short l = 0, short r = 32767) {
		return uniform_int_distribution<short>(l, r)(rd);
	}
	double GetDouble(double l = 0, double r = 1) {
		return uniform_real_distribution<double>(l, r)(rd);
	}
}Rand;


struct State {
	short v[8][14]{};

	State() {
		for (int i = 0; i < 8; i++) for (int j = 0; j < 14; j++)
			v[i][j] = Rand.GetInt(0, 9);
	}

	void SetState(string DB[]) {
		for (int i = 0; i < 8; i++) for (int j = 0; j < 14; j++)
			v[i][j] = DB[i][j] & 15;
	}
	void SetState(State& target){
	    for (int i = 0; i < 8; i++) for (int j = 0; j < 14; j++)
			v[i][j] = target.v[i][j];
	}

	bool Check(int n) const {
		string s = to_string(n);

		short check[8][14][10]{}; memset(check, -1, sizeof check);
		auto DFS = [&](int x, int y, int dep, auto&& DFS) -> bool {
			if (dep == s.size()) return 1;
			if (v[x][y] != (s[dep] & 15)) return 0;
			auto& ret = check[x][y][dep];
			if (~ret) return ret;
			for (int k = 0; k < 8; k++) {
				int nx = x + "10001222"[k] - '1';
				int ny = y + "22100012"[k] - '1';
				if (nx < 0 || nx >= 8 || ny < 0 || ny >= 14) continue;
				if (DFS(nx, ny, dep + 1, DFS)) return ret = 1;
			}
			return ret = 0;
		};

		for (int i = 0; i < 8; i++) for (int j = 0; j < 14; j++)
			if (DFS(i, j, 0, DFS)) return 1;
		return 0;
	}
	int GetBlankScore() const {
		int ret = 8500;
		for(int i=0;i<=8500;i++){
			if(!Check(i))ret--;
		}
		return ret;
	}
	int GetScore() const {
		int ret = 1;
		for (; Check(ret); ret++);
		return ret;
	}
};


constexpr int SZ = 1;
State st[SZ];
int mx[SZ];
int lineNum = 1;
int totalModif = 0, perc = 1;
bool ended[SZ];
State cur[SZ];

string DB[SZ][8] = {
};





int ThreadSA(int stateID, int lim = 10'000, double T = 10.0, double d = 0.9999) {
	int ret = cur[stateID].GetScore(), score = ret;
	for (int i = 0, t = 1; i < lim; i++, T *= d) {
		totalModif++;
		if(totalModif > lim*SZ*perc/10){
			perc+=1;
			gotoxy(45,lineNum-1);
			printf("Loading : %d percent done...\t\t",(10*(perc-1)));
			gotoxy(45,lineNum);
			printf("%d / %d\t\t",totalModif,lim*SZ);
			T*=0.9;
        	}
		State nxt = cur[stateID];

		//int nx=xd(generator),ny=yd(generator),nv=vd(generator);
		int nx=Rand.GetInt(0,7),ny=Rand.GetInt(0,13),nv=Rand.GetInt(0,9);
		nxt.v[nx][ny] = nv;
		int nxt_score = nxt.GetScore();

		// score<nxt_score -> move
		// score>nxt_score -> stochastic descent
		double p = exp(-(score - nxt_score) / T);
		//double np = pd(generator);
		double np = Rand.GetDouble(0,1);
		if (p > np) score = nxt_score, cur[stateID] = nxt;
		ret = max(ret, score);
	}
	ended[stateID] = true;
    	return ret;
}
void ThreadSAAlgorithm(){
	for(int i=0;i<SZ;i++){
		st[i].SetState(DB[i]);
		mx[i] = st[i].GetScore();
		cout<<"Case #"<<i+1<<" Score : "<<st[i].GetScore();cout<<"\n";lineNum++;
	}

	double T = 5.0, d = 0.9999; int lim = 20'000;
	cout<<"Input Temperature : ";
	cin>>T;lineNum++;
	if(cin.fail()){
		T = 10.0;
	}
	cout<<"T : "<<T<<" lim : "<<lim;cout<<"\n";lineNum++;
	double orgT = T,orglim = lim;

	for (int c = 0; ; c++) {
        totalModif = 0, perc = 1;
        gotoxy(1,lineNum);
	cout << "cur_T : " << T << '\n';lineNum++;
	cout << "cur_lim : " << lim << '\n';lineNum++;
	gotoxy(45,lineNum-2);
        printf("Depth : %d",c+1);
	vector<thread> works;

	for(int i=0;i<SZ;i++){
		cur[i] = st[i];
    		ended[i] = false;
            	works.push_back(thread(ThreadSA,i,lim,T,d));
	}
	for(int i=0;i<SZ;i++){
    		works[i].join();
	}
	bool flag = false;
	do{
    		flag = true;
            	for(int i=0;i<SZ;i++){
		flag = flag && ended[i];
		}
	}while(!flag);
        gotoxy(1,lineNum);
        int progressed = 0;
	for (int idx = 0; idx < SZ; idx++) {
		int score = cur[idx].GetScore();
		cout <<"Case #"<<idx+1<<" Score : " << mx[idx] << " -> " << score;cout<<"\n";lineNum++;

		if (mx[idx] >= score){
                	continue;
		}
		mx[idx] = score;
		progressed++;

		st[idx].SetState(cur[idx]);
		for (int i = 0; i < 8; i++) {
                	cout<<"    \"";
			for (int j = 0; j < 14; j++) cout << (st[idx]).v[i][j];
                	cout<<"\"";
                	if(i!=7)cout<<",";
				cout << '\n';lineNum++;
			}
		}

		if (T < 120.0) T *= 1.7;
		else T = 120;
		if (lim < 300'000) lim *= 1.5;
		else lim = lim;
		if (progressed*3 >= SZ){
            		T = orgT;
            		lim = orglim;
		}
	}
}

int main() {
	ThreadSAAlgorithm();
}
