// a optimized SA implemented in c++
//
// same idea as 'sa.py',
// just randomly swap and a chosen one and a candidate one.
//
// but in each round, fastly calculate nearest bacon neighbors
// h*log(b) vs. h*b
//
// we utilize STL set's order feature (red-black tree actually underlying)
//
// one notable parameter that you might like to change is the scale
// how temperature decreses. for better results, you can use a higher scale
// variable (< 1), but the runtime is longer as well.
//
// g++ -O3 saop.cpp
// ./a.out < file  or  ./a.out file
#include <vector>
#include <set>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

typedef pair<int,int> point;

class findbacons {
	vector<point> houses, bacons, candidates;
	typedef set<pair<int, point> > set_t;
	vector<set_t> mindists;
public:
	findbacons(vector<point> &h, vector<point> &c, int b) : houses (h)
	{
		random_shuffle(c.begin(), c.end());
		for (vector<point>::iterator vi = c.begin(); vi != c.end(); ++vi) {
			if (vi < c.begin() + b)
				bacons.push_back(*vi);
			else
				candidates.push_back(*vi);
		}

		for (vector<point>::iterator vi = h.begin(); vi != h.end(); ++vi) {
			set_t s;
			for (int i = 0; i < b; i++)
				s.insert(make_pair(dist(*vi, bacons[i]), bacons[i]));
			mindists.push_back(s);
		}
	}

	inline int dist(point &a, point &b)
	{
		return abs(a.first - b.first) + abs(a.second - b.second);
	}

	inline double randpos()
	{
		return random() % RAND_MAX / (double) RAND_MAX;
	}

	inline int cost()
	{
		int ret = 0;
		for (vector<set_t>::iterator vi = mindists.begin();
		    vi != mindists.end(); ++vi)
			ret += vi->begin()->first;
		return ret;
	}

	inline void add(point &a)
	{
		for (vector<set_t>::iterator vi = mindists.begin();
		    vi != mindists.end(); ++vi) 
			vi->insert(make_pair(dist(houses[vi-mindists.begin()], a), a));
	}

	inline void remove(point &a)
	{
		for (vector<set_t>::iterator vi = mindists.begin();
		    vi != mindists.end(); ++vi)
			vi->erase(make_pair(dist(houses[vi-mindists.begin()], a), a));
	}

	void run()
	{
		double temperature = 1e10;
		int c = cost();
		fprintf(stderr, "%d\n", c);
		while (temperature > 1e-300) {
			int k = random() % bacons.size(),
			    m = random() % candidates.size();
			remove(bacons[k]);
			add(candidates[m]);
			int nc = cost();
			if (nc < c || randpos() < exp((c - nc) / temperature)) {
				swap(bacons[k], candidates[m]);
				if (nc < c)
					fprintf(stderr, "%d\n", nc);
				c = nc;
			} else {
				remove(candidates[m]);
				add(bacons[k]);
			}
			temperature *= 0.9; // you might want to change this
		}
		printf("%d\n", c);
		for (vector<point>::iterator vi = bacons.begin();
		    vi != bacons.end(); ++vi)
			printf("(%d,%d)\n", vi->first, vi->second);
		printf("%d\n", c);
	}
};

int
main(int argc, char *argv[])
{
	srandom(time(NULL));
	if (argc == 2)
		freopen(argv[1], "r", stdin);
	int w, h, b;
	scanf("%dx%d%d", &w, &h, &b);
	vector<point> p, c;
	for (int i = 0; i < h; i++) {
		static char line[1024];
		scanf("%s", line);
		for (int j = 0; j < w; j++)
			if (line[j] == 'P')
				p.push_back(make_pair(i, j));
			else
				c.push_back(make_pair(i, j));
	}
	findbacons(p, c, b).run();
	return 0;
}
