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


constexpr int SZ = 6;
State st[SZ];
int mx[SZ];
int lineNum = 1;
int totalModif = 0, perc = 1;
bool ended[SZ];
State cur[SZ];

string DB[SZ][8] = {
{
    "96884667538681",
    "95427425403416",
    "71379321369769",
    "78091637127502",
    "14825865402830",
    "61452780130913",
    "28301937216452",
    "79059144564283"
},
{
    "16679832458122",
    "84300584269874",
    "82412617436907",
    "39672169312013",
    "93057482524531",
    "52183753065486",
    "45812914371904",
    "37049707822032"
},
{
    "64598665034635",
    "83527424503418",
    "21379321869769",
    "68091637127502",
    "94825865402830",
    "21452780130912",
    "18301937216453",
    "77459144564233"
},
{
    "45599832071321",
    "48300584269874",
    "02412617436907",
    "19672169312013",
    "53057482524531",
    "32183953065486",
    "50812914371968",
    "71468200822356"
},
{
    "64381548832212",
    "63427402513413",
    "11379320169769",
    "78091637627502",
    "96825865402834",
    "21452780130914",
    "74301932716455",
    "09559114538233"
},
{
    "39911052877218",
    "13427590116058",
    "21372391330626",
    "98091637627537",
    "76825825402867",
    "21452748130914",
    "84301994616451",
    "77837532263253"
}
};





int ThreadSA(int stateID, int lim = 10'000, double T = 10.0, double d = 0.9999) {
    /*static thread_local mt19937 generator;
    uniform_int_distribution<int> xd(0,7);
    uniform_int_distribution<int> yd(0,13);
    uniform_int_distribution<int> vd(0,9);
    uniform_real_distribution<double> pd(0,1);*/

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
        //State a;
        //st[i] = a;
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
